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
    applyVelocityBoundaryConditions();
    applyPressureBoundaryConditions();
}

// initialize velocity boundary conditions function
void Simulation::applyVelocityBoundaryConditions()
{
    int N = config.N;

    // Left wall
    for (int j = 0; j < N; j++)
    {
        u(0, j) = 0.0;
        v(0, j) = 0.0;
    }

    // Right wall
    for (int j = 0; j < N; j++)
    {
        u(N - 1, j) = 0.0;
        v(N - 1, j) = 0.0;
    }

    // Bottom wall
    for (int i = 0; i < N; i++)
    {
        u(i, 0) = 0.0;
        v(i, 0) = 0.0;
    }

    // Top wall (moving lid)
    for (int i = 0; i < N; i++)
    {
        u(i, N - 1) = config.lidVelocity;
        v(i, N - 1) = 0.0;
    }
}

// initialize pressure boundary conditions function
void Simulation::applyPressureBoundaryConditions()
{

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
}

// initialize solve pressure equation function
void Simulation::solvePressure()
{
}

// initialize correct velocity function
void Simulation::correctVelocity()
{
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