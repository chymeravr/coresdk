// TestAppWindowsLauncher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

float color = 0.0f;

void renderScene(void)
{
    while (1){
        if (color > 1.0) color = 0.0;
        color += 0.001;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color, 0.0, 0.0, 1.0);//clear red

        glutSwapBuffers();
    }
}

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

    glEnable(GL_DEPTH_TEST);
    // register callbacks
    glutDisplayFunc(renderScene);

    glutMainLoop();

    return 0;
}

