#include "csv_writer.h"

#include <fstream>
#include <iostream>

// initialize the write matrix function
void CSVWriter::writeMatrix(const Matrix& matrix,
                            const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open "
                  << filename
                  << std::endl;
        return;
    }

    int rows = matrix.rows();
    int cols = matrix.cols();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            file << matrix(i, j);
            if (j != cols - 1)
            {
                file << ",";
            }
        }

        file << "\n";
    }

    file.close();

    std::cout << "Saved: "
              << filename
              << std::endl;
}

// initialize the write vector function
void CSVWriter::writeVector(const std::vector<double>& values,
    const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open file: " + filename);
    }

    file << "Iteration,Residual\n";

    for (size_t i = 0; i < values.size(); i++)
    {
        file
            << i
            << ","
            << values[i]
            << "\n";
    }

    file.close();
}

// initialize the write coordinate function
void CSVWriter::writeCoordinates(
    const std::vector<double>& values,
    const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open " + filename);
    }

    for(double value : values)
    {
        file << value << "\n";
    }

    file.close();
}