// TestAppWindowsLauncher.cpp : Defines the entry point for the console application.
//

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stdafx.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <memory>
#include <assert.h>

#include <fstream>

// CoreEngine Dependencies
#include <coreEngine/events/EventQueue.h>
#include <coreEngine/events/EventKeyPress.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotion.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/modifier/ImagePNGLoader.h>
#include <coreEngine/modifier/ImageJPEGLoader.h>
#include <coreEngine/ui/UIFactory.h>
#include <coreEngine/modifier/ImageBMPLoader.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>

// GLImplementation Dependencies
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/opengl/DiffuseTextureCubeMapGLFactory.h>
#include <glImplementation/factory/opengl/DiffuseTextureGLFactory.h>
#include <glImplementation/factory/CameraGLFactory.h>
#include <glImplementation/factory/opengl/UniformColorFactoryGL.h>
#include <glImplementation/factory/opengl/TextMaterialFactoryGL.h>

// WindowsImplementation Dependencies
#include <windowsImplementation/LoggerFactoryWindows.h>
#include <windowsImplementation/MutexLockWindows.h>
#include <windowsImplementation/GazeListenerFactoryWindows.h>

// Windows Renderer
#include <renderer/RendererNoHMD.h>
#include <renderer/RendererNoHMDStereo.h>

// Application Dependency
#include <image360/Image360.h>
#include <image360/Image360Mono.h>
#include <image360/Image360Stereo.h>

//  Windowing Library
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

//enum APPLICATION_MODE { MONO, STEREO };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << "Key Pressed " << std::endl;
	
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Mouse Clicked " << std::endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (application->notifyMeListener->inFocus()){
			std::cout << "Notification Pressed" << std::endl;
		}
		else if (application->closeMeListener->inFocus()){
			std::cout << "Close Box Pressed" << std::endl;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

// Window dimensions
const int WIDTH = 1600, HEIGHT = 1000;

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
	//glViewport(0, 0, width, height);


	std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryWindows());

	cout << "TestAppWindowsLauncer" << endl;
	logger = loggerFactory->createLogger("TestAppWindowsLauncher: ");
	logger->log(LOG_DEBUG, "Testing logger.");
	std::unique_ptr<ISceneFactory> sceneFactory(new SceneGLFactory(loggerFactory.get()));
	std::unique_ptr<IModelFactory> modelFactory(new ModelGLFactory(loggerFactory.get()));
	std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory(new DiffuseTextureGLFactory(loggerFactory.get()));
	std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory(new DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

	

	std::unique_ptr<ICameraFactory> cameraFactory(new CameraGLFactory(loggerFactory.get()));

	std::unique_ptr<IMutexLock> mutexLock(new MutexLockWindows);
	eventQueue = std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

	std::unique_ptr<ITransformTreeFactory> transformTreeFactory(new TransformTreeFactory(loggerFactory.get()));

	std::unique_ptr<IModelFactory> uiModelFactory(new ModelGLFactory(loggerFactory.get()));
	std::unique_ptr<IUniformColorFactory> uiUniformColorFactory(new UniformColorFactoryGL(loggerFactory.get()));
	std::unique_ptr<ITransformTreeFactory> uiTransformTreeFactory(new TransformTreeFactory(loggerFactory.get()));
	std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory(new GazeDetectorFactory);

	std::unique_ptr<ITextMaterialFactory> textMaterialFactory(new TextMaterialFactoryGL(loggerFactory.get()));
	std::unique_ptr<UIFactory> uiFactory(new UIFactory(loggerFactory.get(), std::move(uiModelFactory), std::move(uiUniformColorFactory),
		std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

	std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory(new GazeListenerFactoryWindows(loggerFactory.get()));
	std::string fontFilePath = "C:\\Users\\robin_chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\fonts\\arial.ttf";
	//std::string fontFilePath = "fonts/arial.ttf";

	IMAGE_MODE appMode = MONO;
	
	if (appMode == STEREO){
		std::unique_ptr<IRenderer> renderer(new RendererNoHMDStereo());
		application = std::unique_ptr<Image360Stereo>(new Image360Stereo(std::move(renderer),
			std::move(sceneFactory),
			std::move(modelFactory),
			std::move(diffuseTextureFactory),
			std::move(diffuseTextureCubeMapFactory),
			std::move(transformTreeFactory),
			std::move(cameraFactory),
			eventQueue.get(),
			loggerFactory.get(),
			std::move(uiFactory),
			std::move(gazeDetectorFactory),
			std::move(eventGazeListenerFactory),
			fontFilePath));

		application->start();

		ImageJPEGLoader imageJPEGLoader(logger.get());

		std::vector< std::unique_ptr<Image> > textureImages;
		TEXTURE_MAP_MODE mode = EQUIRECTANGULAR_MAP_MODE; //CUBE_MAP_MODE_SINGLE_IMAGE; // image mode

		switch (mode){
		case CUBE_MAP_MODE_SINGLE_IMAGE:
			//textureImages.push_back(imagePNGLoader.loadImage("cubemap_current.png"));
			textureImages.push_back(imageJPEGLoader.loadImage("cubemap_desert.jpg"));
			//textureImages.push_back(imagePNGLoader.loadImage("C:\\Users\\robin_chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\cubemap_current2.jpg"));
			break;
		case CUBE_MAP_MODE_SIX_IMAGES:
			/*textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_front.bmp"));
			textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_left.bmp"));
			textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_back.bmp"));
			textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_right.bmp"));
			textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_top.bmp"));
			textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_bottom.bmp"));*/
			break;
		case EQUIRECTANGULAR_MAP_MODE:
			//textureImages.push_back(imageBMPLoader.loadImage("tex_current.bmp"));
			//textureImages.push_back(imageJPEGLoader.loadImage("equirectangular_desert2.jpg"));
			//textureImages.push_back(imageJPEGLoader.loadImage("360images\\WitnessSquare-Smartphone-360-Stereo-2016-05-04-00-11-48.jpg"));
			textureImages.push_back(imageJPEGLoader.loadImage("C:\\Users\\robin_chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\360images\\Witcher-BoatSunset-SmartPhone-360-Stereo-2016-05-03-22-13-08.jpg"));
			break;
		}

		application->initialize(mode, textureImages);

		//// register callbacks
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, mouse_pos_callback);


		// Game loop
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
			
			glViewport(0, 0, width, height);
			application->drawInit();

			//glEnable(GL_SCISSOR_TEST);

			glViewport(0, 0, width / 2, height);
			//glScissor(0, 0, width , height);
			application->draw(LEFT);

			glViewport(width / 2, 0, width /2 , height);
			//glScissor(0, 0, width , height);
			application->draw(RIGHT);

			application->drawComplete();

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}

		application->stop();
		application->deinitialize();

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// Terminate GLFW, clearing any resources allocated by GLFW.
		glfwTerminate();

	} 
	else{
		std::unique_ptr<IRenderer> renderer(new RendererNoHMD());
	application = std::unique_ptr<Image360Mono>(new Image360Mono(std::move(renderer),
		std::move(sceneFactory),
		std::move(modelFactory),
		std::move(diffuseTextureFactory),
		std::move(diffuseTextureCubeMapFactory),
		std::move(transformTreeFactory),
		std::move(cameraFactory),
		eventQueue.get(),
		loggerFactory.get(),
		std::move(uiFactory),
		std::move(gazeDetectorFactory),
		std::move(eventGazeListenerFactory),
		fontFilePath));

	application->start();

	ImageJPEGLoader imageJPEGLoader(logger.get());

	std::vector< std::unique_ptr<Image> > textureImages;
	TEXTURE_MAP_MODE mode = EQUIRECTANGULAR_MAP_MODE; //CUBE_MAP_MODE_SINGLE_IMAGE; // image mode

	switch (mode){
	case CUBE_MAP_MODE_SINGLE_IMAGE:
		//textureImages.push_back(imagePNGLoader.loadImage("cubemap_current.png"));
		textureImages.push_back(imageJPEGLoader.loadImage("cubemap_desert.jpg"));
		//textureImages.push_back(imagePNGLoader.loadImage("C:\\Users\\robin_chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\cubemap_current2.jpg"));
		break;
	case CUBE_MAP_MODE_SIX_IMAGES:
		/*textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_front.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_left.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_back.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_right.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_top.bmp"));
		textureImages.push_back(imageBMPLoader.loadImage("cubemap_geo_bottom.bmp"));*/
		break;
	case EQUIRECTANGULAR_MAP_MODE:
		//textureImages.push_back(imageBMPLoader.loadImage("tex_current.bmp"));
		//textureImages.push_back(imageJPEGLoader.loadImage("equirectangular_desert2.jpg"));
		textureImages.push_back(imageJPEGLoader.loadImage("C:\\Users\\robin_chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\equirectangular_desert2.jpg"));
		break;
	}

	application->initialize(mode, textureImages);

	//// register callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		application->drawInit();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	application->stop();
	application->deinitialize();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	glfwTerminate();
}
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 1;
}