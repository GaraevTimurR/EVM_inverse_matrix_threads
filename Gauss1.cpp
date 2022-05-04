#include "Func.h"

int Gauss1(double* Matrix, double* InvMatrix, int n, double eps,  int p, int number, int* swap, double* Max, pthread_mutex_t* mutex, int* ans, long double* time){
        double prob;
	synchronize(p);
	*time=get_time();
	//double start = clock();
        double Del, BigDel;
	double max;
	int maxJ;
        int n1,  n2, k;
	//int N1, N2;
	//N1=number*(n/p);
        //if(number==p-1){N2=n;}else{N2=(number+1)*(n/p);}
        double* MultInd;
	double* MultInd2;
	
	if(number ==0){	
		for(int i=0; i<n; i++){swap[i]=i;}
	}
        for(int i=0; i<n; i++){
	        // поиск максимального по строчке

		n1=number*((n-i)/p);
		if(number==p-1){n2=n-i;}else{n2=(number+1)*((n-i)/p);}
                MultInd= Matrix+i*n;
		max=MultInd[i];
		maxJ=i;
		//if(i>n1){j=i;}else{j=n1;}
		
		if(number==0){
                	for(int j=i; j<n; j++){
                        	if(fabs(max)<fabs(MultInd[j])){
                                	max=MultInd[j];
                                	maxJ=j;
                        	}
			
        	        }
		
		
                	if(fabs(max)<eps){
				*ans=-2;
				synchronize(p);
                        	return -2;
                	}
	             	k=swap[i];
                        swap[i]=swap[maxJ];
                        swap[maxJ]=k;
		
			for(k=0; k<n; k++){
	                        prob=Matrix[k*n+i];
	                        Matrix[k*n+i]=Matrix[k*n+maxJ];
	                        Matrix[k*n+maxJ]=prob;
			}
		}
                // поиск максимального по строчке
                //меняем столбцы	
		synchronize(p);
			if(*ans==-2){return -2;}
		synchronize(p);
                //меняем столбцы
		BigDel=Matrix[n*i+i];

		
                // диагонализируем
		
		//printf("n1 %d, n2 %d, i %d \n", n1, n2, i);
		//for(int k=0; k<n*n; k++){Del = Matrix[n*k+i]/BigDel; synchronize(p);}

                for(int k= number; k<n; k+=p){
                        if(k!=i){
                                Del = Matrix[n*k+i]/BigDel;
                                for(int t=0; t<n; t++){
                                        Matrix[k*n+t]=Matrix[k*n+t]-Del*Matrix[i*n+t];
        	                        InvMatrix[k*n+t]=InvMatrix[k*n+t]-Del*InvMatrix[i*n+t];
	                        }
                        }
                }
                // диагонализируе
		synchronize(p);
        }
	
	synchronize(p);
	*Max=0;
	synchronize(p);
	double* MultInd3=Matrix+(n-1)*n;

	for(k=n-1; k>-1; k--){
                MultInd2=InvMatrix+k*n;
                MultInd=Matrix+k*n;
		n1=(number*(n-k-1))/p;

		BigDel=Matrix[k*n+k];
		synchronize(p);
		
		if(number==p-1){n2=n-k-1;}else{n2=((number+1)*(n-k-1))/p;}
		for(int i=0; i<n; i++){
			MultInd3[i]=MultInd2[i];
		}
		for(int i=number; i<n; i+=p){
			Del=0;
			for(int j=k+1; j<n; j++){
				Del+=MultInd[j]*InvMatrix[j*n+i];
			}
			MultInd3[i]=MultInd3[i]-Del;
			
		}
		synchronize(p);
		if(number==0){
			for(int i=0; i<n; i++){
	                	MultInd2[i]=(MultInd3[i])/BigDel;
			}
                }

        }

	synchronize(p);
	if(number==0){
		for(int i=0; i<n; i++){
			while(swap[i]!=i){
				int j=swap[i];
				MultInd=InvMatrix+j*n;
				MultInd2=InvMatrix+i*n;
				for(int t=0; t<n; t++){
                                        Del=MultInd[t];
                                        MultInd[t]=MultInd2[t];
                                        MultInd2[t]=Del;
                                }
				swap[i]=swap[j];
				swap[j]=j;
			}
		}
	}
	synchronize(p);
	*time=get_time()-*time;

        //printf("time; %Le \n", *time);
        return 0;
}

