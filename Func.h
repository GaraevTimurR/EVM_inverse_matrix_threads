#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED


#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "Struct.h"
double ErNorm(double* Matrix, double* InvMatrix, int n, int p, int number,pthread_mutex_t* mutex, long double* time);
//int Gauss(double* Matrix, double* InvMatrix, int n, int p);
int Gauss1(double* Matrix, double* InvMatrix, int n, double eps, int p, int number, int* swap, double* Max, pthread_mutex_t* mutex, int* ans, long double* time);
		//, int* maxJ, pthread_mutex_t* mutex, double* Max, int number, int* swap
		//, pthread_mutex_t* s_mutex, pthread_cond_t* condvar_in, pthread_cond_t* condvar_out,int* s_In ,int* s_Out
		
int Input(double *A,char *file_name, int n, double* Norm);
long double get_time();
double generate(int k, int n, int i, int j);
void synchronize(int p);
void POut(double *Matrix,int l,int n,int m);
#endif // FUNC_H_INCLUDED
