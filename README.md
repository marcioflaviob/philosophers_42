# philosophers


Introducing multithreading in c. Dining philosophers is a classic computer science problem that treats threads as living beings who need to eat, sleep, and think. 
The goal is to make them live as long as possible.

## Installation


```bash
git clone https://github.com/marcioflaviob/philosophers_42.git && cd philosophers_42
```

## Usage

```bash
make
```

The program has 5 arguments and the last one is optional :

- numbers_of_philo
- time_to_die
- time_to_eat
- time_to_sleep
- number_of_time_each_philo_must_eat

Each argument can be between 1 and int MAX.

Below is an example of how to run:

```bash
./philo 10 500 200 200 10
```

In this example, the program will stop when all philosophers ate 10 times.
A message is displayed when a philosopher eats enough, and the thread is stopped.
