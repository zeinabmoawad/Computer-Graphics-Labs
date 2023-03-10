#include <iostream>
#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

GLuint load_shader(const std::string &path, GLenum shader_type)
{
    //=======================1- Reading File=====================//
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCodeCstr = sourceCode.c_str();
    //======================2- Creating Shader=================//
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &sourceCodeCstr, nullptr);
    glCompileShader(shader);
//===================3- check shader in compiling=============//
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

    //===================3- Creating window===========//
    GLFWwindow *window = glfwCreateWindow(
        640, 360, "Example 1", nullptr, nullptr);
    //===================4- Checking window===========//
    if (!window)
    {
        /* code */
        glfwTerminate();
        std::cerr << "Failed to Create window" << std::endl;
        exit(-1);
    }
    //===========5- Making window contain whole context of OpenGL======//
    // to make current window before loading
    glfwMakeContextCurrent(window);
    //=============6- load all function by name (take string return pointer)====//
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to Load OpenGl" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    //===========-create Shaders & link it to program ==================//
    GLuint program = glCreateProgram();
    GLuint vs = load_shader("assets/shader/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("assets/shader/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    //================7- Vertex Array============================//
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array); // 1:for one object
    //==================8- condition when user click on close button to start==========//
    while (!glfwWindowShouldClose(window))
    {
        //==============9- Clear Window========================//
        float time = (float)glfwGetTime();
        glClearColor(0.5 * sin(time) + 0.5,
                     0.5 * sin(time + 1.0) + 0.5,
                     0.5 * sin(time + 2.0) + 0.5,
                     1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //==============10- using vertex  of program========================//
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 4); // call vertex shader 3 times
        //==============11- to display in buffer (to make it front buffer)================//
        glfwSwapBuffers(window);
        //==============12- sensing input(all action in screen)==========================//
        glfwPollEvents();
    }
    //================13- delete program =============//
    glDeleteProgram(program);
    //===================14- destroy window for ===========//
    glfwDestroyWindow(window);
    //==============15- terminate GLFW=================//
    glfwTerminate();
}
