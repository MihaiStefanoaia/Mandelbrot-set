# Mandelbrot Set Visualiser

## A simple mandelbrot set visualiser made with sfml 2.5.1 64 bit

### How to use:

- use the arrow keys or WASD to move the camera

- use the '+' and '-' keys to zoom in and zoom out respectively (both the numpad and the number row keys work)

- wait for the program to render and display the frame (might take a few seconds)

### Notes:

- The resolution has been lowered significantly in order to allow this to run in an even remotely interactive way

- the program is prone to crashing and rendering artifacts. I have been working on fixing them, and so far have been moderately successful

### To be done:

- switch the brute force computation from the CPU to the GPU (probably with CUDA or OpenCL) | for now managed to get CPU multithreading to work using OpenMP

- add a high resolution saving menu for the current view

- generally optimize the program to run better 
