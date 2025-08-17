# ColourSpaceVisualiser
A simple demo to show different colour spaces for picking colours, including RGB, HSV, and OkLAB.
The project uses ImGui to display a UI consisting of radio buttons to select a colour space and sliders to edit the colour space values. The background buffer is then cleared (set to) using the colour defined in the sliders. 
This was produced in a very short time to be used like a colour picker in a future project.

Rationale

### Running the Project
  - Dependencies to setup: ImGui, GLFW, OpenGL
  - Edit the run.sh as necessary. (Ensure you can compile C++ and run bash scripts)
  - Create a build folder (and Dependencies folder with all your setup dependencies or other name depending on your run.sh file).
  - Execute the compiled program from the base directory "./build/main.exe"

OpenGL was used because, through my experience using it for my 3D Renderer, I could very quickly produce this product. No new technologies needed to be learned, and the boilerplate code could be taken directly from that project.

Sources
OkLab source: https://bottosson.github.io/posts/oklab/ 
