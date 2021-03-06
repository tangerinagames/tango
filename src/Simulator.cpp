#include <Simulator.h>
#include <iostream>
#include <GL/glfw.h>
#include <lua-headers/moai_lua.h>

#include <aku/AKU.h>
#include <aku/AKU-luaext.h>
#include <aku/AKU-untz.h>

#define NUM_JOYS 4
#define LOG(arg) std::cout << arg << std::endl;


static void joystickButtonThread(void *arg)
{
  int numbuttons = glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_BUTTONS);
  unsigned char *prevButtons = new unsigned char[numbuttons];
  unsigned char *buttons = new unsigned char[numbuttons];
  std::fill_n(prevButtons, numbuttons, GLFW_RELEASE);

  while (true)
  {
    glfwGetJoystickButtons(GLFW_JOYSTICK_1, buttons, numbuttons);
    for (int button = 0; button < numbuttons; button++)
    {
      if (buttons[button] != prevButtons[button])
      {
        AKUEnqueueJoystickEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::JOYSTICK_1, button, buttons[button] == GLFW_PRESS);
      }
    }
    memcpy(prevButtons, buttons, numbuttons);
  }
}


static void keyEventHandler(int key, int action)
{
  AKUEnqueueKeyboardEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::KEYBOARD, key, action == GLFW_PRESS);
}


static void mouseButtonEventHandler(int button, int action)
{
  bool down = action == GLFW_PRESS;
  switch (button)
  {
    case GLFW_MOUSE_BUTTON_LEFT:
      AKUEnqueueButtonEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_LEFT, down);
    break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      AKUEnqueueButtonEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_MIDDLE, down);
    break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      AKUEnqueueButtonEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_RIGHT, down);
    break;
  }
}


static void mousePosEventHandler(int x, int y)
{
  AKUEnqueuePointerEvent(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::POINTER, x, y);
}


static void enterFullscreenMode()
{
  GLFWvidmode desktop;
  glfwGetDesktopMode(&desktop);
  glfwCloseWindow();
  if (!glfwOpenWindow(desktop.Width, desktop.Height, 0, 0, 0, 0, 32, 0, GLFW_FULLSCREEN))
  {
    LOG("Error on switch to Fullscreen mode.");
  }
}


static void exitFullscreenMode()
{
  GLFWvidmode desktop;
  glfwGetDesktopMode(&desktop);
  glfwCloseWindow();
  if (!glfwOpenWindow(desktop.Width, desktop.Height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
  {
    LOG("Error on switch to Window mode.");
  }
}


static void openWindow(const char* title, int width, int height)
{
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

  if (!glfwOpenWindow(width, height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
  {
    LOG("Failed to open GLFW window")
    glfwTerminate();
  }

  glfwSetWindowTitle(title);

  glfwSetKeyCallback(keyEventHandler);
  glfwSetMouseButtonCallback(mouseButtonEventHandler);
  glfwSetMousePosCallback(mousePosEventHandler);
  glfwCreateThread(joystickButtonThread, NULL);

  AKUDetectGfxContext();
  AKUSetScreenSize(width, height);
}


static void createContext()
{
  AKUCreateContext();

  AKUExtLoadLuacrypto();
  AKUExtLoadLuacurl();
  AKUExtLoadLuafilesystem();
  AKUExtLoadLuasocket();
  AKUExtLoadLuasql();
  AKUUntzInit();

  AKUSetInputConfigurationName("TANGO_GLFW");

  AKUReserveInputDevices(TangoInputDeviceID::TOTAL);
  AKUSetInputDevice(TangoInputDeviceID::DEVICE, "device");

  AKUReserveInputDeviceSensors(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::TOTAL);
  AKUSetInputDeviceKeyboard(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::KEYBOARD, "keyboard");
  AKUSetInputDevicePointer(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::POINTER, "pointer");
  AKUSetInputDeviceButton(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_LEFT, "mouseLeft");
  AKUSetInputDeviceButton(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_MIDDLE, "mouseMiddle");
  AKUSetInputDeviceButton(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::MOUSE_RIGHT, "mouseRight");
  AKUSetInputDeviceJoystick(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::JOYSTICK_1, "joystick1");
  AKUSetInputDeviceJoystick(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::JOYSTICK_2, "joystick2");
  AKUSetInputDeviceJoystick(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::JOYSTICK_3, "joystick3");
  AKUSetInputDeviceJoystick(TangoInputDeviceID::DEVICE, TangoInputDeviceSensorID::JOYSTICK_4, "joystick4");

  AKUSetFunc_EnterFullscreenMode(enterFullscreenMode);
  AKUSetFunc_ExitFullscreenMode(exitFullscreenMode);
  AKUSetFunc_OpenWindow(openWindow);

  AKURunBytecode(moai_lua, moai_lua_SIZE);
}


int startTango(const char *filename)
{
  if (!glfwInit())
  {
    LOG("Failed to initialize GLFW")
  }

  createContext();
  AKURunScript(filename);

  while (glfwGetWindowParam(GLFW_OPENED))
  {
    AKUUpdate();
    AKURender();
    glfwSwapBuffers();
  }

  AKUFinalize();
  glfwTerminate();

  return 0;
}
