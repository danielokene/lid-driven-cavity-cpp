#pragma once

#include <vector>

struct Matrix 
{
    int n{}; // Number of rows
    std::vector<double> data; // 2D vector to hold matrix data

    Matrix() = default; // Default constructor

    Matrix(int n_, double value = 0.0)
        : n(n_), data(n_ * n_, value)
        {

        } // Constructor to initialize matrix with size n

        double& operator()(int i, int j) 
        {
            return data[i * n + j]; // Access element at (i, j)
        }

        const double& operator()(int i, int j) const 
        {
            return data[i * n + j]; // Access element at (i, j) (const version)
        }

            // Number of rows
        int rows() const
        {
            return n;
        }

        // Number of columns
        int cols() const
        {
            return n;
        }
};