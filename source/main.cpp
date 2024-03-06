#define ASIO_STANDALONE

#define _GLIBCXX_HAS_GTHREADS

#include <iostream>
#include "Winter.h"

int main() {

    Winter app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    return 0;
}
