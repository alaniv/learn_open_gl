#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow* initLibrariesAndCreateWindow(int width, int height, const char *title);
GLuint VBO, VAO;

void displayInit(){

    float vertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void displayUpdate(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush();
}

void displayTerminate(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int main(){
    GLFWwindow* window = initLibrariesAndCreateWindow(800, 600, "Learn OpenGL");
    Shader ourShader("./vertex.vert", "./fragment.frag");

    displayInit();

    while (!glfwWindowShouldClose(window)){
        processInput(window);

        ourShader.use();
        displayUpdate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    displayTerminate();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void displayOnResize(__attribute__((unused)) GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* initLibrariesAndCreateWindow(int width, int height, const char *title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL){
        glfwTerminate();
        throw std::runtime_error(std::string("ERROR: Failed to initialize GLFW window"));
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, displayOnResize);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        throw std::runtime_error(std::string("ERROR: Failed to initialize GLAD"));
    }
    
    return window;
}