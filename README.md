Mrityunjay Mishra
mm94424
May 7, 2021

# Overview

This is my final project for CS354. For my final project, I have implemented an
L-systems parser. This parser has the ability to generate and interpret/parse
D0L-systems under the turtle interpretation. To test my parser, I created a driver
file that renders a 2D forest scene with seasons. Please note that this project has
only been tested in Ubuntu 18.04. However, it should still work under other
operating systems and architectures. 

# Build Instructions 
_Assuming one is in the root of this project_

```
    mkdir build
    cd build
    cmake ..
    make -j8
```

# Run Instructions
_Assuming one is in the root of this project_

```
    cd build
    ./main
```

# IMPORTANT NOTE

Please note that when initializing GLFW, the following lines are not present
in this project (however, they are commented out in main.cpp starting from 
line 191).

```c++
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

I commented these lines out because they were causing rendering issues in my system
(Ubuntu 18.04). However, if you have any build/rendering issues due to these lines, 
please feel free to comment them out in main.cpp (however, once again, this did not
work in my system, so the behavior of this program afteer commenting these lines out 
is not known/guaranteed). I aplogize for any inconvenience. For this exact reason, I 
have included a video demonstration along with my final submission. For any other 
issues, please feel free to reach out to me. 

# Key Functionalities

Please note that the following functionalities are for a D0L-System. Support for 
pL-Systems have not been added.

* Set of 12 alphabets (more an be added with relative ease)
    * 'F' : move forward (while drawing a line)
    * '-' : turn right
    * '+' : turn left
    * '[' : push / save configuration of the turtle
    * ']' : pop / restore configuration of the turtle
    * 'X' : placeholder alphabet
    * '(' : begin paramter for setting thickness
    * ')' : end parameter for setting thickness
    * '{' : begin parameter for setting length
    * '}' : end parameter for setting length
    * '$' : begin and end parameters for setting rotation (in radians)
    * '|' : rotate turtle by PI / 2
* Support for parameterization (through parameter alphabets - this implies that parameters can be written directly intro the generated sentence, as is the case in this project)
* Context-free grammar
* Turtle interpretation of sentences
    * Note that current turtle interpretation is limited to 2D space. However, this can be extended to 3D space with relative ease. 
* Line drawing with turtle interpretation using Bresenham's algorithm

For more information regarding this project, please refer to Final_Report.pdf.
