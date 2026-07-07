#include <iostream>
#include "simulation.h"

int main()
{
    std::cout << "Lid-Driven Cavity Solver\n"; // Print a solver message to the console

    Config config;
    Simulation simulation(config); // Create an instance of the Simulation class

    simulation.initialize(); // Initialize the simulation parameters
    simulation.run(); // Run the simulation
    simulation.writeResults(); // Write the simulation results to output files

    return 0;
}