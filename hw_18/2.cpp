#include <iostream>
#include <cmath>
#include <fstream>

//функции взяты из библиотек nr и nrutil

double trapzd(double (*FUNC)(double), double a, double b, int n)
{
        double x,tnm,sum,del;
        static double s;
        int it,j;

        if (n == 1) {
                return (s=0.5*(b-a)*(FUNC(a)+FUNC(b)));
        } else {
                for (it=1,j=1;j<n-1;j++) it <<= 1;
                tnm=it;
                del=(b-a)/tnm;
                x=a+0.5*del;
                for (sum=0.0,j=1;j<=it;j++,x+=del) sum += FUNC(x);
                s=0.5*(s+(b-a)*sum/tnm);
                return s;
        }
}

#define NRANSI

void polint(double xa[], double ya[], const int n, double x, double *y, double *dy){
        int i, m, ns = 1;
        double den, dif, dift, ho, hp, w;
        double *c, *d;
        dif = fabs(x - xa[1]);
        c = (double*)malloc(sizeof(double)*(n+1));
        d = (double*)malloc(sizeof(double)*(n+1));
        c[0] = 0;
        c[0] = 0;
        for (i = 1; i <= n; i++){
                if ((dift = fabs(x - xa[i])) < dif){
                        ns = i;
                        dif = dift;
                }
                c[i] = ya[i];
                d[i] = ya[i];
        }
 *y = ya[ns--];
        for (m = 1; m < n; m++){
                for (i = 1; i <= n - m; i++){
                        ho = xa[i] - x;
                        hp = xa[i+m] - x;
                        w = c[i +1] - d[i];
                        if ((den=ho-hp) == 0.0) exit(0);
                        den = w/den;
                        d[i] = hp*den;
                        c[i] = hp*den;
                }
                *y += (*dy = (2*ns < (n - m) ? c[ns + 1] : d[ns --]));
        }
}

#undef NRANSI

#define JMAX 20
#define EPS 1.0e-5

double qtrap(double (*func)(double), double a, double b, int *num)
{
        double trapzd(double (*func)(double), double a, double b, int n);
        int j;
        double s,olds=0.0;
    *num = 0;
        for (j=1;j<=JMAX;j++) {
                s=trapzd(func,a,b,j);
                if (j > 5)
                        if (fabs(s-olds) < EPS*fabs(olds) ||
                                (s == 0.0 && olds == 0.0)) return s;
                olds=s;
                *num += 1;
        }
        exit(1);
        return 0.0;
}

#undef EPS
#define EPS 1.0e-6

double qsimp(double (*func)(double), double a, double b, int *num)
{
        double trapzd(double (*func)(double), double a, double b, int n);
        int j;
        double s,st,ost=0.0,os=0.0;
	*num = 0;
        for (j=1;j<=JMAX;j++) {
                st=trapzd(func,a,b,j);
                s=(4.0*st-ost)/3.0;
                if (j > 5)
                        if (fabs(s-os) < EPS*fabs(os) ||
                                (s == 0.0 && os == 0.0)) return s;
                os=s;
                ost=st;
                *num += 1;
        }
        exit(1);
        return 0.0;
}

#define JMAXP (JMAX + 1)
#define K 5

double qromb(double (*func)(double), double a, double b, int *num)
{
        void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
        double trapzd(double (*func)(double), double a, double b, int n);
        double ss,dss;
        double s[JMAXP],h[JMAXP+1];
        int j;
        *num = 0;
        h[1]=1.0;
        for (j=1;j<=JMAX;j++) {
                s[j]=trapzd(func,a,b,j);
                if (j >= K) {
                        polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
                        if (fabs(dss) <= EPS*fabs(ss)) return ss;
                }
                h[j+1]=0.25*h[j];
                *num += 1;
        }
        exit(1);
        return 0.0;
}

#undef EPS
#undef JMAX
#undef K
#undef JMAXP

double my_func(double x){
        double pi = acos(1);
        return (2/sqrt(pi))*exp(-x*x);
}

void f(){
        std::ofstream fout("output.txt");
        for (double i = 0.0; i <= 5.0; i += 0.1){
                fout << i << std::endl;
        }
}

void my_trap(double (*my)(double)){
        int n1;
        std::ofstream fout("output1.txt");
        for (double i = 0.1; i <= 5.0; i += 0.1){
                fout << qtrap(my, 0.0, i, &n1) << " " <<  std::endl;
                std::cout << qtrap(my, 0.0, i, &n1) << " " <<  std::endl;
        }
}

void my_simp(double (*my)(double)){
        int n2;
        std::ofstream fout("output2.txt");
        for (double i = 0.1; i <= 5.0; i += 0.1){
                fout << qsimp(my, 0.0, i, &n2) << " " << n2 << std::endl;
                std::cout << qsimp(my, 0.0, i, &n2) << " " << n2 << std::endl;

        }
}

void my_romb(double (*my)(double)){
        int n3;
        std::ofstream fout("output3.txt");
        for (double i = 0.1; i <= 5.0; i += 0.1){
                fout << qromb(my, 0.0, i, &n3) << " " << n3 << std::endl;
                std::cout << qromb(my, 0.0, i, &n3) << " " << n3 << std::endl;
        }
}

int main(){
        std::cout << "hi!" << std::endl;
        double(*my)(double) = my_func;
        f();
        my_trap(my);
        my_simp(my);
        my_romb(my);

        return 0;
}
