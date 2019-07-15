#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <iostream>
using namespace std;

typedef struct {
    char name[4];
    int age;
} people;

int main(int argc, char const* argv[]) {
    int shm_id = shmget((key_t)2022, 4096, IPC_CREAT | 0666);
    if (shm_id < 0) {
        cerr << "shmget error" << endl;
        exit(0);
    }
    people* p_map = (people*)shmat(shm_id, nullptr, 0);
    if (p_map == (people*)-1) {
        cerr << "shamt error" << endl;
        exit(0);
    }
    pid_t pid = fork();
    if (pid == 0) {
        sleep(2);
        for (int i = 0; i < 10; ++i) {
            cout << "name: "<<(*(p_map+i)).name <<" age: " << (*(p_map+i)).age <<endl;
        }
        if (shmdt(p_map) < 0) {
            cout << "shmdt error" << endl;
        }
    } else if (pid > 0) {
        char c = 'a';
        for (int i = 0; i < 10; ++i) {
            c += 1;
            memcpy((*(p_map + i)).name, &c, 2);
            (*(p_map + i)).age = 20 + i;
        }
        if (shmdt(p_map) < 0) {
            cerr << "shmdt error" << endl;
        }
    } else {
        cerr << "fork error" << endl;
    }
    wait(nullptr);
    
    return 0;
}

/**
 * MacBook-Pro:fork qingdujun$ g++ shm.cpp 
 * MacBook-Pro:fork qingdujun$ ./a.out 
 * name: b age: 20
 * name: c age: 21
 * name: d age: 22
 * name: e age: 23
 * name: f age: 24
 * name: g age: 25
 * name: h age: 26
 * name: i age: 27
 * name: j age: 28
 * name: k age: 29
 */