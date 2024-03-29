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

#define MAX_LIGHTS 3

const unsigned int wWidth = 1000;
const unsigned int wHeight = 1000;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju
std::vector<float> createSphere(float radius, int stacks, int slices, float centerX, float centerY, float centerZ);

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

    float sunVertices[] = 
    {
        //X      Y      Z      NX     NY     NZ
        // prednja strana
        -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole desno
        -0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore desno
        -0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore levo
        
        // desna strana
         0.1f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,   //dole levo
         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,   //gore levo

         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.1f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,   //gore desno
         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,   //gore levo


        // leva strana
        -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,   //dole levo
        -0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,   //dole desno
        -0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,   //gore levo

        -0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,   //dole desno
        -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,   //gore desno
        -0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,   //gore levo


        // zadnja strana
         0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //dole levo
        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //gore levo

        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //dole desno
        -0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //gore desno
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   //gore levo

        
        // donja strana
        -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,   //dole levo
         0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,   //dole desno
        -0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,   //gore levo

         0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,   //dole desno
         0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,   //gore desno
        -0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,   //gore levo


        // gornja strana
        -0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,   //dole levo
         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,   //dole desno
        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,   //gore levo

         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,   //dole desno
         0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,   //gore desno
        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,   //gore levo
    };

    float chimneyVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        // prednja strana
         0.2f,  0.4f,  0.4f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.3f,  0.4f,  0.4f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.2f,  0.6f,  0.4f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.3f,  0.4f,  0.4f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.3f,  0.6f,  0.4f,  0.0f,  0.0f,  1.0f,   //gore desno
         0.2f,  0.6f,  0.4f,  0.0f,  0.0f,  1.0f,   //gore levo

        // prednja strana nazad
         0.3f,  0.4f,  0.4f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.2f,  0.4f,  0.4f,  0.0f,  0.0f, -1.0f,   //dole levo
         0.2f,  0.6f,  0.4f,  0.0f,  0.0f, -1.0f,   //gore levo

         0.3f,  0.4f,  0.4f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.2f,  0.6f,  0.4f,  0.0f,  0.0f, -1.0f,   //gore levo
         0.3f,  0.6f,  0.4f,  0.0f,  0.0f, -1.0f,   //gore desno
        
        // desna strana
         0.3f,  0.4f,  0.4f,  1.0f,  0.0f,  0.0f,   //dole levo
         0.3f,  0.4f,  0.3f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.3f,  0.6f,  0.4f,  1.0f,  0.0f,  0.0f,   //gore levo

         0.3f,  0.4f,  0.3f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.3f,  0.6f,  0.3f,  1.0f,  0.0f,  0.0f,   //gore desno
         0.3f,  0.6f,  0.4f,  1.0f,  0.0f,  0.0f,   //gore levo

        // desna strana nazad
         0.3f,  0.4f,  0.3f, -1.0f,  0.0f,  0.0f,   //dole desno
         0.3f,  0.4f,  0.4f, -1.0f,  0.0f,  0.0f,   //dole levo
         0.3f,  0.6f,  0.4f, -1.0f,  0.0f,  0.0f,   //gore levo

         0.3f,  0.4f,  0.3f, -1.0f,  0.0f,  0.0f,   //dole desno
         0.3f,  0.6f,  0.4f, -1.0f,  0.0f,  0.0f,   //gore levo
         0.3f,  0.6f,  0.3f, -1.0f,  0.0f,  0.0f,   //gore desno

        // leva strana
         0.2f,  0.4f,  0.3f, -1.0f,  0.0f,  0.0f,   //dole levo
         0.2f,  0.4f,  0.4f, -1.0f,  0.0f,  0.0f,   //dole desno
         0.2f,  0.6f,  0.3f, -1.0f,  0.0f,  0.0f,   //gore levo

         0.2f,  0.4f,  0.4f, -1.0f,  0.0f,  0.0f,   //dole desno
         0.2f,  0.6f,  0.4f, -1.0f,  0.0f,  0.0f,   //gore desno
         0.2f,  0.6f,  0.3f, -1.0f,  0.0f,  0.0f,   //gore levo

        // leva strana nazad
         0.2f,  0.4f,  0.4f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.2f,  0.4f,  0.3f,  1.0f,  0.0f,  0.0f,   //dole levo
         0.2f,  0.6f,  0.3f,  1.0f,  0.0f,  0.0f,   //gore levo

         0.2f,  0.4f,  0.4f,  1.0f,  0.0f,  0.0f,   //dole desno
         0.2f,  0.6f,  0.3f,  1.0f,  0.0f,  0.0f,   //gore levo
         0.2f,  0.6f,  0.4f,  1.0f,  0.0f,  0.0f,   //gore desno

        // zadnja strana
         0.3f,  0.4f,  0.3f,  0.0f,  0.0f, -1.0f,   //dole levo
         0.2f,  0.4f,  0.3f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.3f,  0.6f,  0.3f,  0.0f,  0.0f, -1.0f,   //gore levo

         0.2f,  0.4f,  0.3f,  0.0f,  0.0f, -1.0f,   //dole desno
         0.2f,  0.6f,  0.3f,  0.0f,  0.0f, -1.0f,   //gore desno
         0.3f,  0.6f,  0.3f,  0.0f,  0.0f, -1.0f,   //gore levo

        // zadnja strana nazad
         0.2f,  0.4f,  0.3f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.3f,  0.4f,  0.3f,  0.0f,  0.0f,  1.0f,   //dole levo
         0.3f,  0.6f,  0.3f,  0.0f,  0.0f,  1.0f,   //gore levo

         0.2f,  0.4f,  0.3f,  0.0f,  0.0f,  1.0f,   //dole desno
         0.3f,  0.6f,  0.3f,  0.0f,  0.0f,  1.0f,   //gore levo
         0.2f,  0.6f,  0.3f,  0.0f,  0.0f,  1.0f,   //gore desno
    };

    float grassVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        -3.0f, -0.7f,  3.0f,  0.0f,  1.0f,  0.0f,   //dole levo
         3.0f, -0.7f,  3.0f,  0.0f,  1.0f,  0.0f,   //dole desno
        -3.0f, -0.7f, -3.0f,  0.0f,  1.0f,  0.0f,   //gore levo

         3.0f, -0.7f,  3.0f,  0.0f,  1.0f,  0.0f,   //dole desno
         3.0f, -0.7f, -3.0f,  0.0f,  1.0f,  0.0f,   //gore desno
        -3.0f, -0.7f, -3.0f,  0.0f,  1.0f,  0.0f,   //gore levo
    };

    float doorVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        -0.1f, -0.7f,  0.51f, 0.0f,  0.0f,  1.0f,   //dole levo
         0.1f, -0.7f,  0.51f, 0.0f,  0.0f,  1.0f,   //dole desno
        -0.1f, -0.25f, 0.51f, 0.0f,  0.0f,  1.0f,   //gore levo

         0.1f, -0.7f,  0.51f, 0.0f,  0.0f,  1.0f,   //dole desno
         0.1f, -0.25f, 0.51f, 0.0f,  0.0f,  1.0f,   //gore desno
        -0.1f, -0.25f, 0.51f, 0.0f,  0.0f,  1.0f,   //gore levo
    };

    float treeVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        // prednja strana
        -1.8f, -0.7f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole levo
        -1.7f, -0.7f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole desno
        -1.8f,  0.2f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore levo

        -1.7f, -0.7f,  0.1f,  0.0f,  0.0f,  1.0f,   //dole desno
        -1.7f,  0.2f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore desno
        -1.8f,  0.2f,  0.1f,  0.0f,  0.0f,  1.0f,   //gore levo


        // leva strana
        -1.8f, -0.7f,  0.0f,  -1.0f,  0.0f,  0.0f,   //dole levo
        -1.8f, -0.7f,  0.1f,  -1.0f,  0.0f,  0.0f,   //dole desno
        -1.8f,  0.2f,  0.0f,  -1.0f,  0.0f,  0.0f,   //gore levo

        -1.8f, -0.7f,  0.1f,  -1.0f,  0.0f,  0.0f,   //dole desno
        -1.8f,  0.2f,  0.1f,  -1.0f,  0.0f,  0.0f,   //gore desno
        -1.8f,  0.2f,  0.0f,  -1.0f,  0.0f,  0.0f,   //gore levo

        // desna strana
        -1.7f, -0.7f,  0.1f,  1.0f,  0.0f,  0.0f,   //dole levo
        -1.7f, -0.7f,  0.0f,  1.0f,  0.0f,  0.0f,   //dole desno
        -1.7f,  0.2f,  0.1f,  1.0f,  0.0f,  0.0f,   //gore levo

        -1.7f, -0.7f,  0.0f,  1.0f,  0.0f,  0.0f,   //dole desno
        -1.7f,  0.2f,  0.0f,  1.0f,  0.0f,  0.0f,   //gore desno
        -1.7f,  0.2f,  0.1f,  1.0f,  0.0f,  0.0f,   //gore levo

        // zadnja strana
        -1.7f, -0.7f,  0.0f,  0.0f,  0.0f, -1.0f,   //dole levo
        -1.8f, -0.7f,  0.0f,  0.0f,  0.0f, -1.0f,   //dole desno
        -1.7f,  0.2f,  0.0f,  0.0f,  0.0f, -1.0f,   //gore levo

        -1.8f, -0.7f,  0.0f,  0.0f,  0.0f, -1.0f,   //dole desno
        -1.8f,  0.2f,  0.0f,  0.0f,  0.0f, -1.0f,   //gore desno
        -1.7f,  0.2f,  0.0f,  0.0f,  0.0f, -1.0f    //gore levo
    };

    float bottomRoomWallsVertices[] =
    {
        //X      Y      Z      NX     NY     NZ     S      T
        // desna strana
         0.5f, -0.7f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.7f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
         0.5f, -0.2f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo

         0.5f, -0.7f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
         0.5f, -0.2f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, //gore desno
         0.5f, -0.2f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo


        // desna strana
        -0.5f, -0.7f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  //dole levo
        -0.5f, -0.7f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
        -0.5f, -0.2f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo

        -0.5f, -0.7f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
        -0.5f, -0.2f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, //gore desno
        -0.5f, -0.2f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo


        // zadnja strana
        -0.5f, -0.7f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.7f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
        -0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo

         0.5f, -0.7f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
         0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //gore desno
        -0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo
    };

    float secondFloorVertices[] =
    {
        //X      Y      Z      NX     NY     NZ
        // prvi sprat pod
        -0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole levo
         0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
        -0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo

         0.5f, -0.2f,  0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
         0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore desno
        -0.5f, -0.2f, -0.5f,  0.0f,  1.0f,  0.0f,   //gore levo

        //prvi sprat plafon
        -0.5f,  0.3f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole levo
         0.5f,  0.3f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
        -0.5f,  0.3f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo

         0.5f,  0.3f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
         0.5f,  0.3f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore desno
        -0.5f,  0.3f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo
    };

    float firstFloorVertices[] =
    {
        //prizemlje plafon
        -0.5f, -0.2f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole levo
         0.5f, -0.2f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
        -0.5f, -0.2f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo

         0.5f, -0.2f, -0.5f,  0.0f, -1.0f,  0.0f,   //dole desno
         0.5f, -0.2f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore desno
        -0.5f, -0.2f,  0.5f,  0.0f, -1.0f,  0.0f,   //gore levo

        //prizemlje pod
        -0.5f, -0.69f, 0.5f,  0.0f,  1.0f,  0.0f,   //dole levo
         0.5f, -0.69f, 0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
        -0.5f, -0.69f,-0.5f,  0.0f,  1.0f,  0.0f,   //gore levo
                  
         0.5f, -0.69f, 0.5f,  0.0f,  1.0f,  0.0f,   //dole desno
         0.5f, -0.69f,-0.5f,  0.0f,  1.0f,  0.0f,   //gore desno
        -0.5f, -0.69f,-0.5f,  0.0f,  1.0f,  0.0f,   //gore levo
    };

    float topRoomWallsVertices[] =
    {
        //X      Y      Z      NX     NY     NZ     S      T
        // desna strana
         0.5f, -0.2f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.2f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
         0.5f,  0.3f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo

         0.5f, -0.2f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
         0.5f,  0.3f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, //gore desno
         0.5f,  0.3f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo


        // desna strana
        -0.5f, -0.2f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  //dole levo
        -0.5f, -0.2f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
        -0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo

        -0.5f, -0.2f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, //dole desno
        -0.5f,  0.3f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, //gore desno
        -0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, //gore levo


        // zadnja strana
        -0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
        -0.5f,  0.3f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo

         0.5f, -0.2f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
         0.5f,  0.3f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //gore desno
        -0.5f,  0.3f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo
    };

    float fenceLeftVertices[] =
    {
        //X      Y      Z      NX     NY     NZ 
        // prednja strana
        -1.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole levo
        -0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
        -1.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo

        -0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
        -0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore desno
        -1.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo


        // leva strana
        -1.5f, -0.7f,  1.9f, -1.0f,  0.0f,  0.0f,  //dole levo
        -1.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
        -1.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo

        -1.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
        -1.5f, -0.3f,  2.0f, -1.0f,  0.0f,  0.0f,  //gore desno
        -1.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo


        // desna strana
        -0.5f, -0.7f,  2.0f,  1.0f,  0.0f,  0.0f,  //dole levo
        -0.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo

        -0.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  1.9f,  1.0f,  0.0f,  0.0f,  //gore desno
        -0.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo


        // zadnja strana
        -0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole levo
        -1.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
        -0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo

        -1.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
        -1.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore desno
        -0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo


        // gornja strana
        -1.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole levo
        -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
        -1.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo

        -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore desno
        -1.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo
    };

    float fenceMiddleVertices[] =
    {
        //X      Y      Z      NX     NY     NZ 
        // prednja strana
        -0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole levo
         0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
        -0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo

         0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
         0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore desno
        -0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo


        // leva strana
        -0.5f, -0.7f,  1.9f, -1.0f,  0.0f,  0.0f,  //dole levo
        -0.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo

        -0.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  2.0f, -1.0f,  0.0f,  0.0f,  //gore desno
        -0.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo


        // desna strana
         0.5f, -0.7f,  2.0f,  1.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo

         0.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  1.9f,  1.0f,  0.0f,  0.0f,  //gore desno
         0.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo


        // zadnja strana
         0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole levo
        -0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
         0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo

        -0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
        -0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore desno
         0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo


        // gornja strana
        -0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole levo
         0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
        -0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo

         0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore desno
        -0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo
    };

    float fenceRightVertices[] =
    {
        //X      Y      Z      NX     NY     NZ 
        // prednja strana
         0.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole levo
         1.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
         0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo

         1.5f, -0.7f,  2.0f,  0.0f,  0.0f,  1.0f,  //dole desno
         1.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore desno
         0.5f, -0.3f,  2.0f,  0.0f,  0.0f,  1.0f,  //gore levo


        // leva strana
         0.5f, -0.7f,  1.9f, -1.0f,  0.0f,  0.0f,  //dole levo
         0.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo

         0.5f, -0.7f,  2.0f, -1.0f,  0.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  2.0f, -1.0f,  0.0f,  0.0f,  //gore desno
         0.5f, -0.3f,  1.9f, -1.0f,  0.0f,  0.0f,  //gore levo


        // desna strana
         1.5f, -0.7f,  2.0f,  1.0f,  0.0f,  0.0f,  //dole levo
         1.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
         1.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo

         1.5f, -0.7f,  1.9f,  1.0f,  0.0f,  0.0f,  //dole desno
         1.5f, -0.3f,  1.9f,  1.0f,  0.0f,  0.0f,  //gore desno
         1.5f, -0.3f,  2.0f,  1.0f,  0.0f,  0.0f,  //gore levo


        // zadnja strana
         1.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole levo
         0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
         1.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo

         0.5f, -0.7f,  1.9f,  0.0f,  0.0f, -1.0f,  //dole desno
         0.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore desno
         1.5f, -0.3f,  1.9f,  0.0f,  0.0f, -1.0f,  //gore levo


        // gornja strana
         0.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole levo
         1.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
         0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo

         1.5f, -0.3f,  2.0f,  0.0f,  1.0f,  0.0f,  //dole desno
         1.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore desno
         0.5f, -0.3f,  1.9f,  0.0f,  1.0f,  0.0f,  //gore levo
    };

    float nameVertices[] =
    {
        //X      Y      Z      NX     NY     NZ     S      T
        -0.2f, -0.24f,  2.8f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, //dole levo
         0.0f, -0.24f,  2.8f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
        -0.2f, -0.14f,  2.8f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo

         0.0f, -0.24f,  2.8f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //dole desno
         0.0f, -0.14f,  2.8f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //gore desno
        -0.2f, -0.14f,  2.8f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //gore levo
    };

    std::vector<float> smokeVertices = createSphere(0.05, 30, 30, 0.25, 0.5, 0.35);

    std::vector<float> leavesVertices = createSphere(0.3, 30, 30, -1.75, 0.5, 0.05);

    unsigned int stride = (3 + 3) * sizeof(float);
    unsigned int textureStride = (3 + 3 + 2) * sizeof(float);

    unsigned int VAO[18];
    unsigned int VBO[18];
    glGenVertexArrays(18, VAO);
    glGenBuffers(18, VBO);

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
    // ******************************************************************************* SUNCE *******************************************************************************
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* SUNCE *******************************************************************************
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(chimneyVertices), chimneyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* DIM *******************************************************************************
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);

    glBufferData(GL_ARRAY_BUFFER, smokeVertices.size() * sizeof(float), smokeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* TRAVA *******************************************************************************
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* VRATA *******************************************************************************
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* STABLO *******************************************************************************
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(treeVertices), treeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* LISCE *******************************************************************************
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);

    glBufferData(GL_ARRAY_BUFFER, leavesVertices.size() * sizeof(float), leavesVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* DONJA SOBA *******************************************************************************
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomRoomWallsVertices), bottomRoomWallsVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)((3 + 3) * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* GORNJA SOBA *******************************************************************************
    glBindVertexArray(VAO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(topRoomWallsVertices), topRoomWallsVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)((3 + 3) * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* PRIZEMLJE POD I PLAFON *******************************************************************************
    glBindVertexArray(VAO[11]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(firstFloorVertices), firstFloorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* PRVI SPRAT POD I PLAFON *******************************************************************************
    glBindVertexArray(VAO[12]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(secondFloorVertices), secondFloorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* LEVA OGRADA *******************************************************************************
    glBindVertexArray(VAO[13]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(fenceLeftVertices), fenceLeftVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* SREDNJA OGRADA *******************************************************************************
    glBindVertexArray(VAO[14]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(fenceMiddleVertices), fenceMiddleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* DESNA OGRADA *******************************************************************************
    glBindVertexArray(VAO[15]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[15]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(fenceRightVertices), fenceRightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ********************************************************************************************************************************************************************
    // ******************************************************************************* IME *******************************************************************************
    glBindVertexArray(VAO[16]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[16]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, textureStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)((3 + 3) * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ********************************************************************************************************************************************************************
    // test
    /*glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);

    std::vector<float> ballVertices = createSphere(0.3, 30, 30, 0.0, 0.0, -0.8);

    glBufferData(GL_ARRAY_BUFFER, ballVertices.size() * sizeof(float), ballVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    //

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ SEJDERI +++++++++++++++++++++++++++++++++++++++++++++++++
    unsigned int phongShader = createShader("phong.vert", "phong.frag");

    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(phongShader, "uM");

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int viewLoc = glGetUniformLocation(phongShader, "uV");

    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(phongShader, "uP");

    unsigned int viewPosLoc = glGetUniformLocation(phongShader, "uViewPos");

    unsigned int lightPosLoc = glGetUniformLocation(phongShader, "uLight.pos");
    unsigned int lightALoc = glGetUniformLocation(phongShader, "uLight.kA");
    unsigned int lightDLoc = glGetUniformLocation(phongShader, "uLight.kD");
    unsigned int lightSLoc = glGetUniformLocation(phongShader, "uLight.kS");
    unsigned int lightConstantLoc = glGetUniformLocation(phongShader, "uLight.constant");
    unsigned int lightLinearLoc = glGetUniformLocation(phongShader, "uLight.linear");
    unsigned int lightQuadraticLoc = glGetUniformLocation(phongShader, "uLight.quadratic");

    unsigned int lightDirDirectionLoc = glGetUniformLocation(phongShader, "uLightDir.direction");
    unsigned int lightDirALoc = glGetUniformLocation(phongShader, "uLightDir.kA");
    unsigned int lightDirDLoc = glGetUniformLocation(phongShader, "uLightDir.kD");
    unsigned int lightDirSLoc = glGetUniformLocation(phongShader, "uLightDir.kS");

    unsigned int lightRefPositionLoc = glGetUniformLocation(phongShader, "uLightRef.position");
    unsigned int lightRefDirectionLoc = glGetUniformLocation(phongShader, "uLightRef.direction");
    unsigned int lightRefCutOffLoc = glGetUniformLocation(phongShader, "uLightRef.cutOff");
    unsigned int lightRefALoc = glGetUniformLocation(phongShader, "uLightRef.kA");
    unsigned int lightRefDLoc = glGetUniformLocation(phongShader, "uLightRef.kD");
    unsigned int lightRefSLoc = glGetUniformLocation(phongShader, "uLightRef.kS");

    unsigned int materialShineLoc = glGetUniformLocation(phongShader, "uMaterial.shine");
    unsigned int materialALoc = glGetUniformLocation(phongShader, "uMaterial.kA");
    unsigned int materialDLoc = glGetUniformLocation(phongShader, "uMaterial.kD");
    unsigned int materialSLoc = glGetUniformLocation(phongShader, "uMaterial.kS");

    unsigned int texLoc = glGetUniformLocation(phongShader, "uTex");
    unsigned int textureEnabledLoc = glGetUniformLocation(phongShader, "uTextureEnabled");


    unsigned wallTexture = loadImageToTexture("res/tapeta1.jpg");
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(texLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)

    unsigned nameTexture = loadImageToTexture("res/ime_nobackground.png"); //Ucitavamo teksturu
    glBindTexture(GL_TEXTURE_2D, nameTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(texLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)


    Model ker("res/DogBrown.obj");
    Model stickman("res/Stickman.obj");
    //Tjemena i baferi su definisani u model klasi i naprave se pri stvaranju objekata

    Shader unifiedShader("basic.vert", "basic.frag");



    bool firstFloorLight = false;
    bool secondFloorLight = false;
    float fenceOpenness = 0.0;
    float fov = 90.0f;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while (!glfwWindowShouldClose(window))
    {
        // ******************************************************************************* ROTACIJA KAMERE *******************************************************************************
        float cameraRotation = (float)glfwGetTime() * 10;
        view = glm::rotate(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::radians(cameraRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 cameraPosition = glm::vec3(glm::inverse(view)[3]);
        glUniform3f(viewPosLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
        // ********************************************************************************************************************************************************************* 

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            view = glm::rotate(view, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            view = glm::rotate(view, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            view = glm::rotate(view, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            glUseProgram(phongShader);
            view = glm::rotate(view, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUseProgram(0);
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            firstFloorLight = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            firstFloorLight = false;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            secondFloorLight = true;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            secondFloorLight = false;
        }

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && fenceOpenness < 35.0)
        {
            fenceOpenness += 0.3;
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && fenceOpenness > 0.0)
        {
            fenceOpenness -= 0.3;
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && fov > 45.0f)
        {
            fov -= 0.5f;
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && fov < 90.0f)
        {
            fov += 0.5f;
        }

        projectionP = glm::perspective(glm::radians(fov), (float)wWidth / (float)wHeight, 0.1f, 100.0f);


        glClearColor(0.192, 0.855, 0.949, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ******************************************************************************* PODESAVANJE OSVETLJENJA *******************************************************************************
        glUseProgram(phongShader);

        glUniform3f(lightDirDirectionLoc, -1.8, -1.8, 0.0);
        glUniform3f(lightDirALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightDirDLoc, 0.8, 0.8, 0.8);
        glUniform3f(lightDirSLoc, 1.0, 1.0, 1.0);

        glUniform3f(lightRefPositionLoc, 0.0f, -0.20f, 0.52f);
        glUniform3f(lightRefALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightRefDLoc, 0.8, 0.8, 0.8);
        glUniform3f(lightRefSLoc, 1.0, 1.0, 1.0);
        glUniform3f(lightRefDirectionLoc, 0.0f, -1.0f, 0.0f);
        glUniform1f(lightRefCutOffLoc, glm::cos(glm::radians(15.5f)));

        glUniform1i(textureEnabledLoc, false);

        glUseProgram(0);
        // *********************************************************************************************************************************************************************
        // ******************************************************************************* KER *******************************************************************************
        unifiedShader.use();

        unifiedShader.setVec3("uLightPos", 1.9, 1.9, 0.0);
        unifiedShader.setVec3("uViewPos", 0, 0, 2.0);
        unifiedShader.setVec3("uLightColor", 1, 1, 1);
        unifiedShader.setMat4("uP", projectionP);
        unifiedShader.setMat4("uV", view);

        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(1.3, -0.7, 0.0));
        model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
        unifiedShader.setMat4("uV", view);
        unifiedShader.setMat4("uM", model);
        ker.Draw(unifiedShader);

        glUseProgram(phongShader);

        float time = glfwGetTime();
        glUniform3f(lightPosLoc, model[3][0], model[3][1], model[3][2]);
        glUniform3f(lightALoc, abs(sin(time)), abs(sin(time)), abs(sin(time)));
        glUniform3f(lightDLoc, abs(sin(time)), abs(sin(time)), abs(sin(time)));
        glUniform3f(lightSLoc, abs(sin(time)), abs(sin(time)), abs(sin(time)));
        glUniform1f(lightConstantLoc, 1.0);
        glUniform1f(lightLinearLoc, 0.7);
        glUniform1f(lightQuadraticLoc, 1.8);


        model = glm::mat4(1.0);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* KUCA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));


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


        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.2, 0.0, 0.0);//Tamna crvena
        glUniform3f(materialDLoc, 1.0, 0.0, 0.0); //Obicna crvena
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(roofVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* SUNCE *******************************************************************************
        glUseProgram(phongShader);
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::translate(sunModel, glm::vec3(1.9, 1.9, 0.0));
        sunModel = glm::scale(sunModel, glm::vec3(abs(sin(glfwGetTime())) + 1));
        sunModel = glm::rotate(sunModel, glm::radians((float)glfwGetTime() * 50.0f), glm::vec3(1.0f, 1.0f, 0.0f));

        glUniform3f(lightDirALoc, 1.0, 1.0, 1.0); //posebno osvetljenje za sunce

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sunModel));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));


        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 1.0, abs(sin(glfwGetTime())), 0.0);
        glUniform3f(materialDLoc, 0.0, 0.0, 0.0);
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sunVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* ODZAK *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.122, 0.122, 0.122);//Tamna crna
        glUniform3f(materialDLoc, 0.251, 0.251, 0.251); //Siva
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(chimneyVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* DIM *******************************************************************************
        glUseProgram(phongShader);

        float smokeY = 0.1 * glfwGetTime();
        smokeY = fmod(smokeY, 0.6);

        glm::mat4 smokeModel = glm::mat4(1.0f);
        smokeModel = glm::translate(smokeModel, glm::vec3(0.0, smokeY, 0.0));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(smokeModel));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.0, 0.0, 0.0);//Crna
        glUniform3f(materialDLoc, 0.0, 0.0, 0.0); //Crna
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLES, 0, smokeVertices.size() / 6);
        glDisable(GL_BLEND);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* TRAVA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 0.6 * 128); // Materijal: emerald
        glUniform3f(materialALoc, 0.0215, 0.1745, 0.0215);
        glUniform3f(materialDLoc, 0.07568, 0.61424, 0.07568);
        glUniform3f(materialSLoc, 0.633, 0.727811, 0.633);

        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(grassVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* VRATA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(doorVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* STABLO *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(treeVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* LISCE *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.0, 0.3, 0.0);//Tamno zelena
        glUniform3f(materialDLoc, 0.0, 0.3, 0.0); //Tamno zelena
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[8]);
        glDrawArrays(GL_TRIANGLES, 0, leavesVertices.size() / 6);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* LEVA OGRADA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[13]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(fenceLeftVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* SREDNJA OGRADA *******************************************************************************
        glUseProgram(phongShader);


        model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 1.95f));
        model = glm::rotate(model, glm::radians(fenceOpenness), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, -1.95f));


        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[14]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(fenceMiddleVertices) / stride);
        model = glm::mat4(1.0);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* DESNA OGRADA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[15]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(fenceRightVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* IME *******************************************************************************
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(phongShader);

        //model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.8f));

        //// Rotate the rectangle to face the camera (assuming it's facing the positive Z-axis)
        //glm::vec3 toCamera = glm::normalize(cameraPosition - glm::vec3(model[3]));
        //float angle = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), toCamera)));
        //model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-cameraRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glBindVertexArray(VAO[16]);
        glUniform1f(textureEnabledLoc, true);
        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(nameVertices) / textureStride);
        glDisable(GL_BLEND);
        glUniform1f(textureEnabledLoc, false);

        model = glm::mat4(1.0);
        // *********************************************************************************************************************************************************************
        // ******************************************************************************* PRIZEMLJE OSVETLJENJE *******************************************************************************
        glUseProgram(phongShader);

        glUniform3f(lightDirDirectionLoc, -1.8, -1.8, 0.0);
        glUniform3f(lightDirALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightDirDLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightDirSLoc, 0.0, 0.0, 0.0);


        glUniform3f(lightRefPositionLoc, 0.0f, -0.20f, 0.52f);
        glUniform3f(lightRefALoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefDLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefSLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefDirectionLoc, 0.0f, -1.0f, 0.0f);
        glUniform1f(lightRefCutOffLoc, glm::cos(glm::radians(15.5f)));

        if (firstFloorLight) 
        {
            glUniform3f(lightPosLoc, 0.0f, -0.21f, 0.0f);
            glUniform3f(lightALoc, 0.4, 0.4, 0.4);
            glUniform3f(lightDLoc, 0.8, 0.8, 0.8);
            glUniform3f(lightSLoc, 1.0, 1.0, 1.0);
            glUniform1f(lightConstantLoc, 1.0);
            glUniform1f(lightLinearLoc, 0.7);
            glUniform1f(lightQuadraticLoc, 1.8);
        }
        else
        {
            glUniform3f(lightPosLoc, 0.0f, -0.2f, 0.0f);
            glUniform3f(lightALoc, 0.0, 0.0, 0.0);
            glUniform3f(lightDLoc, 0.0, 0.0, 0.0);
            glUniform3f(lightSLoc, 0.0, 0.0, 0.0);
            glUniform1f(lightConstantLoc, 1.0);
            glUniform1f(lightLinearLoc, 0.7);
            glUniform1f(lightQuadraticLoc, 1.8);
        }
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* DONJA SOBA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[9]);
        glUniform1f(textureEnabledLoc, true);
        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(bottomRoomWallsVertices) / textureStride);
        glUniform1f(textureEnabledLoc, false);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* PRIZEMLJE POD I PLAFON *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[11]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(firstFloorVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* PRVI SPRAT OSVETLJENJE *******************************************************************************
        glUseProgram(phongShader);

        glUniform3f(lightDirDirectionLoc, -1.8, -1.8, 0.0);
        glUniform3f(lightDirALoc, 0.4, 0.4, 0.4);
        glUniform3f(lightDirDLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightDirSLoc, 0.0, 0.0, 0.0);


        glUniform3f(lightRefPositionLoc, 0.0f, -0.20f, 0.52f);
        glUniform3f(lightRefALoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefDLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefSLoc, 0.0, 0.0, 0.0);
        glUniform3f(lightRefDirectionLoc, 0.0f, -1.0f, 0.0f);
        glUniform1f(lightRefCutOffLoc, glm::cos(glm::radians(15.5f)));

        if (secondFloorLight) 
        {
            glUniform3f(lightPosLoc, 0.0f, 0.29f, 0.0f);
            glUniform3f(lightALoc, 0.4, 0.4, 0.4);
            glUniform3f(lightDLoc, 0.8, 0.8, 0.8);
            glUniform3f(lightSLoc, 1.0, 1.0, 1.0);
            glUniform1f(lightConstantLoc, 1.0);
            glUniform1f(lightLinearLoc, 0.7);
            glUniform1f(lightQuadraticLoc, 1.8);
        }
        else
        {
            glUniform3f(lightPosLoc, 0.0f, -0.2f, 0.0f);
            glUniform3f(lightALoc, 0.0, 0.0, 0.0);
            glUniform3f(lightDLoc, 0.0, 0.0, 0.0);
            glUniform3f(lightSLoc, 0.0, 0.0, 0.0);
            glUniform1f(lightConstantLoc, 1.0);
            glUniform1f(lightLinearLoc, 0.7);
            glUniform1f(lightQuadraticLoc, 1.8);
        }
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* GORNJA SOBA *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[10]);
        glUniform1f(textureEnabledLoc, true);
        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(topRoomWallsVertices) / textureStride);
        glUniform1f(textureEnabledLoc, false);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* PRVI SPRAT POD I PLAFON *******************************************************************************
        glUseProgram(phongShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));

        glUniform1f(materialShineLoc, 1); // 1 - kad materijal nije opste sjajan
        glUniform3f(materialALoc, 0.11, 0.051, 0);//Tamna braon
        glUniform3f(materialDLoc, 0.388, 0.176, 0); //Braon
        glUniform3f(materialSLoc, 0.0, 0.0, 0.0); //Nema odsjaja

        glBindVertexArray(VAO[12]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(firstFloorVertices) / stride);
        // ********************************************************************************************************************************************************************
        // ******************************************************************************* STICKMAN *******************************************************************************
        unifiedShader.use();

        if (secondFloorLight)
        {
            unifiedShader.setVec3("uLightPos", 0.0, 0.29, 0.0);
            unifiedShader.setVec3("uViewPos", 0, 0, 2.0);
            unifiedShader.setVec3("uLightColor", 1, 1, 1);
        }
        else
        {
            unifiedShader.setVec3("uLightPos", 0.0, 0.29, 0.0);
            unifiedShader.setVec3("uViewPos", 0, 0, 2.0);
            unifiedShader.setVec3("uLightColor", 0, 0, 0);
        }
        

        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.3, -0.1, 0.0));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        unifiedShader.setMat4("uV", view);
        unifiedShader.setMat4("uM", model);
        stickman.Draw(unifiedShader);


        model = glm::mat4(1.0);
        // ********************************************************************************************************************************************************************


        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    glDeleteBuffers(18, VBO);
    glDeleteVertexArrays(18, VAO);
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

std::vector<float> createSphere(float radius, int stacks, int slices, float centerX, float centerY, float centerZ) {
    std::vector<float> vertices;

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            float phi1 = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(stacks);
            float phi2 = glm::two_pi<float>() * static_cast<float>(i + 1) / static_cast<float>(stacks);

            float theta1 = glm::pi<float>() * static_cast<float>(j) / static_cast<float>(slices);
            float theta2 = glm::pi<float>() * static_cast<float>(j + 1) / static_cast<float>(slices);

            // Vertices for the first triangle
            float x1 = centerX + radius * glm::sin(phi1) * glm::sin(theta1);
            float y1 = centerY + radius * glm::cos(theta1);
            float z1 = centerZ + radius * glm::cos(phi1) * glm::sin(theta1);

            float x2 = centerX + radius * glm::sin(phi2) * glm::sin(theta1);
            float y2 = centerY + radius * glm::cos(theta1);
            float z2 = centerZ + radius * glm::cos(phi2) * glm::sin(theta1);

            float x3 = centerX + radius * glm::sin(phi1) * glm::sin(theta2);
            float y3 = centerY + radius * glm::cos(theta2);
            float z3 = centerZ + radius * glm::cos(phi1) * glm::sin(theta2);

            // Normal vectors
            glm::vec3 normal1 = glm::normalize(glm::vec3(x1 - centerX, y1 - centerY, z1 - centerZ));
            glm::vec3 normal2 = glm::normalize(glm::vec3(x2 - centerX, y2 - centerY, z2 - centerZ));
            glm::vec3 normal3 = glm::normalize(glm::vec3(x3 - centerX, y3 - centerY, z3 - centerZ));

            // Push vertices for the first triangle
            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(z1);
            vertices.push_back(normal1.x);
            vertices.push_back(normal1.y);
            vertices.push_back(normal1.z);

            vertices.push_back(x2);
            vertices.push_back(y2);
            vertices.push_back(z2);
            vertices.push_back(normal2.x);
            vertices.push_back(normal2.y);
            vertices.push_back(normal2.z);

            vertices.push_back(x3);
            vertices.push_back(y3);
            vertices.push_back(z3);
            vertices.push_back(normal3.x);
            vertices.push_back(normal3.y);
            vertices.push_back(normal3.z);

            // Vertices for the second triangle
            x1 = x3;
            y1 = y3;
            z1 = z3;

            x2 = x2;
            y2 = y2;
            z2 = z2;

            x3 = centerX + radius * glm::sin(phi2) * glm::sin(theta2);
            y3 = centerY + radius * glm::cos(theta2);
            z3 = centerZ + radius * glm::cos(phi2) * glm::sin(theta2);

            // Recalculate normal vectors
            normal1 = glm::normalize(glm::vec3(x1 - centerX, y1 - centerY, z1 - centerZ));
            normal2 = glm::normalize(glm::vec3(x2 - centerX, y2 - centerY, z2 - centerZ));
            normal3 = glm::normalize(glm::vec3(x3 - centerX, y3 - centerY, z3 - centerZ));

            // Push vertices for the second triangle
            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(z1);
            vertices.push_back(normal1.x);
            vertices.push_back(normal1.y);
            vertices.push_back(normal1.z);

            vertices.push_back(x2);
            vertices.push_back(y2);
            vertices.push_back(z2);
            vertices.push_back(normal2.x);
            vertices.push_back(normal2.y);
            vertices.push_back(normal2.z);

            vertices.push_back(x3);
            vertices.push_back(y3);
            vertices.push_back(z3);
            vertices.push_back(normal3.x);
            vertices.push_back(normal3.y);
            vertices.push_back(normal3.z);
        }
    }

    return vertices;
}


