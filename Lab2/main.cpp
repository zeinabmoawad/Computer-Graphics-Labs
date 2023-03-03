#include <iostream>
#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

struct Vec3
{
    float x, y, z;
};
struct Color
{
    uint8_t r, g, b, a;
};
struct vertex
{
    Vec3 position;
    Color color;
};
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
    //================18- GPU memory====================//
    vertex verteices[6] = {
        {{-0.5,
          -0.5,
          0.0},
         {255,
          0,
          0,
          255}},
        {{0.5,
          -0.5,
          0.0},
         {0,
          255,
          0,
          255}},
        {{-0.5,
          0.5,
          0.0},
         {0,
          0,
          255,
          255}},

        {{0.5,
          0.5,
          0.0},
         {255,
          0,
          0,
          255}},
    };
    uint16_t elements[6]{0, 1, 2, 3, 1, 2};
    //=================vertext buffer=======================//
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    //(buffer,size,position)
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), verteices, GL_STATIC_DRAW);
    //=================Element buffer=======================//
    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    //(buffer,size,position)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint16_t), elements, GL_STATIC_DRAW);
    //================7- Vertex Array============================//
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array); // 1:for one object
    glBindVertexArray(vertex_array);// to record that orders is for vertx buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); 
   //==========Determined loctions
    GLuint position_loc = 0; // glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(vertex), 0);

    GLuint color_loc = 1; // glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 3, GL_UNSIGNED_BYTE, true, sizeof(vertex), (void *)offsetof(vertex, color));
    //=====bind vertex array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    //==================unbind
    glBindVertexArray(0);
    //=================15- get loction for uniforms(vert==>c++)========//
    //== each varible in shaders has loction in memory=======//
    //=====if not exit wiil return 0 =====//
    GLuint time_loc = glGetUniformLocation(program, "time");
    std::cout << "Time uniform loction" << time_loc << std::endl;
    //==================8- condition when user click on close button to start==========//
    while (!glfwWindowShouldClose(window))
    {
        //==============9- Clear Window========================//
        float time = (float)glfwGetTime();
        // glClearColor(0.5 * sin(time) + 0.5,
        //              0.5 * sin(time + 1.0) + 0.5,
        //              0.5 * sin(time + 2.0) + 0.5,
        //              1.0);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //==============10- using vertex of program========================//
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        //=============17- after use program ========================//
        //=======(loction,value)
        glUniform1f(time_loc, time);
        // glDrawArrays(GL_TRIANGLES, 0, 6); // call vertex shader 3 times
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *)0);
        // glUniform1f(time_loc, time + 1.0); // not in same phase
        // glDrawArrays(GL_TRIANGLES, 0, 3);

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