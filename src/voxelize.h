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
    VOX__dt__opengl_error_info_log_length = 1024
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
    VOX__et__glew_initialization_failure,

    // shaders
    VOX__et__vertex_shader_compilation_failure,
    VOX__et__fragment_shader_compilation_failure,
    VOX__et__shader_linking_failure
} VOX__et;

typedef struct VOX__error {
    VOX__et p_type;
    VOX__bt p_has_extra_data;
    char* p_opengl_log;
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

VOX__error VOX__create__error__vertex_shader_compilation_failure(char* opengl_log) {
    VOX__error output;

    output.p_type = VOX__et__vertex_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__error VOX__create__error__fragment_shader_compilation_failure(char* opengl_log) {
    VOX__error output;

    output.p_type = VOX__et__fragment_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__error VOX__create__error__shader_linking_failure(char* opengl_log) {
    VOX__error output;

    output.p_type = VOX__et__shader_linking_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__bt VOX__check__error__has_error_occured(VOX__error* error) {
    return ((*error).p_type != VOX__et__no_error) == VOX__bt__true;
}

/* Allocation */
void* VOX__create__allocation(unsigned long long length) {
    return malloc(length);
}

void VOX__destroy__allocation(void* allocation, unsigned long long length) {
    free(allocation);

    return;
}

/* Deallocate Error */
void VOX__destroy__error(VOX__error error) {
    if (error.p_has_extra_data == VOX__bt__true) {
        switch (error.p_type) {
        case VOX__et__vertex_shader_compilation_failure:
        case VOX__et__fragment_shader_compilation_failure:
        case VOX__et__shader_linking_failure:
            VOX__destroy__allocation(error.p_opengl_log, VOX__dt__opengl_error_info_log_length);
            break;
        default:
            break;
        }
    }

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

/* Shaders */
typedef struct VOX__shader {
    GLuint p_shader_ID;
    VOX__buffer p_program;
} VOX__shader;

typedef struct VOX__shaders_program {
    GLuint p_program_ID;
    VOX__shader p_vertex_shader;
    VOX__shader p_fragment_shader;
} VOX__shaders_program;

VOX__shader VOX__create_null__shader() {
    VOX__shader output;

    output.p_shader_ID = 0;
    output.p_program = VOX__create__buffer__add_address(0, 0);

    return output;
}

VOX__shader VOX__compile__shader(VOX__error* error, VOX__buffer shader_data, GLenum shader_type) {
    VOX__shader output;
    VOX__buffer shader_with_null_termination;
    GLint error_log_length;
    char* opengl_error_log;

    // setup output
    output = VOX__create_null__shader();

    // setup shader copy with null termination
    shader_with_null_termination = VOX__create__buffer_from_buffer__add_null_termination(shader_data);

    // create shader space
    output.p_shader_ID = glCreateShader(shader_type);

    // send program to gpu
    glShaderSource(output.p_shader_ID, 1, (const GLchar* const*)&shader_with_null_termination.p_data, NULL);

    // compile shader
    glCompileShader(output.p_shader_ID);

    // check for errors
    glGetShaderiv(output.p_shader_ID, GL_INFO_LOG_LENGTH, &error_log_length);
    if (error_log_length > 0) {
        opengl_error_log = VOX__create__allocation(sizeof(char) * VOX__dt__opengl_error_info_log_length);

        glGetShaderInfoLog(output.p_shader_ID, VOX__dt__opengl_error_info_log_length, NULL, opengl_error_log);

        if (shader_type == GL_VERTEX_SHADER) {
            *error = VOX__create__error__vertex_shader_compilation_failure(opengl_error_log);
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            *error = VOX__create__error__fragment_shader_compilation_failure(opengl_error_log);
        }
    }

    // delete shader copy
    VOX__destroy__buffer(shader_with_null_termination);

    return output;
}

VOX__shaders_program VOX__compile__shaders_program(VOX__error* error, VOX__buffer vertex_shader, VOX__buffer fragment_shader) {
    VOX__shaders_program output;
    GLint error_log_length;
    char* opengl_error_log;

    // setup output
    output.p_program_ID = 0;
    output.p_vertex_shader = VOX__create_null__shader();
    output.p_fragment_shader = VOX__create_null__shader();
    
    // compile shaders
    output.p_vertex_shader = VOX__compile__shader(error, vertex_shader, GL_VERTEX_SHADER);
    if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
        return output;
    }

    output.p_fragment_shader = VOX__compile__shader(error, fragment_shader, GL_FRAGMENT_SHADER);
    if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
        return output;
    }

    // create shader program
    output.p_program_ID = glCreateProgram();

    // setup shader linking
    glAttachShader(output.p_program_ID, output.p_vertex_shader.p_shader_ID);
    glAttachShader(output.p_program_ID, output.p_fragment_shader.p_shader_ID);
    
    // link shaders
    glLinkProgram(output.p_program_ID);
    
    // check for errors
    glGetProgramiv(output.p_program_ID, GL_LINK_STATUS, &error_log_length);
    if (error_log_length > 0) {
        opengl_error_log = VOX__create__allocation(sizeof(char) * VOX__dt__opengl_error_info_log_length);

        glGetProgramInfoLog(output.p_program_ID, VOX__dt__opengl_error_info_log_length, NULL, opengl_error_log);

        *error = VOX__create__error__shader_linking_failure(opengl_error_log);
    }

    return output;
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
