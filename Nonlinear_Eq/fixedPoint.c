#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double g(double x){
    return pow(M_E, -x);  //change this
}

double fixedPoint(double x0, int n){
    double x = x0, x_next, tol = 1e-6, error;

    for(int i=0; i < n; i++){
        x_next = g(x);

        error = fabs(x_next - x);
        printf("iter: %d -> G(%d) = %.4f (Error = %f)\n", i+1, i, x_next, error);

        if(error < tol){
            printf("Converged in %d iterations.\n", i+1);
            return x_next;
        }

        x = x_next;
    }

    printf("Max iters\n");
    return x; //max iters
}

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send n\n"); return EXIT_FAILURE;
    }
    double x0 = 0.5000;
    int n = atoi(argv[1]);

    printf("Result is: %f\n", fixedPoint(x0, n));

    
    return EXIT_SUCCESS;
}
