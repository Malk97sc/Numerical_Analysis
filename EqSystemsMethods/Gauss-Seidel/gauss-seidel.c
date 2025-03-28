#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void gaussSeidel(double **A, double *b, double *x, int n);
void readCoefficients(const char *file, int *size, double ***A);
void readConstants(const char *file, int *size, double **b, double **x);
double normTwo(double *xk, double *xk_prev, int n);

void showMatx(double **A, int n); 
void showVect(double *b, int n);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send coefficients and constants files\n");
        return EXIT_FAILURE;
    }
    int n;
    double **A, *x, *b;

    readCoefficients(argv[1], &n, &A);
    readConstants(argv[2], &n, &b, &x);

    gaussSeidel(A, b, x, n);

    printf("\nThe solution of the system is:\n");
    printf("--------------------------------\n");

    for(int i=0; i < n; i++){
        printf("x[%d] = %.4lf\n", i+1, x[i]);
    }

    printf("--------------------------------\n");

    for(int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(x);
    return EXIT_SUCCESS;
}

void gaussSeidel(double **A, double *b, double *x, int n){
    double *x_old, ea, sum, tol = 1e-4;
    int iter = 0, max = 100;

    x_old = (double *) calloc(n, sizeof(double));
    if(!x_old){
        perror("Fail malloc");
        exit(1);
    }

    printf("\n%s", "Iter\t");
    for(int i = 0; i < n; i++) printf("  x%d\t", i+1);
    printf("  Ea\n----------------------------------------\n");

    do {
        printf("%3d\t", iter + 1);
        for (int i = 0; i < n; i++) {
            x_old[i] = x[i];
        }

        for (int i = 0; i < n; i++) {
            sum = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sum -= A[i][j] * x[j];
                }
            }
            x[i] = sum / A[i][i]; 
            printf("%.4lf\t", x[i]);  
        }

        ea = normTwo(x, x_old, n); 
        printf("%.4lf\n", ea);  

        iter++;
    } while (ea > tol && iter < max);

    free(x_old);
}

double normTwo(double *xk, double *xk_prev, int n){
    double sum = 0.0;
    for(int i=0; i < n; i++){
        sum += pow(xk[i] - xk_prev[i], 2);
    }
    return sqrt(sum);
}

void readCoefficients(const char *file, int *size, double ***A){
    FILE *fl = fopen(file, "r");
    if(!fl){
        perror("Fail to open file\n");
        return;
    }

    fscanf(fl, "%d", size);
    //printf("Size is: %d\n", *size);

    *A = (double **) calloc(*size, sizeof(double*)); //coefficient matrix
    if(!*A){
        perror("Fail malloc");
        exit(1);
    }

    for(int i=0; i < *size; i++){
        (*A)[i] = (double*) calloc(*size, sizeof(double));
        if(!(*A)[i]){
            perror("Fail malloc");
            exit(1);
        }
    }

    for(int i=0; i < *size; i++){
        for(int j=0; j < *size; j++){
            fscanf(fl, "%lf", &(*A)[i][j]);
        }
    }

    fclose(fl);
}


void readConstants(const char *file, int *size, double **b,  double **x){
    FILE *fl = fopen(file, "r");
    if(!fl){
        perror("Fail to open file\n");
        return;
    }

    fscanf(fl, "%d", size);
    //printf("Amount of constants is: %d\n", *size);

    *b = (double *) calloc(*size, sizeof(double)); //constants vector
    if(!*b){
        perror("Fail malloc");
        exit(1);
    }

    *x = (double *) calloc(*size, sizeof(double)); //solution vector
    if(!*x){
        perror("Fail malloc");
        exit(1);
    }

    for(int i=0; i < *size; i++){
        fscanf(fl, "%lf", &(*b)[i]);
    }

    fclose(fl);
}

void showMatx(double **A, int n){
    for(int i=0; i < n; i++){
        for(int j=0; j < n; j++){
            printf("%f\t", A[i][j]);
        }
        printf("\n");
    }
}

void showVect(double *b, int n){
    for(int i=0; i < n; i++){
        printf("%.4lf ", b[i]);
    }
    printf("\n");
}