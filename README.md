# MPI Program for Estimating Pi Using the Monte Carlo Method

This repository hosts an MPI-based C program for estimating the value of π using the Monte Carlo method. The program leverages parallel computation with MPI, distributing tasks across multiple processes to enhance performance. It includes metrics to evaluate efficiency, speedup, and result quality under different configurations.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Performance Analysis](#performance-analysis)
- [Contributing](#contributing)
- [License](#license)

## Overview

The objective of this project is to implement and analyze a parallel program that estimates π using the Monte Carlo method. The program evaluates performance through metrics like execution time, efficiency, and quality under various configurations, demonstrating how MPI-based parallelism improves computationally intensive tasks.

## Features

- Parallel computation of π using the Monte Carlo method.
- Configurable number of iterations and processes to analyze scalability.
- Performance metrics including efficiency, speedup, and quality.

## Requirements

- **MPI (Message Passing Interface)**: Ensure MPI is installed on your system.
- **C Compiler**: Requires a compiler compatible with MPI, such as `mpicc`.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/pabloseijo/ProgramaMPISimple.git
    cd ProgramaMPISimple
    ```

2. Compile the program using `mpicc`:
    ```bash
    mpicc -o pi_montecarlo pi_montecarlo.c
    ```

## Usage

To execute the program, use the following command:

```bash
mpirun -np <num_processes> ./pi_montecarlo <num_iterations>
```

- **`<num_processes>`**: Number of processes to use (e.g., 4).
- **`<num_iterations>`**: Number of iterations for the Monte Carlo simulation (e.g., 1000000).

### Example

```bash
mpirun -np 4 ./pi_montecarlo 1000000
```

This command runs the Monte Carlo simulation with 1,000,000 iterations using 4 parallel processes.

## Performance Analysis

The program calculates key performance metrics to evaluate the effectiveness of parallel computation:

- **Execution Time**: Measures the time taken to complete the computation.
- **Efficiency**: Calculated as the ratio of speedup to the number of processes used.
- **Speedup**: The ratio of sequential execution time to parallel execution time.
- **Quality**: Defined as the inverse of the product of execution time and error, indicating the balance between computation time and result accuracy.

Graphs and analysis of these metrics under different configurations provide insight into the performance improvements achieved through parallelism.

## Contributing

Contributions are welcome! If you’d like to contribute, please fork the repository, create a new branch, and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
