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
    public:
        Simulation(const Config& config); // Constructor

        void initialize(); // Initialize the simulation parameters and fields
        void run(); // Run the simulation
        void writeResults(); // Write the simulation results to output files
    
    private:

        void initializeGrid(); // Initialize the computational grid
        void initializeFields(); // Initialize the velocity and pressure fields

        void applyBoundaryConditions(); // Apply boundary conditions to the velocity and pressure fields
        void applyVelocityBoundaryConditions(); // Apply velocity boundary conditions
        void applyPressureBoundaryConditions(); // Apply pressure boundary conditions

        void computeTimeStep(); // Compute the time step size based on CFL condition
        void solveMomentum(); // Solve the momentum equations for velocity fields
        void solvePressure(); // Solve the pressure Poisson equation
        void correctVelocity(); // Correct the velocity field to satisfy continuity
        void updateResiduals(); // Update the residuals for convergence monitoring
        bool hasConverged() const; // Check if the simulation has converged based on residuals

        Config config; // Configuration parameters

        double dx{}; // Grid spacing in x-direction
        double dy{}; // Grid spacing in y-direction

        std::vector<double> x; // x-coordinates of the grid
        std::vector<double> y; // y-coordinates of the grid

        Matrix u; // Velocity in x-direction
        Matrix v; // Velocity in y-direction
        Matrix p; // Pressure field

        Matrix speed;
        Matrix vorticity;

        std::vector<double> residualHistory; // History of residuals for convergence monitoring
};