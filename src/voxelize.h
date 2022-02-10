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

/* Defines */
// define type
typedef enum VOX__dt {
    VOX__dt__keyboard_input_byte_length = 32
} VOX__dt;

/* Boolean */
// boolean type
typedef enum VOX__bt {
    VOX__bt__false,
    VOX__bt__true,
} VOX__bt;

/* Error */
// error type
typedef enum VOX__et {
    // no error
    VOX__et__no_error,

    // graphics
    VOX__et__sdl2_initialization_failure,
    VOX__et__opengl_window_initialization_failure,
    VOX__et__opengl_context_initialization_failure,
    VOX__et__glew_initialization_failure
} VOX__et;

typedef struct VOX__error {
    VOX__et p_type;
    VOX__bt p_has_extra_data;
    union {};
} VOX__error;

VOX__error VOX__create__error__no_error() {
    VOX__error output;

    output.p_type = VOX__et__no_error;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__sdl2_initialization_failure() {
    VOX__error output;

    output.p_type = VOX__et__sdl2_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__opengl_window_initialization_failure() {
    VOX__error output;

    output.p_type = VOX__et__opengl_window_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__opengl_context_initialization_failure() {
    VOX__error output;

    output.p_type = VOX__et__opengl_context_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__glew_initialization_failure() {
    VOX__error output;

    output.p_type = VOX__et__glew_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

void VOX__destroy__error(VOX__error error) {
    return;
}

/* Allocation */
void* VOX__create__allocation(unsigned long long length) {
    return malloc(length);
}

void VOX__destroy__allocation(void* allocation, unsigned long long length) {
    free(allocation);

    return;
}

/* Buffer */
typedef struct VOX__buffer {
    void* p_data;
    unsigned long long p_length;
} VOX__buffer;

VOX__buffer VOX__create__buffer(unsigned long long length) {
    VOX__buffer output;

    output.p_data = VOX__create__allocation(length);
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
    VOX__destroy__allocation(buffer.p_data, buffer.p_length);

    return;
}

VOX__buffer VOX__create__buffer_from_buffer__add_null_termination(VOX__buffer buffer) {
    VOX__buffer output;

    // allocate
    output = VOX__create__buffer(buffer.p_length + 1);

    // copy
    for (unsigned long long i = 0; i < buffer.p_length; i++) {
        ((char*)output.p_data)[i] = ((char*)buffer.p_data)[i];
    }

    // add null
    ((char*)output.p_data)[buffer.p_length] = 0;

    return output;
}

/* Graphics */
typedef struct VOX__window_configuration {
    VOX__buffer p_title;
    int p_width;
    int p_height;
    unsigned short p_flags; // currently unused
} VOX__window_configuration;

typedef struct VOX__graphics {
    SDL_Window* p_window_context;
    SDL_GLContext p_sdl2_opengl_context;
} VOX__graphics;

VOX__graphics VOX__create_null__graphics() {
    VOX__graphics output;

    output.p_window_context = 0;
    output.p_sdl2_opengl_context = 0;

    return output;
}

VOX__window_configuration VOX__create__window_configuration(VOX__buffer title_string_pointer_no_null, int width, int height, unsigned short flags) {
    VOX__window_configuration output;

    output.p_title = VOX__create__buffer_from_buffer__add_null_termination(title_string_pointer_no_null);
    output.p_width = width;
    output.p_height = height;
    output.p_flags = flags;

    return output;
}

void VOX__destroy__window_configuration(VOX__window_configuration window_configuration) {
    VOX__destroy__buffer(window_configuration.p_title);

    return;
}

VOX__graphics VOX__open__graphics__new_window(VOX__error* error, VOX__window_configuration window_configuration) {
    VOX__graphics output;

    // setup output
    output = VOX__create_null__graphics();

    // initialize SDL2
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
        *error = VOX__create__error__sdl2_initialization_failure();

        return output;
    }

    // initalize window
    output.p_window_context = SDL_CreateWindow((const char*)window_configuration.p_title.p_data, 0, 0, window_configuration.p_width, window_configuration.p_height, SDL_WINDOW_OPENGL);
    if (output.p_window_context == 0) {
        *error = VOX__create__error__opengl_window_initialization_failure();

        return output;
    }

    // initalize opengl context
    output.p_sdl2_opengl_context = SDL_GL_CreateContext(output.p_window_context);
    if (output.p_sdl2_opengl_context == 0) {
        *error = VOX__create__error__opengl_context_initialization_failure();

        return output;
    }

    // initalize glew
    if (glewInit() != GLEW_OK) {
        *error = VOX__create__error__glew_initialization_failure();

        return output;
    }

    return output;
}

void VOX__close__graphics__old_window(VOX__graphics window) {
    SDL_GL_DeleteContext(window.p_sdl2_opengl_context);
    SDL_DestroyWindow(window.p_window_context);
    SDL_Quit();

    return;
}

/* Events */
typedef struct VOX__keyboard_input {
    unsigned char p_quit;
} VOX__keyboard_input;

VOX__keyboard_input VOX__create_null__keyboard_input() {
    VOX__keyboard_input output;

    // zero flags
    output.p_quit = 0;

    return output;
}

VOX__keyboard_input VOX__create__keyboard_input__from_sdl2_events() {
    VOX__keyboard_input output;
    SDL_Event e;
    SDL_Event* e_pointer;

    // setup variables;
    e_pointer = &e;

    // get all events
    while (SDL_PollEvent(e_pointer)) {
        if (e.type == SDL_QUIT) {
            output.p_quit = VOX__bt__true;
        }
    }

    return output;
}

/* Game Loop */
void VOX__play(VOX__error* error) {
    VOX__graphics graphics;
    VOX__window_configuration configuration;
    VOX__keyboard_input keyboard_input;
    VOX__buffer title;

    // setup title
    title.p_data = (void*)"Hello Cubic World!";
    title.p_length = 18;

    // setup congifuration
    configuration = VOX__create__window_configuration(title, 720, 480, 0);

    // open window
    graphics = VOX__open__graphics__new_window(error, configuration);

    // setup opengl
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    // run window
    while (VOX__bt__true) {
        // get keyboard input
        keyboard_input = VOX__create__keyboard_input__from_sdl2_events();
        
        // check if should quit
        if (keyboard_input.p_quit == VOX__bt__true) {
            break;
        }

        // clear window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // display new buffer
        SDL_GL_SwapWindow(graphics.p_window_context);
    }

    // clean up
    VOX__close__graphics__old_window(graphics);
    VOX__destroy__window_configuration(configuration);

    return;
}

#endif
