#include "Func.h"

double ErNorm(double* Matrix, double* InvMatrix, int n, int p, int number,pthread_mutex_t* mutex, long double* time)
{
	*time=get_time();
        double shet, s=0, Norm=0;
        int i, j,  k;
        for(i=number; i<n; i+=p){
		shet=0;
                for(j=0; j<n; j++){
                        s=0;
                        if(i==j){
				for(k=0; k<n; k++){
                                	s+=Matrix[i*n+k]*InvMatrix[k*n+j];
                        	}
                                s=s-1;
                        }else{
				for(k=0; k<n; k++){
                                	s+=Matrix[i*n+k]*InvMatrix[k*n+j];
                        	}
			}
			shet+=abs(s);
                }
		pthread_mutex_lock(mutex);
		Norm+=shet;
		pthread_mutex_unlock(mutex);
		//synchronize(p);
        }
	*time=get_time()-*time;
        return Norm;
}
