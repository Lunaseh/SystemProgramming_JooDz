# Project Part 1: Line Tracer with C
Soon, your team will start an engaging project involving a Raspbot. Before we delve into the
main project, we will develop line tracing code in the C programming language, which will be
integral to our upcoming project. In the first part of this project, we will use an IR tracking
sensor to trace a black line. You are free to use any C library that suits your needs.

### Goal
- Implementing a line tracing program with C. (not C++)

  
### Prerequisites
- Set up your Raspbot following the provided guidelines.
- Install the WiringPi library on your Raspberry Pi. (While not mandatory, this library is highly recommended for its utility.)


### Requirements
- Your code must compile with the simple execution of the make command. This requires you to create a Makefile in the same directory as your source code.
- The executable binary resulting from the compilation must be named linetracer.
      i.e., Your program will be executed with the command: “./linetracer.”
  
### Notes
- Sensor Data Usage: To utilize data from sensors, it is important to determine which GPIO port each sensor is connected to. This information can be obtained from the Raspbot's official website.
- Code References: For additional insights, you may refer to Python code examples available at Yahboom's Raspbot study page: http://www.yahboom.net/study/Raspbot
- For the final demo, we will use the common black insulation tape (절연테이프) to make a track, whose width is 19mm. Please refer to the images below
