#ifndef __SIMULATOR_H_
#define __SIMULATOR_H_

namespace TangoInputDeviceID {
  enum {
    DEVICE,
    TOTAL,
  };
}

namespace TangoInputDeviceSensorID {
  enum {
    JOYSTICK_1,
    JOYSTICK_2,
    JOYSTICK_3,
    JOYSTICK_4,
    KEYBOARD,
    POINTER,
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,
    TOTAL,
  };
}

int startTango(const char*);

#endif // __SIMULATOR_H_
