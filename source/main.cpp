#define ASIO_STANDALONE
#define _GLIBCXX_HAS_GTHREADS

#include <iostream>
#include "../core/Winter.h"

void sighandler(int signum){
    size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    printf("Thread %zu got signal %d\n", threadId, signum);
    std::terminate();
}


int main() {

    signal(SIGSEGV, sighandler);

    Winter app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
