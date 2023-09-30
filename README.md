# Dining Philosophers Problem

The “Philosophers” project is part of the curriculum at 42 school and this repository presents a solution to the Dining Philosophers problem — a classic synchronization and concurrency challenge. The problem involves a group of philosophers who alternate between eating, thinking, and sleeping, all while sharing a round table with a limited number of forks. The goal is to design a solution that ensures the philosophers can eat without resulting in deadlocks or contention.

&nbsp;
### Problem Description
* Several philosophers sit at a round table.
* A large bowl of spaghetti is placed in the middle of the table.
* Philosophers alternate between three states: eating, thinking, and sleeping.
* Each philosopher needs two forks to eat (one for each hand), and there are as many forks as there are philosophers.
* Philosophers must avoid starvation.
* There are no direct communication or coordination between philosophers.

&nbsp;

### Implementation Details
* Each philosopher is represented as a thread.
* To prevent philosophers from competing and duplicating forks, each fork has to be protected with a mutex

&nbsp;
### Program Arguments
To run the program, use the following command-line arguments:

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
``` 
 
&nbsp;
* **number_of_philosophers:** The number of philosophers and forks;

* **time_to_die (in milliseconds):** The time limit for a philosopher to start eating; otherwise, they die;

* **time_to_eat (in milliseconds):** The time a philosopher takes to eat while holding two forks;

* **time_to_sleep (in milliseconds):** The time a philosopher spends sleeping;

* **number_of_times_each_philosopher_must_eat (optional):** If specified, the simulation stops when all philosophers have eaten this many times.
