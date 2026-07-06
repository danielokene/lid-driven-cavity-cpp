# Lid-Driven Cavity Flow Solver in C++

A two-dimensional incompressible Navier–Stokes solver for the classical lid-driven cavity problem using the Finite Difference Method (FDM) in C++.

This project was developed as part of my Computational Fluid Dynamics (CFD) learning journey after completing the 12 Steps to Navier–Stokes Python course by Barba Group. The objective is to translate the numerical methods learned in Python into modern C++ while improving software design and numerical programming skills.

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/Python-post--processing-green.svg" alt="Python post-processing">
  <img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="MIT License">
  <a href="https://danielokene.github.io/">
    <img src="" alt="Portfolio">
  </a>
</p>

---

## Overview

The lid-driven cavity problem is one of the most widely used benchmark problems in Computational Fluid Dynamics.

A square cavity is filled with an incompressible fluid. Three walls remain stationary while the top wall moves with a constant velocity, generating a recirculating flow inside the cavity.

This project solves the incompressible Navier–Stokes equations using:

- Finite Difference Method (FDM)
- Explicit time integration
- Pressure Poisson Equation
- Projection Method for pressure–velocity coupling
- Central Difference discretization for diffusion and pressure
- Upwind (Backward Difference) discretization for convection

---

## Governing Equations

### Continuity Equation

\[
\nabla \cdot \mathbf{u}=0
\]

### Momentum Equations

\[
\frac{\partial \mathbf{u}}{\partial t}
+
(\mathbf{u}\cdot\nabla)\mathbf{u}
=
-\frac{1}{\rho}\nabla p
+
\nu\nabla^2\mathbf{u}
\]

where

- **u** = velocity vector
- **p** = pressure
- **ρ** = density
- **ν** = kinematic viscosity

---

## Numerical Method

The solver follows the following algorithm:

1. Initialize velocity and pressure fields.
2. Compute the Pressure Poisson source term.
3. Solve the Pressure Poisson Equation.
4. Update the velocity field using the momentum equations.
5. Apply boundary conditions.
6. Repeat until the desired number of time steps is reached.

---

## Boundary Conditions

Velocity

Top wall:

- u = 1
- v = 0

Bottom wall:

- u = 0
- v = 0

Left wall:

- u = 0
- v = 0

Right wall:

- u = 0
- v = 0

Pressure

- dp/dx = 0 on left and right walls
- dp/dy = 0 on bottom wall
- p = 0 on the top wall

---

## Repository Structure

```
lid-driven-cavity-cpp/
│
├── README.md
├── LICENSE
├── .gitignore
├── CMakeLists.txt
│
├── src/
│   ├── main.cpp
│   ├── solver.cpp
│   ├── pressure.cpp
│   ├── boundary.cpp
│   └── output.cpp
│
├── include/
│   ├── solver.h
│   ├── pressure.h
│   ├── boundary.h
│   └── output.h
│
├── docs/
│   ├── governing_equations.md
│   └── numerical_method.md
│
└── results/
    ├── pressure.png
    ├── velocity.png
    ├── streamlines.png
    └── cavity.gif
```

---

## Build

Clone the repository

```bash
git clone https://github.com/yourusername/lid-driven-cavity-cpp.git
```

Create a build directory

```bash
mkdir build
cd build
```

Generate the project

```bash
cmake ..
```

Compile

```bash
make
```

Run

```bash
./lid_cavity
```

---

## Results

Example outputs include:

- Pressure contours
- Velocity vectors
- Streamlines
- Velocity magnitude
- Flow evolution animation

*(Results will be added as the project develops.)*

---

## Learning Objectives

This project is intended to strengthen understanding of:

- Computational Fluid Dynamics
- Finite Difference Methods
- Numerical Linear Algebra
- Pressure–Velocity Coupling
- Poisson Solvers
- Modern C++
- Scientific Computing

---

## Future Improvements

- Successive Over-Relaxation (SOR)
- Multigrid Pressure Solver
- OpenMP Parallelization
- MPI Parallel Solver
- Adaptive Time Stepping
- VTK Output for ParaView
- Benchmark validation against Ghia et al.

---

## References

1. Lorena A. Barba, *CFD Python: 12 Steps to Navier–Stokes*. https://github.com/barbagroup/CFDPython

2. Ghia, U., Ghia, K. N., & Shin, C. T. (1982). *High-Re solutions for incompressible flow using the Navier–Stokes equations and a multigrid method.*

3. Ferziger, J. H., & Perić, M. *Computational Methods for Fluid Dynamics.*

---

## Acknowledgements

This project is inspired by the excellent **CFDPython** educational materials developed by Professor Lorena A. Barba and collaborators, which provide an accessible introduction to Computational Fluid Dynamics. The implementation in this repository is my own C++ version created for learning and practice.

---

## License

This project is licensed under the MIT License.