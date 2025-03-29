#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

void executeGaussSeidel(const char *program, char *args[]); 

void saveVector(const char *fileName, double *v, int n);
void saveMatrix(const char *fileName, double **A, int n);
void saveValues(int size, double **b, double ***A, double **x, double **fx);

void initCoefficients(int *size, double ***A);
void initConstants(int *size, double **b,  double **x);
void swapRows(double **A, double *b, int row1, int row2, int n);
int makeDiagonallyDominant(double **A, double *b, int n);

void readFile(const char *file, int *size, double **vec);
void showVect(double *b, int n);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send x values and f(x) values\n");
        return EXIT_FAILURE;
    }
    int n;
    double **A, *b, *x0, *x, *fx;
    char *program = "../../EqSystemsMethods/Gauss-Seidel/gauss-seidel";

    readFile(argv[1], &n, &x);
    readFile(argv[2], &n, &fx);

    initCoefficients(&n, &A);
    initConstants(&n, &b, &x0);
    saveValues(n, &b, &A, &x, &fx);

    if (!makeDiagonallyDominant(A, b, n)) {
        fprintf(stderr, "Warning: Matrix is not diagonally dominant. Gauss-Seidel may not converge.\n");
    }

    saveMatrix("coeffMatrix.dat", A, n);
    saveVector("constVects.dat", b, n);
    saveVector("initialVector.dat", x0, n);    

    char *args[] = {"gaussSeidel", "coeffMatrix.dat", "constVects.dat", "initialVector.dat", "1", NULL};

    for(int i=0; i < n; i++){   
        free(A[i]);
    }
    free(A);
    free(b);
    free(x0);
    free(x);
    free(fx);

    executeGaussSeidel(program, args);

    return EXIT_SUCCESS;
}

void executeGaussSeidel(const char *program, char *args[]){
    pid_t pid = fork();

    if(!pid){
        execvp(program, args);
        perror(program);
        exit(EXIT_FAILURE);
    }else{
        int status;
        waitpid(pid, &status, 0); //wait child

        if (WIFEXITED(status)) {
            printf("Gauss-Seidel finished: %d\n", WEXITSTATUS(status));
        } else {
            printf("Gauss-Seidel did not terminate correctly\n");
        }
    }
}

void swapRows(double **A, double *b, int row1, int row2, int n) {
    double *tempRow = A[row1];
    A[row1] = A[row2];
    A[row2] = tempRow;
    
    double tempB = b[row1];
    b[row1] = b[row2];
    b[row2] = tempB;
}

int makeDiagonallyDominant(double **A, double *b, int n) {
    for(int i = 0; i < n; i++){
        double maxVal = fabs(A[i][i]);
        int maxRow = i;

        for(int j = i + 1; j < n; j++){
            if(fabs(A[j][i]) > maxVal){
                maxVal = fabs(A[j][i]);
                maxRow = j;
            }
        }
        
        if(maxRow != i){
            swapRows(A, b, i, maxRow, n);
        }
        
        double sum = 0;
        for(int j=0; j < n; j++){
            if (j != i) sum += fabs(A[i][j]);
        }
        
        if(fabs(A[i][i]) <= sum) return 0;
    }
    return 1;
}

void saveMatrix(const char *fileName, double **A, int n){
    FILE *file = fopen(fileName, "w");
    if (!file) { perror("Fail fopen\n"); exit(1); }

    fprintf(file, "%d\n", n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.4f ", A[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void saveVector(const char *fileName, double *v, int n){
    FILE *file = fopen(fileName, "w");
    if (!file) { perror("Fail fopen"); exit(1); }

    fprintf(file, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(file, "%.4f\n", v[i]);
    }
    fclose(file);
}

void saveValues(int size, double **b, double ***A, double **x, double **fx){
    for(int i=0; i < size; i++){
        for(int j = 0; j < size; j++){
            (*A)[i][j] = pow((*x)[i], (size - 1 - j)); //Vandermonde Matrix
        }
        (*b)[i] = (*fx)[i]; //constants vect
    }
}

void readFile(const char *file, int *size, double **vec){
    FILE *fl;
    fl = fopen(file, "r");
    if(!fl){
        perror("Fail to load file\n");
        exit(1);
    }

    fscanf(fl, "%d", size);
    //printf("Amount of numbers: %d\n", *size);

    *vec = (double *)malloc(*size * sizeof(double));
    if(!(*vec)){
        perror("Fail Malloc\n");
        exit(1);
    }

    for(int i=0; i < *size; i++){
        fscanf(fl, "%lf", &(*vec)[i]);
    }

    fclose(fl);
}

void initConstants(int *size, double **b,  double **x){
    *b = (double *) calloc(*size, sizeof(double)); //constants vector
    if(!*b){
        perror("Fail Malloc\n");
        exit(1);
    }

    *x = (double *) calloc(*size, sizeof(double)); //solution vector
    if(!*x){
        perror("Fail Malloc\n");
        exit(1);
    }
}

void initCoefficients(int *size, double ***A){
    *A = (double **) calloc(*size, sizeof(double*)); //coefficient matrix
    if(!*A){
        perror("Fail Malloc\n");
        exit(1);
    }

    for(int i=0; i < *size; i++){
        (*A)[i] = (double*) calloc(*size, sizeof(double));
        if(!(*A)[i]){
            perror("Fail Malloc\n");
            exit(1);
        }
    }
}

void showVect(double *b, int n){
    for(int i=0; i < n; i++){
        printf("%.4lf ", b[i]);
    }
    printf("\n");
}