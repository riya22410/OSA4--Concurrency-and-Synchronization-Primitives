This code represents a simple simulation of passengers boarding and disembarking from a car using pthreads and
semaphores in C.

************************************ CODE EXPLANATION & LOGIC *****************************************

1) Semaphore Initialization:
A) sem_t variables are used for synchronization:
a) mutex: Mutex for car access
b) boardsem: For passengers to wait before boarding.
c) unloadsem: used during unloading of passengers.
d) unboard: used during boarding of passengers. 

B) Load and Unload Functions:
a) load: Simulates loading the car. It prints a loading message and then signals the all_aboard semaphore when
the car is fully loaded.
b) unload: Simulates unloading the car. It prints an unloading message and signals the unloading semaphore when 
the passengers have disembarked.

C) Car Thread (car function):
This car has a limited capacity and can only set off when fully occupied, with a maximum of C passengers on 
board (where C is less than the total number of passengers). 
It loads passengers, signals when the car is full, waits for a delay while it runs, unloads, 
waits for disembarkment of passenegrs and signals unboarding when it is complete.

D) Passenger Thread (passenger function):
Each passenger thread represents an individual passenger.
It waits for the mutex to be available, checks if there is space on the car, boards the car, and signals boardsem if the car is full.
If the car is already full, the passenger waits for the signal to board.
After the boarding is complete and car's capcity is full, it simulates the ride, disembarks, and signals unboard.

E) Main Function:
Initializes semaphores and threads.
Waits for passenger threads to complete.
Destroys semaphores.

*************************** HOW WE AVOIDED CONCURRENCY BUGS IN CODE ***********************************

i) The use of semaphores (mutex, boardsem, unloadsem, unboard ) ensures proper synchronization between 
threads, preventing race conditions.
ii) The critical sections are protected by sem_wait, sem_post mutex calls, ensuring exclusive access to shared 
resources like the car and passenger count.
iii) The order of signaling and waiting ensures that operations are performed in a synchronized manner, 
avoiding conflicts.
