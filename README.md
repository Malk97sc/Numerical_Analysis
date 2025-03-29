# Numerical Analysis

## Non-Linear Equations
Use the following command to compile the code:

```bash
gcc secant.c -lm -o secant.out
```
Run:

```bash
./secant.out 10 
```
Note: The number 10 represents the number of iterations.

## Methods for Solving Systems of Equations
use this command to compile the code:

```bash
gcc jacobian.c -lm -o jacobian.out
```
Run:
```bash
./jacobian.out coefficients.dat constantTerms.dat
```
**Note**: Ensure that `coefficients.dat` and `constantTerms.dat` are present in the directory with the program and the first value in the file represents the number of elements.

## Interpolation

### Lagrange
#### Compilation and Execution

#### Simple version
```bash
gcc lagrange.c -o lagrange
```
Run:
```bash
./lagrange xValues.dat yValues.dat
```

Debug mode (with additional output for debugging):
```bash
gcc lagrange.c -D DEBUG -o lagrange
```
Run:
```bash
./lagrange xValues.dat yValues.dat 
```

#### Multi-threaded version

```bash
gcc lagrangeWithThreads.c -lm -o lagrangeThreads
```

Run:
```bash
./lagrangeThreads 2 xValues.dat yValues.dat
```
**Note**: `num_threads` specifies the number of threads to use (first argument).

**Note**: Ensure that `xValues.dat` and `yValues.dat` are present in the directory with the program.

### System of Equations Solver (SEL)
The program solves a system of linear equations using the Gauss-Seidel method and prints the corresponding interpolating polynomial

#### Compilation
```bash
gcc sel.c -lm -o sel.out
```
Run:
```bash
./sel.out x.dat fx.dat
```
**Note**: Ensure that `x.dat` and `fx.dat` are present in the directory with the program.

**Note**: The program internally calls `exec` to run Gauss-Seidel and print the results (in Gauss-Seidel).

### Input File Format  

Both `xValues.dat` and `yValues.dat` must follow this format:  

- The first value in the file represents the number of data points.  
- The following values are the respective x or y coordinates.  