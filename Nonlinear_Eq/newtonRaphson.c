#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double g(double x);
double f(double x);
double fPrim(double x);
void Newton_Raphson(double a);

int main(int argc, char **argv){
    double x0 = -1.4;

    Newton_Raphson(x0);

    return EXIT_SUCCESS;
}

double g(double x){
    return x - (f(x) / fPrim(x));
}

double f(double x){
    return pow(x, 3) - x + 2;
}

double fPrim(double x){
    return (3 * pow(x, 2) - 1);
}

void Newton_Raphson(double a){
    double next, ea = 0.0, x = a, tol = 1e-6;
    int i = 0;

    do{
        next = g(x);
        ea = fabs(next - x);
        printf("Iter: %d | g(x): %.14f |  |f(x)|: %.14f |  Ea: %.14f\n", i+1, next, f(x), ea);
        x = next;
        i++;
    }while(ea > tol);
}