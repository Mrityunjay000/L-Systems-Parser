/*
 * Mrityunjay Mishra - mm94424
 * May 7, 2021
 * 
 * Defines a turtle object that can move and draw lines
 * in 2D space. Line drawing has been implemented using
 * Bresenham's algorithm. 
 */

#ifndef TURTLE_HPP
#define TURTLE_HPP

#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int window_width = 1000, window_height = 800;

class Turtle {
    private:
        float length;                   //the distance the turtle can travel
        float rotationAngle;            //the rotationAngle of the turtle's rotation (clockwise) in radians
        float currentRotation;          //the current rotation rotationAngle of the turtle
        int thickness;                  //the thickness of the line that the turtle will draw
        std::vector<GLfloat> color;     //color of the line
        int x,y;                        //(x,y) coordinates of the turtle
    
    public:
        Turtle() {
            length = 100.0f;
            rotationAngle = M_PI / 4;
            currentRotation = 0.0f;
            thickness = 2;
            x = 0;
            y = 0;

            color.push_back(1.0f);
            color.push_back(1.0f);
            color.push_back(1.0f);
            color.push_back(1.0f);
        }

        void setLength(float length_) { length = length_; }
        int getLength() { return length; }
        void setRotationAngle(float rotationAngle_) { rotationAngle = rotationAngle_; }
        float getRotationAngle() { return rotationAngle; }
        void setCurrentRotation(float rotation) { currentRotation = rotation; }
        float getCurrentRotation() { return currentRotation; }
        void turnRight() { currentRotation -= rotationAngle; }
        void turnLeft() { currentRotation += rotationAngle; }
        void turnBackwards() { currentRotation -= M_PI; }
        void setThickness(int thickness_) { thickness = thickness_; }
        int getThickness() { return thickness; }
        int getPosX() { return x; }
        int getPosY() { return y; }
        void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
            color.clear();
            color.push_back(r);
            color.push_back(g);
            color.push_back(b);
            color.push_back(a);
        }

        /*
         * Places the turtle at (x_, y_), does not draw any lines
         */ 
        void goTo(int x_, int y_) {
            x = x_;
            y = y_;
        }

        /*
         * Moves the turtle by LENGTH in the direction it is facing
         * (based on ANGLE); a line in the turtle's direction of 
         * movement is also drawn
         */ 
        void move() {
            int x1 = x + (int)(length * cos(currentRotation));
            int y1 = y + (int)(length * sin(currentRotation));
            bresenhamLine(x, y, x1, y1);
            x = x1;
            y = y1;
        }

        /*
         * Draws a lines using Bresenham's algorithm starting 
         * from (x0, y0) to (x1, y1)
         * 
         * Algorithm from https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
         */ 
        void bresenhamLine(int x0, int y0, int x1, int y1) {
            if(abs(y1 - y0) < abs(x1 - x0)) {
                if(x0 > x1) {
                    bresenhamLineLow(x1, y1, x0, y0);
                } else {
                    bresenhamLineLow(x0, y0, x1, y1);
                }
            } else {
                if(y0 > y1) {
                    bresenhamLineHigh(x1, y1, x0, y0);
                } else {
                    bresenhamLineHigh(x0, y0, x1, y1);
                }
            }
        }

         /*
         * Helper function for bresenhamLine(). Refer to 
         * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
         * for more details.
         */
        void bresenhamLineLow(int x0, int y0, int x1, int y1) {
            int dx = x1 - x0;
            int dy = y1 - y0;
            int yi = 1;

            if (dy < 0) {
                yi = -1;
                dy = -dy;
            }

            int D = (2 * dy) - dx;
            int y = y0;

            for (int x = x0; x < x1; x++) {
                plotPoint(x, y);
                if(D > 0) {
                    y += yi;
                    D += (2 * (dy - dx));
                } else {
                    D += 2 * dy;
                }
            }
        }

        /*
         * Helper function for bresenhamLine(). Refer to 
         * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
         * for more details.
         */
        void bresenhamLineHigh(int x0, int y0, int x1, int y1) {
            int dx = x1 - x0;
            int dy = y1 - y0;
            int xi = 1;

            if (dx < 0) {
                xi = -1;
                dx = -dx;
            }

            int D = (2 * dx) - dy;
            int x = x0;

            for (int y = y0; y < y1; y++) {
                plotPoint(x, y);
                if(D > 0) {
                    x += xi;
                    D += (2 * (dx - dy));
                } else {
                    D += 2 * dx;
                }
            }
        }

        /*
         * Draws a point at (x,y) with a color as predefined
         * by the variable COLOR
         */
        void plotPoint(int x, int y) {
            GLfloat vert[] = {(GLfloat) x, (GLfloat) y};
            GLfloat rgba[] = {color.at(0), color.at(1), color.at(2), color.at(3)};
            
            glEnable(GL_POINT_SMOOTH);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, rgba);
            glPointSize(thickness);
            glVertexPointer(2, GL_FLOAT, 0, vert);
            glDrawArrays(GL_POINTS, 0, 1);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisable(GL_POINT_SMOOTH);
        }
};

#endif //TURTLE_H