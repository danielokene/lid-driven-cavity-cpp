#include "simulation.h"
#include "io.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace fs = std::filesystem;

namespace 
{
    // helper functions

    // convert text to lowercase
    std::string toLower(const std::string text)
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
    std::string toUpper(const std::string text)
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
    initializeGrid();
    initializeFields();
    applyBoundaryConditions();
}

// initialize grid function
void Simulation::initializeGrid()
{
    uStar = Matrix(config.N);
    vStar = Matrix(config.N);
}

// initialize fields function
void Simulation::initializeFields()
{
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
    p(0, 0) = 0.0;
}

// initialize run simulation function
void Simulation::run()
{
    while (!hasConverged())
    {
        computeTimeStep();
        solveMomentum();
        solvePressure();
        correctVelocity();
        applyBoundaryConditions();
        updateResiduals();
    
    }
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
        0.25 * dx * dx / config.nu; // diffusive stability limit

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
            double diffusionU = config.nu * laplaceU; // u-diffusion term
            double diffusionV = config.nu * laplaceV; // v-diffusion term
            
            //----------------------------
            // Update u* and v*
            //----------------------------
            uStar(i,j) = u(i,j) + dt * ( -convectionU -(dpdx/config.rho)+diffusionU); 
            
            vStar(i,j) = v(i,j) + dt * ( -convectionV -(dpdy/config.rho)+diffusionV); 
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

                double neighborAverage = (p(i+1,j) + p(i-1,j) + p(i,j+1) + p(i,j-1)) / 4.0; // Average of neighboring cells pressures
                //

                double newPressure = neighborAverage - (dx * dx / 4.0) * source; // Update pressure using Jacobi formula

                // Tracking convergence
                double change = std::abs(newPressure - p(i,j));
                maxPressureChange = std::max(maxPressureChange, change);

                pNew(i,j) = newPressure; // Store the new pressure value in a temporary matrix
            }
        }

        // Update the pressure field
        for (int i = 1; i < N - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                p(i,j) = pNew(i,j);
            }
        }

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


// initialize update residuals function
void Simulation::updateResiduals()
{
}

// initialize the has converged function
bool Simulation::hasConverged() const
{
    return true;
}

// write results function
void Simulation::writeResults()
{
}