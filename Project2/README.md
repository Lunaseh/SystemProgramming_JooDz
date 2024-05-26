# Project Part 2: QRcode Recognition with C++
For this project part 2, you will implement QRcode Recognition using C++ language. You
will use the opencv library to implement code. You will use the Raspbot camera to
perform this assignment.
### Goal
- Implement a QR code recognition program using C++ (not C).
- Successfully read information from QR codes.
### Prerequisites
- Understand the basic components of QR codes.
- Have access to a camera sensor on the Raspbot.
- Install OpenCV (version 4.0.0 or higher).
- Refer to the Raspbot's official website for guidance if you encounter difficulties.
http://www.yahboom.net/study/Raspbot
### Requirements
- Your code must compile with the simple execution of the make command. This requires you to create a Makefile in the same directory as your source code.
- The executable binary resulting from the compilation must be named qr recognition. i.e., Your program will be executed with the command: “./qrrecognition.”
### Notes
- After following the installation instructions for the package, you may find that your system has two versions of the OpenCV library installed: 3.2.0 and 4.x.x.
- OpenCV versions earlier than 4.0.0 do not support QR code-related libraries.
- Thus, to utilize QR code-related classes or functions, you should compile your .cpp files with the appropriate options to ensure the correct library version is used.
- For example, you can use the command pkg-config --cflags --libs opencv4 to compile, as shown below:
- $ g++ main.c -o run.out `pkg-config --cflags --libs opencv4
