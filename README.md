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
**Note**: Ensure that `coefficients.dat` and `constantTerms.dat` are present in the directory with the program.

## Interpolation

### Lagrange
Use these commands to compile the code:

In normal mode (only the final result)
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
**Note**: Ensure that `xValues.dat` and `yValues.dat` are present in the directory with the program.