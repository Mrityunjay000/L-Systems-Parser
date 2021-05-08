/*
 * Mrityunjay Mishra - mm94424
 * May 7, 2021
 * 
 * For generating and parsing D0L-systems with support for 
 * parameterization.
 */

#ifndef LSYSTEMS_PARSER_HPP
#define LSYSTEMS_PARSER_HPP

#include <iostream>
#include "turtle.hpp"
#include <stack>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>

// Used to store the current configuration of the turtle onto the stack
struct Config {
    int length;         //the amount to move forward by
    float turnAngle;    //the amount to turn by
    float currentAngle; //the current rotation angle of the turtle
    int x, y;           //the current position (x,y) of the turtle
    int thickness;      //thickness of the line drawn by the turtle

    Config() {}

    Config(int len, float tAngle, float cAngle, int x_, int y_, int thick) {
        length = len;
        turnAngle = tAngle; 
        currentAngle = cAngle; 
        x = x_; 
        y = y_; 
        thickness = thick;
    }
};

/*
 * This class can be used to both parse and generate D0L-systems. This 
 * class also supports parameterization of the said L-systems. Note that
 * this class uses the turtle interpretation of the alphabets in the 
 * said L-system. Refer to the Turtle class for more information regarding
 * specific turtle functionalities.
 */
class Parser{
    private:
        // Defining the set of alphabets for a D0L-System
        char forward;            //F
        char turnRight;          //-
        char turnLeft;           //+
        char saveConfig;         //[
        char restorePrevConfig;  //]
        char placeHolder;        //X
        char setThicknessBegin;  //(
        char setThicknessEnd;    //)
        char setLengthBegin;     //{
        char setLengthEnd;       //}
        char setRotation;        //$
        char turnBackwards;      //|

        //Other necessary components for the L-System
        std::stack<Config> states;

        Turtle turtle;

    public:
        Parser() : turtle() {
            forward = 'F';
            turnRight = '-';
            turnLeft = '+';
            saveConfig = '[';
            restorePrevConfig = ']';
            placeHolder = 'X';
            setThicknessBegin = '(';
            setThicknessEnd = ')';
            setLengthBegin = '{';
            setLengthEnd = '}';
            setRotation = '$';
            turnBackwards = '|';

            srand(time(NULL));
        }

        // For setting the configurations of the turtle. 
        // Should be called before parse()
        void setConfigs(int len, float tAngle, float cAngle, 
                        int x_, int y_, int thick, std::vector<GLfloat> rgba) {
            turtle.setLength(len);
            turtle.setRotationAngle(tAngle);
            turtle.setCurrentRotation(cAngle);
            turtle.goTo(x_, y_);
            turtle.setThickness(thick);
            turtle.setColor(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
        }

        // Parses a given D0L-system with support for parameterization.
        // Calls on relevant turtle functions based on interpretation of the alphabet
        void parse(std::string sentence) {
            for(int i = 0; i < sentence.length(); i++) {
                char alphabet = sentence[i];

                if(alphabet == forward) {
                    turtle.move();
                } else if(alphabet == turnRight) {
                    turtle.turnRight();
                } else if(alphabet == turnLeft) {
                    turtle.turnLeft();
                } else if(alphabet == saveConfig) {

                    Config state(turtle.getLength(), turtle.getRotationAngle(),
                                 turtle.getCurrentRotation(), turtle.getPosX(),
                                 turtle.getPosY(), turtle.getThickness());

                    states.push(state);

                } else if(alphabet == restorePrevConfig) {

                    Config prev = states.top();

                    turtle.setLength(prev.length);
                    turtle.setRotationAngle(prev.turnAngle);
                    turtle.setCurrentRotation(prev.currentAngle);
                    turtle.goTo(prev.x, prev.y);
                    turtle.setThickness(prev.thickness);

                    states.pop();

                } else if(alphabet == placeHolder) {
                    continue;
                } else if(alphabet == setLengthBegin) {

                    i++;
                    int begin = i;
                    while(sentence[i] != setLengthEnd) i++;
                    std::string number = sentence.substr(begin, i - begin);
                    std::stringstream s(number);
                    int len;
                    s >> len;
                    turtle.setLength(len);

                } else if(alphabet == setThicknessBegin) {

                    i++;
                    int begin = i;
                    while(sentence[i] != setThicknessEnd) i++;
                    std::string number = sentence.substr(begin, i - begin);
                    std::stringstream s(number);
                    int thickness;
                    s >> thickness;
                    turtle.setThickness(thickness);

                } else if(alphabet == setRotation) {

                    i++;
                    int begin = i; 
                    while(sentence[i] != setRotation) i++;
                    std::string number = sentence.substr(begin, i - begin);
                    float angle = std::stof(number);
                    turtle.setRotationAngle(angle);

                } else if(alphabet == turnBackwards) {
                    turtle.turnBackwards();
                } else {
                    std::cout << "INVALID ALPHABET: " << alphabet << std::endl;
                }
            }
        }

        // Generates the string string for a Koch snowflake for depth == n
        // with a scale of 1/3
        void kochCurveStringGenerator(std::string sentence, int n) {
            if(n <= 0) { 
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if(sentence[i] == forward) {
                    temp += "F+F--F+F";
                } else {
                    temp += sentence[i];
                }
            }

            turtle.setLength(turtle.getLength() / 3);
            kochCurveStringGenerator(temp, n-1);
        }

        /* Generates the string for a simple grass structure for depth == n
         * with a scale of 0.95.
         * Adapted from https://www.nodebox.net/code/index.php/Mark_Meyer_%7C_L-system
         */
        void grassGenerator(std::string sentence, int n) {
            if(n <= 0) {
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if(sentence[i] == forward) {
                    temp += "F[-X][+X]";
                } else if(sentence[i] == placeHolder) {
                    temp += "F[-X]F[+F]";
                } else {
                    temp += sentence[i];
                }
            }

            turtle.setLength(turtle.getLength() * 0.95);
            grassGenerator(temp, n-1);
        }

        /*
         * Generates the string for a bush structure, for depth == n
         * Adapted from Paul Bourke's L-system Bushes: http://paulbourke.net/fractals/lsys/
         */
        void shrubGenerator(std::string sentence, int n) {
            if(n <= 0) {
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if(sentence[i] == forward) {
                    temp += "F[+FF][-FF][+++FF][---FF]F[-FF][+FF][---F][+++F]F";
                } else {
                    temp += sentence[i];
                }
            }

            shrubGenerator(temp, n-1);
        }

        /* Generates the string for a tree structure for depth == n, 
         * width == w, and length == s. This is a parametric D0L-system
         * adapted from http://algorithmicbotany.org/papers/sigcourse.2003/2-1-lsystems.pdf
         * with input paramters of S and W, and other constant paramteres found below.
         * Specifically, this is an adaptation of the D0L-system provided in section 6.3. 
         */
        void treeGenerator(std::string sentence, float s, float w, int n) {
            float r1 = 0.58;
            float q = 0.4;
            float e = 0.5;
            float alpha1 = 25 * (M_PI / 180);
            float alpha2 = 60 * (M_PI / 180);
            float alpha3 = 75 * (M_PI / 180);

            if(n <= 0) {
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if((sentence[i] == placeHolder)) {
                    if(s >= 5) { // maintain a minimum length for each branchs
                        temp += "(" + std::to_string(w) + "){" + std::to_string(s) + "}F[$" + 
                                std::to_string(alpha1) + "$+X][$" + std::to_string(alpha2)  + 
                                "$-X][$" + std::to_string(alpha3) + "$+X]";
                    }
                } else {
                    temp += sentence[i];
                }
            }

            treeGenerator(temp, s * r1, w * pow(q, e), n-1); // calculate parameters for the next recursive call
        }

        /*
         * Generates the string for a cherryblossom flower using the pentaplexity curve.
         * From Paul Bourke's L-system Pentaplexity: http://paulbourke.net/fractals/lsys/
         */
        void pentaplexityFlowerGenerator(std::string sentence, int n) {
            if(n <= 0) {
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if(sentence[i] == forward) {
                    temp += "F++F++F|F-F++F";
                } else {
                    temp += sentence[i];
                }
            }

            pentaplexityFlowerGenerator(temp, n-1);
        }

        /*
         * Generates the string for a leaf using the sqaure sierpinski curve.
         * From Paul Bourke's L-system Sqaure Sierpienski: http://paulbourke.net/fractals/lsys/
         */
        void squareSierpinskiLeafGenerator(std::string sentence, int n) {
            if(n <= 0) {
                parse(sentence);
                return;
            }

            std::string temp;
            for(int i = 0; i < sentence.length(); i++) {
                if(sentence[i] == placeHolder) {
                    temp += "XF-F+F-XF+F+XF-F+F-X";
                } else {
                    temp += sentence[i];
                }
            }

            squareSierpinskiLeafGenerator(temp, n-1);
        }

};

#endif //LSYSTEMS_PARSER_HPP