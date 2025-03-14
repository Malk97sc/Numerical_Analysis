#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x);
double secant(double x0, double x1, int n);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send N iters\n");
        return EXIT_FAILURE;
    }
    double x0 = -2.6, x1 = -2.4;
    int n = atoi(argv[1]);

    secant(x0, x1, n);

    return EXIT_SUCCESS;
}

double secant(double x0, double x1, int n){
    double x2, tol = 1e-15, error = 0;

    printf("|Iter\t|x_{i+1}\t\t| f(x_{i+1})\t\t| Ea\t\t|\n");

    for(int i=0; i < n; i++){
        x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
        error = fabs(x2 - x1);        

        printf("|%d\t|%.15lf\t| %.15lf\t| %.15lf|\n", i, x2, f(x2), error);

        if (error < tol){
            printf("Converged\n");
            break;
        }

        x0 = x1;
        x1 = x2;
    }
}

double f(double x){
    return pow(x, 3) - 3*x + 2;
}
