#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <cstddef>
#include <vector>

int window_width  = 1280;
int window_height = 720;

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

struct Vertex2DUV {

    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(){};
    Vertex2DUV(const glm::vec2 pos, const glm::vec2 text):position(pos),texture(text){};

};

GLuint uTime = 45;


glm::mat3 translate(float tx, float ty){
    glm::mat3 trans = {glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(tx,ty,1)};
    return trans;
}

glm::mat3 scale(float sx, float sy){
    glm::mat3 sc = {glm::vec3(sx,0,0), glm::vec3(0,sy,0), glm::vec3(0,0,1)};
    return sc;
}

glm::mat3 rotate(float a){

    glm::mat3 rot = {glm::vec3(glm::cos(glm::radians(a)),glm::sin(glm::radians(a)),0), glm::vec3(-glm::sin(glm::radians(a)),glm::cos(glm::radians(a)),0), glm::vec3(0,0,1)};
    return rot;
}


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
    

    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DUV vertices[] = { 
    Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0.0, 1.0)),
    Vertex2DUV(glm::vec2(1, -1), glm::vec2(1.0, 1.0)),
    Vertex2DUV(glm::vec2(0, 1), glm::vec2(0.5, 0.0)),
    

};
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    glBindBuffer(0,vbo);

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    //const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), 0);
    glBindBuffer(0,vbo);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, texture)));
    glBindBuffer(0,vbo);
    glBindVertexArray(0);

    glimac::FilePath imgPath("/home/elisa/OPENGLMAX/GLImac-Template/assets/texture/triforce.png");
    std::unique_ptr<glimac::Image> triforce = glimac::loadImage(imgPath);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, (*triforce).getWidth(), (*triforce).getHeight(), 0, GL_RGBA, GL_FLOAT, (*triforce).getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glimac::FilePath applicationPath = "/home/elisa/OPENGLMAX/GLImac-Template/TP3/shaders";
    std::cout << applicationPath << std::endl;
    glimac::Program program = loadProgram(applicationPath.dirPath() + "shaders/text2D.vs.glsl", applicationPath.dirPath() + "shaders/text2D.fs.glsl");
    //glimac::Program program = loadProgram(applicationPath.dirPath() + argv[1], applicationPath.dirPath() + argv[2]);
    program.use();

    GLint uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");

    float rot = 0.1;

    glm::mat3 translationTR = translate(0.5,0.5);
    glm::mat3 translationTL = translate(-0.5,0.5);
    glm::mat3 translationBL = translate(-0.5,-0.5);
    glm::mat3 translationBR = translate(0.5,-0.5);
    glm::mat3 scaling = scale(0.25,0.25);

    
    GLint uColor = glGetUniformLocation(program.getGLId(), "uColor");
    glm::vec3 color1 = {0.75,0.20,0.89};
    glm::vec3 color2 = {0.20,0.20,0.80};
    glm::vec3 color3 = {0.20,0.80,0.20};
    glm::vec3 color4 = {0.85,0.20,0.20};

    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");




    

    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(uTexture, 0);

        glm::mat3 rotation = rotate(rot);
        glm::mat3 rotation_inverse = rotate(-rot);

         glUniformMatrix3fv(uModelMatrix,1,GL_FALSE, glm::value_ptr(translationBL*rotation*scaling));
        glUniform3fv(uColor,1,glm::value_ptr(color1));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrix,1,GL_FALSE, glm::value_ptr(translationTL*(rotation_inverse)*scaling));
        glUniform3fv(uColor,1,glm::value_ptr(color2));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrix,1,GL_FALSE, glm::value_ptr(translationTR*rotation*scaling));
        glUniform3fv(uColor,1,glm::value_ptr(color3));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix3fv(uModelMatrix,1,GL_FALSE, glm::value_ptr(translationBR*(rotation_inverse)*scaling));
        glUniform3fv(uColor,1,glm::value_ptr(color4));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D,0);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
        rot += 0.1;
    }

    
    glDeleteBuffers(0,&vbo);
    glDeleteVertexArrays(0,&vao);
    //glDeleteTextures(0,&texture);

    glfwTerminate();
    return 0;
}