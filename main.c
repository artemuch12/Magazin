#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_t tid_client[3];
pthread_t tid_loader;
int magazin[5] = {1000, 1000, 1000, 1000, 1000};
int needs[3] = {10000, 10000, 10000};


void *client(void *ptr)
{
	int num_mag;
	int *i;
	i = (int *)ptr;
	srand(time(NULL));
	while (needs[*i] > 0)
	{
		num_mag = rand() % 5;
		needs[*i] = needs[*i] - magazin[num_mag];
		magazin[num_mag] = 0;
		printf("Buyer number %d bought everything at store %d. Buyer's needs number %d are: %d.\n", *i, num_mag, *i, needs[*i]);
		sleep(3);
	}
	printf("The buyer has the %d over needs.\n", *i);
	pthread_exit(0);
}
void *loader()
{
	int num_mag;
	srand(time(NULL));
	while (1)
	{
		num_mag = rand() % 5;
		magazin[num_mag] = 500 + magazin[num_mag];
		printf("The loader replenished the store number %d of goods for 500 pcs. The volume of goods in the store number %d is: %d pcs.\n", num_mag, num_mag, magazin[num_mag]);
		sleep(2);
	}
}
int main()
{
	void **status;
	int max_rand;
	int i = 0;
	int err_tid;


	
	pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
	
	
	srand(time(NULL));
	for(i = 0; i < 5; i++)
	{
		max_rand = -100 + rand() % 201;
		magazin[i] = magazin[i] - max_rand;
	}
	for(i = 0; i < 3; i++)
	{
		max_rand = -1000 + rand() % 2001;
		needs[i] = needs[i] + max_rand;
	}
	for (i = 0; i < 3; i++)
	{
		pthread_mutex_lock(&mut);
		pthread_create(&tid_client[i], NULL, client, &i);
		puts("Create flow");
		pthread_mutex_unlock(&mut);
	}
	pthread_create(&tid_loader, NULL, loader, NULL);
	puts("Create flow");
	for (i = 0; i < 3; i++)
	{
		pthread_join(tid_client[i], status);
	}
	printf("Buyers ran out of needs.\n");
	err_tid = pthread_cancel(tid_loader);
	if(err_tid != 0)
	{
		printf("Error %d", err_tid);
		exit(err_tid);
	}
	exit(0);
}
