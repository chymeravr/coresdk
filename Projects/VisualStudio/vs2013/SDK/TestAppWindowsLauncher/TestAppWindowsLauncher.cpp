// TestAppWindowsLauncher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

float color = 1.0f;


/***
Threading and mutex from here https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927(v=vs.85).aspx
*/


/****
Glut tutorial http://www.lighthouse3d.com/tutorials/glut-tutorial/
*/
void renderScene()
{
    if (color < 0.0) color = 1.0;
    color -= 0.001;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color, 0.0, 0.0, 1.0);//clear red
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

    glEnable(GL_DEPTH_TEST);
    // register callbacks
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutMainLoop();

    return 0;
}
