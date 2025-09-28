#include <stdlib.h>
#include <stdio.h>

void readValues(const char *file, double **vec, int *size);
double **dividedDiff(double *x, double *y, int n);
void newtonPoly(double **table, double *x, int n, double **poly);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send the file\n");
        return EXIT_FAILURE;
    }
    double *x_values, *y_values, **table, *poly;
    int n1, n2;
 
    readValues(argv[1], &x_values, &n1);
    readValues(argv[2], &y_values, &n2);

    if(n1 != n2){
        perror("Error: X and Y values must have the same size.\n");
        free(x_values);
        free(y_values);
        return EXIT_FAILURE;
    }

    table = dividedDiff(x_values, y_values, n1); //n1 must be equal to n2

    printf("Divided Differences\n");
    for(int i=0; i < n1; i++){
        for(int j=0; j < n1 - i; j++){
            printf("%.4f\t", table[j][i]);
        }
        printf("\n");
    }

    printf("Newton Interpolating Polynomial:\n");
    
    newtonPoly(table, x_values, n1, &poly);

    for(int i=0; i < n1; i++){
        if(i>0) printf(" + ");
        printf("(%f)x^%d", poly[i], i);
    }
    printf("\n");

    for(int i=0; i < n1; i++) free(table[i]);
    free(table);
    free(poly);
    free(x_values);
    free(y_values);
    return EXIT_SUCCESS;
}

void newtonPoly(double **table, double *x, int n, double **poly){
    double *temp = (double*) calloc(n, sizeof(double));
    double *prod = (double*) calloc(n, sizeof(double));
    *poly = (double*) calloc(n, sizeof(double));

    for(int k=0; k < n; k++){
        for(int i=0; i < n; i++) prod[i] = 0;
        prod[0] = 1.0;

        
        for(int j=0; j < k; j++){
            for(int i=n-1; i >= 0; i--) temp[i] = 0;  
            for(int i=0; i < n-1; i++){
                if(prod[i] != 0){
                    temp[i] += -x[j]*prod[i]; //const
                    temp[i+1] += prod[i]; //x
                }
            }
            for(int i=0; i < n; i++) prod[i] = temp[i];
        }

        for(int i=0; i < n; i++){
            (*poly)[i] += table[0][k] * prod[i];
        }
    }

    free(temp);
    free(prod);
}

double **dividedDiff(double *x, double *y, int n){
    double **table;
    table = (double**) malloc(n * sizeof(double*));
    if(!table){
        perror("Fail malloc\n");
        exit(1);
    }

    for(int i=0; i < n; i++){
        table[i] = (double*) malloc(n * sizeof(double));
        if(!table[i]){
            perror("Fail malloc\n");
            exit(1);
        }
    }

    for(int i=0; i < n; i++){
        table[i][0] = y[i]; //the first column are the y values
    }

    for(int i=1; i < n; i++){
        for(int j=0; j < n - i; j++){
            table[j][i] = (table[j+1][i-1] - table[j][i-1]) / (x[j+i] - x[j]);  
        }
    }
    
    return table;
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