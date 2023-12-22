#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int num_passengers;
int capacity;
int passengers_on_board = 0;
int count_passenger = 0;

sem_t mutex;    // Mutex for car access
sem_t boardsem;
sem_t unloadsem;
sem_t unboard;

void load() {
    printf("Car: Loading started...\nCar: Loading complete. Boarding will start soon!\n");
    sleep(1);  // Increase the loading time for demonstration purposes
}

void unload() {
    printf("Car: Unloading started...\nCar: Unloading finished. Passengers will start disembarking soon... \n");
    sleep(1);
}

void board(int passenger_id) {
    printf("Passenger %d: Boarding the car. (%d/%d)\n", passenger_id, passengers_on_board + 1, capacity);
    passengers_on_board++;
    count_passenger++;
}

void offboard(int passenger_id) {
    passengers_on_board--;
    printf("Passenger %d unboarding: Getting off the car. \n", passenger_id);
    sleep(1);
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);
    printf("Passenger %d: Waiting to board the car.\n", passenger_id);
    sem_wait(&mutex);
    if (passengers_on_board < capacity) {
        board(passenger_id);
        if (passengers_on_board == capacity) {
            sem_post(&boardsem);
        }
        sem_post(&mutex);  // Unlock the car
        sleep(1);

        sem_wait(&unboard);  // Wait for the signal that passengers have finished disembarking
        sem_post(&mutex);    // Unlock the car
    } else {
        sem_post(&mutex);  // Unlock the car
        printf("Passenger %d: Car is full. Waiting for the next ride.\n", passenger_id);
        sem_wait(&boardsem);  // Wait for the signal that the car is ready for boarding
    }

    return NULL;
}

void* car(void* args) {
    free(args);
    while (1) {
        load();

        while (passengers_on_board < capacity) {
            sem_post(&boardsem);
        }

        sem_wait(&mutex);  // Lock the car for unloading
        unload();
        sem_post(&unboard);  // Signal that passengers can start disembarking
        sem_post(&mutex);    // Unlock the car

        // Wait for all passengers to finish disembarking
        for (int i = 0; i < capacity; i++) {
            sem_wait(&unboard);
        }

        // Check if there are enough passengers left for another round
        if (num_passengers - count_passenger >= capacity) {
            printf("Car: Enough passengers left. Boarding again!\n");
            sem_post(&boardsem);  // Signal that the car is ready for boarding
            count_passenger += capacity;  // Increment the total boarded passengers
        } else {
            printf("Car: Not enough passengers left. Cannot move.\n");
            break;  // Break out of the loop if there are not enough passengers
        }
    }
    return NULL;
}

 
int main() {
    printf("Enter the total number of passengers: ");
    scanf("%d", &num_passengers);

    printf("Enter the capacity of the car: ");
    scanf("%d", &capacity);

    if (num_passengers <= 0 || capacity <= 0 || capacity >= num_passengers) {
        printf("Invalid input. The capacity must be less than the number of passengers. Exiting.\n");
        return 1;
    }

    pthread_t car_thread;
    pthread_t passenger_threads[num_passengers];

    sem_init(&mutex, 0, 1);
    sem_init(&boardsem, 0, 0);
    sem_init(&unboard, 0, 0);
    sem_init(&unloadsem, 0, 0);

    pthread_create(&car_thread, NULL, car, 0);

    for (int i = 0; i < num_passengers; i++) {
        int* passenger_id = malloc(sizeof(int));
        *passenger_id = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, passenger_id);
    }

    // Wait for the car thread to finish
    pthread_join(car_thread, NULL);

    sem_destroy(&mutex);
    sem_destroy(&boardsem);
    sem_destroy(&unloadsem);
    sem_destroy(&unboard);

    printf("All passengers have disembarked successfully!\n");

    return 0;
}
