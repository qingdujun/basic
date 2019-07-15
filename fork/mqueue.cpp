#include <fcntl.h> //

// POSIX <mqueue.h> is not supported on macOS

/**
 * https://blog.csdn.net/sty23122555/article/details/51132158
 */
int main(int argc, char const *argv[]) {
    mqd_t msgq_id;

    return 0;
}
