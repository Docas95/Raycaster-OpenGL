# Raycaster-OpenGL
A Raycaster written with C++ and OpenGL using Visual Studio.

## What is a raycaster?
Raycasting is used to efficiently render a 3D world from a 2D playing field using a simple one-dimensional scan over the horizontal width of the screen. Early first-person shooters used 2D ray casting as a technique to create a 3D effect from a 2D world. While the world appears 3D, the player cannot look up or down or only in limited angles with shearing distortion. Read more [here](https://en.wikipedia.org/wiki/Ray_casting#Ray_casting_in_early_computer_games).

## Showcase
[![SHOWCASE VIDEO](https://img.youtube.com/vi/nwDxqbrdZoY/0.jpg)](https://www.youtube.com/watch?v=nwDxqbrdZoY)

## How to build

### Prerequisites
- Visual Studio: This project requires Visual Studio to be installed with the Desktop development with C++ workload. Download Visual Studio [here](https://visualstudio.microsoft.com/).
### Steps to build

- #### Clone the Repository:
   ```bash
   git clone https://github.com/Docas95/Raycaster-OpenGL
  ```

- #### Open the project in Visual Studio
  Open the .sln file using Visual Studio.

- #### Configure project settings
  Got to Projects -> Properties -> VC++ Directories and add the 'libs' and 'include' project file paths to 'Library Directories' and 'Include Directories', respectively.

  Got to Projects -> Properties -> Linker -> Input and add 'glfw3.lib' to 'Additional Dependencies'.

- #### Build and Run the project
  You should be able to build the project in Visual Studio using 'Ctrl + Shift + B' and run it using 'F5'.
  
