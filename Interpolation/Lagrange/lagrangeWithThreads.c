#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

pthread_mutex_t mutex;

typedef struct{
    int i;
    int end;
    int numElements;
} ThreadData;

double *xValues, *yValues, *coeffs;

void readValues(const char *file, double **vec, int *size);
void createThreads(int numThreads, int numElements);
void *calculateLagrange(void *arg);
void printPolynomial(double *coeffs, int degree);
void lagrangebasisPoly(int i, double *x_values, int n, double **poly);
void multiplyPolynomials(double *pol1, int deg1, double *pol2, int deg2, double **result);

int main(int argc, char **argv){
    if(argc < 4){
        perror("Send the number of threads and the files\n");
        return EXIT_FAILURE;
    }

    int numsX, numsY, numThreads = atoi(argv[1]);

    readValues(argv[2], &xValues, &numsX);
    readValues(argv[3], &yValues, &numsY);

    if (numsX != numsY) {
        perror("Error: X and Y values must have the same size.\n");
        free(xValues);
        free(yValues);
        return EXIT_FAILURE;
    }

    printf("Points loaded:\n");
    for (int i = 0; i < numsX; i++){
        printf("(%lf, %lf) ", xValues[i], yValues[i]);
    }
    printf("\n\n");

    coeffs = (double*) calloc(numsX, sizeof(double));
    if(!coeffs){
        perror("Fail Malloc in coeffs\n");
        free(xValues);
        free(yValues);
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&mutex, NULL);
    createThreads(numThreads, numsX); //cause numsX and numsY are the same size
    pthread_mutex_destroy(&mutex);

    printf("Interpolated polynomial: P(x) = ");
    printPolynomial(coeffs, numsX-1);

    free(coeffs);
    free(xValues);
    free(yValues);
    return EXIT_SUCCESS;
}

void createThreads(int numThreads, int numElements){
    int chunk = numElements / numThreads;
    pthread_t threads[numThreads];
    ThreadData data[numThreads];

    for(int i=0; i < numThreads; i++){
        data[i].i = chunk * i;
        data[i].end = (i == numThreads - 1)? numElements: chunk * (i+1);
        data[i].numElements = numElements;
        pthread_create(&threads[i], NULL, calculateLagrange, (void*)&data[i]);
    }

    for(int i=0; i < numThreads; i++){
        pthread_join(threads[i], NULL);
    }
}

void *calculateLagrange(void *arg){
    ThreadData *data = (ThreadData *)arg;
    double *L, *coefftemp;

    printf("Thread %ld processing L_i(x) in range [%d, %d]\n", 
           pthread_self(),
           data->i, 
           data->end - 1);

    for(int i=data->i; i < data->end; i++){
        lagrangebasisPoly(i, xValues, data->numElements, &L);

        coefftemp = (double*)calloc(data->numElements, sizeof(double));
        if(!coefftemp){perror("Fail Malloc\n"); free(L); pthread_exit(NULL); }
        for(int j=0; j < data->numElements; j++){
            coefftemp[j] += yValues[i] * L[j];
        }

        pthread_mutex_lock(&mutex);
        for(int j=0; j < data->numElements; j++){
            coeffs[j] += coefftemp[j]; //minimize mutex time while updating coeffs using coefftemp
        }
        pthread_mutex_unlock(&mutex);
        
        free(L);
        free(coefftemp);
    }
    pthread_exit(NULL);
}

void lagrangebasisPoly(int i, double *x_values, int n, double **poly){
    (*poly) = (double*)calloc(n, sizeof(double));
    double denom, factor[2], *temp;

    if(!(*poly)){
        perror("Fail Malloc\n");
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
        perror("Fail Malloc\n");
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

void printPolynomial(double *coeffs, int degree) {
    int firstTerm = 1;

    printf("P(x) = ");
    for(int i=degree; i >= 0; i--){
        if(coeffs[i] != 0.0){
            if(!firstTerm){
                printf(" %c ", (coeffs[i] < 0) ? '-' : '+');
            }else if(coeffs[i] < 0){
                printf("-");
            }

            printf("%.5lf", fabs(coeffs[i]));

            if(i > 0){
                printf("x");
                if(i > 1){
                    printf("^%d", i);
                }
            }

            firstTerm = 0;
        }
    }

    if (firstTerm) {
        printf("0.00000");
    }

    printf("\n");
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