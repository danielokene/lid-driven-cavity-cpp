#include "csv_writer.h"

#include <fstream>
#include <iostream>

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