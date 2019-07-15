#include <sys/sem.h>//semun, semop, semget, semctl
#include <unistd.h> //fork
#include <iostream>
using namespace std;

void p(int& sem_id) {
    struct sembuf sem_buffer = {0, -1, SEM_UNDO};
    if (semop(sem_id, &sem_buffer, 1) < 0) {
        cerr << "semop error" << endl;
        exit(0);
    }
}

void v(int& sem_id) {
    struct sembuf sem_buffer = {0, 1, SEM_UNDO};
    if (semop(sem_id, &sem_buffer, 1) < 0) {
        cerr << "semop error" << endl;
        exit(0);
    }
}

/**
 * https://zh.cppreference.com/w/c/language/struct_initialization
 * https://blog.csdn.net/ljianhui/article/details/10243617
 */
int main(int argc, char const *argv[]) {
    int producer = semget((key_t)2019, 1, 0666 | IPC_CREAT);
    int consumer = semget((key_t)2020, 1, 0666 | IPC_CREAT);
    union semun producer_init_value = {1}, consumer_init_value = {0};
    if (semctl(producer, 0, SETVAL, producer_init_value) <  0 || semctl(consumer, 0, SETVAL, consumer_init_value) <  0) {
        cerr << "semctl error" << endl;
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0) { //child process
        for (int i = 0; i < 5; ++i) {
            p(consumer);
            cout << "eating apple" << endl;
            v(producer);
        }
    } else if (pid > 0) { //father process
        for (int i = 0; i < 5; ++i) {
            p(producer);
            cout << "making apple" << endl;
            v(consumer);
        }
    } else {
        cerr << "fork error" << endl;
    }
    wait(nullptr);

    return 0;
}

/**
 * MacBook-Pro:fork qingdujun$ g++ pv.cpp 
 * MacBook-Pro:fork qingdujun$ ./a.out 
 * making apple
 * eating apple
 * making apple
 * eating apple
 * making apple
 * eating apple
 * making apple
 * eating apple
 * making apple
 * eating apple
 */