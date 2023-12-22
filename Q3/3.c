#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <signal.h>
#include<semaphore.h>

sem_t left_sem1, right_sem2, bridge_sem1;
int no_of_car_on_right, no_of_car_on_left,total_car=0;
int dir; 
void* left(void* args) {
     int car_left = *((int*)args);
     dir= 1;
     sem_wait(&left_sem1);
     sem_wait(&bridge_sem1);
     printf("Left car %d is crossing the bridge.\n", car_left);
     total_car+=1;
     no_of_car_on_left+=1;
     sleep(1);
     sem_post(&bridge_sem1);
     sem_post(&left_sem1);
     printf("Left car %d has crossed the bridge.\n", car_left);
     return NULL;
}                       



void* right(void* args){
     int car_right = *((int*)args);
     dir=2;
     sem_wait(&right_sem2);
     sem_wait(&bridge_sem1);
     printf("Right car %d is crossing the bridge.\n", car_right);
     total_car+=1;
     no_of_car_on_right+=1;
     sleep(1);
     sem_post(&bridge_sem1);
      sem_post(&right_sem2);
     printf("Right car %d has crossed the bridge.\n", car_right);
     return NULL;
}                
   

   
void passing(int dir){
     if(dir==2){
          printf("Car is moving from right direction. \n");
     }
     else if(dir==1){
          printf("Car is moving from left direction. \n");
     }
}
int main(){
    int left_cars,right_cars;
    int final_left_cars, final_right_cars;
    int final_left_cars_leftover,final_right_cars_leftover;
    printf("Enter the number of cars on the left side: ");
    scanf("%d", &left_cars);
    if(left_cars>5){
    final_left_cars_leftover=left_cars-5;
    final_left_cars=5;
    }
    else{
     final_left_cars=left_cars;
     }
    printf("Enter the number of cars on the right side: ");
    scanf("%d", &right_cars);
    if(right_cars>5){
    final_right_cars_leftover=right_cars-5;
    final_right_cars=5;
    }
    else{
     final_right_cars=right_cars;
     }
    
    sem_init(&left_sem1, 0, 5); 
    sem_init(&right_sem2, 0, 5); 
    sem_init(&bridge_sem1, 0, 1);

    pthread_t left_threads[final_left_cars], right_threads[final_right_cars];

    int left_nums[final_left_cars], right_nums[final_right_cars];
    int left_nums_new[final_left_cars_leftover], right_nums_new[final_right_cars_leftover];

   for (int i = 0; i < final_left_cars; i++) {
        left_nums[i] = i + 1;
        pthread_create(&left_threads[i], NULL, left, &left_nums[i]);
        passing(dir);
    }

    for(int i = 0; i < final_right_cars; i++){
        right_nums[i] = i + 1;
        // printf("j %d\n", right_nums[i]);
        pthread_create(&right_threads[i], NULL, right, &right_nums[i]);
        passing(dir);
    }
    
    if(final_left_cars_leftover>0){
    for (int i = 0; i < final_left_cars_leftover; ++i) {
    int car_number = final_left_cars + i + 1;
        left_nums_new[i] = car_number;
        pthread_create(&left_threads[i], NULL, left, &left_nums_new[i]);
        passing(dir);
    }
    }
    
    if(final_right_cars_leftover>0){
    for (int i = 0; i < final_right_cars_leftover; ++i) {
    int car_number1 = final_right_cars + i + 1;
        right_nums_new[i] = car_number1;
        pthread_create(&left_threads[i], NULL, right , &right_nums_new[i]);
        passing(dir);
    }
    }
    for(int i = 0; i < final_left_cars; i++){
        pthread_join(left_threads[i], NULL);
    }

    for(int i = 0; i < final_right_cars; i++){
        pthread_join(right_threads[i], NULL);
    }
       sem_destroy(&left_sem1);
     sem_destroy(&right_sem2);
    sem_destroy(&bridge_sem1);
    

printf("Total_car run over bridge: %d\n",total_car);

    return 0;
}
