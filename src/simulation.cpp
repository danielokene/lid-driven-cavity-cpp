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
}

// initialize fields function
void Simulation::initializeFields()
{
}

// initialize boundary conditions function
void Simulation::applyBoundaryConditions()
{
}

// run simulation function
void Simulation::run()
{
}

// write results function
void Simulation::writeResults()
{
}