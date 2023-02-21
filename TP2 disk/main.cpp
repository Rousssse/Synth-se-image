#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>
#include <vector>

int window_width  = 800;
int window_height = 800;

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
{
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

struct Vertex2DColor {

    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor(){};
    Vertex2DColor(const glm::vec2 pos, const glm::vec3 col):position(pos),color(col){};

};


int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[])
{
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }



    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    /* Hook input callbacks */
    glimac::FilePath applicationPath ="/home/elisa/OPENGLMAX/GLImac-Template/TP2/shaders/";
    std::cout << applicationPath << std::endl;
    glimac::Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl", applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<Vertex2DColor> N;
    const float NombrePoint = 100;
    const float rayon = 0.5;
    for(float i= 0; i<NombrePoint ; ++i){
        N.push_back(Vertex2DColor(glm::vec2(rayon*glm::cos(2.f*glm::pi<float>()*(i/NombrePoint)), rayon*glm::sin(2.f*glm::pi<float>()*(i/NombrePoint))), glm::vec3(1,0,1)));
        N.push_back(Vertex2DColor(glm::vec2(rayon*glm::cos(2.f*glm::pi<float>()*((i+1)/NombrePoint)), rayon*glm::sin(2.f*glm::pi<float>()*((i+1)/NombrePoint))), glm::vec3(1,1,0)));
        N.push_back(Vertex2DColor(glm::vec2(0.0,0.0),glm::vec3(0,1,1)));

    }
//     Vertex2DColor vertices[] = { 
//     Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(1, 1, 0)),
//     Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(0, 1, 1)),
//     Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(1, 0, 1)),
//     Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(1, 0, 1)),
//     Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 1, 1)),
//     Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(1, 1, 0))

// };
    glBufferData(GL_ARRAY_BUFFER, N.size() * sizeof(Vertex2DColor), N.data(), GL_STATIC_DRAW);
    glBindBuffer(0,vbo);
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    //const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), 0);
    glBindBuffer(0,vbo);
    glEnableVertexAttribArray(8);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(8,3,GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));
    glBindBuffer(0,vbo);
    glBindVertexArray(0);

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, N.size());
        glBindVertexArray(0);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    glDeleteBuffers(0,&vbo);
    glDeleteVertexArrays(0,&vao);
    return 0;
}