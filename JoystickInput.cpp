#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <string>
#include <sys/ioctl.h>
#include <cstring>

class JoystickInfo {
    public:
        char name[128];
        char path[256]; //temp path, may change next time
        int numAxes;
        int numButtons;
        
        JoystickInfo() : numAxes(0), numButtons(0) { // constructor
            std::strcpy(name, "Unknown");
            std::strcpy(path, "Unknown");
        }

        void printInfo() const { // print basic info
            std::cout << "Joystick Name: " << name << std::endl;
            std::cout << "Joystick Path: " << path << std::endl;
            std::cout << "Number of Axes: " << numAxes << std::endl;
            std::cout << "Number of Buttons: " << numButtons << std::endl;
        }
};

int main() {
    // Prompt for device path and check the path
    std::string devicePathStr;
    std::cout << "Enter the joystick device path (e.g. /dev/input/js0): ";
    std::cin >> devicePathStr;

    int js = open(devicePathStr.c_str(), O_RDONLY);
 
    if (js == -1) {
        std::cerr << "Could not open joystick at " << devicePathStr << std::endl;
        return -1;
    }

    // Retrieve Joystick info, create object, and print info
    std::cout << "-------------- Joystick Info --------------" << std::endl;
    JoystickInfo joystickInfo;
    std::strcpy(joystickInfo.path, devicePathStr.c_str());
    if (ioctl(js, JSIOCGNAME(sizeof(joystickInfo.name)), joystickInfo.name) < 0) {
        std::strcpy(joystickInfo.name, "Unknown");
    }
    ioctl(js, JSIOCGAXES, &joystickInfo.numAxes);
    ioctl(js, JSIOCGBUTTONS, &joystickInfo.numButtons);
    joystickInfo.printInfo();
 
    // Read joystick events
    std::cout << "-------------- Receiving Inputs --------------" << std::endl;
    struct js_event event;
    while (read(js, &event, sizeof(event)) > 0) {
        // Filter and print the events
        switch (event.type) {
            case JS_EVENT_BUTTON:
                std::cout << "Button " << (int)event.number
<< " is " << (event.value ? "pressed" : "released") << std::endl;
                break;
            case JS_EVENT_AXIS:
                std::cout << "Axis " << (int)event.number
<< " is at position " << event.value << std::endl;
                break;
            default:
                break; // Ignore init events
        }
    }
 
    if (errno != EAGAIN) {
        std::cerr << "Error reading from joystick" << std::endl;
    }
 
    close(js);
    return 0;
}