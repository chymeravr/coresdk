// TestAppWindowsLauncher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include <image360/Image360.h>
#include <assert.h>

#include <fstream>

#include <windowsImplementation/LoggerFactoryWindows.h>
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengl/DiffuseTextureCubeMapGLFactory.h>
#include <glImplementation/factory/opengl/DiffuseTextureGLFactory.h>
#include <glImplementation/factory/CameraGLFactory.h>
#include <coreEngine/components/transform/TransformCameraFactory.h>
#include <coreEngine/components/transform/TransformModelFactory.h>
#include <coreEngine/events/EventQueue.h>
#include <coreEngine/events/EventKeyPress.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotion.h>
#include <windowsImplementation/MutexLockWindows.h>
#include <renderer/RendererNoHMD.h>
#include <coreEngine/modifier/ImageBMPLoader.h>
#include <coreEngine/modifier/ImagePNGLoader.h>


#include <GLFW/glfw3.h>


using namespace std;
using namespace cl;

float color = 1.0f;

/***
Threading and mutex from here https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927(v=vs.85).aspx
*/

/****
Glut tutorial http://www.lighthouse3d.com/tutorials/glut-tutorial/
*/

std::unique_ptr<Image360> application;
std::unique_ptr<ILogger> logger;
std::unique_ptr<IEventQueue> eventQueue = nullptr;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else{
		std::unique_ptr<IEvent> keyPressEvent(new EventKeyPress((EventKeyPressListener*)(Image360*)application.get(), key, action, mode));
		eventQueue->push(std::move(keyPressEvent));
	}
	
}

void mouse_pos_callback(GLFWwindow* window, double mouseXPos, double mouseYPos)
{
	std::unique_ptr<IEvent> mousePassiveEvent(new EventPassiveMouseMotion((EventPassiveMouseMotionListener*)(Image360*)application.get(), mouseXPos, mouseYPos));
	eventQueue->push(std::move(mousePassiveEvent));
}

void mouse(int button, int state,
	int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP){
			logger->log(LOG_DEBUG, "Left button up at " + std::to_string(x) + "," + std::to_string(y));
		}
		else if (state == GLUT_DOWN){
			logger->log(LOG_DEBUG, "Left button down at " + std::to_string(x) + "," + std::to_string(y));
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP){
			logger->log(LOG_DEBUG, "Middle button up at " + std::to_string(x) + "," + std::to_string(y));
		}
		else if (state == GLUT_DOWN){
			logger->log(LOG_DEBUG, "Middle button down at " + std::to_string(x) + "," + std::to_string(y));
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP){
			logger->log(LOG_DEBUG, "Right button up at " + std::to_string(x) + "," + std::to_string(y));
		}
		else if (state == GLUT_DOWN){
			logger->log(LOG_DEBUG, "Right button down at " + std::to_string(x) + "," + std::to_string(y));
		}
		break;
	}
}

void mousePassiveMotion(int x, int y){
	std::unique_ptr<IEvent> mousePassiveEvent(new EventPassiveMouseMotion((EventPassiveMouseMotionListener*)(Image360*)application.get(), x, y));
	eventQueue->push(std::move(mousePassiveEvent));
}

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;


int _tmain(int argc, _TCHAR** argv)
{
	char ** argvTyped = (char **)argv;
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryWindows());

	cout << "TestAppWindowsLauncer" << endl;
	logger = loggerFactory->createLogger("TestAppWindowsLauncher: ");
	logger->log(LOG_DEBUG, "Testing logger.");
	std::unique_ptr<ISceneFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
	std::unique_ptr<IModelFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
	std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory(new DiffuseTextureGLFactory(loggerFactory.get()));
	std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLFactory(loggerFactory.get()));
	std::unique_ptr<IRenderer> renderer(new RendererNoHMD());
	std::unique_ptr<ITransformCameraFactory> transformCameraFactory(new TransformCameraFactory(loggerFactory.get()));
	std::unique_ptr<ITransformModelFactory> transformModelFactory(new TransformModelFactory(loggerFactory.get()));
	std::unique_ptr<ICameraFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));
	std::unique_ptr<IMutexLock> mutexLock(new MutexLockWindows);
	eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));
	application = std::unique_ptr<Image360>(new Image360(std::move(renderer),
		std::move(sceneFactory),
		std::move(modelFactory),
		std::move(diffuseTextureFactory),
		std::move(diffuseTextureCubeMapFactory),
		std::move(transformCameraFactory),
		std::move(transformModelFactory),
		std::move(cameraFactory),
		eventQueue.get(),
		loggerFactory.get()));

	//// register callbacks
	application->start();
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);

	ImageBMPLoader imageBMPLoader(logger.get());
	ImagePNGLoader imagePNGLoader(logger.get());
	std::vector< std::unique_ptr<Image> > textureImages;
	TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SINGLE_IMAGE; // image mode

	switch (mode){
	case CUBE_MAP_MODE_SINGLE_IMAGE:
		textureImages.push_back(imagePNGLoader.loadImage("cubemap_current.png"));
		break;
	case CUBE_MAP_MODE_SIX_IMAGES:
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_front.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_left.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_back.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_right.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_top.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_bottom.bmp"));
		break;
	case EQUIRECTANGULAR_MAP_MODE:
		textureImages.push_back(imageBMPLoader.loadImage("tex_current.bmp"));
		break;
	}

	application->initialize(mode, textureImages);
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		application->draw();
	
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	
	application->stop();
	application->deinitialize();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 1;
}