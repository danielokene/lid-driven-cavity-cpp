#pragma once

#include <vector>
#include <string>
#include "matrix.h"

// defines the configuration parameters for the simulation
struct Config
{
    //Grid size
    int N = 130;

    // Cavity size
    double length = 1.0;
    double height = 1.0;

    // Flow conditions
    double Re = 100; // Reynolds number
    double lidVelocity = 1.0; // lid velocity

    // Fluid properties
    double rho = 1.0; // fluid density
    double kinematicViscosity() const
    {
        return lidVelocity * length / Re;
    } // kinematic viscosity

    // Solver settings
    double cfl = 0.5; // CFL number for stability
    double pressureTolerance = 1e-6; // pressure convergence tolerance
    double velocityTolerance = 1e-6; // velocity convergence tolerance
    int maxIterations = 1000; // maximum number of iterations
    int pressureIterations = 100; // number of iterations for pressure solver

};


class Simulation
{
    public:
        Simulation(const Config& config); // Constructor

        void initialize(); // Initialize the simulation parameters and fields
        void run(); // Run the simulation
        void writeResults(); // Write the simulation results to output files
    
    private:

        void initializeMatrices(); // Initialize the computational matrix

        void applyBoundaryConditions(); // Apply boundary conditions to the velocity and pressure fields
        void applyVelocityBoundaryConditions(Matrix& U, Matrix& V); // Apply velocity boundary conditions
        void applyPressureBoundaryConditions(); // Apply pressure boundary conditions

        void computeTimeStep(); // Compute the time step size based on CFL condition
        void solveMomentum(); // Solve the momentum equations for velocity fields
        void solvePressure(); // Solve the pressure Poisson equation
        void correctVelocity(); // Correct the velocity field to satisfy continuity
        double computeVelocityResidual(); // computes maximum velocity change
        double computeMaximumVelocity(); // computes the maximum velocity of the current iteration when called
        double computeMaximumDivergence(const Matrix& U, const Matrix& V); // compute maximum divergence


        void writeSimulationInfo(const std::string& outputFolder); // writes simulation metadata to file

        Config config; // Configuration parameters

        double dx{}; // Grid spacing in x-direction
        double dy{}; // Grid spacing in y-direction
        double dt{}; // time step size
        int iteration = 0; // number of iteration
        double elapsedTime{};   // Total simulation runtime in seconds


        std::vector<double> x; // x-coordinates of the grid
        std::vector<double> y; // y-coordinates of the grid

        Matrix u; // Velocity in x-direction
        Matrix v; // Velocity in y-direction
        Matrix uStar; // intermediate velocity in x-direction
        Matrix vStar; // intermediate velocity in y-direction
        Matrix uOld; // previous velocity in x-direction
        Matrix vOld; // previous velocity in y-direction
        Matrix p; // Pressure field


        std::vector<double> residualHistory; // History of residuals for convergence monitoring
};