// TestAppWindowsLauncher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include <coreEngine/IApplication.h>
#include <testApp/TestApp.h>


#include <fstream>


#include <windowsImplementation/LoggerFactoryWindows.h>
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/MaterialDiffuseTextureGLFactory.h>
#include <glImplementation/factory/ShaderDiffuseTextureGLFactory.h>
#include <glImplementation/factory/TextureGLFactory.h>
#include <glImplementation/factory/CameraGLFactory.h>
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>

#include <renderer/RendererNoHMD.h>

using namespace std;
using namespace cl;

float color = 1.0f;


/***
Threading and mutex from here https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927(v=vs.85).aspx
*/


/****
Glut tutorial http://www.lighthouse3d.com/tutorials/glut-tutorial/
*/

std::unique_ptr<IApplication> application;
std::unique_ptr<ILogger> logger;


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()){
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()){
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }



    GLint Result = GL_FALSE;
    int InfoLogLength;



    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0){
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

GLuint loadBMP_custom(const char * imagepath){

    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath, "rb");
    if (!file)							    { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, file) != 54){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M'){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if (*(int*)&(header[0x1E]) != 0)         { printf("Not a correct BMP file\n");    return 0; }
    if (*(int*)&(header[0x1C]) != 24)         { printf("Not a correct BMP file\n");    return 0; }

    // Read the information about the image
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file wan be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete[] data;

    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Return the ID of the texture we just created
    return textureID;
}


void testRender(){
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint m = glGetUniformLocation(programID, "m");
    GLuint v = glGetUniformLocation(programID, "v");
    GLuint p = glGetUniformLocation(programID, "p");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Load the texture using any two methods
    //GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    GLuint Texture = loadBMP_custom("uvtemplate.bmp");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "diffuseTexture");
    
    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    // Two UV coordinatesfor each vertex. They were created withe Blender.
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f - 0.000004f,
        0.000103f, 1.0f - 0.336048f,
        0.335973f, 1.0f - 0.335903f,
        1.000023f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.336024f, 1.0f - 0.671877f,
        0.667969f, 1.0f - 0.671889f,
        1.000023f, 1.0f - 0.000013f,
        0.668104f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.000059f, 1.0f - 0.000004f,
        0.335973f, 1.0f - 0.335903f,
        0.336098f, 1.0f - 0.000071f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.336024f, 1.0f - 0.671877f,
        1.000004f, 1.0f - 0.671847f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.668104f, 1.0f - 0.000013f,
        0.335973f, 1.0f - 0.335903f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.668104f, 1.0f - 0.000013f,
        0.336098f, 1.0f - 0.000071f,
        0.000103f, 1.0f - 0.336048f,
        0.000004f, 1.0f - 0.671870f,
        0.336024f, 1.0f - 0.671877f,
        0.000103f, 1.0f - 0.336048f,
        0.336024f, 1.0f - 0.671877f,
        0.335973f, 1.0f - 0.335903f,
        0.667969f, 1.0f - 0.671889f,
        1.000004f, 1.0f - 0.671847f,
        0.667979f, 1.0f - 0.335851f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader, 
        // in the "MVP" uniform
        glUniformMatrix4fv(m, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(v, 1, GL_FALSE, &View[0][0]);
        glUniformMatrix4fv(p, 1, GL_FALSE, &Projection[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
            );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

}

void renderScene()
{
    application->initialize();
    application->draw();
    //testRender();
    /*if (color < 0.0) color = 1.0;
    color -= 0.001;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color, 0.0, 0.0, 1.0);//clear red*/

    glutSwapBuffers();
}

/*
void startThread(void){
    ghMutex = CreateMutex(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             // unnamed mutex

    if (ghMutex == NULL)
    {
        printf("CreateMutex error: %d\n", GetLastError());
    }

    HANDLE aThread[1];
    DWORD ThreadID;
    aThread[0] = CreateThread(
        NULL,       // default security attributes
        0,          // default stack size
        (LPTHREAD_START_ROUTINE)renderScene,
        NULL,       // no thread function arguments
        0,          // default creation flags
        &ThreadID); // receive thread identifier

    if (aThread[0] == NULL)
    {
        printf("CreateThread error: %d\n", GetLastError());
    }

    WaitForSingleObject(aThread, INFINITE);
    CloseHandle(aThread[0]);
    CloseHandle(ghMutex);
}
*/

int _tmain(int argc, _TCHAR** argv)
{
    char ** argvTyped = (char **)argv;
    glutInit(&argc, argvTyped);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(500, 500);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL First Window");
    glewInit();
    if (glewIsSupported("GL_VERSION_4_5"))
    {
        //std::cout << " GLEW Version is 4.5\n ";
    }
    else
    {
        //std::cout << "GLEW 4.5 not supported\n ";
    }
    
    std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryWindows());

    cout << "TestAppWindowsLauncer" << endl;
    logger = loggerFactory->createLogger("TestAppWindowsLauncher: ");
    logger->log(LOG_DEBUG, "Testing logger.");
    std::unique_ptr<ISceneFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
    std::unique_ptr<IModelFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
    std::unique_ptr<IMaterialDiffuseTextureFactory> materialDiffuseFactory(new MaterialDiffuseTextureGLFactory(loggerFactory.get()));
    std::unique_ptr<IShaderDiffuseTextureFactory> shaderDiffuseFactory(new ShaderDiffuseTextureGLFactory(loggerFactory.get()));
    std::unique_ptr<ITextureFactory> textureFactory(new TextureGLFactory(loggerFactory.get()));
    std::unique_ptr<IRenderer> renderer(new RendererNoHMD());
    std::unique_ptr<ITransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
    std::unique_ptr<ITransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));
    std::unique_ptr<ICameraFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));
    application = std::unique_ptr<IApplication> (new TestApp(std::move(renderer),
        std::move(sceneFactory),
        std::move(modelFactory),
        std::move(textureFactory),
        std::move(materialDiffuseFactory),
        std::move(shaderDiffuseFactory),
        std::move(transformCameraFactory),
        std::move(transformModelFactory),
        std::move(cameraFactory),
        loggerFactory.get()));
    
    // register callbacks
    application->start();
    //application->initialize();
    glutDisplayFunc(renderScene);
    //glutIdleFunc(renderScene);
    glutMainLoop();
    return 0;
}
