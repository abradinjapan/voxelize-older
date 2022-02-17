#include <stdio.h>

#include "voxelize.h"

int main() {
    VOX__error error;

    // setup error
    error = VOX__create__error__no_error();

    // run engine
    VOX__play(&error);

    // print error code
    printf("Error Code: %llu\nError Log: \n%s\n", (unsigned long long)error.p_type, error.p_opengl_log);

    // tell player game is done
    printf("Thanks for playing!\n");

    return 0;
}