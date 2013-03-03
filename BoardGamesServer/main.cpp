#include "server.h"

/**
 * @brief main function of the server
 * @param argc number of arguments from commandline
 * @param argv commandline arguments
 * @return return code 0 - ok, 1 - error
 */
int main(int argc, char ** argv) {
    bg::server server;
    return server.run();
}
