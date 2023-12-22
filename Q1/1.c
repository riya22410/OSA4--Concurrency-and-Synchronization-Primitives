#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <signal.h>

pthread_mutex_t forks[5];
pthread_cond_t forks_cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t bowls[2];
int phi1, phi2, phi3, phi4, phi5;
pthread_t philosophers[5];
int philospher[5];
int total_philospheres = 5;
pthread_mutex_t bowl_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bowl_cond = PTHREAD_COND_INITIALIZER;
int phi[5] = {0};
int phiThink[5] = {0};
int phiEat[5]={0};
int done = 0;

void eat(int phil) {
    printf("Philosopher %d is eating\n", phil);
    for (int i = 0; i < total_philospheres; i++) {
        if (philospher[i] == phil) {
            phi[i]++;
            break;
        }
    }
    sleep(1);
    printf("Philosopher %d has stopped eating\n", phil);
}

void think(int phil) {
    printf("Philosopher %d is thinking\n", phil);
    for (int i = 0; i < total_philospheres; i++) {
        if (philospher[i] == phil) {
            phiThink[i]++;
            break;
        }
    }
    sleep(1);
}

void* philosopher(void* args) {
    int philosopher_num = *((int*)args);
        int right_fork = philosopher_num;
        int left_fork = (philosopher_num + 1) % 5;
         printf("Philosopher %d is here\n", philosopher_num);
         sleep(1);
    while (1) {
        think(philosopher_num);
        pthread_mutex_lock(&forks[right_fork]);
        pthread_mutex_lock(&forks[left_fork]);
        
        int done = 1;
        pthread_mutex_lock(&bowl_mutex);
        printf("Philosopher %d picked up bowl\n", philosopher_num);

        while (done == 0) {
           pthread_cond_wait(&bowl_cond, &bowl_mutex);
        }
        pthread_mutex_unlock(&bowl_mutex);

        eat(philosopher_num);
        sleep(1);
        pthread_mutex_unlock(&bowl_mutex);
        printf("Philosopher %d left bowl\n", philosopher_num);

        pthread_cond_signal(&bowl_cond);

        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);
      
    }
}

int main() {
    printf("Execution begin\n");

    int ids[5];
    
  for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_mutex_init(&bowls[i], NULL);
    }
    
    for (int i = 0; i < 5; i++) {
        philospher[i] = i;
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }
    

    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    for(int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    for(int i = 0; i < 2; i++) {
        pthread_mutex_destroy(&bowls[i]);
    }
    printf("Execution ends\n");
    return 0;
}
