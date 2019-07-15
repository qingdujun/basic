#include <iostream>
#include <unistd.h> //fork, pipe
#include <string>
using namespace std;

/**
 * https://blog.csdn.net/skyroben/article/details/71513385
 * fd[0] - read
 * fd[1] - write
 */
int main(int argc, char const *argv[]) {
    int fd[2];
    int ret = pipe(fd);
    if (ret < 0) {
        cerr << "pipe error" << endl;
    }
    pid_t pid = fork();
    //you can exchange, let pid > 0 in here
    if (pid == 0) { //child process
        close(fd[0]);
        const char* hello = "hello, pipe!";
        for (int i = 0; i < 5; ++i) {
            write(fd[1], hello, strlen(hello) + 1);
            sleep(1);
        }
        close(fd[1]);
    //and here, you can let pid == 0
    } else if (pid > 0) { //father process
        close(fd[1]);
        char buffer[128];
        for (int i = 0; i < 5; ++i) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t length = read(fd[0], buffer, sizeof(buffer));
            if (length > 0) {
                buffer[length - 1] = '\0';
            }
            cout << buffer << endl;
        }
        close(fd[0]);
    } else {
        cerr << "fork error" << endl;
    }

    return 0;
}

/**
 * MacBook-Pro:fork qingdujun$ g++ pipe.cpp 
 * MacBook-Pro:fork qingdujun$ ./a.out 
 * hello, pipe!
 * hello, pipe!
 * hello, pipe!
 * hello, pipe!
 * hello, pipe!
 */
