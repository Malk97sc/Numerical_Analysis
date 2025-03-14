#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int BARSIZE = 89;

double g(double x);
double f(double x);
double fPrim(double x);
void Newton_Raphson(double a);
void showBar(int size);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send x0\n");
        return EXIT_FAILURE;
    }
    double x0 = atof(argv[1]);

    Newton_Raphson(x0);

    return EXIT_SUCCESS;
}

double g(double x){
    return x - (f(x) / fPrim(x));
}

double f(double x){
    return pow(x, 3) - 151.000000/50000.000000;
}

double fPrim(double x){
    return 3 * pow(x, 2);
}

void Newton_Raphson(double a){
    double next, ea = 0.0, x = a, tol = 1e-6;
    int i = 0;

    printf("| Iteration\t| g(x)\t\t\t| f(x)\t\t\t| Ea\t\t\t|\n");
    showBar(BARSIZE);

    do{
        next = g(x);
        ea = fabs(next - x);
        printf("| %2d\t\t| %.14f\t| %.14f\t| %.14f\t|\n", i, next, fabs(f(x)), ea);
        x = next;
        i++;
    }while(ea > tol);
    showBar(BARSIZE);

    printf("Converged after %d iterations.\n", i);
}

void showBar(int size){
    for (int i = 0; i < size; i++) {
        printf("-");
    }
    printf("\n");
}