#include "simulation.h"
#include "csv_writer.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <fstream>

namespace fs = std::filesystem;

namespace 
{
    // helper functions

    // convert text to lowercase
    std::string toLower(std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            { 
                return std::tolower(c);
            }
        );
        return text;
    }

    // convert text to uppercase
    std::string toUpper(std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            { 
                return std::toupper(c);
            }
        );
        return text;
    }

    // returns the absolute value of the maximum element in a vector
    double maxAbs(const Matrix& M)
    {
        double maximum = 0.0;
        for (double value : M.data)
            maximum = std::max(maximum, std::abs(value));
       
        return maximum;
    }

    // returns the sum of all elements in a vector
    double meanAll(const Matrix& M)
    {
        if (M.data.empty())
            return 0.0;

        double sum = 0.0;
        for (double value : M.data)
            sum += value;
           
        return sum / M.data.size();
    }

    // returns true if the matrix contains any NaN values
    bool allFinite(const Matrix& M)
    {
        for (double value : M.data)
        {
            if (!std::isfinite(value))
                return false;
        }
        return true;
    }

    // returns the first derivative of x
    double firstDerivativeX( const Matrix& field,
        int i, int j, double dx
    )
    {
        return (
            field(i+1, j) - field(i-1, j)
        ) / (2.0 * dx);
    }

    // returns the first derivative of y
    double firstDerivativeY( const Matrix& field,
        int i, int j, double dy
    )
    {
        return (
            field(i, j+1) - field(i, j-1)
        ) / (2.0 * dy);
    }

    // returns the second derivative of x
    double secondDerivativeX( const Matrix& field,
        int i, int j, double dx
    )
    {
        return (
            field(i+1, j) - 2.0 * field(i, j) + field(i-1, j)
        ) / (dx * dx);
    }

    // returns the second derivative of y
    double secondDerivativeY( const Matrix& field,
        int i, int j, double dy
    )
    {
        return (
            field(i, j+1) - 2.0 * field(i, j) + field(i, j-1)
        ) / (dy * dy);
    }

    // returns the Laplacian of a field
    double laplacian( const Matrix& field,
        int i, int j, double dx, double dy
    )
    {
        return secondDerivativeX(field, i, j, dx) +
               secondDerivativeY(field, i, j, dy);
    }
}

// 
Simulation::Simulation(const Config& config)
    : config(config)
{
}

// The overall initialization function
void Simulation::initialize()
{
    initializeMatrices();

    dx = config.length / (config.N - 1);
    dy = config.height / (config.N - 1);

    x.resize(config.N);
    y.resize(config.N);

    for (int i = 0; i < config.N; i++)
    {
        x[i] = i * dx;
        y[i] = i * dy;
    }

    applyBoundaryConditions();

    residualHistory.clear(); // clears residual history for fresh simulation
}

// initialize matrix function
void Simulation::initializeMatrices()
{
    u = Matrix(config.N);
    v = Matrix(config.N);

    uStar = Matrix(config.N);
    vStar = Matrix(config.N);

    uOld = Matrix(config.N);
    vOld = Matrix(config.N);

    p = Matrix(config.N);
}


// initialize boundary conditions function
void Simulation::applyBoundaryConditions()
{
    applyVelocityBoundaryConditions(u, v);
    applyPressureBoundaryConditions();
}

// initialize velocity boundary conditions function
void Simulation::applyVelocityBoundaryConditions(Matrix& U, Matrix& V)
{
    int N = config.N;

    // Left and Right wall
    for (int j = 0; j < N; j++)
    {
        U(0, j) = 0.0;
        V(0, j) = 0.0;
    
        U(N - 1, j) = 0.0;
        V(N - 1, j) = 0.0;
    }

    // Bottom and Top wall
    for (int i = 0; i < N; i++)
    {
        U(i, 0) = 0.0;
        V(i, 0) = 0.0;
 
        U(i, N - 1) = config.lidVelocity;
        V(i, N - 1) = 0.0;
    }

    // Explicitly kill the lid corners
    U(0, N - 1) = 0.0;
    U(N - 1, N - 1) = 0.0;
}

// initialize pressure boundary conditions function
void Simulation::applyPressureBoundaryConditions()
{
    int N = config.N;

    // Left & Right
    for(int j = 0; j < N; j++)
    {
        p(0, j) = p(1, j);
        p(N-1, j) = p(N-2, j);
    }
    
    // Bottom & Top
    for(int i = 0; i < N; i++)
    {
        p(i, 0) = p(i, 1);
        p(i, N-1) = p(i, N-2);
    }

    // Reference pressure
    // p(0, 0) = 0.0;
}

// initialize run simulation function
void Simulation::run()
{
    auto startTime = std::chrono::high_resolution_clock::now(); // start run-time counter
    
    initialize();

    iteration = 0;  // setting iteration = 0 for new simulation
    while (iteration <= config.maxIterations)
    {
        uOld = u; // Store the previous velocity field
        vOld = v; // Store the previous velocity field

        // advance one CFD iteration
        computeTimeStep();
        solveMomentum();

        // // Debugging: calculates and print out max divergence before velocity correction
        // std::cout
        //     << "Div before pressure = "
        //     << computeMaximumDivergence(uStar, vStar)
        //     << "\n";

        solvePressure();
        removePressureMean();
        correctVelocity();

        // // Debugging: calculates and print out max divergence after velocity correction
        // std::cout
        //     << "Div after pressure = "
        //     << computeMaximumDivergence(u, v)
        //     << "\n";
            
        // NaN check
        if(!allFinite(u) || !allFinite(v) || !allFinite(p))
        {
            throw std::runtime_error(
                "Solution diverged."
            );
        }

        // compute velocity residual
        double velocityResidual = computeVelocityResidual();
        residualHistory.push_back(velocityResidual); // saving the residual history

        // Print progress every 100 iterations
        if (iteration % 100 == 0)
        {
            double maxVelocity = computeMaximumVelocity();
            double maxPressure = maxAbs(p);

            std::cout
                << "\n----------------------------------------\n";

            std::cout
                << "Iteration : "
                << iteration
                << "\n";

            std::cout
                << "dt        : "
                << dt
                << "\n";

            std::cout
                << "Residual  : "
                << velocityResidual
                << "\n";

            std::cout
                << "Max Speed : "
                << maxVelocity
                << "\n";

            std::cout
                << "Max pressure = "
                << maxPressure
                << "\n";

            std::cout
                << "----------------------------------------\n";
        }

        // compute convergence
        if (velocityResidual < config.velocityTolerance)
        {
            std::cout
                << "\nSolver converged after "
                << iteration
                << " iterations.\n";
                
                break;
        }

        // checking if maximum iteration is reached before convergence
        if (iteration == config.maxIterations)
        {
            std::cout
                << "\nMaximum iterations reached before convergence.\n";
        }
        iteration++;
    
    }

    // stops runtime conter and display time
    auto endTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<double>(endTime-startTime).count();
    std::cout << "\nSimulation Time : " << elapsedTime << " s\n";
    
    // saves the results
    writeResults();
}

// initialize computeTimeStep function
void Simulation::computeTimeStep()
{
    double umax = maxAbs(u);
    double vmax = maxAbs(v);

    double velocity = std::max(umax, vmax);

    if (velocity < 1e-12)
        velocity = config.lidVelocity; // to avoid division by zero set the lid velocity to the neede velocity

    double dtConvective = 
        config.cfl * dx / velocity; // convective stability limit

    double dtDiffusive =
        0.25 * dx * dx / config.kinematicViscosity(); // diffusive stability limit

    dt = std::min(dtConvective, dtDiffusive); // choose the minimun stablity limit as the dt value
}

// initialize solve momentum equation function
void Simulation::solveMomentum()
{
    int N = config.N;

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            //----------------------------
            // Compute derivatives
            //----------------------------

            // u-components derivatives
            double dudx = firstDerivativeX(u, i, j, dx);
            double dudy = firstDerivativeY(u, i, j, dy);
            double dpdx = firstDerivativeX(p, i, j, dx);
            double laplaceU = laplacian(u, i, j, dx, dy);

            // v-components derivatives
            double dvdx = firstDerivativeX(v, i, j, dx);
            double dvdy = firstDerivativeY(v, i, j, dy);
            double dpdy = firstDerivativeY(p, i, j, dy); 
            double laplaceV = laplacian(v, i, j, dx, dy);

            //----------------------------
            // Compute convection terms
            //----------------------------
            double convectionU = u(i,j) * dudx + v(i,j) * dudy; // u-convection term
            double convectionV = u(i,j) * dvdx + v(i,j) * dvdy; // v-convection term

            //----------------------------
            // Compute diffusion terms
            //--------------------------
            double diffusionU = config.kinematicViscosity() * laplaceU; // u-diffusion term
            double diffusionV = config.kinematicViscosity() * laplaceV; // v-diffusion term
            
            //----------------------------
            // Update u* and v*
            //----------------------------
            uStar(i,j) = u(i,j) + dt * ( -convectionU -(dpdx/config.rho) + diffusionU); 
            
            vStar(i,j) = v(i,j) + dt * ( -convectionV -(dpdy/config.rho) + diffusionV); 
        }
    }
    // reapplyng the boundary conditions to the intermediate velocity
    applyVelocityBoundaryConditions(uStar, vStar);
}

// initialize solve pressure equation function
void Simulation::solvePressure()
{
    int N = config.N;

    // perform jacobi iterations
    for (int iter = 0; iter < config.pressureIterations; iter++)
    {
        double maxPressureChange = 0.0; // to monitor convergence of the pressure solver

        // Sweeping over interior cells
        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                double divergence = firstDerivativeX(uStar, i, j, dx) + firstDerivativeY(vStar, i, j, dy); // Compute divergence of u*
                double source = (config.rho / dt) * divergence; //pressure source term

                double newPressure =
                (
                    p(i + 1, j)
                  + p(i - 1, j)
                  + p(i, j + 1)
                  + p(i, j - 1)
                  - dx * dx * source
                ) / 4.0;

                maxPressureChange =
                    std::max(
                        maxPressureChange,
                        std::abs(newPressure - p(i, j))
                    );

                // Gauss-Seidel update
                p(i, j) = newPressure;
            }
        }

        // // Debugging: prints max pressure change after every 20 iterations
        // if(iter % 20 == 0)
        // {
        //     std::cout
        //         << "Pressure iter "
        //         << iter
        //         << " change = "
        //         << maxPressureChange
        //         << "\n";
        // }

        applyPressureBoundaryConditions(); // apply boundary conditions to the pressure field

        // Check for convergence
        if (maxPressureChange < config.pressureTolerance)
        {
            break;
        }
    }
}

// initialize correct velocity function
void Simulation::correctVelocity()
{
    int N = config.N;

    for (int i = 1; i < config.N - 1; i++)
    {
        for (int j = 1; j < config.N - 1; j++)
        {
            double dpdx = firstDerivativeX(p, i, j, dx);
            double dpdy = firstDerivativeY(p, i, j, dy);

            u(i,j) = uStar(i,j) - (dt / config.rho) * dpdx; // Correct u-velocity
            v(i, j) = vStar(i,j) - (dt / config.rho) * dpdy; // Correct v-velocity 
        }
    }
    applyVelocityBoundaryConditions(u, v);
}

// initialize compute velocity residuals function
double Simulation::computeVelocityResidual()
{
    double maxVelocityChange = 0.0;

        for (int i = 1; i < config.N - 1; i++)
        {
            for (int j = 1; j < config.N - 1; j++)
            {
                double du = std::abs(u(i, j) - uOld(i, j));
                double dv = std::abs(v(i, j) - vOld(i, j));

                maxVelocityChange =
                    std::max(maxVelocityChange, du);

                maxVelocityChange =
                    std::max(maxVelocityChange, dv);
            }
        }
    
    return maxVelocityChange;
}

// write results function
void Simulation::writeResults()
{
    const std::string outputFolder = "results/";
    fs::create_directories(outputFolder); //automatically create the output folder
    
    CSVWriter::writeMatrix(u, outputFolder + "velocity_u.csv"); // writes u-velocity
    CSVWriter::writeMatrix(v, outputFolder + "velocity_v.csv"); // writes v-velocity
    CSVWriter::writeMatrix(p, outputFolder + "pressure.csv"); // writes pressure

    // computes velocity magnitude
    Matrix velocityMagnitude(config.N);
    for (int i = 0; i < config.N; i++)
    {
        for (int j = 0; j < config.N; j++)
        {
            velocityMagnitude(i, j) = std::sqrt( u(i, j) * u(i,j)
                                        + v(i, j) * v(i,j));
        }
    }
    // writes velocity magnitude
    CSVWriter::writeMatrix(velocityMagnitude, outputFolder + "velocity_magnitude.csv");

    // writes residual History
    CSVWriter::writeVector(residualHistory, outputFolder + "residual_history.csv");

    // writes computational grid coordinates
    CSVWriter::writeCoordinates(x, outputFolder + "x.csv"); // x-coordinate
    CSVWriter::writeCoordinates(y, outputFolder + "y.csv"); // y-coordinate

    // writes simulation info to file
    writeSimulationInfo(outputFolder);

    std::cout << "\nResults written successfully.\n"; // success message
}

// initialize the write simulation info to file function
void Simulation::writeSimulationInfo(const std::string& outputFolder)
{
    std::ofstream file(outputFolder + "simulation_info.txt");

    if (!file.is_open())
    {
        throw std::runtime_error("Could not create simulation_info.txt");
    }

    file << "=========================================\n";
    file << "2D Lid-Driven Cavity Simulation\n";
    file << "=========================================\n\n";

    file << "Grid Information\n";
    file << "---------------------------\n";
    file << "Grid Size           : " << config.N << " x " << config.N << "\n";
    file << "Domain Length       : " << config.length << "\n";
    file << "Domain Height       : " << config.height << "\n";
    file << "dx                  : " << dx << "\n";
    file << "dy                  : " << dy << "\n\n";

    file << "Flow Parameters\n";
    file << "---------------------------\n";
    file << "Reynolds Number     : " << config.Re << "\n";
    file << "Lid Velocity        : " << config.lidVelocity << "\n";
    file << "Density             : " << config.rho << "\n";
    file << "Kinematic Viscosity : " << config.kinematicViscosity() << "\n\n";

    file << "Solver Settings\n";
    file << "---------------------------\n";
    file << "CFL Number          : " << config.cfl << "\n";
    file << "Pressure Tolerance  : " << config.pressureTolerance << "\n";
    file << "Velocity Tolerance  : " << config.velocityTolerance << "\n";
    file << "Maximum Iterations  : " << config.maxIterations << "\n";
    file << "Pressure Iterations : " << config.pressureIterations << "\n\n";

    file << "Simulation Results\n";
    file << "---------------------------\n";
    file << "Final Time Step     : " << dt << "\n";
    file << "Iterations Performed: " << iteration << "\n";
    file << "Simulation Time   : " << elapsedTime << " s\n";

    if (!residualHistory.empty())
    {
        file << "Final Residual      : "
             << residualHistory.back() << "\n";
    }

    file.close();
}

// initialze teh compute maximum velocity function
double Simulation::computeMaximumVelocity()
{
    double maxVelocity = 0.0;

    for (int i = 0; i < config.N; i++)
    {
        for (int j = 0; j < config.N; j++)
        {
            double velocity =
                std::sqrt(
                    u(i,j)*u(i,j)
                    +
                    v(i,j)*v(i,j)
                );

            maxVelocity =
                std::max(maxVelocity, velocity);
        }
    }

    return maxVelocity;
}

// initialize compute maximmum divergence function
double Simulation::computeMaximumDivergence(const Matrix& U,
                                            const Matrix& V)
{
    double maxDiv = 0.0;

    for (int i = 1; i < config.N - 1; ++i)
    {
        for (int j = 1; j < config.N - 1; ++j)
        {
            double dudx = firstDerivativeX(U, i, j, dx);
            double dvdy = firstDerivativeY(V, i, j, dy);

            maxDiv = std::max(
                maxDiv,
                std::abs(dudx + dvdy)
            );
        }
    }

    return maxDiv;
}

// initialize the remove pressure mean function
void Simulation::removePressureMean()
{
    int N = config.N;
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum += p(i, j);
            count++;
        }
    }

    double mean = sum / count;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            p(i, j) -= mean;
}