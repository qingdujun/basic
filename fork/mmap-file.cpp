#include <iostream>
#include <unistd.h>
#include <sys/mman.h> //PROT_READ
#include <fcntl.h> //O_CREAT
#include <string>
using namespace std;

typedef struct {
    char name[4];
    int age;
} people;

int main(int argc, char const *argv[]) {
    int fd = open("mmap.txt", O_CREAT | O_RDWR | O_TRUNC, 00777);
    if (fd < 0) {
        cerr << "open error" << endl;
        exit(0);
    }
    lseek(fd, sizeof(people)*5-1, SEEK_SET);
    write(fd, " ", 1); //set file length
    people* p_map = (people*)mmap(nullptr, sizeof(people) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (p_map == nullptr) {
        cerr << "mmap error" << endl;
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0) { //child process
        for (int i = 0; i < 10; ++i) {
            cout << "name: " << (*(p_map+i)).name << " age: "<<(*(p_map+i)).age<<endl;
        }
        munmap(p_map, sizeof(people) * 10);
    } else if (pid > 0) { //father process
        char c = 'a';
        for (int i = 0; i < 10; ++i) {
            c += 1;
            memcpy((*(p_map+i)).name, &c, 2);
            (*(p_map+i)).age = 20+i;
        }
        munmap(p_map, sizeof(people)*10);
    } else {
        cerr << "fork error" << endl;
    }
    wait(nullptr);
    
    return 0;
}
