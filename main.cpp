#include "server.hpp"

int main() {
    Server server;
    server.start(6667); // Port IRC par défaut
    return 0;
}