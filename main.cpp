#include "Func.h"

void* ErrorNorm(void* arg);
void* ErrorNorm(void* arg){
	MyPer *Struct=(MyPer*)arg;
	Struct->Matrix[0]=ErNorm(Struct->Matrix, Struct->InvMatrix, Struct->n, Struct->p, Struct->number, Struct->mutex, Struct->time);
	return NULL;
}

void* Start(void* arg);
void* Start(void* arg){
	MyPer *Struct=(MyPer*)arg;
	//printf("%d\n", Struct->n);
	//int n=Struct->n;
	//Gauss1(double* Matrix, double* InvMatrix, int n, double eps, int p, int number, int* swap, double* Max, pthread_mutex_t* mutex, int* ans, long double* time);
	*(Struct->ans)=Gauss1(Struct->Matrix, Struct->InvMatrix, Struct->n, Struct->eps, Struct->p, Struct->number, Struct->swap, Struct->Max, Struct->mutex, Struct->ans, Struct->time);
	return NULL;
}
long double get_time()
{
	    struct timeval t;
	        gettimeofday(&t, 0);
		    return t.tv_sec + t.tv_usec/1000000.0;
}
double mach_eps(void);
double mach_eps(void)
{
	double eps = 1.0;

	while (1.0 + eps / 2.0 > 1.0)
	{
		eps /= 2.0;
	}
	return eps;
} // машинное эпсилон

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]), p= atoi(argv[2]), m = atoi(argv[3]), prob, ans, k=atoi(argv[4]);
	double* MultInd;
	double ForNorm, Norm, eps=mach_eps();
	long double t;
	MyPer* Struct=(MyPer*)malloc(p * (sizeof(MyPer)));
	pthread_t* tid=(pthread_t*)malloc(p * (sizeof(pthread_t)));
	pthread_mutex_t* mutex=new(pthread_mutex_t);
	double* Max=new(double);

	if(argc<5){
		printf("Can not initilaze mutex\n");
		free(tid);
		free(Struct);
		delete(mutex);
		delete(Max);
	}
	if(p<0){
		printf("Incorrect input\n");
                free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
	        return -1;
	}
	if (pthread_mutex_init(mutex, 0)){
		printf("Can not initilaze mutex\n");
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		return -1;
	}


	if (n == 0 || n < m) {
		printf("Incorrect input\n");
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		return -1;
	}

	if ((k < 0) || (k > 4)) {
		printf("Incorrect input\n");
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		return -1;
	}

	double* Matrix = (double*)malloc(n * n * (sizeof(double)));
	if (tid==NULL){
		printf("No memory allocated\n");
                free(Matrix);
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
                return -1;
	}

	if (Matrix == NULL)
	{
		printf("No memory allocated\n");
		free(Matrix);
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		return -1;
	}
	if ((argc == 6) && (k == 0)) {
		
		prob = Input(Matrix, argv[5], n, &Norm);
		if (prob == -1) {
			printf("File not found\n");
			free(Matrix);
			free(tid);
	                free(Struct);
	                delete(mutex);
	                delete(Max);
			return -1;
		}
		if (prob == -2)
		{
			printf("Insufficient data \n");
			free(Matrix);
			free(tid);
	                free(Struct);
	                delete(mutex);
	                delete(Max);
			return -1;
		}
	}
	if((argc!=6)&&(k==0))
        {
                printf("File don't exist\n");
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		free(Matrix);
                return -1;
        }
        if(k>0){
                for(int i=0; i<n ; i++){
                        ForNorm=0;
                        for(int j=0; j<n; j++){
                                Matrix[i*n+j]=generate(k, n, i, j);
                                ForNorm+=fabs(generate(k, n, i, j));
                        }
                        if(ForNorm>Norm){Norm=ForNorm;}
                }
        }

	//      }
	//}

	double* InvMatrix = (double*)malloc(n * n * (sizeof(double)));;

	if (InvMatrix == NULL)
	{
		printf("No memory allocated\n");
		free(Matrix);
		free(InvMatrix);
		free(tid);
                free(Struct);
                delete(mutex);
                delete(Max);
		return -1;
	}
	for(int i=0; i< n*n; i++){InvMatrix[i] = 0;}
	for (prob = 0; prob < n; prob++) { InvMatrix[prob * n + prob] = 1; }
	//инициализация и заполнение

	//решение
	printf("Matrix : \n");
	POut(Matrix, n, n, m);
	printf("\n");
	Norm = 1 / Norm;
	printf("Norm\n");
	for (int i = 0; i < n; i++)
	{
		MultInd = Matrix + i * n;
		for (int j = 0; j < n; j++)
		{
			MultInd[j] = MultInd[j] * Norm;
		}
	}
	
	int* swap=(int*)malloc(n *  (sizeof(int)));
	Struct[0].Max=Max;
	Struct[0].mutex=mutex;
	ans=0;
	for (int i=0; i<p; i++){
		Struct[i].eps=eps;
        	Struct[i].Matrix=Matrix;
        	Struct[i].InvMatrix=InvMatrix;
        	Struct[i].n=n;
        	Struct[i].ans=&ans;
        	Struct[i].p=p;
		Struct[i].number=i;
		Struct[i].swap=swap;
        	Struct[i].Max=Struct[0].Max;
		Struct[i].mutex=Struct[0].mutex;
		Struct[i].time=new(long double);
		Struct[i].ans=&ans;
	}

	if(Norm<eps*eps){
	        printf("Degenerate matrix\n");
                delete(Struct[0].Max);
                delete(Struct[0].mutex);
                for(int j=0; j<p; j++){delete(Struct[j].time);}
                free(Struct);
                free(Matrix);
                free(swap);
                free(InvMatrix);
                free(tid);
                return -1;
        }
	
	for(int i=0; i<p; i++){
		if(pthread_create(&tid[i], NULL, Start, &Struct[i])==1){
			printf("не создано\n");
			delete(Struct[0].Max);
	                delete(Struct[0].mutex);
	                for(int j=0; j<p; j++){delete(Struct[j].time);}
	                free(Struct);
	                free(Matrix);
	                free(swap);
	                free(InvMatrix);
	                free(tid);
			return -1;
		}
	}
	printf("\n");
	for(int i=0; i<p; i++){
		pthread_join(tid[i],NULL);
		if(i==0){t=*Struct[i].time;}
		if(fabs(t-*Struct[i].time)>(long double)(eps)){t=*Struct[i].time;}
	}

	if (ans == -2) {
		printf("Degenerate matrix");
	        delete(Struct[0].Max);
	        delete(Struct[0].mutex);
	        for(int i=0; i<p; i++){delete(Struct[i].time);}
	        free(Struct);
	        free(Matrix);
	        free(swap);
	        free(InvMatrix);
	        free(tid);
		return -1;
	}
	
	printf("time; %Le \n", t);
	for (int i = 0; i < n; i++)
	{
		MultInd = InvMatrix + i * n;
		for (int j = 0; j < n; j++)
		{
			MultInd[j] = MultInd[j] * Norm;
		}
	}
	printf("Inverse Matrix: \n");
	POut(InvMatrix, m, n, m);

	if ((argc == 6) && (k == 0)) {
		prob = Input(Matrix, argv[5], n, &Norm);
		if (prob == -1) {
			printf("File not found\n");
		        delete(Struct[0].Max);
	                delete(Struct[0].mutex);
	                for(int i=0; i<p; i++){delete(Struct[i].time);}
			free(Struct);
		        free(Matrix);
		        free(swap);
		        free(InvMatrix);
		        free(tid);
			return -1;
		}
		if (prob == -2)
		{
			printf("Insufficient data \n");
	                delete(Struct[0].Max);
	                delete(Struct[0].mutex);
	                for(int i=0; i<p; i++){delete(Struct[i].time);}
		        free(Struct);
		        free(Matrix);
		        free(swap);
		        free(InvMatrix);
		        free(tid);
			return -1;
		}
	}
	
	if(k>0){
                for(int i=0; i<n ; i++){
                        ForNorm=0;
                        for(int j=0; j<n; j++){
                                Matrix[i*n+j]=generate(k, n, i, j);
                                ForNorm+=fabs(generate(k, n, i, j));
                        }
                        if(ForNorm>Norm){Norm=ForNorm;}
                }
        }

	for(int i=0; i<p; i++){
		//printf("Flag\n");
                if(pthread_create(&tid[i], NULL, ErrorNorm, &Struct[i])==1){
			printf("не создал\n");
                	delete(Struct[0].Max);
                	delete(Struct[0].mutex);
			for(int j=0; j<i+1; j++){delete(Struct[i].time);}
			free(Struct);
			free(Matrix);
			free(swap);
			free(InvMatrix);
			free(tid);
			return -1;
		}
	}
        printf("\n");
	for(int i=0; i<p; i++){
		//printf("Flag\n");
		pthread_join(tid[i],NULL);
		if(i==0){t=t+*Struct[i].time;}
		//printf("%Le\n", *Struct[i].time);
		//if(fabs(t-*Struct[i].time)>(long double)(mach_eps())){t=*Struct[i].time;}
	}

	printf("%s : residual = %e elapsed = %Le s = %d n = %d m = %d p = %d\n", argv[0], eps, t, k, n, m, p);
	//printf("Error norm : %e\n", NormSol(Matrix, InvMatrix, n));
	delete(mutex);
	delete(Struct[0].Max);
	for(int i=0; i<p; i++){delete(Struct[i].time);}
	free(Struct);
	free(Matrix);
	free(swap);
	free(InvMatrix);
	free(tid);
	return 0;
}

#include "Func.h"

void synchronize(int p)
{
	static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t s_condvar_in = PTHREAD_COND_INITIALIZER;
	static pthread_cond_t s_condvar_out = PTHREAD_COND_INITIALIZER;
	static int threads_in = 0;
	static int threads_out = 0;

	pthread_mutex_lock(&s_mutex);

	threads_in=threads_in+1;
	//printf("%d SYNCH IN \n", threads_in);
	if (threads_in >= p)
	{
		threads_out = 0;
	        pthread_cond_broadcast(&s_condvar_in);
	}
	else
	{
		while(threads_in<p)
			pthread_cond_wait(&s_condvar_in, &s_mutex);
	}
	threads_out=threads_out+1;
	//printf("%d SYNCH OUT \n", threads_out);
	if (threads_out >= p)
	{
		threads_in = 0;
	        pthread_cond_broadcast(&s_condvar_out);
	}
	else
	{
		while (threads_out < p)
			pthread_cond_wait(&s_condvar_out, &s_mutex);
	}
	
	pthread_mutex_unlock(&s_mutex);
}







