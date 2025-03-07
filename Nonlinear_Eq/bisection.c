#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int BARSIZE = 169;

double f(double x);
void showBar(int size); //show table
void bisection(double lowerLim, double upperLim, int n);

int main(int argc, char **argv){
    if(argc < 4){
        perror("Send lowe Limit, Upper Limit and N iters\n");
        return EXIT_FAILURE;
    }

    bisection(atof(argv[1]), atof(argv[2]), atoi(argv[3])); //double, double and integer

    return 0;
}

void bisection(double lowerLim, double upperLim, int n){
    double a, c, b, fa, fc, fb, cPrev, errorA = 0, tol = 0;

    a = lowerLim;
    b = upperLim;

    printf("| Iteration\t| Lower Limit\t\t| Midpoint\t\t\t| Upper Limit\t\t| f(a)\t\t| f(c)\t\t| f(b)\t\t| Error\t\t\t|\n");
    showBar(BARSIZE);

    for(int i=0; i < n; i++){
        c = (a+b) / 2.0;
        fa = f(a); //lower limit
        fb = f(b); //upper limit
        fc = f(c); //midpoint

        if(i == 0){
            printf("| %2d\t\t| %.10f\t\t| %.16f\t\t| %.10f\t\t| %.10f\t| %.10f\t| %.10f\t| N/A\t\t\t|\n", i, a, c, b, fa, fc, fb);
            cPrev = c;
        }else{
            errorA = fabs(c - cPrev);
            printf("| %2d\t\t| %.10f\t\t| %.16f\t\t| %.10f\t\t| %.10f\t| %.10f\t| %.10f\t| %.16f\t|\n", i, a, c, b, fa, fc, fb, errorA);
            cPrev = c;

            if(errorA <= tol){
                printf("Converged after %d iterations.\n", i);
                break;
            }            
        }

        if(fa * fc < 0){ //limits
            b = c;
        }else if(fb * fc < 0){
            a = c;
        }else{
            printf("Converged\n");
            break;
        }
    }
    showBar(BARSIZE);
}

double f(double x){
    return pow(x,4) - 2*pow(x,3) - 4*pow(x,2) + 4*x + 4; //f(x) = x^4 - 2x^3 - 4x^2 + 4x + 4
}

void showBar(int size){
    for (int i = 0; i < size; i++) {
        printf("-");
    }
    printf("\n");
}