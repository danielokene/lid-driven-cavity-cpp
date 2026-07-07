#pragma once

#include <vector>
#include <string>
#include "matrix.h"

// defines the configuration parameters for the simulation
struct Config
{
    //Grid size
    int N = 130;

    // Fluid properties
    double rho = 1.0; // fluid density
    double nu = 0.01; // kinematic viscosity

    // Flow conditions
    double Re = 100; // Reynolds number
    double lidVelocity = 1.0; // lid velocity

    // Solver settings
    double dt = 0.01; // time step size
    double tolerance = 1e-8; // convergence tolerance
    int maxIterations = 1000; // maximum number of iterations

};


class Simulation
{
    private:
        Config config; // Configuration parameters

        Matrix u; // Velocity in x-direction
        Matrix v; // Velocity in y-direction
        Matrix p; // Pressure field

        Matrix speed;
        Matrix vorticity;


        std::vector<double> x; // x-coordinates of the grid
        std::vector<double> y; // y-coordinates of the grid

        std::vector<double> residualHistory; // History of residuals for convergence monitoring

    public:
        void initialize(); // Initialize the simulation parameters and fields
        void run(); // Run the simulation
        void writeResults(); // Write the simulation results to output files

};