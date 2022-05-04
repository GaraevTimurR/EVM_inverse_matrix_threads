#include"Func.h"

struct MyPer{
        double* Matrix;
        double* InvMatrix;
        double eps;
        int n;
	int number;	
	
	int p;
	//int *maxJ;
	double* Max;
	pthread_mutex_t* mutex;
	//pthread_mutex_t* s_mutex; 
	//pthread_cond_t* condvar_in; 
	//pthread_cond_t* condvar_out;
	//int* s_In;
	//int* s_Out;
	int* ans;
	long double* time;
	int* swap;
};
