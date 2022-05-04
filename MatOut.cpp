#include "Func.h"
void POut(double *Matrix,int l,int n,int m)
{
        int c,d;
        double *f;
        c = l+m-(abs(l-m)+l+m)/2;
        d = n+m-(abs(n-m)+n+m)/2;

        double eps = 1.0;
        while (1.0 + eps/ 2.0 > 1.0)
        {
                eps /= 2.0;
        }
        eps=eps*1000;

        for(int i = 0;i<c;i++)
        {
                f = Matrix+i*n;
                for(int j = 0;j<d;j++){
                        if(fabs(f[j])<eps){f[j]=0;}
                        printf("%e ",f[j]);
                }
                printf("\n");
   }
}
