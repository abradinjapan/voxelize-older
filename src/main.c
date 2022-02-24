#include <stdio.h>

#include "voxelize.h"

int main() {
    VOX__error error;

    // setup error
    error = VOX__create__error__no_error();

    // run engine
    VOX__play(&error);

    // print error code
    VOX__print__error(error);
    
    // tell player game is done
    printf("Thanks for playing!\n");

    // clean up
    VOX__destroy__error(error);

    return 0;
}