#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

void framebuffer_size_callback(GLFWwindow *win, int w, int h);
void process_input(GLFWwindow *win);
void check_shader_compilation_failure(unsigned int shader);
void check_shader_linking_failure(unsigned int program);

int main(void)
{
    // setting up OpenGL core with GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "First OpenGL program", NULL, NULL);
    if (!win)
    {
        fputs("Failed to create GLFW window", stderr);
        return 1;
    }
    glfwMakeContextCurrent(win);

    // initializing GLAD
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            fputs("Failed to initialize GLAD", stderr);
            return 1;
        }

        // makes the opengl viewport the same size as the glfw window
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
        // makes the viewport resize every time the window is resized
        glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Vertex Buffer Object - memory stored in GPU
    unsigned int VBO;
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // copies buffer data to VBO in GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    // Vertex Array Object - stores vertex data and attrib configurations
    // VAO is mandatory to be able to draw anything with opengl
    //
    // note: this is necessary cause it reduces the amount of setup calls necessary 
    // every time the context is switched to another object

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // creating and compiling vertex shader
    const char *vertex_shader_source = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\0";
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    check_shader_compilation_failure(vertex_shader);

    // creating and compiling fragment shader
    const char *const fragment_shader_source =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    check_shader_compilation_failure(fragment_shader);

    // creating a program and linking the compile shaders to it
    // this allows shaders to be used by opengl
    unsigned int shader_program = glCreateProgram();
    if (!shader_program)
    {
        fputs("Failed to create program", stderr);
        return 1;
    }
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    check_shader_linking_failure(shader_program);
    // they're no longer needed after being linked into program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glUseProgram(shader_program);

    // tells opengl how to interpret the vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        process_input(win);
        glfwPollEvents();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(win);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *win, int w, int h)
{
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow *win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(win, true);
    }
}

void check_shader_compilation_failure(unsigned int shader)
{
    int compiled = 0;
    char compile_info[512] = {'\n'};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glGetShaderInfoLog(shader, 512, NULL, compile_info);
        fputs("COMPILING ERROR:", stderr);
        fputs(compile_info, stderr);
    }
}

void check_shader_linking_failure(unsigned int program)
{
    int linked = 0;
    char link_info[512] = {'\n'};
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(program, 512, NULL, link_info);
        fputs("LINKING ERROR:", stderr);
        fputs(link_info, stderr);
    }
}
