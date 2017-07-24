// TestAppWindowsLauncher.cpp : Defines the entry point for the console
// application.
//

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <iostream>
#include "stdafx.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <assert.h>
#include <memory>

#include <fstream>

// CoreEngine Dependencies
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/events/EventKeyPress.h>
//#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotion.h>
#include <coreEngine/events/EventMouse.h>
#include <coreEngine/events/EventQueue.h>
#include <coreEngine/events/EventCloseApplication.h>

#include <coreEngine/modifier/ImageBMPLoader.h>
#include <coreEngine/modifier/ImageJPEGLoader.h>
#include <coreEngine/modifier/ImagePNGLoader.h>
#include <coreEngine/ui/UIFactory.h>

// GLImplementation Dependencies
#include <glImplementation/factory/CameraGLFactory.h>
#include <glImplementation/factory/DiffuseTextureCubeMapGLFactory.h>
#include <glImplementation/factory/DiffuseTextureGLFactory.h>
#include <glImplementation/factory/ModelGLFactory.h>
#include <glImplementation/factory/SceneGLFactory.h>
#include <glImplementation/factory/TextMaterialFactoryGL.h>
#include <glImplementation/factory/UniformColorFactoryGL.h>
#include <glImplementation/factory/UniformFadeColorFactoryGL.h>

// WindowsImplementation Dependencies
#include <windowsImplementation/GazeListenerFactoryWindows.h>
#include <windowsImplementation/LoggerFactoryWindows.h>
#include <windowsImplementation/MutexLockWindows.h>
#include <windowsImplementation/GlfwEventCloseApplicationListener.h>

// Windows Renderer
#include <renderer/RendererNoHMD.h>
#include <renderer/RendererNoHMDStereo.h>

// Application Dependency
#include <image360/Image360.h>

#include <image360/FadeScreen.h>
#include <image360/Buttons.h>
#include <image360/Constants.h>
#include <image360/Controller.h>
#include <image360/FPSCamera.h>
#include <image360/MonoCubeMap.h>
#include <image360/MonoSphere.h>
#include <image360/StereoSphere.h>
#include <image360/CameraReticle.h>

#include <image360/Image360EventKeyPressListener.h>
#include <image360/Image360EventPassiveMouseMotionListener.h>
#include <image360/Image360EventMouseListener.h>
#include <image360/Image360EventBeginFade.h>

//  Windowing Library
#include <GLFW/glfw3.h>

using namespace std;
using namespace cl;

float color = 1.0f;

/***
Threading and mutex from here
https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927(v=vs.85).aspx
*/

/****
Glut tutorial http://www.lighthouse3d.com/tutorials/glut-tutorial/
*/

// Application Components
std::unique_ptr<Controller> controller;
std::unique_ptr<StereoSphere> stereoSphere;
std::unique_ptr<MonoSphere> monoSphere;
std::unique_ptr<MonoCubeMap> monoCubeMap;
std::unique_ptr<Buttons> buttons;
std::unique_ptr<FPSCamera> fpsCamera;
std::unique_ptr<FadeScreen> fadeScreen;
std::unique_ptr<CameraReticle> cameraReticle;

std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;

std::unique_ptr<Image360> application;
std::unique_ptr<ILogger> logger;
std::unique_ptr<IEventQueue> eventQueue = nullptr;
std::unique_ptr<ILoggerFactory> loggerFactory(new LoggerFactoryWindows());

std::unique_ptr<EventKeyPressListener> eventKeyPressListener;
std::unique_ptr<EventPassiveMouseMotionListener> eventPassiveMouseMotionListener;
std::unique_ptr<EventMouseListener> eventMouseListener;

std::unique_ptr<EventCloseApplication> eventCloseApplication;
std::unique_ptr<EventCloseApplicationListener> eventCloseApplicationListener;
std::unique_ptr<IEvent> image360EventBeginFade;

int mouse_button_wrapper(int button){
	switch (button){
	case GLFW_MOUSE_BUTTON_LEFT:
		return cl::LEFT_BUTTON;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return cl::MIDDLE_BUTTON;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return cl::RIGHT_BUTTON;
	default:
		return -1;
	}
}

int mouse_action_wrapper(int action){
	switch (action){
	case GLFW_PRESS:
		return cl::MOUSE_BUTTON_DOWN;
	case GLFW_RELEASE:
		return cl::MOUSE_BUTTON_UP;
	default:
		return -1;
	}
}

// enum APPLICATION_MODE { MONO, STEREO };

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode) {
  std::cout << "Key Pressed " << key << std::endl;

  // todo - use buttons directly here
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  } else {
    std::unique_ptr<IEvent> keyPressEvent(
        new EventKeyPress(eventKeyPressListener.get(),
                          key, action, mode));
    eventQueue->push(std::move(keyPressEvent));
  }
}



void mouse_pos_callback(GLFWwindow* window, double mouseXPos,
                        double mouseYPos) {
  std::unique_ptr<IEvent> mousePassiveEvent(new EventPassiveMouseMotion(
      eventPassiveMouseMotionListener.get(), mouseXPos,
      mouseYPos));
  eventQueue->push(std::move(mousePassiveEvent));
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods) {
  std::cout << "Mouse Clicked " << std::endl;

  // todo - this kind of testing should be handled by the application
  auto image360_button = (cl::MOUSE_BUTTON)mouse_button_wrapper(button);
  auto image360_action = (cl::MOUSE_BUTTON_STATE)mouse_action_wrapper(action);
  
  std::unique_ptr<IEvent> mouseButtonEvent(new EventMouse(
	  eventMouseListener.get(), image360_button, image360_action));
  eventQueue->push(std::move(mouseButtonEvent));
}

// External Files required for this application
// use of hard coded file paths enables debugging in visual studio. 
std::string fontFilePath =
"C:\\Users\\robin_"
"chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher"
"\\Debug\\fonts\\arial.ttf";
// std::string fontFilePath = "fonts/arial.ttf";

std::string laserTextureFilePath = "C:\\Users\\robin_"
"chimera\\SDK\\Projects\\VisualStudio\\Image360WindowsLa"
"uncher\\Debug\\laserTexture.png";

std::string controllerTextureFilePath = "C:\\Users\\robin_"
"chimera\\SDK\\Projects\\VisualStudio\\Image360WindowsLa"
"uncher\\Debug\\ddcontroller_idle.png";

std::string controllerModelPath("C:\\Users\\robin_"
	"chimera\\Documents\\SDK\\Projects\\VisualStudio\\Image360WindowsLa"
	"uncher\\Debug\\ddController.obj");

std::string stereoEquirectangleImageFilePath = "C:\\Users\\robin_"
"chimera\\SDK\\Projects\\VisualStudio\\Image360WindowsLauncher\\Debug\\"
"360images\\Witcher-BoatSunset-SmartPhone-360-Stereo.jpg";

std::string monoCubeMapImageFilePath = "C:\\Users\\robin_"
"chimera\\SDK\\Projects\\VisualStudio\\Image360WindowsLa"
"uncher\\Debug\\360images\\cubemap_desert.jpg";

std::string monoEquirectangleImageFilePath = "C:\\Users\\robin_"
	  "chimera\\SDK\\Projects\\VisualStudio\\Image360WindowsLa"
	  "uncher\\Debug\\360images\\equirectangular_desert.jpg";

// Window dimensions
const int WIDTH = 1600, HEIGHT = 1000;

int _tmain(int argc, _TCHAR** argv) {
  char** argvTyped = (char**)argv;
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window =
      glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // Define the viewport dimensions
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  
  cout << "TestAppWindowsLauncer" << endl;
  logger = loggerFactory->createLogger("TestAppWindowsLauncher: ");
  logger->log(LOG_DEBUG, "Testing logger.");
  

  eventCloseApplicationListener = std::unique_ptr<EventCloseApplicationListener>(
	  new GlfwEventCloseApplicationListener(*window));
  
  eventCloseApplication = std::unique_ptr<EventCloseApplication>(new EventCloseApplication(eventCloseApplicationListener.get()));

  std::unique_ptr<ISceneFactory> sceneFactory(
      new SceneGLFactory(loggerFactory.get()));
  std::unique_ptr<IModelFactory> modelFactory(
      new ModelGLFactory(loggerFactory.get()));
  std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory(
      new DiffuseTextureGLFactory(loggerFactory.get()));
  std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory(
      new DiffuseTextureCubeMapGLFactory(loggerFactory.get()));

  std::unique_ptr<ICameraFactory> cameraFactory(
      new CameraGLFactory(loggerFactory.get()));

  std::unique_ptr<IMutexLock> mutexLock(new MutexLockWindows);
  eventQueue =
      std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutexLock)));

  std::unique_ptr<ITransformTreeFactory> transformTreeFactory(
      new TransformTreeFactory(loggerFactory.get()));

  std::unique_ptr<IModelFactory> uiModelFactory(
      new ModelGLFactory(loggerFactory.get()));
  std::unique_ptr<IUniformColorFactory> uiUniformColorFactory(
      new UniformColorFactoryGL(loggerFactory.get()));
  std::unique_ptr<IUniformFadeColorFactory> uiUniformFadeColorFactory(
      new UniformFadeColorFactoryGL(loggerFactory.get()));
  std::unique_ptr<ITransformTreeFactory> uiTransformTreeFactory(
      new TransformTreeFactory(loggerFactory.get()));
  std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory(
      new GazeDetectorFactory);

  std::unique_ptr<ITextMaterialFactory> textMaterialFactory(
      new TextMaterialFactoryGL(loggerFactory.get()));
  std::unique_ptr<UIFactory> uiFactory(new UIFactory(
      loggerFactory.get(), std::move(uiModelFactory),
      std::move(uiUniformColorFactory), std::move(uiUniformFadeColorFactory),
      std::move(uiTransformTreeFactory), std::move(textMaterialFactory)));

  std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory(
      new GazeListenerFactoryWindows(loggerFactory.get()));
  

  gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

  ImageJPEGLoader imageJPEGLoader(logger.get());
  ImagePNGLoader imagePNGLoader(logger.get());

  std::unique_ptr<Image> laserBeamTexture =
	  imagePNGLoader.loadImage(laserTextureFilePath);

  std::unique_ptr<Image> controllerTexture =
	  imagePNGLoader.loadImage(controllerTextureFilePath);

  std::vector<std::unique_ptr<Image> > stereoTextureImages;

  stereoTextureImages.push_back(imageJPEGLoader.loadImage(stereoEquirectangleImageFilePath));

  std::vector<std::unique_ptr<Image> > monoCubeTextureImages;

  monoCubeTextureImages.push_back(imageJPEGLoader.loadImage(monoCubeMapImageFilePath));

  std::vector<std::unique_ptr<Image> > monoSphereTextureImages;

  monoSphereTextureImages.push_back(imageJPEGLoader.loadImage(monoEquirectangleImageFilePath));

  IMAGE_MODE appMode = MONO;

  if (appMode == STEREO) {
    std::unique_ptr<IRenderer> renderer(new RendererNoHMDStereo());

	controller = std::unique_ptr<Controller>(new Controller(*loggerFactory, *modelFactory,
		*transformTreeFactory, *diffuseTextureFactory, *uiFactory,
		std::move(controllerTexture), std::move(laserBeamTexture),
		controllerModelPath));

	stereoSphere = std::unique_ptr<StereoSphere>(new StereoSphere(
		*loggerFactory, *modelFactory,
		*diffuseTextureFactory, *transformTreeFactory,
		std::move(stereoTextureImages[0])));

	fpsCamera = std::unique_ptr<FPSCamera>(new FPSCamera(
		*loggerFactory, *transformTreeFactory, *cameraFactory
		));

	auto gazeTransformSource = fpsCamera->getCameraTransformTree();

	fadeScreen = std::unique_ptr<FadeScreen>(new FadeScreen(
		*loggerFactory, *uiFactory, *gazeTransformSource, *eventCloseApplicationListener));

	image360EventBeginFade = std::unique_ptr<IEvent>(new Image360EventBeginFade(*fadeScreen, *loggerFactory));

	buttons = std::unique_ptr<Buttons>(new Buttons(
		*loggerFactory, *uiFactory,
		*gazeDetectorContainer, *gazeDetectorFactory,
		*eventGazeListenerFactory,
		gazeTransformSource, fontFilePath, *image360EventBeginFade));

	cameraReticle = std::unique_ptr<CameraReticle>(new CameraReticle(*loggerFactory, *uiFactory, *gazeTransformSource));
	
    application = std::unique_ptr<Image360>(
        new Image360(std::move(renderer), *sceneFactory,
                     *transformTreeFactory, *cameraFactory,
                     *eventQueue, *loggerFactory,
					 *uiFactory, *gazeDetectorContainer,
					 *eventGazeListenerFactory));

	
	eventKeyPressListener = std::unique_ptr<EventKeyPressListener>(
		new Image360EventKeyPressListener(application.get(), loggerFactory.get()));

	eventPassiveMouseMotionListener = std::unique_ptr<EventPassiveMouseMotionListener>(
		new Image360EventPassiveMouseMotionListener(*fpsCamera, *loggerFactory));

	eventMouseListener = std::unique_ptr<EventMouseListener>(
		new Image360EventMouseListener(*buttons, *loggerFactory.get()));
	
	application->addApplicationObject(stereoSphere.get());
	application->addApplicationObject(fpsCamera.get());
	application->addStereoObject(stereoSphere.get());
	application->addApplicationObject(controller.get());
	application->addApplicationObject(buttons.get());
	application->addApplicationObject(fadeScreen.get());
	application->addApplicationObject(cameraReticle.get());

    application->start();

    application->initialize();

	std::cout << buttons->getActionButtonText() << std::endl;
	buttons->setActionButtonText(std::string("Download"));
	std::cout << buttons->getActionButtonText() << std::endl;

    //// register callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
      // Check if any events have been activiated (key pressed, mouse moved
      // etc.) and call corresponding response functions
      
      glfwPollEvents();

      glViewport(0, 0, width, height);
      application->drawInit();

      // glEnable(GL_SCISSOR_TEST);

      glViewport(0, 0, width / 2, height);
      // glScissor(0, 0, width , height);
      application->drawStereoLeft();

      glViewport(width / 2, 0, width / 2, height);
      // glScissor(0, 0, width , height);
      application->drawStereoRight();

      application->drawComplete();

      // Swap the screen buffers
      glfwSwapBuffers(window);
    }

    application->stop();
    application->deinitialize();

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

  } else {

  TEXTURE_MAP_MODE mode = CUBE_MAP_MODE_SINGLE_IMAGE;
	// EQUIRECTANGULAR_MAP_MODE;  // CUBE_MAP_MODE_SINGLE_IMAGE; // image mode

    std::unique_ptr<IRenderer> renderer(new RendererNoHMD());

	controller = std::unique_ptr<Controller>(new Controller(*loggerFactory, *modelFactory,
		*transformTreeFactory, *diffuseTextureFactory, *uiFactory,
		std::move(controllerTexture), std::move(laserBeamTexture),
		controllerModelPath));

	fpsCamera = std::unique_ptr<FPSCamera>(new FPSCamera(
		*loggerFactory, *transformTreeFactory, *cameraFactory
		));

	stereoSphere = std::unique_ptr<StereoSphere>(new StereoSphere(
		     *loggerFactory, *modelFactory,
		     *diffuseTextureFactory, *transformTreeFactory,
			 std::move(stereoTextureImages[0])));

	monoSphere = std::unique_ptr<MonoSphere>(new MonoSphere(
		*loggerFactory, *modelFactory,
		*diffuseTextureFactory, *transformTreeFactory,
		std::move(monoSphereTextureImages[0])));
	
	monoCubeMap = std::unique_ptr<MonoCubeMap>(new MonoCubeMap(
		*loggerFactory, *modelFactory, 
		*diffuseTextureCubeMapFactory, *transformTreeFactory,
		std::move(monoCubeTextureImages[0])));

	auto gazeTransformSource = fpsCamera->getCameraTransformTree();

	fadeScreen = std::unique_ptr<FadeScreen>(new FadeScreen(
		*loggerFactory, *uiFactory, *gazeTransformSource, *eventCloseApplicationListener));

	cameraReticle = std::unique_ptr<CameraReticle>(new CameraReticle(*loggerFactory, *uiFactory, *gazeTransformSource));

	image360EventBeginFade = std::unique_ptr<IEvent>(new Image360EventBeginFade(*fadeScreen, *loggerFactory));

	buttons = std::unique_ptr<Buttons>(new Buttons(
		*loggerFactory, *uiFactory,
		*gazeDetectorContainer, *gazeDetectorFactory,
		*eventGazeListenerFactory,
		gazeTransformSource, fontFilePath, *image360EventBeginFade));

    application = std::unique_ptr<Image360>(
        new Image360(std::move(renderer), *sceneFactory,
                     *transformTreeFactory, *cameraFactory,
                     *eventQueue, *loggerFactory,
					 *uiFactory, *gazeDetectorContainer,
                     *eventGazeListenerFactory));

	
	if (mode == CUBE_MAP_MODE_SINGLE_IMAGE){
		application->addApplicationObject(monoCubeMap.get());
	}
	if (mode == EQUIRECTANGULAR_MAP_MODE){
		application->addApplicationObject(monoSphere.get());
	}

	application->addApplicationObject(fpsCamera.get());
	application->addApplicationObject(buttons.get());
	application->addApplicationObject(controller.get());
	application->addApplicationObject(fadeScreen.get());
	application->addApplicationObject(cameraReticle.get());

	eventKeyPressListener = std::unique_ptr<EventKeyPressListener>(
		new Image360EventKeyPressListener(application.get(), loggerFactory.get()));

	eventPassiveMouseMotionListener = std::unique_ptr<EventPassiveMouseMotionListener>(
		new Image360EventPassiveMouseMotionListener(*fpsCamera, *loggerFactory));

	eventMouseListener = std::unique_ptr<EventMouseListener>(
		new Image360EventMouseListener(*buttons, *loggerFactory.get()));

    application->start();

	application->initialize();

	buttons->setActionButtonText(std::string("Download"));
	
    //// register callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
      // Check if any events have been activiated (key pressed, mouse moved
      // etc.) and call corresponding response functions
      glfwPollEvents();

      application->drawInit();
	  application->drawMono();
	  application->drawComplete();

      // Swap the screen buffers
      glfwSwapBuffers(window);
    }

    application->stop();
    application->deinitialize();

    
    glfwTerminate();
  }

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  return 1;
}