#include <stdio.h>
#include <stdlib.h>

void readValues(const char *file, double **vec, int *size);
void printPolynomial(double *coeffs, int degree);
void lagrangePolynomial(double *xVec, double *yVec, int n, double **coeffs);
void lagrangebasisPoly(int i, double *x_values, int n, double **poly);
void multiplyPolynomials(double *pol1, int deg1, double *pol2, int deg2, double **result);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send x file values and y file values\n");
        return EXIT_FAILURE;
    }
    double *xValues, *yValues, *coeffs;
    int n1, n2;

    readValues(argv[1], &xValues, &n1);
    readValues(argv[2], &yValues, &n2);

    if (n1 != n2) {
        perror("Error: X and Y values must have the same size.\n");
        free(xValues);
        free(yValues);
        return EXIT_FAILURE;
    }

    printf("Points loaded:\n");
    for (int i = 0; i < n1; i++){
        printf("(%lf, %lf) ", xValues[i], yValues[i]);
    }
    printf("\n\n");

    lagrangePolynomial(xValues, yValues, n1, &coeffs);

    printf("Interpolated polynomial: P(x) = ");
    printPolynomial(coeffs, n1 - 1);
    printf("\n");
    
    free(xValues);
    free(yValues);
    free(coeffs);
    return EXIT_SUCCESS;
}

void lagrangePolynomial(double *xVec, double *yVec, int n, double **coeffs){
    (*coeffs) = (double*)calloc(n, sizeof(double));
    double *L;

    if(!(*coeffs)){
        perror("Fail Malloc\n");
        exit(1);
    }

    #ifdef DEBUG
        printf("\n----------------------------Building Lagrange Polynomial----------------------------\n");  
    #endif

    for(int i=0; i < n; i++){
        #ifdef DEBUG
            printf("==================== Processing Basis Polynomial L_%d ====================\n", i);
        #endif
        
        lagrangebasisPoly(i, xVec, n, &L);

        #ifdef DEBUG
            printf("L_%d(x) coefficients: ", i);
            printPolynomial(L, n-1);
            printf("\n\n");
        #endif

        for(int j=0; j < n; j++){            
            (*coeffs)[j] += yVec[i] * L[j];
        }
        
        #ifdef DEBUG
            printf("Saving values in P(x) = ");
            printPolynomial(*coeffs, n-1);
            printf("\n\n");
        #endif
        free(L);
    }
}

void lagrangebasisPoly(int i, double *x_values, int n, double **poly){
    (*poly) = (double*)calloc(n, sizeof(double));
    double denom, factor[2], *temp;

    if(!(*poly)){
        perror("Memory allocation failed\n");
        exit(1);
    }

    (*poly)[0] = 1.0;
    #ifdef DEBUG
        printf("\nComputing L_%d(x):\n\n", i);
    #endif
    
    for(int j=0; j < n; j++){
        if(j != i){
            factor[0] = -x_values[j];
            factor[1] = 1.0;
            denom = x_values[i] - x_values[j];

            #ifdef DEBUG
                printf("Multiplying L_%d(x) by: (x - %lf) / %lf\n", i, x_values[j], denom);
                printf("Current polynomial before multiplication: ");
                printPolynomial(*poly, n-1);
            #endif

            multiplyPolynomials(*poly, j, factor, 1, &temp); 

            #ifdef DEBUG
                printf("After multiplication (before division by %lf): ", denom);
                printPolynomial(temp, n-1);
            #endif

            for(int k=0; k < n; k++){
                (*poly)[k] = temp[k] / denom;
            }

            #ifdef DEBUG
                printf("\nAfter division: ");
                printPolynomial(*poly, n-1);
                printf("\n\n");
            #endif

            free(temp);
        }        
    }
}

void multiplyPolynomials(double *pol1, int deg1, double *pol2, int deg2, double **result){
    int newDeg = deg1 + deg2;
    *result = (double*)calloc(newDeg+1, sizeof(double));
    if (!(*result)) {
        perror("Memory allocation failed\n");
        exit(1);
    }

    #ifdef DEBUG
        printf("\nMultiplying polynomials: (");
        printPolynomial(pol1, deg1);
        printf(") * (");
        printPolynomial(pol2, deg2);
        printf(")\n");
    #endif

    for(int i=0; i <= deg1; i++){
        for(int j=0; j <= deg2; j++){
            (*result)[i + j] += pol1[i] * pol2[j];
        }
    }

    #ifdef DEBUG
        printf("Result Multiplication: ");
        printPolynomial(*result, newDeg);
        printf("\n\n");      
    #endif
}

void printPolynomial(double *coeffs, int degree){
    for (int i = degree; i >= 0; i--) {
        if (coeffs[i] != 0.0) {
            printf("%.5lf", coeffs[i]);
            if (i > 0) printf("x^%d ", i);
            if (i > 0 && coeffs[i - 1] >= 0) printf("+ ");
        }
    }
}

void readValues(const char *file, double **vec, int *size){
    FILE *fl = fopen(file, "r");
    if(!fl){
        perror("Fail to open file\n");
        return;
    }
    fscanf(fl, "%d", size);

    (*vec) = (double*) calloc(*size, sizeof(double));
    if(!(*vec)){
        perror("Fail Malloc\n");
        exit(1);
    }

    for(int i=0; i < *size; i++){
        fscanf(fl, "%lf", &(*vec)[i]);
    }

    fclose(fl);
}