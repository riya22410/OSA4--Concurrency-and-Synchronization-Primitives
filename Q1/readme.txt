************************************** LOGIC OF THE CODE ******************************************
In this,each philosopher is represented by a thread. The philosopher function simulates the actions of a
philosopher.
As, there are two bowls in the middle of the table-
A philosopher can only eat if they have both forks and one sauce bowl
A philosopher can use any sauce bowl at any point of time
A sauce bowl can be used by only one philosopher at a time
So, we are using Mutex locks (pthread_mutex_t) are used to represent forks held by philosophers.
There are five forks, one for each philosopher, managed by the forks array. Additionally, there's a bowl 
represented by a mutex lock that allows or disallows philosophers to start eating. 
The eat() and think() functions simulate eating and thinking, respectively, by outputting messages indicating 
the actions of each philosopher.

************************ WHY DEADLOCKS CAN OCCUR IN THE PROBLEM SETUP **********************************
Deadlocks can occur in this problemset due to circular wait conditions. As in this ,each philosopher need two forks 
to eat which he/she can pick form either of the direction one at a time and deadlock can arise if each philosopher 
pick up one fork and waits for the adjacent fork, which is held by another philosopher who is also waiting for a 
another fork to start dinner.

************************ HOW OUR PROPOSED SOLUTION AVOIDS DEADLOCKS? **********************************
The proposed solution uses a resource allocation strategy where each philosopher tries to acquire both forks before
eating, to avoid the circular wait condition. It uses a mutex lock to each fork and ensures that a philosopher 
attempts to acquire both forks simultaneously. The mutex locks prevent multiple philosophers from accessing the 
same fork at the same time,thus, avoiding deadlock.

****************************** FAIRNESS OF THE SOLUTION ***********************************************
In this implementation, all 5 philosophers can eat, first they think and then eat.
