#include <iostream>
#include<omp.h>

void task_1(int num_threads) {
#pragma omp parallel num_threads(num_threads)
    {
        int myid = omp_get_thread_num();
        int threads_count = omp_get_num_threads();
        printf("Hello world, thread number %d, threads: %d\n", myid, threads_count);
    }
}

void task_2() {
	int threads_num = 3;

    #pragma omp parallel num_threads(threads_num) if(threads_num > 2)
	{
		int thread_id = omp_get_thread_num();
		int threads_count = omp_get_num_threads();
		printf("Threads: %d; Thread id: %d \n", threads_count, thread_id);
	}

	threads_num = 2;

#pragma omp parallel num_threads(threads_num) if(threads_num > 2)
	{
		int thread_id = omp_get_thread_num();
		int threads_count = omp_get_num_threads();
		printf("Threads: %d; Thread id: %d \n", threads_count, thread_id);
	}
}

void task_3() {
    int a = 0;
    int b = 0;

    printf("Before a=%d b=%d\n", a, b);
#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int thread_id = omp_get_thread_num();
        a = 0;
        a += thread_id;
        b += thread_id;
        printf("In a=%d b=%d thread_num=%d\n ", a, b, thread_id);
    }
    printf("After a=%d b=%d\n\n", a, b);


    printf("Before a=%d b=%d\n", a, b);
#pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int thread_id = omp_get_thread_num();
        b = 0;
        a -= thread_id;
        b -= thread_id;
        printf("In a=%d b=%d thread_num=%d\n ", a, b, thread_id);
    }
    printf("After a=%d b=%d\n\n", a, b);
}

void task_4(int N) {
    int* a = new int[N];
    int* b = new int[N];

    for (int i = 0; i < N; i++) {
        a[i] = rand();
        b[i] = rand();
        printf("a[%d] = %d, b[%d] = %d \n", i, a[i], i, b[i]);
    }

    int max = INT_MIN;
    int min = INT_MAX;

#pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();
        if (thread_id == 0)
        {
            for (int i = 0; i < N; i++)
            {
                if (min > a[i]) {
                    min = a[i];
                }

            }

        }
        else if (thread_id == 1)
        {
            for (int i = 0; i < N; i++)
            {
                if (max < b[i]) {
                    max = b[i];
                }

            }
        }
    }
    printf("min in a=%d  max in b=%d", min, max);
}

void task_5() {
	int d[6][8];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			d[i][j] = rand();
		}
	}

#pragma omp parallel sections shared(d) num_threads(3)
	{
		#pragma omp section
		{
			int sum = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					sum += d[i][j];
				}
			}
			printf("Thread num: %d; Srednee arifm = %d\n", omp_get_thread_num(), sum / 48);
		}

		#pragma omp section 
		{
			int min = INT_MAX;
			int max = INT_MIN;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (min > d[i][j]) {
						min = d[i][j];
					}

					if (max < d[i][j]) {
						max = d[i][j];
					}

				}
			}
			printf("Thread num: %d; max = %d; min = %d\n", omp_get_thread_num(), max, min);

		}

#pragma omp section 
		{
			int res = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (d[i][j] % 3 == 0) {
						res += 1;
					}

				}
			}
			printf("Thread num: %d; Count of nums kratnix 3: %d\n", omp_get_thread_num(), res);
		}
	}
}

void task_6() {
	int a[100];
	for (int i = 0; i < 100; i++)
	{
		a[i] = rand()%10;
	}
	printf("\n");
	int sum = 0;

	#pragma omp parallel for reduction(+: sum) num_threads(5)
		for (int i = 0; i < 100; i++)
		{
			sum += a[i];
			
		}

	int sum2 = 0;
	printf("Avg reduction: %d\n", sum / 100);

	#pragma omp parallel num_threads(5)
	{
		#pragma omp parallel for
		for (int i = 0; i < 100; i++)
		{
			sum2 += a[i];

		}
	}
	printf("Avg without reduction: %d\n", sum2 / 100);
}

void task_7() {
	int a[12], b[12], c[12];

#pragma omp parallel num_threads(3)
	{
		printf("Current thread = %d\nThreads count = %d\n", omp_get_thread_num(), omp_get_num_threads());
#pragma omp for schedule(static, 4)
		for (int i = 0; i < 12; i++) {
			a[i] = rand();
			printf("A[%d] = %d\n", i, a[i]);
			b[i] = rand();
			printf("B[%d] = %d\n", i, b[i]);
		}
	}

#pragma omp parallel num_threads(4)
	{
		printf("Current thread = %d\nThreads count = %d\n", omp_get_thread_num(), omp_get_num_threads());
#pragma omp for schedule(dynamic, 3)
		for (int i = 0; i < 12; i++) {
			c[i] = a[i] + b[i];
			printf("C[%d] = %d\n", i, c[i]);
		}
	}
}

void task_8() {
	const int N = 16000;
	int a[N];
	int b[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = i;
	}

	double start = omp_get_wtime();
#pragma omp parallel num_threads(8)
	{
#pragma omp for schedule(static, 8)
		for (int i = 1; i < N - 1; i++)
		{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	double end = omp_get_wtime();
	printf("Time with static %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel num_threads(8)
	{
#pragma omp for schedule(dynamic, 600)
		for (int i = 1; i < N - 1; i++)
		{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with dynamic %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel num_threads(8)
	{
#pragma omp for schedule(guided, 100)
		for (int i = 1; i < N - 1; i++)
		{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with guided %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel num_threads(8)
	{
#pragma omp for schedule(runtime)
		for (int i = 1; i < N - 1; i++)
		{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with runtime %f\n", end - start);
}

void task_9(int N) {
	int** matrix;
	matrix = new int*[N];
	int *vector = new int[N];
	int *res = new int[N];

	for (int i = 0; i < N; i++)
	{
		matrix[i] = new int[N];
		vector[i] = rand();
		for (int j = 0; j < N; j++)
		{
			matrix[i][j] = rand();
		}
	}

	double start = omp_get_wtime();
	for (int i = 0; i < N; i++)
	{
		res[i] = 0;
		for (int j = 0; j < N; j++)
		{
			res[i] += matrix[i][j] * vector[j];
		}
	}
	double end = omp_get_wtime();
	printf("No parallel time %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(guided, 100) num_threads(4)
	for (int i = 0; i < N; i++)
	{
		res[i] = 0;
		for (int j = 0; j < N; j++)
		{
			res[i] += matrix[i][j] * vector[j];
		}
	}
	end = omp_get_wtime();
	printf("Parallel guided time %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 120) num_threads(4)
	for (int i = 0; i < N; i++)
	{
		res[i] = 0;
		for (int j = 0; j < N; j++)
		{
			res[i] += matrix[i][j] * vector[j];
		}
	}
	end = omp_get_wtime();
	printf("Parallel dynamic time %f\n", end - start);

	start = omp_get_wtime();
#pragma omp parallel for schedule(static) num_threads(4)
	for (int i = 0; i < N; i++)
	{
		res[i] = 0;
		for (int j = 0; j < N; j++)
		{
			res[i] += matrix[i][j] * vector[j];
		}
	}
	end = omp_get_wtime();
	printf("Parallel static time %f\n", end - start);
}

void task_10() {
	const int N = 6;
	const int M = 8;

	int d[N][M];
	int max = INT_MIN;
	int min = INT_MAX;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			d[i][j] = rand();
			printf("%d ", d[i][j]);
		}
	}

#pragma omp parallel for num_threads(4)
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (d[i][j]>max)
			#pragma omp critical
			{
				max = d[i][j];
			}

			if (d[i][j] < min)
			#pragma omp critical
			{
				min = d[i][j];
			}

		}
	}
	printf("Max: %d; Min: %d", max, min);
}

void task_11() {
	const int N = 30;
	int a[N];
	for (int i = 0; i < N; i++)
	{
		a[i] = rand() % 1000;
		printf("%d ",a[i]);
	}
	printf("\n");

	int res = 0;

#pragma omp parallel for num_threads(4)
	for (int i = 0; i < N; i++)
	{
		if (a[i] % 9 == 0)
		{
#pragma omp atomic
			res++;
		}
	}
	printf("Result: %d", res);
}

void task_12(int N) {
	int *a = new int[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = rand();
	}

	int max = INT_MIN;
	
#pragma omp parallel for firstprivate(a) num_threads(4)
	for (int i = 0; i < N; i++)
	{
		if (a[i] % 7 == 0)
		{
#pragma omp critical
			{
				if (max < a[i])
				{
					max = a[i];
				}
			}
		}
	}

	printf("Max: %d", max);
}


void task_13() {
	//1
	int myid, i, all;
	int a[8]; 
#pragma omp parallel shared(a) private(all,myid,i) num_threads(8)
	{
		all = omp_get_num_threads();
#pragma omp for
		for (i = 0; i < all; i++) {
			a[i] = omp_get_thread_num();
		}
#pragma omp for ordered
		for (i = all - 1; i >= 0; i--) {
#pragma omp ordered
			{
				printf("Hello World, I'm thread number %d and all treads is %d\n", a[i], omp_get_num_threads());
			}
		}
	}

	printf("\n");

	//2
#pragma omp parallel num_threads(8)
	{
		int nthreads = omp_get_num_threads();
		for (int i = nthreads - 1; i >= 0; i--)
		{
#pragma omp barrier
			{
				if (i == omp_get_thread_num())
				{
#pragma omp critical
					printf("Hello World, I'm thread number %d and all treads is %d\n", i, omp_get_num_threads());
				}
			}
		}
	}
}

int main()
{
    //task_1(8);
	//task_2();
    //task_3();
    //task_4(5);
	//task_5();
	//task_6();
	//task_7();
	//task_8();
	//task_9(1000);
	//task_10();
	//task_11();
	//task_12(10);
	task_13();
}
