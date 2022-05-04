#include "Func.h"
int Input (double *Matrix,char *file_name,int n, double* Norm)
{
        FILE *In;
	*Norm=0;
	double NormShet;
	double* MultInd;
	
	double eps = 1.0;

        while (1.0 + eps / 2.0 > 1.0)
        {
                eps /= 2.0;
        }

        In = fopen (file_name,"r");
        if (In==NULL)
        {
                fclose(In);
                return -1;
        }
	*Norm=0;
        MultInd=Matrix;
        for(int j=0; j<n; j++){
        	if(fscanf(In,"%lf",&MultInd[j])!=1)
                {
                	fclose(In);
                        return -2;
                }
                *Norm+=fabs(MultInd[j]);
        }
        

        for (int i = 1; i<n ;i++)
        {
		NormShet=0;
		MultInd=Matrix+i*n;
		for(int j=0; j<n; j++){
                	if(fscanf(In,"%lf",&MultInd[j])!=1)
                	{
                        	fclose(In);
                        	return -2;
                	}
			NormShet+=abs(MultInd[j]);
		}
		if(NormShet-*Norm>eps){*Norm=NormShet;}
        }

        fclose(In);
        return 0;
}  // чтение файла

double generate (int k, int n, int i, int j)
{

                if(k==1){
                	return (double)(n+1+(-1*abs(i-j)-abs(i+1)-abs(j+1))/2);
                }
                if(k==2){
			if(i>j){
				return i*1.0;
			}else{
				return j*1.0;
                        }
                }
                if(k==3){
                	return abs(i-j)*1.0;
                }
                if(k==4){
                        return (double)(1.0/(i+j+1));
                }
		return 0;
}
