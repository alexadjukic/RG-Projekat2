//Opis: Primjer ucitavanja modela upotrebom ASSIMP biblioteke
//Preuzeto sa learnOpenGL

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"

const unsigned int wWidth = 1000;
const unsigned int wHeight = 1000;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

int main()
{
    if(!glfwInit())
    {
        std::cout << "GLFW fail!\n" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "Na kraj sela zuta kuca", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window fail!\n" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() !=GLEW_OK)
    {
        std::cout << "GLEW fail! :(\n" << std::endl;
        return -3;
    }

    //float vertices[] =
    //{   //Kocka
    //    //Normale su potrebne za racun osvjetljenja.
    ////X     Y      Z       NX    NY     NZ
    //-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    //-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    //-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    // 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    // 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    // 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    // 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    // 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    // 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    //-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    // 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    // 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    // 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    //-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    // 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    // 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    // 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    //};
    float houseVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        // prednja strana
        //      ispod donjih prozora
        -0.5f, -0.7f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.5f, -0.7f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.5f, -0.7f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
        -0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      levo od donjeg prozora
        -0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
        -0.35f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        -0.35f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.35f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
        -0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      izmedju donjih prozora
        -0.25f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.25f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.25f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.25f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.25f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.25f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      desno od donjeg prozora
         0.35f,-0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.35f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.5f, -0.4f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
         0.35f,-0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      izmedju spratova
        -0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.5f, -0.25f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      levo od gornjeg prozora
        -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
        -0.35f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        -0.35f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.35f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
        -0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      izmedju gornjih prozora
        -0.25f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.25f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.25f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.25f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.25f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.25f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      desno od gornjeg prozora
         0.35f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.35f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore desno
         0.35f, 0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        //      iznad gornjih prozora
        -0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.5f,  0.15f, 0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,   //gore levo

        
        // desna strana
         0.5f, -0.7f,  0.5f,  1.0f,  0.0f,  0.0f,   //dole levo
         0.5f, -0.7f, -0.5f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,   //gore levo

         0.5f, -0.7f, -0.5f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.5f,  0.3f, -0.5f,  1.0f,  0.0f,  0.0f,   //gore desno
         0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,   //gore levo


        // leva strana
        -0.5f, -0.7f, -0.5f, -1.0f,  0.0f,  0.0f,   //dole levo
        -0.5f, -0.7f,  0.5f, -1.0f,  0.0f,  0.0f,   //dole desno
        -0.5f,  0.3f, -0.5f, -1.0f,  0.0f,  0.0f,   //gore levo

        -0.5f, -0.7f,  0.5f, -1.0f,  0.0f,  0.0f,   //dole desno
        -0.5f,  0.3f,  0.5f, -1.0f,  0.0f,  0.0f,   //gore desno
        -0.5f,  0.3f, -0.5f, -1.0f,  0.0f,  0.0f,   //gore levo


        // zadnja strana
         0.5f, -0.7f, -0.5f,  0.0f,  0.0f, -1.0f,   //dole levo
        -0.5f, -0.7f, -0.5f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,   //gore levo

        -0.5f, -0.7f, -0.5f,  0.0f,  0.0f, -1.0f,   //dole desno
        -0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,   //gore desno
         0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,   //gore levo

        
        // donja strana
        -0.5f, -0.7f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole levo
         0.5f, -0.7f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
        -0.5f, -0.7f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo

         0.5f, -0.7f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
         0.5f, -0.7f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore desno
        -0.5f, -0.7f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo


        // gornja strana
        -0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole levo
         0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
        -0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo

         0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
         0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore desno
        -0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo


        // sprat
        -0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole levo
         0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
        -0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo

         0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
         0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore desno
        -0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo
    };

    float roofVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        // prednja strana
        -0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  1.0f,   //dole levo
         0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  1.0f,   //dole desno
         0.0f,  0.8f,  0.0f,  0.0f,  1.0f,  1.0f,   //gore

        // desna strana
         0.5f,  0.3f,  0.5f,  1.0f,  1.0f,  0.0f,   //dole levo
         0.5f,  0.3f, -0.5f,  1.0f,  1.0f,  0.0f,   //dole desno
         0.0f,  0.8f,  0.0f,  1.0f,  1.0f,  0.0f,   //gore

        // leva strana
        -0.5f,  0.3f, -0.5f, -1.0f,  1.0f,  0.0f,   //dole levo
        -0.5f,  0.3f,  0.5f, -1.0f,  1.0f,  0.0f,   //dole desno
         0.0f,  0.8f,  0.0f, -1.0f,  1.0f,  0.0f,   //gore

        // zadnja strana
         0.5f,  0.3f, -0.5f,  0.0f,  1.0f, -1.0f,   //dole levo
        -0.5f,  0.3f, -0.5f,  0.0f,  1.0f, -1.0f,   //dole desno
         0.0f,  0.8f,  0.0f,  0.0f,  1.0f, -1.0f,   //gore
    };

    unsigned int stride = (3 + 3) * sizeof(float);

    unsigned int VAO[2];
    unsigned int VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // ******************************************************************************* KUCA *******************************************************************************
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* KROV *******************************************************************************
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(roofVertices), roofVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ SEJDERI +++++++++++++++++++++++++++++++++++++++++++++++++
    unsigned int phongShader = createShader("phong.vert", "phong.frag");

    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(phongShader, "uM");

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int viewLoc = glGetUniformLocation(phongShader, "uV");

    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(phongShader, "uP");

    unsigned int viewPosLoc = glGetUniformLocation(phongShader, "uViewPos");

    unsigned int lightPosLoc = glGetUniformLocation(phongShader, "uLight.pos");
    unsigned int lightALoc = glGetUniformLocation(phongShader, "uLight.kA");
    unsigned int lightDLoc = glGetUniformLocation(phongShader, "uLight.kD");
    unsigned int lightSLoc = glGetUniformLocation(phongShader, "uLight.kS");

    unsigned int materialShineLoc = glGetUniformLocation(phongShader, "uMaterial.shine");
    unsigned int materialALoc = glGetUniformLocation(phongShader, "uMaterial.kA");
    unsigned int materialDLoc = glGetUniformLocation(phongShader, "uMaterial.kD");
    unsigned int materialSLoc = glGetUniformLocation(phongShader, "uMaterial.kS");


    //Model lija("res/DogBrown.obj");
    ////Tjemena i baferi su definisani u model klasi i naprave se pri stvaranju objekata

    //Shader unifiedShader("basic.vert", "basic.frag");

    ////Render petlja
    //unifiedShader.use();
    //unifiedShader.setVec3("uLightPos", 0, 1, 3);
    //unifiedShader.setVec3("uViewPos", 0, 0, 5);
    //unifiedShader.setVec3("uLightColor", 1, 1, 1);
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    //unifiedShader.setMat4("uP", projection);
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //unifiedShader.setMat4("uV", view);
    //glm::mat4 model = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while (!glfwWindowShouldClose(window))
    {

        /*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        unifiedShader.setMat4("uM", model);
        lija.Draw(unifiedShader);

        glfwSwapBuffers(window);
        glfwPollEvents();*/

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }

        glClearColor(0.192, 0.855, 0.949, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ******************************************************************************* KUCA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform3f(viewPosLoc, 0.0, 0.0, 2.0); //Isto kao i pozicija kamere

        glUniform3f(lightPosLoc, 0.0, 0.25, 2.0);
        glUniform3f(lightALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightDLoc, 0.8, 0.8, 0.8);
        glUniform3f(lightSLoc, 1.0, 1.0, 1.0);

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.341, 0.341, 0);//Tamna zuta (kad nije pod direktnim svetlom)
        glUniform3f(materialDLoc, 1.0, 1.0, 0.0); //Obicna zuta (kad je pod direktnim svetlom)
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(houseVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* KROV *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform3f(viewPosLoc, 0.0, 0.0, 2.0); //Isto kao i pozicija kamere

        glUniform3f(lightPosLoc, 0.0, 0.25, 2.0);
        glUniform3f(lightALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightDLoc, 0.8, 0.8, 0.8);
        glUniform3f(lightSLoc, 1.0, 1.0, 1.0);

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.341, 0.341, 0);//Tamna zuta (kad nije pod direktnim svetlom)
        glUniform3f(materialDLoc, 1.0, 1.0, 0.0); //Obicna zuta (kad je pod direktnim svetlom)
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(roofVertices) / stride);
        // ********************************************************************************************************************************************************************

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteProgram(phongShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}


