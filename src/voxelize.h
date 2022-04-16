#ifndef __VOXELIZE__
#define __VOXELIZE__

/* Includes */
// Graphics
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

// C Standard Library
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/* Defines */
// define type
typedef enum VOX__dt {
    VOX__dt__opengl_error_info_log_length = 1024,
    VOX__dt__chunk_side_length = 8,
    VOX__dt__chunk_block_count = VOX__dt__chunk_side_length * VOX__dt__chunk_side_length * VOX__dt__chunk_side_length
} VOX__dt;

// base types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double f64;

/* Boolean */
// boolean type
typedef enum VOX__bt {
    VOX__bt__false,
    VOX__bt__true
} VOX__bt;

/* Error */
// error type
typedef enum VOX__et {
    // no error
    VOX__et__no_error,

    // other opengl error
    VOX__et__other_opengl,

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
    GLenum p_opengl_error_code;
} VOX__error;

VOX__error VOX__create__error__no_error() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__no_error;
    output.p_has_extra_data = VOX__bt__false;
    output.p_opengl_log = 0;
    output.p_opengl_error_code = GL_NO_ERROR;
    
    return output;
}

VOX__error VOX__create__error__sdl2_initialization_failure() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__sdl2_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__opengl_window_initialization_failure() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__opengl_window_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__opengl_context_initialization_failure() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__opengl_context_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__glew_initialization_failure() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__glew_initialization_failure;
    output.p_has_extra_data = VOX__bt__false;
    
    return output;
}

VOX__error VOX__create__error__vertex_shader_compilation_failure(char* opengl_log, GLenum opengl_error_code) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__vertex_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    output.p_opengl_error_code = opengl_error_code;
    
    return output;
}

VOX__error VOX__create__error__fragment_shader_compilation_failure(char* opengl_log, GLenum opengl_error_code) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__fragment_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    output.p_opengl_error_code = opengl_error_code;
    
    return output;
}

VOX__error VOX__create__error__shader_linking_failure(char* opengl_log, GLenum opengl_error_code) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__shader_linking_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    output.p_opengl_error_code = opengl_error_code;
    
    return output;
}

VOX__error VOX__try_create__error__other_opengl() {
    VOX__error output;
    GLenum opengl_error;

    // check error
    opengl_error = glGetError();

    if (opengl_error != GL_NO_ERROR) {
        output.p_type = VOX__et__other_opengl;
        output.p_has_extra_data = VOX__bt__true;
        output.p_opengl_log = 0;
        output.p_opengl_error_code = opengl_error;
    } else {
        return VOX__create__error__no_error();
    }

    return output;
}

VOX__bt VOX__check__error__has_error_occured(VOX__error* error) {
    return ((*error).p_type != VOX__et__no_error) == VOX__bt__true;
}

void VOX__print__error(VOX__error error) {
    printf("Voxelize Error Code: %lu\n", (u64)error.p_type);

    if (error.p_has_extra_data == VOX__bt__true) {
        if (error.p_opengl_error_code != GL_NO_ERROR) {
            printf("OpenGL Error Code: %lu\n", (u64)error.p_opengl_error_code);
        }

        if (error.p_opengl_log != 0) {
            printf("OpenGL Error Log:\n\t%s\n", error.p_opengl_log);
        }
    }

    return;
}

/* Allocation */
void* VOX__create__allocation(u64 length) {
    return malloc(length);
}

void VOX__destroy__allocation(void* allocation, u64 length) {
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

/* Copy Bytes */
void VOX__copy__bytes_to_bytes(void* source, u64 length, void* destination) {
    for (u64 i = 0; i < length; i++) {
        ((u8*)destination)[i] = ((u8*)source)[i];
    }

    return;
}

/* Strings */
u64 VOX__string__length_without_null(char* string_start) {
    u64 output;

    // setup output
    output = 0;

    while (string_start[output] != 0) {
        output++;
    }

    return output;
}

/* Buffer - A Pointer With A Length */
typedef struct VOX__buffer {
    void* p_data;
    u64 p_length;
} VOX__buffer;

VOX__buffer VOX__create__buffer(u64 length) {
    VOX__buffer output;

    // setup output
    output.p_data = VOX__create__allocation(length);
    output.p_length = length;

    return output;
}

VOX__buffer VOX__create__buffer__add_address(void* address, u64 length) {
    VOX__buffer output;

    // setup output
    output.p_data = address;
    output.p_length = length;

    return output;
}

VOX__buffer VOX__create__buffer_copy_from_c_string(char* string) {
    VOX__buffer output;

    // setup output
    output = VOX__create__buffer(strlen(string));

    // copy string
    VOX__copy__bytes_to_bytes(string, output.p_length, output.p_data);

    return output;
}

VOX__buffer VOX__create__buffer_from_buffer__add_null_termination(VOX__buffer buffer) {
    VOX__buffer output;

    // allocate
    output = VOX__create__buffer(buffer.p_length + 1);

    // copy
    for (u64 i = 0; i < buffer.p_length; i++) {
        ((char*)output.p_data)[i] = ((char*)buffer.p_data)[i];
    }

    // add null
    ((char*)output.p_data)[buffer.p_length] = 0;

    return output;
}

void VOX__destroy__buffer(VOX__buffer buffer) {
    VOX__destroy__allocation(buffer.p_data, buffer.p_length);

    return;
}

/* Graphics - Getting Window Open */
typedef struct VOX__window_configuration {
    VOX__buffer p_title;
    u32 p_width;
    u32 p_height;
    u16 p_flags; // currently unused
} VOX__window_configuration;

typedef struct VOX__graphics {
    SDL_Window* p_window_context;
    SDL_GLContext p_sdl2_opengl_context;
} VOX__graphics;

VOX__graphics VOX__create_null__graphics() {
    VOX__graphics output;

    // setup output
    output.p_window_context = 0;
    output.p_sdl2_opengl_context = 0;

    return output;
}

VOX__window_configuration VOX__create__window_configuration(VOX__buffer title_string_pointer_no_null, u32 width, u32 height, u16 flags) {
    VOX__window_configuration output;

    // setup output
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

/* Shaders - Programming The GPU */
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

    // setup output
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
            *error = VOX__create__error__vertex_shader_compilation_failure(opengl_error_log, glGetError());
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            *error = VOX__create__error__fragment_shader_compilation_failure(opengl_error_log, glGetError());
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

    if (error_log_length > 1) {
        opengl_error_log = VOX__create__allocation(sizeof(char) * VOX__dt__opengl_error_info_log_length);

        glGetProgramInfoLog(output.p_program_ID, VOX__dt__opengl_error_info_log_length, NULL, opengl_error_log);

        *error = VOX__create__error__shader_linking_failure(opengl_error_log, glGetError());
    }

    return output;
}

void VOX__use__use_shaders(VOX__shaders_program shaders_program) {
    glUseProgram(shaders_program.p_program_ID);

    return;
}

void VOX__close__shaders(VOX__shaders_program shaders_program) {
    glDeleteShader(shaders_program.p_vertex_shader.p_shader_ID);
    glDeleteShader(shaders_program.p_fragment_shader.p_shader_ID);
    glDeleteProgram(shaders_program.p_program_ID);

    return;
}

/* 2D Positions - 2D Coordinate Datum */
typedef struct VOX__2D_position {
    f32 p_x;
    f32 p_y;
} VOX__2D_position;

VOX__2D_position VOX__create__2D_position(f32 x, f32 y) {
    VOX__2D_position output;

    // setup output
    output.p_x = x;
    output.p_y = y;

    return output;
}

/* 3D Positions - 3D Coordinate Datum */
typedef struct VOX__3D_position {
    f32 p_x;
    f32 p_y;
    f32 p_z;
} VOX__3D_position;

VOX__3D_position VOX__create__3D_position(f32 x, f32 y, f32 z) {
    VOX__3D_position output;

    // setup output
    output.p_x = x;
    output.p_y = y;
    output.p_z = z;

    return output;
}

/* Vertex - One OpenGL Vertex */
typedef GLfloat VOX__texture_number_type;

typedef struct VOX__vbo_vertex {
    VOX__3D_position p_positional_coordinate;
    VOX__2D_position p_texture_coordinate;
    VOX__texture_number_type p_texture_number;
} VOX__vbo_vertex;

VOX__vbo_vertex VOX__create__vbo_vertex(VOX__3D_position positional_coordinate, VOX__2D_position texture_coordinate, VOX__texture_number_type texture_number) {
    VOX__vbo_vertex output;

    // setup output
    output.p_positional_coordinate = positional_coordinate;
    output.p_texture_coordinate = texture_coordinate;
    output.p_texture_number = texture_number;

    return output;
}

void VOX__send__vbo_attributes() {
    // physical positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VOX__vbo_vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // texture positions
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VOX__vbo_vertex), (void*)sizeof(VOX__3D_position));
    glEnableVertexAttribArray(1);

    // texture numbers
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VOX__vbo_vertex), (void*)(sizeof(VOX__3D_position) + sizeof(VOX__2D_position)));
    glEnableVertexAttribArray(2);

    return;
}

/* Vertices - Multiple OpenGL Vertices */
typedef struct VOX__vbo_vertices {
    VOX__buffer p_vertices;
    u64 p_vertex_count;
} VOX__vbo_vertices;

VOX__vbo_vertices VOX__create_null__vbo_vertices() {
    VOX__vbo_vertices output;

    // setup output
    output.p_vertices = VOX__create__buffer__add_address(0, 0);
    output.p_vertex_count = 0;

    return output;
}

VOX__vbo_vertices VOX__create__vbo_vertices(u64 vertex_count) {
    VOX__vbo_vertices output;

    // setup output
    output.p_vertices = VOX__create__buffer(sizeof(VOX__vbo_vertex) * vertex_count);
    output.p_vertex_count = vertex_count;

    return output;
}

void VOX__write__vbo_vertex_to_vbo_vertices(VOX__vbo_vertices vbo_vertices, u64 index, VOX__vbo_vertex vbo_vertex) {
    ((VOX__vbo_vertex*)vbo_vertices.p_vertices.p_data)[index] = vbo_vertex;

    return;
}

VOX__vbo_vertex VOX__read__vbo_vertex_from_vbo_vertices(VOX__vbo_vertices vbo_vertices, u64 index) {
    return ((VOX__vbo_vertex*)vbo_vertices.p_vertices.p_data)[index];
}

void VOX__draw__send__vbo_vertices_to_gpu(VOX__vbo_vertices vbo_vertecies) {
    glBufferData(GL_ARRAY_BUFFER, vbo_vertecies.p_vertices.p_length, vbo_vertecies.p_vertices.p_data, GL_DYNAMIC_DRAW);

    return;
}

void VOX__destroy__vbo_vertices(VOX__vbo_vertices vbo_vertices) {
    VOX__destroy__buffer(vbo_vertices.p_vertices);

    return;
}

/* Element - One OpenGL Elements Buffer Vertex */
typedef GLushort VOX__ebo_index;

typedef struct VOX__ebo_vertex {
    VOX__ebo_index p_index;
} VOX__ebo_vertex;

VOX__ebo_vertex VOX__create__ebo_vertex(VOX__ebo_index index) {
    VOX__ebo_vertex output;

    // setup output
    output.p_index = index;

    return output;
}

/* Elements - Multiple Element Vertices */
typedef struct VOX__ebo_vertices {
    VOX__buffer p_elements;
    u64 p_element_count;
} VOX__ebo_vertices;

VOX__ebo_vertices VOX__create_null__ebo_vertices() {
    VOX__ebo_vertices output;

    // setup output
    output.p_elements = VOX__create__buffer__add_address(0, 0);
    output.p_element_count = 0;

    return output;
}

VOX__ebo_vertices VOX__create__ebo_vertices(u64 element_count) {
    VOX__ebo_vertices output;

    // setup output
    output.p_elements = VOX__create__buffer(sizeof(VOX__ebo_vertex) * element_count);
    output.p_element_count = element_count;

    return output;
}

void VOX__write__ebo_vertex_to_ebo_vertices(VOX__ebo_vertices ebo_vertices, u64 index, VOX__ebo_vertex ebo_vertex) {
    ((VOX__ebo_vertex*)ebo_vertices.p_elements.p_data)[index] = ebo_vertex;

    return;
}

VOX__ebo_vertex VOX__read__ebo_vertex_from_ebo_vertices(VOX__ebo_vertices ebo_vertices, u64 index) {
    return ((VOX__ebo_vertex*)ebo_vertices.p_elements.p_data)[index];
}

void VOX__draw__send__ebo_vertices_to_gpu(VOX__ebo_vertices ebo_vertecies) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_vertecies.p_elements.p_length, ebo_vertecies.p_elements.p_data, GL_DYNAMIC_DRAW);

    return;
}

void VOX__destroy__ebo_vertices(VOX__ebo_vertices ebo_vertices) {
    VOX__destroy__buffer(ebo_vertices.p_elements);

    return;
}

/* Object Datum - One Drawable Piece's Data */
typedef struct VOX__object_datum {
    VOX__vbo_vertices p_vertices;
    VOX__ebo_vertices p_elements;
} VOX__object_datum;

VOX__object_datum VOX__create_null__object_datum() {
    VOX__object_datum output;

    // setup output
    output.p_vertices = VOX__create_null__vbo_vertices();
    output.p_elements = VOX__create_null__ebo_vertices();

    return output;
}

void VOX__destroy__object_datum(VOX__object_datum object_datum) {
    VOX__destroy__vbo_vertices(object_datum.p_vertices);
    VOX__destroy__ebo_vertices(object_datum.p_elements);
    
    return;
}

/* Object Data - Multiple Object Pieces as One Object */
typedef struct VOX__object_data {
    VOX__buffer p_datums;
    u64 p_datums_count;
} VOX__object_data;

VOX__object_data VOX__create_null__object_data() {
    VOX__object_data output;

    // setup output
    output.p_datums = VOX__create__buffer__add_address(0, 0);
    output.p_datums_count = 0;

    return output;
}

VOX__object_data VOX__create__object_data(u64 datum_count) {
    VOX__object_data output;

    // setup output
    output.p_datums = VOX__create__buffer(sizeof(VOX__object_datum) * datum_count);
    output.p_datums_count = datum_count;

    return output;
}

void VOX__write__object_datum_to_object_data(VOX__object_data object_data, u64 index, VOX__object_datum object_datum) {
    ((VOX__object_datum*)object_data.p_datums.p_data)[index] = object_datum;

    return;
}

VOX__object_datum VOX__read__object_datum_from_object_data(VOX__object_data object_data, u64 index) {
    return ((VOX__object_datum*)object_data.p_datums.p_data)[index];
}

void VOX__destroy__object_data(VOX__object_data object_data) {
    // clear datums buffer
    for (u64 i = 0; i < object_data.p_datums_count; i++) {
        VOX__destroy__object_datum(VOX__read__object_datum_from_object_data(object_data, i));
    }

    // destroy datum buffer itself
    VOX__destroy__buffer(object_data.p_datums);

    return;
}

/* OpenGL Object Handle - One OpenGL Vertex Array Object Handle & Multiple OpenGL Buffer Handles */
typedef struct VOX__opengl_object_handle {
    VOX__buffer p_vbos;
    VOX__buffer p_ebos;
    u64 p_ebos_vbos_count; // ebos count = vbos count
    GLuint p_vao;
} VOX__opengl_object_handle;

VOX__opengl_object_handle VOX__create_null__opengl_object_handle() {
    VOX__opengl_object_handle output;

    // setup output
    output.p_vbos = VOX__create__buffer__add_address(0, 0);
    output.p_ebos = VOX__create__buffer__add_address(0, 0);
    output.p_ebos_vbos_count = 0;
    output.p_vao = 0;

    return output;
}

VOX__opengl_object_handle VOX__open__opengl_object_handle(u64 expected_buffer_pairs_count) {
    VOX__opengl_object_handle output;
    VOX__buffer joint_allocation;
    u64 shared_buffer_length = expected_buffer_pairs_count * sizeof(GLuint);

    // setup output
    output = VOX__create_null__opengl_object_handle();

    // create joint allocation
    joint_allocation = VOX__create__buffer(shared_buffer_length * 2);

    // setup allocations
    output.p_vbos = VOX__create__buffer__add_address(joint_allocation.p_data, shared_buffer_length);
    output.p_ebos = VOX__create__buffer__add_address(joint_allocation.p_data + shared_buffer_length, shared_buffer_length);

    // setup buffer pairs count
    output.p_ebos_vbos_count = expected_buffer_pairs_count;

    // initialize opengl buffers
    glGenVertexArrays(1, &(output.p_vao));
    glGenBuffers(expected_buffer_pairs_count, output.p_vbos.p_data);
    glGenBuffers(expected_buffer_pairs_count, output.p_ebos.p_data);

    return output;
}

void VOX__draw__bind__vbo_and_ebo(VOX__opengl_object_handle opengl_object_handle, u64 index) {
    glBindBuffer(GL_ARRAY_BUFFER, ((GLuint*)opengl_object_handle.p_vbos.p_data)[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLuint*)opengl_object_handle.p_ebos.p_data)[index]);

    return;
}

void VOX__draw__unbind__vbo_and_ebo(VOX__opengl_object_handle opengl_object_handle, u64 index) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return;
}


void VOX__draw__send__vbo_and_ebo_vertices_to_gpu(VOX__vbo_vertices vbo_vertices, VOX__ebo_vertices ebo_vertices) {
    VOX__draw__send__vbo_vertices_to_gpu(vbo_vertices);
    VOX__draw__send__ebo_vertices_to_gpu(ebo_vertices);

    return;
}

void VOX__draw__bind__vao(VOX__opengl_object_handle opengl_object_handle) {
    glBindVertexArray(opengl_object_handle.p_vao);

    return;
}

void VOX__draw__unbind__vao(VOX__opengl_object_handle opengl_object_handle) {
    glBindVertexArray(0);

    return;
}

void VOX__close__opengl_object_handle(VOX__opengl_object_handle opengl_object_handle) {
    // deallocate opengl side
    glDeleteBuffers(opengl_object_handle.p_ebos_vbos_count, opengl_object_handle.p_ebos.p_data);
    glDeleteBuffers(opengl_object_handle.p_ebos_vbos_count, opengl_object_handle.p_vbos.p_data);
    glDeleteVertexArrays(1, &opengl_object_handle.p_vao);

    // ebos are jointly allocated with vbos
    VOX__destroy__buffer(opengl_object_handle.p_vbos);

    return;
}

/* Drawable Object - OpenGL Object RAM Copy & OpenGL Object Handles - For One Object */
typedef struct VOX__drawable_object {
    VOX__object_data p_object_data;
    VOX__opengl_object_handle p_handle;
} VOX__drawable_object;

VOX__drawable_object VOX__open__drawable_object__object_data(VOX__object_data object_data) {
    VOX__drawable_object output;

    // setup object and data
    output.p_object_data = object_data;
    output.p_handle = VOX__open__opengl_object_handle(object_data.p_datums_count);

    return output;
}

void VOX__send__drawable_object_to_opengl(VOX__drawable_object drawable_object) {
    // bind vao
    VOX__draw__bind__vao(drawable_object.p_handle);

    // send each individual buffer
    for (u64 i = 0; i < drawable_object.p_handle.p_ebos_vbos_count; i++) {
        // bind current vbo and ebo buffers
        VOX__draw__bind__vbo_and_ebo(drawable_object.p_handle, i);

        // tell gpu layout of vertices
        VOX__send__vbo_attributes();

        // send current vbo and ebo buffers
        VOX__draw__send__vbo_and_ebo_vertices_to_gpu(VOX__read__object_datum_from_object_data(drawable_object.p_object_data, i).p_vertices, VOX__read__object_datum_from_object_data(drawable_object.p_object_data, i).p_elements);

        // unbind current vbo and ebo buffers
        VOX__draw__unbind__vbo_and_ebo(drawable_object.p_handle, i);
    }

    // unbind vao
    VOX__draw__unbind__vao(drawable_object.p_handle);

    return;
}

void VOX__draw__drawable_object(VOX__drawable_object drawable_object) {
    // bind vao
    VOX__draw__bind__vao(drawable_object.p_handle);

    // draw each individual buffer
    for (u64 i = 0; i < drawable_object.p_handle.p_ebos_vbos_count; i++) {
        // bind current vbo and ebo buffers
        VOX__draw__bind__vbo_and_ebo(drawable_object.p_handle, i);

        // tell gpu layout of vertices
        VOX__send__vbo_attributes();

        // draw current vbo and ebo buffers
        glDrawElements(GL_TRIANGLES, ((VOX__object_datum*)drawable_object.p_object_data.p_datums.p_data)[i].p_elements.p_element_count, GL_UNSIGNED_SHORT, 0);

        // unbind current vbo and ebo buffers
        VOX__draw__unbind__vbo_and_ebo(drawable_object.p_handle, i);
    }

    // unbind vao
    VOX__draw__unbind__vao(drawable_object.p_handle);

    return;
}

void VOX__close__drawable_object(VOX__drawable_object drawable_object) {
    VOX__close__opengl_object_handle(drawable_object.p_handle);

    return;
}

/* 2D Texture Array */
typedef struct VOX__2D_texture_array {
    VOX__buffer p_textures;
    u64 p_single_texture_width;
    u64 p_single_texture_height;
    u64 p_texel_byte_count;
    u64 p_texture_count;
    GLenum p_texture_type;
} VOX__2D_texture_array;

VOX__2D_texture_array VOX__create__2D_texture_array(u64 texture_count, u64 single_texture_width, u64 single_texture_height, u64 texel_byte_count, GLenum texture_type) {
    VOX__2D_texture_array output;

    // setup textures buffer
    output.p_textures = VOX__create__buffer(texture_count * single_texture_width * single_texture_height * texel_byte_count);

    // setup other data
    output.p_single_texture_width = single_texture_width;
    output.p_single_texture_height = single_texture_height;
    output.p_texel_byte_count = texel_byte_count;
    output.p_texture_count = texture_count;
    output.p_texture_type = texture_type;

    return output;
}

u64 VOX__calculate__2D_texture_offset_in_2D_texture_array(VOX__2D_texture_array texture_array, u64 index) {
    return texture_array.p_single_texture_height * texture_array.p_single_texture_width * texture_array.p_texel_byte_count * index;
}

void VOX__write__2D_texture_data_to_2D_texture_array(VOX__2D_texture_array texture_array, VOX__buffer texture_data, u64 index) {
    VOX__copy__bytes_to_bytes(texture_data.p_data, texture_data.p_length, texture_array.p_textures.p_data + VOX__calculate__2D_texture_offset_in_2D_texture_array(texture_array, index));

    return;
}

void VOX__destroy__2D_texture_array(VOX__2D_texture_array texture_array) {
    VOX__destroy__buffer(texture_array.p_textures);

    return;
}

/* Game Textures */
// game textures type
typedef enum VOX__gtt {
    VOX__gtt__block_faces
} VOX__gtt;

typedef struct VOX__game_textures {
    // texture data
    VOX__2D_texture_array p_block_faces;

    // texture handles
    GLuint p_block_faces_handle;

    // active textures
    GLenum p_block_faces_texture_ID;
} VOX__game_textures;

VOX__game_textures VOX__open__game_textures(VOX__2D_texture_array block_faces) {
    VOX__game_textures output;

    // setup texture data
    output.p_block_faces = block_faces;

    // setup texture handles
    glCreateTextures(block_faces.p_texture_type, 1, &(output.p_block_faces_handle));

    // setup active textures
    output.p_block_faces_texture_ID = GL_TEXTURE0 + 0;

    return output;
}

void VOX__draw__bind__specific_game_textures_texture(VOX__game_textures game_textures, VOX__gtt game_textures_type) {
    switch (game_textures_type) {
    case VOX__gtt__block_faces:
        glActiveTexture(game_textures.p_block_faces_texture_ID);
        glBindTexture(game_textures.p_block_faces.p_texture_type, game_textures.p_block_faces_handle);
        break;
    }
}

void VOX__draw__unbind__specific_game_textures_texture(VOX__game_textures game_textures, VOX__gtt game_textures_type) {
    switch (game_textures_type) {
    case VOX__gtt__block_faces:
        glBindTexture(game_textures.p_block_faces.p_texture_type, 0);
        break;
    }
}

void VOX__send__game_textures_to_opengl(VOX__game_textures game_textures) {
    // send block textures
    VOX__draw__bind__specific_game_textures_texture(game_textures, VOX__gtt__block_faces);
    glTexParameteri(game_textures.p_block_faces.p_texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(game_textures.p_block_faces.p_texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(game_textures.p_block_faces.p_texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(game_textures.p_block_faces.p_texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(game_textures.p_block_faces.p_texture_type, 0, GL_RGBA8, game_textures.p_block_faces.p_single_texture_width, game_textures.p_block_faces.p_single_texture_height, game_textures.p_block_faces.p_texture_count, 0, GL_RGBA, GL_UNSIGNED_BYTE, game_textures.p_block_faces.p_textures.p_data);
    VOX__draw__unbind__specific_game_textures_texture(game_textures, VOX__gtt__block_faces);

    return;
}

void VOX__close__game_textures(VOX__game_textures game_textures) {
    // destroy data on cpu
    VOX__destroy__2D_texture_array(game_textures.p_block_faces);

    // destroy data on gpu
    glDeleteTextures(1, &(game_textures.p_block_faces_handle));

    return;
}

/* Blocks */
typedef u16 VOX__block_ID;

typedef struct VOX__block {
    VOX__block_ID p_ID;
} VOX__block;

typedef struct VOX__block_dictionary_entry {
    GLenum p_top_texture_ID;
    GLenum p_bottom_texture_ID;
    GLenum p_left_texture_ID;
    GLenum p_right_texture_ID;
    GLenum p_front_texture_ID;
    GLenum p_back_texture_ID;
} VOX__block_dictionary_entry;

typedef struct VOX__block_dictionary {
    VOX__buffer p_dictionary;
    u64 p_entry_count;
} VOX__block_dictionary;

/* Chunks */
typedef struct VOX__chunk {
    VOX__buffer p_blocks;
} VOX__chunk;

/* Camera */
typedef struct VOX__camera {
    f32 p_yaw;
    f32 p_pitch;
    vec3 p_position;
    versor p_angle;
    mat4 p_matrix_identity;
    mat4 p_model;
    mat4 p_view;
    mat4 p_projection;
    mat4 p_change;
} VOX__camera;

VOX__camera VOX__create__camera() {
    VOX__camera output;

    // camera starting position
    output.p_yaw = 0.0f;
    output.p_pitch = 0.0f;
    glm_vec3_zero(output.p_position);
    glm_quat_identity(output.p_angle);
    glm_mat4_identity(output.p_matrix_identity);
    glm_mat4_identity(output.p_model);
    glm_mat4_identity(output.p_view);
    glm_mat4_identity(output.p_projection);
    glm_mat4_identity(output.p_change);

    return output;
}

VOX__camera VOX__move__camera(VOX__camera camera, VOX__3D_position camera_position_change, VOX__3D_position camera_rotation_change) {
    vec3 direction;
    vec3 front;

    // adjust camera data
    camera.p_position[0] += camera_position_change.p_x;
    camera.p_position[1] += camera_position_change.p_y;
    camera.p_position[2] += camera_position_change.p_z;
    camera.p_yaw = camera_rotation_change.p_x;
    camera.p_pitch = camera_rotation_change.p_y;

    // calculate direction
    direction[0] = cosf(glm_rad(camera.p_yaw)) * cosf(glm_rad(camera.p_pitch));
    direction[1] = sinf(glm_rad(camera.p_pitch));
    direction[2] = sinf(glm_rad(camera.p_yaw)) * cosf(glm_rad(camera.p_pitch));

    // normalize direction
    glm_vec3_norm(direction);

    // calculate camera matrices
    if (camera_rotation_change.p_x != 0) {
        glm_quatv(camera.p_angle, glm_rad(camera.p_yaw), (vec3){ 1.0f, 0.0f, 0.0f });
        glm_quat_rotate(camera.p_model, camera.p_angle, camera.p_model);
    }
    if (camera_rotation_change.p_y != 0) {
        glm_quatv(camera.p_angle, glm_rad(camera.p_pitch), (vec3){ 0.0f, 1.0f, 0.0f });
        glm_quat_rotate(camera.p_model, camera.p_angle, camera.p_model);
    }
    glm_vec3_add(camera.p_position, (vec3){ 0.0f, 0.0f, -1.0f }, front);
    glm_lookat(camera.p_position, front, (vec3){ 0.0f, 1.0f, 0.0f }, camera.p_view);
    glm_perspective(glm_rad(45.0f), 720.0 / 480.0, 0.1f, 100.0f, camera.p_projection);
    
    // prepare matrices for gpu
    glm_mat4_mul(camera.p_projection, camera.p_view, camera.p_change);
    glm_mat4_mul(camera.p_change, camera.p_model, camera.p_change);

    // return changed camera
    return camera;
}

/* Testing - Functions Testing Code */
VOX__object_datum VOX__create__test__object_datum__square(f32 scale, f32 x_screen_offset, f32 y_screen_offset, f32 z_screen_offset, VOX__texture_number_type texture_number) {
    VOX__object_datum output;

    // setup output
    output = VOX__create_null__object_datum();

    // setup vertex & element buffers
    output.p_vertices = VOX__create__vbo_vertices(4);
    output.p_elements = VOX__create__ebo_vertices(6);

    // fill vbo buffer
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 0, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset, z_screen_offset), VOX__create__2D_position(0.0f, 0.0f), texture_number));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 1, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + scale, y_screen_offset, z_screen_offset), VOX__create__2D_position(1.0f, 0.0f), texture_number));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 2, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset + scale, z_screen_offset), VOX__create__2D_position(0.0f, 1.0f), texture_number));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 3, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + scale, y_screen_offset + scale, z_screen_offset), VOX__create__2D_position(1.0f, 1.0f), texture_number));

    // fill ebo buffer
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 0, VOX__create__ebo_vertex(0));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 1, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 2, VOX__create__ebo_vertex(2));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 3, VOX__create__ebo_vertex(3));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 4, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 5, VOX__create__ebo_vertex(2));

    return output;
}

VOX__object_data VOX__create__test__object_data__2_squares(f32 scale, f32 x_screen_offset, f32 y_screen_offset, f32 z_screen_offset) {
    VOX__object_data output;

    // setup sides
    output = VOX__create__object_data(2);

    // write sides
    VOX__write__object_datum_to_object_data(output, 0, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset, 0.0f));
    VOX__write__object_datum_to_object_data(output, 1, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + scale, 1.0f));

    return output;
}

VOX__object_data VOX__create__test__object_data__6_squares(f32 scale, f32 x_screen_offset, f32 y_screen_offset, f32 z_screen_offset) {
    VOX__object_data output;

    // setup sides
    output = VOX__create__object_data(6);

    // write sides
    VOX__write__object_datum_to_object_data(output, 0, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 0.0f), 0.0f));
    VOX__write__object_datum_to_object_data(output, 1, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 1.0f), 1.0f));
    VOX__write__object_datum_to_object_data(output, 2, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 2.0f), 2.0f));
    VOX__write__object_datum_to_object_data(output, 3, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 3.0f), 3.0f));
    VOX__write__object_datum_to_object_data(output, 4, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 4.0f), 4.0f));
    VOX__write__object_datum_to_object_data(output, 5, VOX__create__test__object_datum__square(scale, x_screen_offset, y_screen_offset, z_screen_offset + ((scale / 6.0f) * 5.0f), 5.0f));

    return output;
}

VOX__object_data VOX__create__test__object_data_from_object_datum(VOX__object_datum object_datum) {
    VOX__object_data output;

    // setup output
    output = VOX__create__object_data(1);

    // write datum
    VOX__write__object_datum_to_object_data(output, 0, object_datum);

    return output;
}

VOX__game_textures VOX__create__test__game_textures__1() {
    VOX__2D_texture_array block_faces;
    // blue
    u8 test_texture_1[] = {
        0, 0, 0, 255,
        0, 0, 255, 255,
        0, 0, 255, 255,
        0, 0, 255, 255
    };
    // green
    u8 test_texture_2[] = {
        0, 0, 0, 255,
        0, 255, 0, 255,
        0, 255, 0, 255,
        0, 255, 0, 255
    };
    // red
    u8 test_texture_3[] = {
        0, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255,
        255, 0, 0, 255
    };
    // purple
    u8 test_texture_4[] = {
        0, 0, 0, 255,
        255, 0, 255, 255,
        255, 0, 255, 255,
        255, 0, 255, 255
    };
    // indigo
    u8 test_texture_5[] = {
        0, 0, 0, 255,
        0, 255, 255, 255,
        0, 255, 255, 255,
        0, 255, 255, 255
    };
    // yellow
    u8 test_texture_6[] = {
        0, 0, 0, 255,
        255, 255, 0, 255,
        255, 255, 0, 255,
        255, 255, 0, 255
    };

    // setup block faces
    block_faces = VOX__create__2D_texture_array(6, 2, 2, 4, GL_TEXTURE_2D_ARRAY);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_1, sizeof(test_texture_1)), 0);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_2, sizeof(test_texture_2)), 1);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_3, sizeof(test_texture_3)), 2);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_4, sizeof(test_texture_4)), 3);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_5, sizeof(test_texture_5)), 4);
    VOX__write__2D_texture_data_to_2D_texture_array(block_faces, VOX__create__buffer__add_address((void*)&test_texture_6, sizeof(test_texture_6)), 5);

    return VOX__open__game_textures(block_faces);
}

VOX__shaders_program VOX__create__test__shaders_program__1(VOX__error* error) {
    VOX__shaders_program output;
    VOX__buffer vertex_shader;
    VOX__buffer fragment_shader;

    // create code
    vertex_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nlayout (location = 0) in vec3 l_position_attribute;\nlayout (location = 1) in vec2 l_texture_position_attribute;\nlayout (location = 2) in uint l_texture_number_attribute;\nout vec2 pass_texture_coordinates;\nflat out uint pass_texture_number;\nvoid main() {\n\tpass_texture_coordinates = l_texture_position_attribute;\n\tpass_texture_number = l_texture_number_attribute;\n\tgl_Position = vec4(l_position_attribute, 1.0f);\n}");
    fragment_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nin vec2 pass_texture_coordinates;\nflat in uint pass_texture_number;\nuniform usampler2DArray u_sampler_2D_array;\nout vec4 pass_fragment_color;\nvoid main() {\n\tpass_fragment_color = texture(u_sampler_2D_array, vec3(pass_texture_coordinates, float(pass_texture_number)));\n}");

    // compile shaders
    output = VOX__compile__shaders_program(error, vertex_shader, fragment_shader);

    // destroy code
    VOX__destroy__buffer(vertex_shader);
    VOX__destroy__buffer(fragment_shader);

    return output;
}

VOX__shaders_program VOX__create__test__shaders_program__playground(VOX__error* error) {
    VOX__shaders_program output;
    VOX__buffer vertex_shader;
    VOX__buffer fragment_shader;

    // create code
    //vertex_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nlayout (location = 0) in vec3 l_position_attribute;\nlayout (location = 1) in vec2 l_texture_position_attribute;\nlayout (location = 2) in uint l_texture_number_attribute;\nuniform mat4 u_projection;\nuniform mat4 u_view;\nuniform mat4 u_model;\nout vec2 pass_texture_coordinates;\nflat out uint pass_texture_number;\nvoid main() {\n\tpass_texture_coordinates = l_texture_position_attribute;\n\tpass_texture_number = l_texture_number_attribute;\n\tgl_Position = u_projection * u_view * u_model * vec4(l_position_attribute, 1.0f);\n}");
    //fragment_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nin vec2 pass_texture_coordinates;\nflat in uint pass_texture_number;\nuniform usampler2DArray u_sampler_2D_array;\nout vec4 pass_fragment_color;\nvoid main() {\n\tpass_fragment_color = texture(u_sampler_2D_array, vec3(pass_texture_coordinates, float(pass_texture_number)));\n}");
    vertex_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nlayout (location = 0) in vec3 l_position_attribute;\nlayout (location = 1) in vec2 l_texture_position_attribute;\nlayout (location = 2) in float l_texture_number_attribute;\nuniform mat4 u_camera;\nout vec3 pass_texture_coordinates;\nvoid main() {\n\tpass_texture_coordinates = vec3(l_texture_position_attribute, l_texture_number_attribute);\n\tgl_Position = u_camera * vec4(l_position_attribute, 1.0f);\n}");
    fragment_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nin vec3 pass_texture_coordinates;\nuniform usampler2DArray u_sampler_2D_array;\nout vec4 pass_fragment_color;\nvoid main() {\n\tpass_fragment_color = texture(u_sampler_2D_array, pass_texture_coordinates);\n}");
    
    // compile shaders
    output = VOX__compile__shaders_program(error, vertex_shader, fragment_shader);

    // destroy code
    VOX__destroy__buffer(vertex_shader);
    VOX__destroy__buffer(fragment_shader);

    return output;
}

/* Events - User Input */
typedef struct VOX__user_input {
    f32 p_mouse_x_change;
    f32 p_mouse_y_change;
    VOX__bt p_quit;
    VOX__bt p_w;
    VOX__bt p_a;
    VOX__bt p_s;
    VOX__bt p_d;
} VOX__user_input;

VOX__user_input VOX__create_null__user_input() {
    VOX__user_input output;

    // setup mouse input
    output.p_mouse_x_change = 0.0f;
    output.p_mouse_y_change = 0.0f;

    // zero flags
    output.p_quit = VOX__bt__false;
    output.p_w = VOX__bt__false;
    output.p_a = VOX__bt__false;
    output.p_s = VOX__bt__false;
    output.p_d = VOX__bt__false;

    return output;
}

VOX__user_input VOX__create__user_input__from_sdl2_events() {
    VOX__user_input output;
    SDL_Event e;
    SDL_Event* e_pointer;
    s32 temp_x;
    s32 temp_y;

    // setup variables
    output = VOX__create_null__user_input();
    e_pointer = &e;

    // get mouse position
    SDL_GetRelativeMouseState(&temp_x, &temp_y);
    output.p_mouse_x_change = (f32)temp_x;
    output.p_mouse_y_change = (f32)temp_y;

    // get all events
    while (SDL_PollEvent(e_pointer)) {
        if (e.type == SDL_QUIT) {
            output.p_quit = VOX__bt__true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_w) {
                output.p_w = VOX__bt__true;
            }
            if (e.key.keysym.sym == SDLK_s) {
                output.p_s = VOX__bt__true;
            }
            if (e.key.keysym.sym == SDLK_a) {
                output.p_a = VOX__bt__true;
            }
            if (e.key.keysym.sym == SDLK_d) {
                output.p_d = VOX__bt__true;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_w) {
                output.p_w = VOX__bt__false;
            }
            if (e.key.keysym.sym == SDLK_s) {
                output.p_s = VOX__bt__false;
            }
            if (e.key.keysym.sym == SDLK_a) {
                output.p_a = VOX__bt__false;
            }
            if (e.key.keysym.sym == SDLK_d) {
                output.p_d = VOX__bt__false;
            }
        }
    }

    return output;
}

VOX__3D_position VOX__calculate__player_position_movement(VOX__user_input user_input, f32 speed) {
    VOX__3D_position output;

    // setup output
    output = VOX__create__3D_position(0.0f, 0.0f, 0.0f);

    // adjust output by player movement
    if (user_input.p_w == VOX__bt__true) {
        output.p_z = speed;
    }
    if (user_input.p_a == VOX__bt__true) {
        output.p_x = speed;
    }
    if (user_input.p_s == VOX__bt__true) {
        output.p_z = speed * -1.0f;
    }
    if (user_input.p_d == VOX__bt__true) {
        output.p_x = speed * -1.0f;
    }

    return output;
}

VOX__3D_position VOX__calculate__player_camera_rotation_movement(VOX__user_input user_input, f32 speed) {
    return VOX__create__3D_position((user_input.p_mouse_x_change) * speed, (user_input.p_mouse_y_change) * speed, 0.0f);
}

/* Game Loop - Run Actual Game */
void VOX__play(VOX__error* error) {
    VOX__window_configuration configuration;
    VOX__graphics graphics;
    VOX__buffer title;
    VOX__user_input user_input;
    VOX__shaders_program shaders_program;
    VOX__game_textures game_textures;
    VOX__camera camera;
    VOX__drawable_object pattern;
    f32 affect;

    // setup title
    title = VOX__create__buffer_copy_from_c_string("Voxelize!");

    // setup window configuration
    configuration = VOX__create__window_configuration(title, 720, 480, 0);

    // open window
    graphics = VOX__open__graphics__new_window(error, configuration);

    // setup opengl shaders
    shaders_program = VOX__create__test__shaders_program__playground(error);
    if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
        goto VOX__label__quit_game__shader_failure;
    }

    // user shaders
    VOX__use__use_shaders(shaders_program);

    // setup textures
    game_textures = VOX__create__test__game_textures__1();

    // setup camera
    camera = VOX__create__camera();
    affect = 0.0f;

    // create data
    pattern = VOX__open__drawable_object__object_data(VOX__create__test__object_data__6_squares(1.0f, 0.0f, 0.0f, 0.0f));

    // send data to gpu
    VOX__send__game_textures_to_opengl(game_textures);
    VOX__send__drawable_object_to_opengl(pattern);

    // setup opengl drawing constants
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // run window
    while (VOX__bt__true) {
        // get keyboard input
        user_input = VOX__create__user_input__from_sdl2_events();
        
        // check if should quit
        if (user_input.p_quit == VOX__bt__true) {
            goto VOX__label__quit_game;
        }

        // clear window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup affects
        affect += 0.01f;

        // move camera
        camera = VOX__move__camera(camera, VOX__calculate__player_position_movement(user_input, 0.5f), VOX__calculate__player_camera_rotation_movement(user_input, 1.0f));

        // bind blocks
        VOX__draw__bind__specific_game_textures_texture(game_textures, VOX__gtt__block_faces);
        
        // pass camera and texture data to gpu
        glUniform1i(glGetUniformLocation(shaders_program.p_program_ID, "u_sampler_2D_array"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaders_program.p_program_ID, "u_camera"), 1, GL_FALSE, (const GLfloat*)&camera.p_change);
        
        // display square
        VOX__draw__drawable_object(pattern);
        VOX__draw__unbind__specific_game_textures_texture(game_textures, VOX__gtt__block_faces);

        *error = VOX__try_create__error__other_opengl();
        if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
            goto VOX__label__quit_game;
        }

        // display new buffer
        SDL_GL_SwapWindow(graphics.p_window_context);
    }

    VOX__label__quit_game:

    // clean up textures
    VOX__close__game_textures(game_textures);

    // clean up drawable objects
    VOX__destroy__object_data(pattern.p_object_data);
    VOX__close__drawable_object(pattern);

    VOX__label__quit_game__shader_failure:

    // clean up opengl
    VOX__close__shaders(shaders_program);
    VOX__close__graphics__old_window(graphics);
    VOX__destroy__window_configuration(configuration);

    // clean up extra buffers
    VOX__destroy__buffer(title);

    return;
}

#endif
