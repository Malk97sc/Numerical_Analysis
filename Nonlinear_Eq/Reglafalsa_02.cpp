#include "stdio.h"
#include "conio.h"
#include "math.h"
main()
{
 float a,b,c,fa,fc,fb,antc,tol,EA,fmat(float);
 int i,k;
 printf("Ingrese el limite inferior del intervalo: ");
 scanf("%f",&a);
 printf("Ingrese el limite superior del intervalo: ");
 scanf("%f",&b);
 printf("Ingrese el numero de iteraciones: ");
 scanf("%d",&i);
 printf("Ingrese la tolerancia: ");
 scanf("%f",&tol);
 printf("\n%2s %4s %8s %8s %9s %9s %8s %6s\n","k","a","c","b","fa","fc","fb","EA");
 for(k=0;k<=i-1;k++)
 {
  fa=fmat(a);
  fb=fmat(b);
  c=(a*fb-b*fa)/(fb-fa);
  fc=fmat(c);
  if(k==0)
  {
   printf("%2d %.5f  %.5f  %.5f  %.5f  %.5f  %.5f\n",k,a,c,b,fa,fc,fb);
   antc=c;
  }
  else
  {
   EA=fabs(c-antc);
   printf("%2d %.5f  %.5f  %.5f  %.5f  %.5f  %.5f  %.5f\n",k,a,c,b,fa,fc,fb,EA);
   antc=c;
   if(EA<=tol)
   k=i+1;
  }
  if(fa*fc<0)
  b=c;
  else
  {
   if(fc*fb<0)
   a=c;
   else
   k=i+1;
  }
 }
 getch();
 }
float fmat(float x)
{
 float y;
 y=pow(x,4)-2*pow(x,3)-4*pow(x,2)+4*x+4;//La función a iterar
 return y;
}




