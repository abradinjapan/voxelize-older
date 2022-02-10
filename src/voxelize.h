#ifndef __VOXELIZE__
#define __VOXELIZE__

/* Includes */
// Graphics
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

// C Standard Library
#include <stdlib.h>
#include <stdio.h>

/* Error */
typedef enum VOX__et {
    // no error
    VOX__et__no_error
} VOX__et;

/* Buffer */
typedef struct VOX__buffer {
    void* p_data;
    unsigned long long p_length;
} VOX__buffer;

VOX__buffer VOX__create__buffer(unsigned long long length) {
    VOX__buffer output;

    output.p_data = malloc(length);
    output.p_length = length;

    return output;
}

VOX__buffer VOX__create__buffer__add_address(void* address, unsigned long long length) {
    VOX__buffer output;

    output.p_data = address;
    output.p_length = length;

    return output;
}

void VOX__destroy__buffer(VOX__buffer buffer) {
    free(buffer.p_data);

    return;
}

/* Game Loop */
void VOX__play() {
    return;
}

#endif
