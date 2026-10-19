# Shapes Puzzle

An interactive 2D puzzle game built with **C++** and **OpenGL (GLUT)**, developed as a group project for the Computer Graphics course at Umm Al-Qura University.

## About the Game

The player builds an object by moving and transforming basic shapes (triangle, square, circle, trapezoid). The game has three levels:

1. **House**: place the triangle above the square.
2. **Car**: place two circles under the square, with the trapezoid above it.
3. **Ship**: place a square above the trapezoid, with two circles inside it.

When shapes get close to the right position, a "smart snap" system aligns them automatically. A message appears when the level is completed.

## Computer Graphics Concepts Used

- 2D shape drawing with OpenGL primitives
- Geometric transformations: translation, rotation, and non-uniform scaling
- Texture mapping (background image loaded from a BMP file)
- Real-time text rendering and keyboard interaction
- Double buffering for smooth rendering

## Controls

| Key | Action |
| --- | --- |
| Arrow keys | Move the selected shape |
| X / x | Scale horizontally (bigger / smaller) |
| Y / y | Scale vertically (bigger / smaller) |
| R / r | Rotate right / left |
| Tab | Switch between shapes |
| t | Change the selected shape type |
| N | Go to the next level (after winning) |
| G | Reset the shapes (after winning) |

The selected shape is highlighted in green.

## How to Run

1. Install OpenGL, GLUT, and GLEW (for example, freeglut) and set them up in Visual Studio.
2. Open `ProjectLab.cpp` in a C++ project and build it.
3. Put `background.bmp` in the same folder as the program to show the background.

## Team

Group project (3 members), Computer Graphics course, Umm Al-Qura University, May 2026.

**Author:** Omar Alzahrani
