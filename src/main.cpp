/*
 * Mrityunjay Mishra - mm94424
 * May 7, 2021
 * 
 * A driver for generating a 2D forest using L-systems.
 * D0L-systems have been used in this system with parameterization
 * under the turtle interpretation. Refer to the Parser class for
 * the implementation of the D0L-systems and the Turtle class for 
 * turtle functionality implementations.
 */

#include "turtle.hpp"
#include <iostream>
#include <unistd.h>
#include "lsystems_parser.hpp"

using namespace std;

Parser p; // For generating and parsing L-systems

// Generates GLFW error descriptions
void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

// Generates a Koch snowflake. For testing purposed only.
void testKoch(int count) {
    std::vector<GLfloat> rgba = {1.0f, 1.0f, 1.0f, 1.0f}; // white
    p.setConfigs(600, M_PI / 3, 0.0f, window_width / 4, window_height/1.5, 1, rgba);
    std::string axiom = "F--F--F";
    p.kochCurveStringGenerator(axiom, count);
}

/*
 * Renders grass on the "ground" (bottom of the window). 
 * GENERATION determines the length/structure of the grass.
 * SEASON determines the appropriate color for the grass 
 * to be rendered with.
 */
bool generateGround(int generation, int season) {
    std::vector<GLfloat> rgba = {0.184314f, 0.309804f, 0.184314f, 1.0f}; // dark green
    std::string axiom = "F";

    if(generation >= 6) {
        // Start implementing color by season once the grass is fully
        // grown in order to simplify the coloring. These are varations
        // of the green color.
        if(season == 0) rgba = {0.560784f, 0.737255f, 0.560784f, 1.0f};
        else if(season == 1) rgba = {0.0f, 1.0f, 0.498039f, 1.0f}; 
        else if(season == 2) rgba = {0.498039f, 1.0f, 0.0f, 1.0f}; 
        else rgba = {0.6f, 0.8f, 0.196078f, 1.0f}; 
    }

    for(int i = 0; i < 21; i++) {
        p.setConfigs(15, 10 * (M_PI / 180), M_PI / 2, 50 * i, -30, 0.5, rgba);
        p.grassGenerator(axiom, generation);
    }

    for(int i = 0; i < 21; i++) {
        p.setConfigs(12, 10 * (M_PI / 180), M_PI / 2, (50 * i) - 25, -30, 0.5, rgba);
        p.grassGenerator(axiom, generation);
    }

    return true;
}

/*
 * Renders shrubs. GENERATION determines the length the shrubs.
 * SEASON determines the appropriate color for the shrubs. 
 */
void generateShrubs(int generation, int season) {
    std::vector<GLfloat> rgba = {0.184314f, 0.309804f, 0.184314f, 1.0f}; // dark green
    std::string axiom = "F";

    if(generation >= 3) {
        // Start implementing color by season once the sbrubs are fully
        // grown in order to simplify the coloring. These colors are 
        // variations of the dark green color. 
        if(season == 0) rgba = {0.184314f, 0.529804f, 0.184314f, 1.0f};
        else if(season == 1) rgba = {0.184314f, 0.339804f, 0.184314f, 1.0f}; 
        else if(season == 2) rgba = {0.184314f, 0.309804f, 0.184314f, 1.0f}; 
        else rgba = {0.184314f, 0.429804f, 0.184314f, 1.0f}; 
    }

    p.setConfigs(5, 22.5 * (M_PI / 180), M_PI / 2, window_width / 3, -30, 1, rgba);
    p.shrubGenerator(axiom, generation);
    p.setConfigs(5, 22.5 * (M_PI / 180), M_PI / 2, (window_width / 3) * 2, -30, 1, rgba);
    p.shrubGenerator(axiom, generation);
    p.setConfigs(5, 22.5 * (M_PI / 180), M_PI / 2, 75, -30, 1, rgba);
    p.shrubGenerator(axiom, generation);
    p.setConfigs(5, 22.5 * (M_PI / 180), M_PI / 2, window_width - 75, -30, 1, rgba);
    p.shrubGenerator(axiom, generation);
}

/*
 * Renders the main cherryblossom tree. GENERATION determines its size/length.
 */
void generateTree(int generation) {
    std::vector<GLfloat> rgba = {0.36f, 0.16f, 0.14f, 1.0f}; // brown
    std::string axiom = "X";
    p.setConfigs(100, 30 * (M_PI / 180), M_PI / 2, window_width / 2, 0, 1, rgba);
    p.treeGenerator(axiom, 320, 25, generation);
}

void generateTree2(int generation) {
    std::vector<GLfloat> rgba = {0.36f, 0.16f, 0.14f, 1.0f}; // brown
    std::string axiom = "X";
    p.setConfigs(100, 30 * (M_PI / 180), M_PI / 2, window_width / 3, 0, 1, rgba);
    p.treeGenerator(axiom, 320, 25, generation);
}

/*
 * Renders the leaves of the tree.
 * SEASON determines the appropriate color and type of leaves. 
 */
void generateLeaves(int season) {
    if(season == 0) return; // Do not generate leaves in winter

    srand(time(NULL));
    std::vector<GLfloat> rgba = {1.0f, 0.15f, 0.0f, 1.0f}; // orange
    std::string axiom =  (season == 2 || season == 3) ? "F+XF+F+XF" : "F++F++F++F++F";
    if(season == 1) {
        rgba = {1.0f, 0.72f, 0.77f, 1.0f}; // cherryblossom pink
    } else if (season == 2) {
        rgba = {0.184314f, 0.309804f, 0.184314f, 1.0f}; // dark green
    }

    // Rendering the leaves onto the tree. The tree was split
    // into 4 "regions" in order to render the leaves appropriately

    // Region 1
    for(int i = 0; i < 41; i++) {
        int x = rand() % 159 + 174;
        int y = rand() % 379 + 154;
        if(season == 1) {
            p.setConfigs(10, 36 * (M_PI / 180), M_PI / 2, x, y, 3, rgba);
            p.pentaplexityFlowerGenerator(axiom, 1);
        } else {
            p.setConfigs(1.75, M_PI / 2, M_PI / 2, x, y, 1.75, rgba);
            p.squareSierpinskiLeafGenerator(axiom, 3);
        }
    }

    // Region 2
    for(int i = 0; i < 31; i++) {
        int x = rand() % 175 + 300;
        int y = rand() % 200 + 500;
        if(season == 1) {
            p.setConfigs(10, 36 * (M_PI / 180), M_PI / 2, x, y, 3, rgba);
            p.pentaplexityFlowerGenerator(axiom, 1);
        } else {
            p.setConfigs(1.75, M_PI / 2, M_PI / 2, x, y, 1.75, rgba);
            p.squareSierpinskiLeafGenerator(axiom, 3);
        }
    }

    // Region 3
    for(int i = 0; i < 31; i++) {
        int x = rand() % 200 + 525;
        int y = rand() % 175 + 450;
        if(season == 1) {
            p.setConfigs(10, 36 * (M_PI / 180), M_PI / 2, x, y, 3, rgba);
            p.pentaplexityFlowerGenerator(axiom, 1);
        } else {
            p.setConfigs(1.75, M_PI / 2, M_PI / 2, x, y, 1.75, rgba);
            p.squareSierpinskiLeafGenerator(axiom, 3);
        }
    }

    // Region 4
    for(int i = 0; i < 31; i++) {
        int x = rand() % 180 + 700;
        int y = rand() % 250 + 250;
        if(season == 1) {
            p.setConfigs(10, 36 * (M_PI / 180), M_PI / 2, x, y, 3, rgba);
            p.pentaplexityFlowerGenerator(axiom, 1);
        } else {
            p.setConfigs(1.75, M_PI / 2, M_PI / 2, x, y, 1.75, rgba);
            p.squareSierpinskiLeafGenerator(axiom, 3);
        }
    }
}

int main(int argc, char** argv) {
    std::string window_title = "Final Project: L-Systems Generator";
	if (!glfwInit()) exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);

	// Ask an OpenGL 4.1 core profile context
	// It is required on OSX and non-NVIDIA Linux
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
			&window_title[0], nullptr, nullptr);
	
    if(!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;

	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);

    // int count = 0; //-- testing purposed only

    /*
     * SEASON determines the color of the objects rendered
     * 0 - winter
     * 1 - spring
     * 2 - summer
     * 3 - autumn/fall
     */
    int season = 0; 

    int grassGeneration = 0;
    int shrubGeneration = 0;
    int treeGeneration = 0;

    while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        season++;
        if(season > 3) season = 0;
        if(season == 0) {
            glClearColor(0.85f, 0.85f, 0.85f, 1.0f); // graying-white
        } else if(season == 1) {
            glClearColor(0.196078, 0.6, 0.9, 1.0f); // light blue
        } else if(season == 2) {
            glClearColor(0.196078, 0.6, 0.8, 1.0f); //blue
        } else {
            glClearColor(0.89, 0.47, 0.20, 1.0f); // mandarin ornage
        }

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, 0, window_height, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // FOR BASIC TESTING OF THE L-SYSTEM PARSER
        // testKoch(count);
        // count++;
        // if(count > 4) { count = 0; }

        // RENDERING THE FOREST SCENE
        if(grassGeneration < 6) grassGeneration++;
        generateGround(grassGeneration, season);

        if(grassGeneration >= 6) {
            generateShrubs(3, season);

            if(treeGeneration < 30) {
                treeGeneration += 7;
            }
            if(treeGeneration > 21) {
                generateLeaves(season);
            }
            generateTree(treeGeneration);
        }

        glfwSwapBuffers(window);
        sleep(1);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}