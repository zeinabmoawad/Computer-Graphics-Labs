#include <iostream>
#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

GLuint load_shader(const std::string &path, GLenum shader_type)
{
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCodeCstr = sourceCode.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &sourceCodeCstr, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::cerr << "Error IN " << path << std::endl;
        std::cerr << logStr << std::endl;
        delete[] logStr;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int main(int, char **)
{
    // GLFW only for window but not for Drawing
    //==============1- initiate GLFW=================//
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
   //==============2- configure GLFW=================//
    // verion 3.3 of OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Creating window
    GLFWwindow *window = glfwCreateWindow(
        640, 360, "Example 1", nullptr, nullptr);
    if (!window)
    {
        /* code */
        glfwTerminate();
        std::cerr << "Failed to Create window" << std::endl;
        exit(-1);
    }

    // to make current window before loading
    glfwMakeContextCurrent(window);
    // load all function by name (take string return pointer)
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to Load OpenGl" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    GLuint program = glCreateProgram();
    GLuint vs = load_shader("assets/shader/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("assets/shader/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    // condition when user click on close button
    while (!glfwWindowShouldClose(window))
    {
        // Clear Window
        float time = (float)glfwGetTime();
        glClearColor(0.5 * sin(time) + 0.5,
                     0.5 * sin(time + 1.0) + 0.5,
                     0.5 * sin(time + 2.0) + 0.5,
                     1.0);
        // clear buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vertex_array);
        //call vertex shader 3 times
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // to display in buffer
        glfwSwapBuffers(window);
        // sensing input
        glfwPollEvents();
    }
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    //==============- terminate GLFW=================//
    glfwTerminate();
}
