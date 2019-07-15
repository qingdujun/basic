#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
using namespace std;

typedef struct {
    char name[4];
    int age;
} people;

int main(int argc, char const * argv[]) {//big memory
    people* p_map = (people*)mmap(nullptr, sizeof(people) * 10000000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_map == nullptr) {
        cerr << "mmap error" << endl;
        exit(0);
    }
    pid_t pid = fork();
    if (pid == 0) {
        sleep(2);
        for (int i = 0; i < 5; i++) {
            cout << (*(p_map + i)).age << endl;
        }
        (*(p_map+10000000)).age = 100;
        munmap(p_map, sizeof(people) * 10);
    } else if (pid > 0) {
        char c = 'a';
        for (int i = 0; i < 5; i++) {
            c += 1;
            memcpy((*(p_map + i)).name, &c, 2);
            (*(p_map + i)).age = 20 + i;
        }
        sleep(5);
        cout << (*(p_map+10000000)).age << endl;
        munmap(p_map, sizeof(people) * 10);
    } else {
        cerr << "fork error" << endl;
    }
    wait(nullptr);

    return 0;
}