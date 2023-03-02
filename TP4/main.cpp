#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/common.hpp>
#include <glimac/Image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/getTime.hpp>
#include <glm/gtc/random.hpp>
#include <cstddef>

int window_width  = 1280;
int window_height = 720;
//float ratio = window_width/window_height;

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

struct EarthProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const glimac::FilePath& applicationPath):
        m_Program(glimac::loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(const glimac::FilePath& applicationPath):
        m_Program(glimac::loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
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
    

    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    glimac::Sphere sphere(1, 32, 16);

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sizeof(glimac::ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(0,vbo);

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    //const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex,position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));
    glBindBuffer(0,vbo);
    glBindVertexArray(0);

    
    

    glimac::FilePath imgPath("/home/elisa/OPENGLMAX/GLImac-Template/assets/texture/EarthMap.jpg");
    std::unique_ptr<glimac::Image> earth = glimac::loadImage(imgPath);

    glimac::FilePath imgPath2("/home/elisa/OPENGLMAX/GLImac-Template/assets/texture/MoonMap.jpg");
    std::unique_ptr<glimac::Image> moon = glimac::loadImage(imgPath2);

    glimac::FilePath imgPath3("/home/elisa/OPENGLMAX/GLImac-Template/assets/texture/CloudMap.jpg");
    std::unique_ptr<glimac::Image> cloud = glimac::loadImage(imgPath3);

    glimac::FilePath applicationPath("/home/elisa/OPENGLMAX/GLImac-Template/TP4/shaders");
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);
    
    GLuint texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, (*earth).getWidth(), (*earth).getHeight(), 0, GL_RGBA, GL_FLOAT, (*earth).getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint ctexture;
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &ctexture);
    glBindTexture(GL_TEXTURE_2D, ctexture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, (*cloud).getWidth(), (*cloud).getHeight(), 0, GL_RGBA, GL_FLOAT, (*cloud).getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint mtexture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mtexture);
    glBindTexture(GL_TEXTURE_2D, mtexture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, (*moon).getWidth(), (*moon).getHeight(), 0, GL_RGBA, GL_FLOAT, (*moon).getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);


    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)window_width/(GLfloat)window_height, 0.1f, 100.f);
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-5.0f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    

    std::vector<glm::vec3> axes_rot;
    std::vector<glm::vec3> pos_start;
    int lunes = 32;

    for(int i=0; i <lunes; i++){
        axes_rot.push_back(glm::sphericalRand(0.85f));
        pos_start.push_back(glm::sphericalRand(2.0f));
    }

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        earthProgram.m_Program.use();
        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);

        // dessin de la Terre
        glm::mat4 globalMVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5));

        glm::mat4 earthMVMatrix = glm::rotate(globalMVMatrix, glimac::getTime(), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, 
	    glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, 
	    glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, 
	    glm::value_ptr(ProjMatrix * earthMVMatrix));

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ctexture);
        

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1
        

        // dessin de plusieurs lunes
        moonProgram.m_Program.use();
        glUniform1i(moonProgram.uTexture, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mtexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        for(int i=0; i<lunes ; i++){
            glm::mat4 moonMVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0,-5));
            moonMVMatrix = glm::rotate(moonMVMatrix, glimac::getTime(), axes_rot[i]);
            moonMVMatrix = glm::translate(moonMVMatrix, pos_start[i]);
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3(0.2, 0.2, 0.2));
            
            
            glUniformMatrix4fv(moonProgram.uMVPMatrix,1, GL_FALSE, glm::value_ptr(ProjMatrix*moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uMVMatrix,1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix,1, GL_FALSE, glm::value_ptr(NormalMatrix));

            

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

            
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
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