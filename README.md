# About

This is a numerical solver for the shallow water equations, intended for studying wave propagation and wave–seabed interactions.
The project is currently under active development.

---

## Build

To compile the code, simply run:

```bash
make
```

This will generate the executable `water.sim` in the `bin/` directory.

---

## Usage

After compilation, run the solver from the `bin/` directory:

```bash
cd bin
./water.sim -i ../input/shallowWater_gaussian_seabed.in
```

The input file specifies the physical setup, numerical parameters, and initial conditions.

---

## Directory Structure

```
ShallowWater/
├── bin/            # Compiled executable
│   └── water.sim
├── include/        # Header files
│   ├── config.h
│   ├── setup.h
│   ├── sim.h
│   └── utils.h
├── input/          # Input configuration files
│   ├── shallowWater.in
│   └── shallowWater_gaussian_seabed.in
├── obj/            # Object files generated during compilation
├── src/            # Source files
│   ├── boundary.cpp
│   ├── config.cpp
│   ├── integral.cpp
│   ├── main.cpp
│   ├── riemann_HLLC.cpp
│   ├── setup.cpp
│   ├── setup_wave.cpp
│   ├── setup_dispersion.cpp
│   └── utils.cpp
├── pypkg/          # Python post-processing and visualization outputs
│   ├── data.ipynb
│   └── *.mp4
├── PythonVersion/  # Python implementation / experiments (optional)
└── Makefile
```

---

## Current Status and Limitations

This project is an ongoing project.

- The current version supports **non-flat seabed (bathymetry) construction**, including spatially varying seabed profiles.
- **Dry–wet treatment is not yet implemented**.
  As a result, the seabed elevation must remain **below the water surface everywhere** during the simulation.

---

## Notes

- The `pypkg/` directory contains Python scripts and example visualization outputs used for post-processing.
