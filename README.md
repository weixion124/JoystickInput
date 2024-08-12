# Joystick Input

## Setup environment
Install needed dependencies
```
sudo apt-get update
sudo apt-get install build-essential
```

## Compile program
```
g++ JoystickInput.cpp -o JoystickInput
```

## Run JoystickInput
Find the correct path of your joystick device, it's usually located at /dev/input/\
Then run the program:
```
./JoystickInput
```
