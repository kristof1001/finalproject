// openGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include<filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "Texture.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

typedef long long ll;
// Vertices coordinates
// in the shape of the 3 coordinates xyz then the colors
GLfloat vertices[] =
{
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

int main()
{
    // init function
    glfwInit();

    // config inits
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window height and width set to the same at 800px
    int height, width;
    height = width = 800;
    const char* windowName = "OpenGL - Test";
    bool fullscreen = false;

    // window object
    GLFWwindow* window = glfwCreateWindow(height, width, windowName, NULL, NULL);

    // if window could not be initalized
    if (!window) {
        std::cout << "Could not load window :(\n";
        glfwTerminate();
        return -1;
    }

    // create window instance
    glfwMakeContextCurrent(window);

    // update window screen to black
    gladLoadGL();

    // render view has a shown and buffer loading in the background
    // we want to swap the buffer such that the buffer content gets displayed
    glViewport(0, 0, height, width);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture obamiumTexture("obamiumTexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    obamiumTexture.texUnit(shaderProgram, "tex0", 0);

    // timer
    float rotation = 0.0f;
    double prevTime = glfwGetTime();


    // let OpenGL handle depth renders
    glEnable(GL_DEPTH_TEST);

    // main while loop
    while (!glfwWindowShouldClose(window)) {
        // clear for every frame
        glClearColor(0.18f, 0.18f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // run shader
        shaderProgram.Activate();

        // run 3D renderer
        // create identity matrix for all transforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // time calculations
        double currTime = glfwGetTime();
        if (currTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = currTime;
        }

        // spin
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        // camaera renders at a 45 degree angle for objects from 0.1 to 100 units away
        proj = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f);

        // Outputs the matrices into the Vertex Shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
        glUniform1f(uniID, 0.5f);
        // binds texture
        obamiumTexture.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // swap the buffers on each run
        glfwSwapBuffers(window);

        // runner
        glfwPollEvents();
    }

    /* clean up */
    // gl objects
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    obamiumTexture.Delete();
    shaderProgram.Delete();
    // window instance
    glfwDestroyWindow(window);
    glfwTerminate();

    // std::cout << "Done\n";
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
