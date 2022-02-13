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
} VOX__error;

VOX__error VOX__create__error__no_error() {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__no_error;
    output.p_has_extra_data = VOX__bt__false;
    
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

VOX__error VOX__create__error__vertex_shader_compilation_failure(char* opengl_log) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__vertex_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__error VOX__create__error__fragment_shader_compilation_failure(char* opengl_log) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__fragment_shader_compilation_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__error VOX__create__error__shader_linking_failure(char* opengl_log) {
    VOX__error output;

    // setup output
    output.p_type = VOX__et__shader_linking_failure;
    output.p_has_extra_data = VOX__bt__true;
    output.p_opengl_log = opengl_log;
    
    return output;
}

VOX__error VOX__try_create__error__other_opengl() {
    VOX__error output;
    GLenum opengl_error;

    // check error
    opengl_error = glGetError();

    if (opengl_error != GL_NO_ERROR) {
        output.p_type = VOX__et__other_opengl;
        output.p_has_extra_data = VOX__bt__false;
    } else {
        return VOX__create__error__no_error();
    }

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

/* Copy Bytes */
void VOX__copy__bytes_to_bytes(void* source, unsigned long long length, void* destination) {
    for (unsigned long long i = 0; i < length; i++) {
        ((unsigned char*)destination)[i] = ((unsigned char*)source)[i];
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

    // setup output
    output.p_data = VOX__create__allocation(length);
    output.p_length = length;

    return output;
}

VOX__buffer VOX__create__buffer__add_address(void* address, unsigned long long length) {
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
    for (unsigned long long i = 0; i < buffer.p_length; i++) {
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

    // setup output
    output.p_window_context = 0;
    output.p_sdl2_opengl_context = 0;

    return output;
}

VOX__window_configuration VOX__create__window_configuration(VOX__buffer title_string_pointer_no_null, int width, int height, unsigned short flags) {
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

    if (error_log_length > 1) {
        opengl_error_log = VOX__create__allocation(sizeof(char) * VOX__dt__opengl_error_info_log_length);

        glGetProgramInfoLog(output.p_program_ID, VOX__dt__opengl_error_info_log_length, NULL, opengl_error_log);

        *error = VOX__create__error__shader_linking_failure(opengl_error_log);
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

/* 3D Positions */
typedef struct VOX__3D_position {
    float p_x;
    float p_y;
    float p_z;
} VOX__3D_position;

VOX__3D_position VOX__create__3D_position(float x, float y, float z) {
    VOX__3D_position output;

    // setup output
    output.p_x = x;
    output.p_y = y;
    output.p_z = z;

    return output;
}

/* Vertex */
typedef struct VOX__vbo_vertex {
    VOX__3D_position p_coordinate;
} VOX__vbo_vertex;

VOX__vbo_vertex VOX__create__vbo_vertex(VOX__3D_position coordinate) {
    VOX__vbo_vertex output;

    // setup output
    output.p_coordinate = coordinate;

    return output;
}

void VOX__send__vbo_attributes() {
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VOX__vbo_vertex), (void*)0);
    glEnableVertexAttribArray(0);

    return;
}

/* Vertices */
typedef struct VOX__vbo_vertices {
    VOX__buffer p_vertices;
    unsigned long long p_vertex_count;
} VOX__vbo_vertices;

VOX__vbo_vertices VOX__create_null__vbo_vertices() {
    VOX__vbo_vertices output;

    // setup output
    output.p_vertices = VOX__create__buffer__add_address(0, 0);
    output.p_vertex_count = 0;

    return output;
}

VOX__vbo_vertices VOX__create__vbo_vertices(unsigned long long vertex_count) {
    VOX__vbo_vertices output;

    // setup output
    output.p_vertices = VOX__create__buffer(sizeof(VOX__vbo_vertex) * vertex_count);
    output.p_vertex_count = vertex_count;

    return output;
}

void VOX__write__vbo_vertex_to_vbo_vertices(VOX__vbo_vertices vbo_vertices, unsigned long long index, VOX__vbo_vertex vbo_vertex) {
    ((VOX__vbo_vertex*)vbo_vertices.p_vertices.p_data)[index] = vbo_vertex;

    return;
}

VOX__vbo_vertex VOX__read__vbo_vertex_from_vbo_vertices(VOX__vbo_vertices vbo_vertices, unsigned long long index) {
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

/* Element */
typedef struct VOX__ebo_vertex {
    GLuint p_index;
} VOX__ebo_vertex;

VOX__ebo_vertex VOX__create__ebo_vertex(GLuint index) {
    VOX__ebo_vertex output;

    // setup output
    output.p_index = index;

    return output;
}

/* Elements */
typedef struct VOX__ebo_vertices {
    VOX__buffer p_elements;
    unsigned long long p_element_count;
} VOX__ebo_vertices;

VOX__ebo_vertices VOX__create_null__ebo_vertices() {
    VOX__ebo_vertices output;

    // setup output
    output.p_elements = VOX__create__buffer__add_address(0, 0);
    output.p_element_count = 0;

    return output;
}

VOX__ebo_vertices VOX__create__ebo_vertices(unsigned long long element_count) {
    VOX__ebo_vertices output;

    // setup output
    output.p_elements = VOX__create__buffer(sizeof(VOX__ebo_vertex) * element_count);
    output.p_element_count = element_count;

    return output;
}

void VOX__write__ebo_vertex_to_ebo_vertices(VOX__ebo_vertices ebo_vertices, unsigned long long index, VOX__ebo_vertex ebo_vertex) {
    ((VOX__ebo_vertex*)ebo_vertices.p_elements.p_data)[index] = ebo_vertex;

    return;
}

VOX__ebo_vertex VOX__read__ebo_vertex_from_ebo_vertices(VOX__ebo_vertices ebo_vertices, unsigned long long index) {
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

/* Object Datum */
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

/* Object Data */
typedef struct VOX__object_data {
    VOX__buffer p_datums;
    unsigned long long p_datums_count;
} VOX__object_data;

VOX__object_data VOX__create_null__object_data() {
    VOX__object_data output;

    // setup output
    output.p_datums = VOX__create__buffer__add_address(0, 0);
    output.p_datums_count = 0;

    return output;
}

VOX__object_data VOX__create__object_data(unsigned long long datum_count) {
    VOX__object_data output;

    // setup output
    output.p_datums = VOX__create__buffer(sizeof(VOX__object_datum) * datum_count);
    output.p_datums_count = datum_count;

    return output;
}

void VOX__write__object_datum_to_object_data(VOX__object_data object_data, unsigned long long index, VOX__object_datum object_datum) {
    ((VOX__object_datum*)object_data.p_datums.p_data)[index] = object_datum;

    return;
}

VOX__object_datum VOX__read__object_datum_from_object_data(VOX__object_data object_data, unsigned long long index) {
    return ((VOX__object_datum*)object_data.p_datums.p_data)[index];
}

void VOX__destroy__object_data(VOX__object_data object_data) {
    // clear datums buffer
    for (unsigned long long i = 0; i < object_data.p_datums_count; i++) {
        VOX__destroy__object_datum(VOX__read__object_datum_from_object_data(object_data, i));
    }

    // destroy datum buffer itself
    VOX__destroy__buffer(object_data.p_datums);

    return;
}

/* OpenGL Object Handle */
typedef struct VOX__opengl_object_handle {
    VOX__buffer p_vbos;
    VOX__buffer p_ebos;
    unsigned long long p_ebos_vbos_count; // ebos count = vbos count
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

VOX__opengl_object_handle VOX__open__opengl_object_handle(unsigned long long expected_buffer_pairs_count) {
    VOX__opengl_object_handle output;
    VOX__buffer joint_allocation;
    unsigned long long single_buffer_length = expected_buffer_pairs_count * sizeof(GLuint);

    // setup output
    output = VOX__create_null__opengl_object_handle();

    // crete joint allocation
    joint_allocation = VOX__create__buffer(single_buffer_length * 2);

    // setup allocations
    output.p_vbos = VOX__create__buffer__add_address(joint_allocation.p_data, single_buffer_length);
    output.p_ebos = VOX__create__buffer__add_address(joint_allocation.p_data + single_buffer_length, single_buffer_length);

    // setup buffer pairs count
    output.p_ebos_vbos_count = expected_buffer_pairs_count;

    // initialize opengl buffers
    glGenVertexArrays(1, &(output.p_vao));
    glCreateBuffers(expected_buffer_pairs_count, output.p_vbos.p_data);
    glCreateBuffers(expected_buffer_pairs_count, output.p_ebos.p_data);

    return output;
}

void VOX__draw__bind__vbo_and_ebo(VOX__opengl_object_handle opengl_object_handle, unsigned long long index) {
    glBindBuffer(GL_ARRAY_BUFFER, ((GLuint*)opengl_object_handle.p_vbos.p_data)[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLuint*)opengl_object_handle.p_ebos.p_data)[index]);

    return;
}

void VOX__draw__unbind__vbo_and_ebo(VOX__opengl_object_handle opengl_object_handle, unsigned long long index) {
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

/* Drawable Object */
typedef struct VOX__drawable_object {
    VOX__object_data p_object_data;
    VOX__opengl_object_handle p_handle;
} VOX__drawable_object;

VOX__drawable_object VOX__open__drawable_object(VOX__object_data object_data) {
    VOX__drawable_object output;

    // setup object and data
    output.p_object_data = object_data;
    output.p_handle = VOX__open__opengl_object_handle(1);

    return output;
}

void VOX__send__drawable_object_to_opengl(VOX__drawable_object drawable_object) {
    // bind vao
    VOX__draw__bind__vao(drawable_object.p_handle);

    // send each individual buffer
    for (unsigned long long i = 0; i < drawable_object.p_handle.p_ebos_vbos_count; i++) {
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
    for (unsigned long long i = 0; i < drawable_object.p_handle.p_ebos_vbos_count; i++) {
        // bind current vbo and ebo buffers
        VOX__draw__bind__vbo_and_ebo(drawable_object.p_handle, i);

        // draw current vbo and ebo buffers
        glDrawElements(GL_TRIANGLES, ((VOX__object_datum*)drawable_object.p_object_data.p_datums.p_data)[i].p_elements.p_element_count, GL_UNSIGNED_INT, 0);

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

/* Testing */
VOX__object_datum VOX__create__test__object_datum__square(float scale, float x_screen_offset, float y_screen_offset) {
    VOX__object_datum output;

    // setup output
    output = VOX__create_null__object_datum();

    // setup vertex & element buffers
    output.p_vertices = VOX__create__vbo_vertices(4);
    output.p_elements = VOX__create__ebo_vertices(6);

    // fill vbo buffer
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 0, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 1, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + scale, y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 2, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset + scale, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 3, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + scale, y_screen_offset + scale, 0.0f)));

    // fill ebo buffer
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 0, VOX__create__ebo_vertex(0));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 1, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 2, VOX__create__ebo_vertex(2));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 3, VOX__create__ebo_vertex(3));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 4, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 5, VOX__create__ebo_vertex(2));

    return output;
}

VOX__object_datum VOX__create__test__object_datum__pattern(float scale, float x_screen_offset, float y_screen_offset) {
    VOX__object_datum output;

    // setup output
    output = VOX__create_null__object_datum();

    // setup vertex & element buffers
    output.p_vertices = VOX__create__vbo_vertices(8);
    output.p_elements = VOX__create__ebo_vertices(12);

    // fill vbo buffer
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 0, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 1, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + (scale / 2.0f), y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 2, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset + (scale / 2.0f), 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 3, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + (scale / 2.0f), y_screen_offset + (scale / 2.0f), 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 4, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 5, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + (scale / 2.0f) + 0.5f, y_screen_offset, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 6, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset, y_screen_offset + (scale / 2.0f) + 0.5f, 0.0f)));
    VOX__write__vbo_vertex_to_vbo_vertices(output.p_vertices, 7, VOX__create__vbo_vertex(VOX__create__3D_position(x_screen_offset + (scale / 2.0f) + 0.5f, y_screen_offset + (scale / 2.0f) + 0.5f, 0.0f)));

    // fill ebo buffer
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 0, VOX__create__ebo_vertex(0));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 1, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 2, VOX__create__ebo_vertex(2));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 3, VOX__create__ebo_vertex(3));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 4, VOX__create__ebo_vertex(1));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 5, VOX__create__ebo_vertex(2));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 6, VOX__create__ebo_vertex(4));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 7, VOX__create__ebo_vertex(5));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 8, VOX__create__ebo_vertex(6));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 9, VOX__create__ebo_vertex(7));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 10, VOX__create__ebo_vertex(5));
    VOX__write__ebo_vertex_to_ebo_vertices(output.p_elements, 11, VOX__create__ebo_vertex(6));

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
    VOX__window_configuration configuration;
    VOX__graphics graphics;
    VOX__keyboard_input keyboard_input;
    VOX__buffer title;
    VOX__shaders_program shaders_program;
    VOX__buffer vertex_shader;
    VOX__buffer fragment_shader;
    VOX__drawable_object pattern;

    // setup title
    title.p_data = (void*)"Hello Cubic World!";
    title.p_length = 18;

    // setup shaders
    vertex_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nlayout (location = 0) in vec3 p_position_attribute;\nvoid main() {\n\tgl_Position = vec4(p_position_attribute, 1.0f);\n}");
    fragment_shader = VOX__create__buffer_copy_from_c_string("#version 330 core\nout vec4 p_fragment_color;\nvoid main() {\n\tp_fragment_color = vec4(1.0, 1.0, 0.0, 1.0);\n}");

    // setup congifuration
    configuration = VOX__create__window_configuration(title, 720, 480, 0);

    // open window
    graphics = VOX__open__graphics__new_window(error, configuration);

    // setup opengl shaders
    shaders_program = VOX__compile__shaders_program(error, vertex_shader, fragment_shader);
    if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
        return;
    }

    // user shaders
    VOX__use__use_shaders(shaders_program);

    // setup opengl
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // create data
    pattern = VOX__open__drawable_object(VOX__create__test__object_data_from_object_datum(VOX__create__test__object_datum__pattern(-0.5f, 0.0f, 0.0f)));
    
    // send data to gpu
    VOX__send__drawable_object_to_opengl(pattern);
    
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

        // display square
        VOX__draw__drawable_object(pattern);

        *error = VOX__try_create__error__other_opengl();
        if (VOX__check__error__has_error_occured(error) == VOX__bt__true) {
            return;
        }

        // display new buffer
        SDL_GL_SwapWindow(graphics.p_window_context);
    }

    // clean up drawable objects
    VOX__destroy__object_data(pattern.p_object_data);
    VOX__close__drawable_object(pattern);

    // clean up opengl
    VOX__close__shaders(shaders_program);
    VOX__close__graphics__old_window(graphics);
    VOX__destroy__window_configuration(configuration);

    // clean up extra buffers
    VOX__destroy__buffer(vertex_shader);
    VOX__destroy__buffer(fragment_shader);

    return;
}

#endif
