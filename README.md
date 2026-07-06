# Lid-Driven Cavity Flow Solver in C++
<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/Python-post--processing-green.svg" alt="Python post-processing">
  <img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="MIT License">
</p>

This is a C++17 implementation of the classical two-dimensional lid-driven cavity flow problem.

I developed this project after completing the **CFDPython – 12 Steps to Navier–Stokes** course by the Barba Group. The objective was to deepen my understanding of Computational Fluid Dynamics by translating the numerical methods learned in Python into a structured C++ implementation.

Rather than simply reproducing the Python code, this project focuses on implementing the incompressible Navier–Stokes solver using modern C++ while developing good software engineering practices, numerical programming skills, and a foundation for future high-performance CFD solvers.

This repository is not intended to be a production CFD package. Instead, it serves as a learning-focused baseline that I will continue to improve as I explore advanced numerical methods, parallel computing, and scientific computing.

---

# What the code does

The solver computes the incompressible lid-driven cavity flow on a structured Cartesian grid. The top wall moves with a constant velocity while the remaining walls are stationary, producing the characteristic recirculating flow inside the cavity.

Current implementation includes:

* Serial C++17 solver
* Two-dimensional incompressible Navier–Stokes equations
* Finite Difference Method (FDM)
* Explicit time integration
* Pressure Poisson Equation
* Pressure projection method
* Structured Cartesian grid
* No-slip wall boundary conditions
* CSV output for pressure and velocity fields
* Python scripts for visualization and post-processing

The numerical methodology follows the projection algorithm introduced in **Step 11 (Lid-Driven Cavity Flow)** of the CFDPython course, adapted and implemented independently in C++.

---

# Example Result

The figure below shows a representative solution after the flow has developed inside the cavity.

*(Insert velocity vectors, pressure contours, streamlines, or animation here.)*

---

# Validation

The numerical solution will be validated against the classical benchmark published by Ghia et al. (1982).

The primary comparisons will include:

* Horizontal velocity, **u(y)**, along the vertical centreline (*x = 0.5*)
* Vertical velocity, **v(x)**, along the horizontal centreline (*y = 0.5*)

Future versions of this repository will report:

* L₂ error
* L∞ error
* Mesh convergence
* Reynolds number studies

---

# How it works

The solver advances the incompressible Navier–Stokes equations in pseudo-time using the classical pressure projection method.

Each iteration performs the following steps:

1. Predict the intermediate velocity field.
2. Assemble the source term for the Pressure Poisson Equation.
3. Solve the Pressure Poisson Equation.
4. Correct the velocity and pressure fields.
5. Apply the wall boundary conditions.
6. Repeat until the specified number of time steps is reached.

Further details are provided in **docs/METHODOLOGY.md**.

---

# Running

The project is designed to run on Linux or Windows Subsystem for Linux (WSL).

After building with CMake:

```bash
mkdir build
cd build
cmake ..
make
./lid_cavity
```

Simulation outputs are written as CSV files and can be visualized using the accompanying Python plotting scripts.

---

# Repository Structure

```text
src/            C++ source files
include/        Header files
docs/           Numerical methods and documentation
results/        Simulation outputs and figures
scripts/        Build and plotting scripts
```

---

# Requirements

Solver:

* C++17 compatible compiler
* CMake

Visualization:

* Python 3
* NumPy
* Matplotlib

WSL is recommended for Windows users to provide a Linux-like development environment.

---

# Learning Objectives

This project is part of my Computational Fluid Dynamics learning journey. Through its development, I aim to strengthen my understanding of:

* Finite Difference Methods
* Numerical solution of partial differential equations
* Pressure–velocity coupling
* Incompressible Navier–Stokes equations
* Scientific programming in C++
* Software engineering for scientific computing

---

# Future Improvements

Planned extensions include:

* Higher-order convection schemes
* Successive Over-Relaxation (SOR)
* Multigrid pressure solver
* Adaptive time stepping
* Parallelization with OpenMP
* Distributed computing with MPI
* GPU acceleration using CUDA
* ParaView (VTK) output
* Validation over multiple Reynolds numbers and mesh sizes

---

# References

1. Barba, L. A., et al. **CFDPython: 12 Steps to Navier–Stokes.**

2. Ghia, U., Ghia, K. N., & Shin, C. T. (1982). *High-Re solutions for incompressible flow using the Navier–Stokes equations and a multigrid method.* Journal of Computational Physics, 48(3), 387–411.

3. Ferziger, J. H., & Perić, M. *Computational Methods for Fluid Dynamics.*

---

# Acknowledgements

This project was inspired by the **CFDPython – 12 Steps to Navier–Stokes** educational course developed by Professor Lorena A. Barba and collaborators. The numerical algorithms implemented here are based on concepts learned from that course, while the C++ implementation, repository structure, documentation, and future extensions are my own.

---

# License

Released under the MIT License.
