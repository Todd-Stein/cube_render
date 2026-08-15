#include <iostream>
#include <fstream>

#include "SDL3/SDL.h"

#include "GL/glew.h"
// #include "GL/GL.h"
#include "SDL3/SDL_opengl.h"

#include "include/DataTypes/Primitives.hpp"
#include "include/DataTypes/Matrices.hpp"
#include "include/Containers/Simple_Array.hpp"

#include "include/Math/MatrixMath.hpp"
#include "include/Math/RotationMath.hpp"


#define BASE_WIDTH 640
#define BASE_HEIGHT 480
#define BASE_ASPECT_RATIO BASE_WIDTH/BASE_HEIGHT

//#define Matrix4x4 float[4][4];


SDL_Window* mainWindow;
SDL_Event evt;
SDL_GLContext glCtx;

int main(int, char**){

    /*
        0*0, 0*4, 0*8, 0*12
        1*1, 1*5, 1*9, 1*13
        2*2, 2*6, 2*10, 2*14
        3*3, 3*7, 3*11, 3*15
    */

    /*
        0*0 +   4*1 +   8*2 +   12*3
    */

    Vector3 cubeVerts[8] { //front 4 vertices
                     {-0.5f, 0.5f, -0.5f},
                    {0.5f, 0.5f, -0.5f},
                    {0.5f, -0.5f, -0.5f},
                    {-0.5f, -0.5f, -0.5f},
                    //back 4 vertices
                    {-0.5f, 0.5f, 0.5f},
                    {0.5f, 0.5f, 0.5f},
                    {0.5f, -0.5f, 0.5f},
                    {-0.5f, -0.5f, 0.5f}
    };
    unsigned int cubeIndices[36] {
                                    4, 7, 6,
                                    4, 6, 5,
                                    1, 2, 3,
                                    1, 3, 0,
                                    0, 3, 7,
                                    0, 7, 4,
                                    5, 6, 2, 
                                    5, 2, 1,
                                    4, 5, 1,
                                    4, 1, 0,
                                    3, 2, 6,
                                    3, 6, 7

    };
    std::cout<<"init SDL"<<std::endl;
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr<<"Could not initialize SDL3:\t"<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    mainWindow = SDL_CreateWindow("Main Window", BASE_WIDTH, BASE_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(mainWindow == NULL) {
        std::cerr<<"Could not open window:\t"<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    glCtx = SDL_GL_CreateContext(mainWindow);

    GLenum initGLEW = glewInit();
    if(initGLEW!=GLEW_OK) {
        std::cerr<<"Could not initialize GLEW:\t"<<glewGetErrorString(initGLEW)<<std::endl;
        return EXIT_FAILURE;
    }
    glewExperimental = true;


    SDL_ShowWindow(mainWindow);
    
    Simple_Array<int, 5> arrTest;
    arrTest[0] = 25;
    std::cout<<sizeof(Vector3)<<std::endl;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*8, &cubeVerts[0], GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vertShader, fragShader, shaderProg;
    std::fstream vertShaderFile("../../../../shaders/Flat Color/flatcolor.vert", std::ios::in);
    if(!vertShaderFile.is_open()) {
        std::cout<<"Could not open vertex shader file."<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::fstream fragShaderFile("../../../../shaders/Flat Color/flatcolor.frag", std::ios::in);
    if(!fragShaderFile.is_open()) {
        std::cout<<"Could not open fragment shader file."<<std::endl;
        exit(EXIT_FAILURE);
    }
    vertShaderFile.seekg(0, std::ios::end);
    fragShaderFile.seekg(0, std::ios::end);
    auto vertFileLengthStreamPos = vertShaderFile.tellg();
    auto fragFileLengthStreamPos = fragShaderFile.tellg();

    std::streamsize vertFileLength, fragFileLength;
    vertFileLength = vertFileLengthStreamPos-std::streampos(0);
    fragFileLength = fragFileLengthStreamPos-std::streampos(0);

    vertShaderFile.seekg(0, std::ios::beg);
    fragShaderFile.seekg(0, std::ios::beg);

    char* vertShaderCode = new char[vertFileLength];
    char* fragShaderCode = new char[fragFileLength];

    vertShaderFile.get(vertShaderCode, vertFileLength, '\0');
    fragShaderFile.get(fragShaderCode, fragFileLength, '\0');

    std::cout<<vertShaderCode<<std::endl;

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertShader, 1, &vertShaderCode, NULL);
    glShaderSource(fragShader, 1, &fragShaderCode, NULL);

    int success;
    char errorLog[1024];

    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertShader, 1024, NULL, errorLog);
        std::cout<<"Vertex shader compilation error."<<std::endl;
        std::cout<<errorLog<<std::endl;
        exit(EXIT_FAILURE);
    }
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragShader, 1024, NULL, errorLog);
        std::cout<<"Fragment shader compilation error."<<std::endl;
        std::cout<<errorLog<<std::endl;
        exit(EXIT_FAILURE);
    }

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProg, 1024, NULL, errorLog);
        std::cout<<"Shader link error."<<std::endl;
        std::cout<<errorLog<<std::endl;
        exit(EXIT_FAILURE);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLuint modelId = glGetUniformLocation(shaderProg, "model");
    GLuint projId = glGetUniformLocation(shaderProg, "projection");

    Matrix4x4 model = Identity4x4;
    Matrix4x4 projection = CreatePerspectiveProjectionMatrix(90.0f, 0.1f, 100.0f, 4.f/3.f);
    
    std::cout<<"Projection matrix:\n";
    for(int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++) {
            std::cout<<projection.data[i][j]<<" ";
        }
        std::cout<<"\n";
    }
    
    Vector3 position {0.0f, 0.0f, -5.0f};
    Vector3 rotation {0.0f, 0.0f, 0.0f};
    Vector3 scale {1.0f, 1.0f, 1.0f};
    Quaternion quatRot = MakeQuaternion(rotation);
    model = Dot(CreateTranslationMatrix(position), Identity4x4);
    model = Dot(CreateRotationMatrix(quatRot), model);
    model = Dot(CreateScaleMatrix(scale), model);
    //initial transform so object shows up in the first place until I properly
    //reconstruct the render loop to something that makes more sense it's just
    //in a placeholder state so I can properly hammer out logic first
    bool run = true;
    while(run) {
        SDL_PollEvent(&evt); 
        switch(evt.type) {
                case SDL_EVENT_QUIT:
                    run = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch(evt.key.scancode) {
                        case SDL_SCANCODE_A:
                            position -= Vector3(0.05f, 0.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_D:
                            position += Vector3(0.05f, 0.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_W:
                            position += Vector3(0.0f, 0.0f, 0.05f);
                            break;
                        case SDL_SCANCODE_S:
                            position -= Vector3(0.0f, 0.0f, 0.05f);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            rotation += Vector3(0.0f, 0.5f, 0.0f);
                            break;
                        case SDL_SCANCODE_LEFT:
                            rotation -= Vector3(0.0f, 0.5f, 0.0f);
                            break;
                        case SDL_SCANCODE_UP:
                            rotation += Vector3(0.5f, 0.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_DOWN:
                            rotation -= Vector3(0.5f, 0.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_X:
                            scale += Vector3(1.0f, 0.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_Y:
                            scale += Vector3(0.0f, 1.0f, 0.0f);
                            break;
                        case SDL_SCANCODE_Z:
                            scale += Vector3(0.0f, 0.0f, 1.0f);
                            break;
                    }
                    quatRot = MakeQuaternion(rotation);
                    model = Dot(CreateScaleMatrix(scale), Identity4x4);
                    model = Dot(CreateRotationMatrix(quatRot), model);
                    model = Dot(CreateTranslationMatrix(position), model);
        }
        // std::cout<<"model matrix:\n";
        // for(int i = 0; i<4; i++) {
        //     for(int j = 0; j<4; j++) {
        //         std::cout<<model.data[i][j]<<" ";
        //     }
        //     std::cout<<"\n";
        // }
        std::cout<<"\n\n\n";
        glUseProgram(shaderProg);
        glBindVertexArray(vao);
        glUniformMatrix4fv(modelId, 1, GL_FALSE, &model.data[0][0]);
        glUniformMatrix4fv(projId, 1, GL_FALSE, &projection.data[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &cubeIndices[0]);
        glBindVertexArray(0);
        glUseProgram(0);


        SDL_GL_SwapWindow(mainWindow);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
}
