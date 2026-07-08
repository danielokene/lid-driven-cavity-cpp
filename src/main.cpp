#include <iostream>
#include "simulation.h"


int main()
{
    try
    {
        // Solver Banner
        std::cout << "=========================================\n";
        std::cout << "      2D Lid-Driven Cavity Solver\n";
        std::cout << "=========================================\n\n";

        // Create simulation configuration
        Config config;

        
        // Display configuration
        std::cout << "Grid Spacing       : "
                << config.length / (config.N - 1) << "\n";
        std::cout << "Grid Size          : "
                << config.N << " x " << config.N << "\n";
        std::cout << "Reynolds Number    : "
                << config.Re << "\n";
        std::cout << "Maximum Iterations : "
                << config.maxIterations << "\n\n";
        std::cout << "Starting simulation...\n\n";

        // Create simulation
        Simulation simulation(config);

        // Run solver
        simulation.run();

        // Finished-
        std::cout << "\nSimulation completed successfully.\n";

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nError: "
                  << e.what()
                  << std::endl;

        return EXIT_FAILURE;
    }
}