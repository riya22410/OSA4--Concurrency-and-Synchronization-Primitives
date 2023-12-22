************************************** LOGIC OF THE CODE ******************************************
In the main function, we are taking the number of cars on both the left and right sides as input,
limiting each side to a maximum of 5 cars and then initializing semaphores for left cars (left_sem1), 
right cars (right_sem2), and the bridge (bridge_sem1).

For left cars, we are creating threads using the left() function, each representing a car crossing from the left
side, using semaphores' type sem_post() and sem_wait().
Simalrly for right cars, we are creating threads using the right() function, each representing a car crossing 
from the right side.
Then, 
Waiting for all threads representing left cars and right cars to finish their execution using pthread_join().
Destroying the semaphores (left_sem1, right_sem2, and bridge_sem1) after their use.
It also handles the case for leftover cars on both sides- right and left. 
*************************** HOW YOU AVOID CONCURRENCY BUGS IN CODE? *******************************
We are using semaphores (left_sem1, right_sem2, bridge_sem1) to control access to critical section- here,the 
bridge by limiting the number of cars that can cross at any given time. This prevents race conditions and
ensures mutual exclusion while accessing shared resources.
The use of sem_wait() and sem_post() ensures that threads wait for access to the bridge - by decrementing 
the semaphore count and release access after crossing-by incrementing the semaphore count.
