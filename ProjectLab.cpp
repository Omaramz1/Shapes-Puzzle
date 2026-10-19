//===========================================================================
// game name: shapes pazzle 
// the pople in this project:-
// 1- Firas Abdulaziz Ahmed - 446003675 
// 2- Mohammed Abdullah Alshamrani - 446003660
// 3- Omar Abdullatif Alzahrani - 445006514 
// 
// Description:-
// the game is about build something with shapes
// the game have three (3) levels 
// 1- build a house 
// 2- build a car 
// 3- build a ship
// 
// Controls:-
// use the arrows to move the shape ( left arrow to left right to right... ets) (( Translate ))
// use (X and x) for Scaling horizontal                                         (( Scaling ))
// use (Y and y) for Scaling vertical                                           (( Scaling ))
// use (R and r) for Rotation right and left                                    (( Rotation ))
// use (G) to Resat the game 
// use (N) to go to the next level 
// use (t) to change the shape type 
// press (Tab) on the keyborad so can change between shapes
// 
//==========================================================================

#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages 
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#include <cmath>
#include <string>

#endif


// Global Variables
GLint win_width = 500,
win_hight = 500;

std::string goal = "house";
bool win = false;
int level = 1;

bool visible[4] = { true, true, false, false };

struct Shape {
    float x, y;
    float angle;
    float scaleX, scaleY;
    int type;
}; // 0 triangle, 1 square, 2 circle, 3 trapezoid


Shape shapes[4] = {
    { -3, 0, 0, 1, 1, 0 },  // triangle
    {  3, 0, 0, 1, 1, 1 },  // square
    {  0, 3, 0, 1, 1, 2 },  // circle
    {  0,-3, 0, 1, 1, 3 } }; // trapezoid

int current = 0;
GLdouble PI = 3.14159265359;

// the shapes 

// draw a Circle
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount)));
    }

    glEnd();
}

// draw a Square
void drawSquare() {
    glBegin(GL_QUADS);

    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(-0.5f, -0.5f);

    glEnd();
}

// draw a trapezoid
void drawTrapezoid() {
    glBegin(GL_QUADS);

    glVertex2f(-0.7f, -0.5f);
    glVertex2f(0.7f, -0.5f);
    glVertex2f(0.4f, 0.5f);
    glVertex2f(-0.4f, 0.5f);

    glEnd();
}

// triangle
void drawTriangle() {
    glBegin(GL_TRIANGLES);

    glVertex2f(0.0f, 0.5f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);

    glEnd();
}

// to draw a taxt on the screen 
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}


// code for strong the snap 
void snap(float& a, float target) {
    if (fabs(a - target) < 1.0f)
        a = target;
}

// code for snap so the shape connect Automatically
void applySmartSnap() {

    // triangle above the square
    if (level == 1) {
        for (int i = 0; i < 4; i++) {
            if (shapes[i].type == 0) {             // triangle
                for (int j = 0; j < 4; j++) {
                    if (shapes[j].type == 1) {     // square
                        snap(shapes[i].x, shapes[j].x);
                        snap(shapes[i].y, shapes[j].y + 1.0f);
                    }
                }
            }
        }
    }

    else if (level == 2) {
        // wheels under square 
        for (int i = 0; i < 4; i++) {
            if (shapes[i].type == 2) {
                for (int j = 0; j < 4; j++) {
                    if (shapes[j].type == 1) {
                        snap(shapes[i].y, shapes[j].y - 1.2f);
                    }
                }
            }
        }

    }

    // square above trapezoid and two circles inside it 
    else if (level == 3) {
        int trapezoid = -1;

        for (int i = 0; i < 4; i++)
            if (shapes[i].type == 3)
                trapezoid = i;

        if (trapezoid == -1) return;

        int count = 0;

        for (int i = 0; i < 4; i++) {

            if (shapes[i].type == 2) { // circle


                snap(shapes[i].y, shapes[trapezoid].y - 0.2f);

                if (count == 0)
                    snap(shapes[i].x, shapes[trapezoid].x - 0.6f); // circle in the left
                else
                    snap(shapes[i].x, shapes[trapezoid].x + 0.6f); // circle in the right

                count++;
            }

            if (shapes[i].type == 1) {
                snap(shapes[i].x, shapes[trapezoid].x);
                snap(shapes[i].y, shapes[trapezoid].y + 1.0f);
            }
        }
    }
}

// win Conditions 
bool checkWin() {
    // to win the level 1 the tringle should be above the square  
    if (level == 1) {
        int square = -1, triangle = -1;

        for (int i = 0; i < 4; i++) {
            if (!visible[i]) continue;

            if (shapes[i].type == 1) square = i;
            if (shapes[i].type == 0) triangle = i;
        }

        if (square == -1 || triangle == -1)
            return false;

        float dx = fabs(shapes[square].x - shapes[triangle].x);
        float dy = shapes[triangle].y - shapes[square].y;

        return (dx < 1.2f && dy > 0.7f && dy < 3.0f);
    }

    // to win the level 2 two circul should be under the square and trapezoid above it
    else if (level == 2) {
        int square = -1;
        int wheel1 = -1;
        int wheel2 = -1;
        int trapezoid = -1;

        for (int i = 0; i < 4; i++) {
            if (!visible[i]) continue;

            if (shapes[i].type == 1) square = i;

            if (shapes[i].type == 2) {
                if (wheel1 == -1) wheel1 = i;
                else wheel2 = i;
            }

            if (shapes[i].type == 3) trapezoid = i;
        }

        if (square == -1 || wheel1 == -1 || wheel2 == -1 || trapezoid == -1)
            return false;

        float dy1 = shapes[square].y - shapes[wheel1].y;
        float dy2 = shapes[square].y - shapes[wheel2].y;
        bool wheelsUnder = (dy1 > 0 && dy2 > 0);

        float dx1 = fabs(shapes[square].x - shapes[wheel1].x);
        float dx2 = fabs(shapes[square].x - shapes[wheel2].x);
        bool closeX = (dx1 < 2.0f && dx2 < 2.0f);

        float dyTop = shapes[trapezoid].y - shapes[square].y;
        float dxTop = fabs(shapes[trapezoid].x - shapes[square].x);
        bool roofCorrect = (dyTop > 0.5f && dxTop < 1.5f);

        return wheelsUnder && closeX && roofCorrect;
    }

    // to win level 3 tow circuls should be inside the trapezoid and square above it 
    else if (level == 3) {

        int trapezoid = -1;
        int c1 = -1, c2 = -1;

        for (int i = 0; i < 4; i++) {
            if (!visible[i]) continue;

            if (shapes[i].type == 3) trapezoid = i;

            if (shapes[i].type == 2) {
                if (c1 == -1) c1 = i;
                else c2 = i;
            }
        }

        if (trapezoid == -1 || c1 == -1 || c2 == -1)
            return false;

        // check if the circuls inside the trapezoid 
        float dx1 = fabs(shapes[c1].x - shapes[trapezoid].x);
        float dx2 = fabs(shapes[c2].x - shapes[trapezoid].x);

        float dy1 = fabs(shapes[c1].y - shapes[trapezoid].y);
        float dy2 = fabs(shapes[c2].y - shapes[trapezoid].y);

        bool insideX = (dx1 < 1.5f && dx2 < 1.5f);
        bool insideY = (dy1 < 1.0f && dy2 < 1.0f);

        bool separated = fabs(shapes[c1].x - shapes[c2].x) > 1.0f;

        int square = -1;
        for (int i = 0; i < 4; i++)
            if (shapes[i].type == 1)
                square = i;

        if (square == -1) return false;

        float dy = shapes[square].y - shapes[trapezoid].y;
        float dx = fabs(shapes[square].x - shapes[trapezoid].x);

        bool squareAbove = (dy > 0.5f && dx < 1.5f);

        return insideX && insideY && separated && squareAbove;
    }

    return false;
}

// this code is for reset the shapes to their defult position
void resetShapes() {
    shapes[0] = { -3, 0, 0, 1, 1, 0 };
    shapes[1] = { 3, 0, 0, 1, 1, 1 };
    shapes[2] = { 0, 3, 0, 1, 1, 2 };
    shapes[3] = { 0,-3, 0, 1, 1, 3 };
}

// the main object
void MyMainObj() {

    for (int i = 0; i < 4; i++) {
        if (!visible[i]) continue;

        if (shapes[i].type == 2) continue;

        glPushMatrix();
        glTranslatef(shapes[i].x, shapes[i].y, 0);        // Translate
        glRotatef(shapes[i].angle, 0, 0, 1);              // Rotation
        glScalef(shapes[i].scaleX, shapes[i].scaleY, 1);  // Scaling

        // when you take control over a shape the color of the shape change to Green so you know which one you on
        if (i == current) glColor3f(0, 1, 0);

        else {
            if (shapes[i].type == 0) glColor3f(1, 0.5, 0);
            else if (shapes[i].type == 1) glColor3f(0.2, 0.6, 1);
            else if (shapes[i].type == 3) glColor3f(1, 0, 0);
        }

        if (shapes[i].type == 0) drawTriangle();
        else if (shapes[i].type == 1) drawSquare();
        else if (shapes[i].type == 3) drawTrapezoid();

        glPopMatrix();
    }

    //draw the circuls
    for (int i = 0; i < 4; i++) {
        if (!visible[i]) continue;
        if (shapes[i].type != 2) continue;

        glPushMatrix();
        glTranslatef(shapes[i].x, shapes[i].y, 0);
        glScalef(shapes[i].scaleX, shapes[i].scaleY, 1);

        if (i == current) glColor3f(0, 1, 0);
        else glColor3f(0.7, 0.3, 1);

        drawFilledCircle(0, 0, 0.5);

        glPopMatrix();
    }
}

// the dispaly 
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);

    glColor3f(0.2f, 0.3f, 0.6f);

    glVertex2f(-10, 10);
    glVertex2f(10, 10);
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex2f(10, -10);
    glVertex2f(-10, -10);

    glEnd();
    MyMainObj();
    glColor3f(1, 1, 1);

    // taxts
    if (level == 1)
        drawText("LEVEL 1: Build a HOUSE (square + triangle above it)", -9, 8);

    if (level == 2)
        drawText("LEVEL 2: Build a CAR (square + 2 circles under it + and Trapezoid above it )", -9, 8);

    if (level == 3)
        drawText("LEVEL 3: Build a SHIP (trapezoid + square above it + 2 circuls insaid it)", -9, 8);

    if (checkWin()) {
        win = true;
    }

    if (win) {
        drawText("YOU BUILT THE THING! PRESS G TO RESET OR PRESS N TO GO TO NEXT LEVEL", -7, 0);
    }

    glutSwapBuffers();
}


void reshapeFun(GLint newWidth, GLint newHight) {
    glViewport(0, 0, newWidth, newHight);
    win_width = newWidth;
    win_hight = newHight;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case 't': // change shape type
        shapes[current].type++;
        if (shapes[current].type > 3)
            shapes[current].type = 0;
        break;

    case 'R': // Rotation right
        shapes[current].angle += 10;
        break;

    case 'r': // Rotation left
        shapes[current].angle -= 10;
        break;

    case 'X': // Scaling horizontal make it big
        shapes[current].scaleX += 0.1f;
        break;

    case 'x': // Scaling horizontal make it small
        shapes[current].scaleX -= 0.1f;
        break;

    case 'Y': // Scaling vertical make it big
        shapes[current].scaleY += 0.1f;
        break;

    case 'y': // Scaling vertical make it small
        shapes[current].scaleY -= 0.1f;
        break;

    case '\t': // press (Tab) on the keyborad so can change between shapes
        current++;
        if (current > 3)
            current = 0;
        break;

    case 'G': // to resat the game
        if (win) {
            win = false;

            shapes[0] = { -3, 0, 0, 1, 1, 0 };
            shapes[1] = { 3, 0, 0, 1, 1, 1 };
            shapes[2] = { 0, 3, 0, 1, 1, 2 };
            shapes[3] = { 0,-3, 0, 1, 1, 3 };
        }

        break;

    case 'N': // to go to the next level
        if (win) {

            level++;

            if (level > 3) level = 1;

            resetShapes();

            // the shapes shows every level
            if (level == 1) {
                visible[0] = true;
                visible[1] = true;
                visible[2] = false;
                visible[3] = false;
            }

            if (level == 2) {
                visible[0] = true;
                visible[1] = true;
                visible[2] = true;
                visible[3] = true;
            }

            if (level == 3) {
                visible[0] = true;
                visible[1] = true;
                visible[2] = true;
                visible[3] = true;
            }

            win = false;
        }

        break;
    }

    glutPostRedisplay(); //Update the screen
}

void special(int key, int x, int y) // use the arrows to move 
{
    switch (key) {

    case GLUT_KEY_LEFT:
        shapes[current].x -= 0.2f;
        if (shapes[current].x < -10) shapes[current].x = -10;
        applySmartSnap();
        break;

    case GLUT_KEY_RIGHT:
        shapes[current].x += 0.2f;
        if (shapes[current].x > 10) shapes[current].x = 10;
        applySmartSnap();
        break;

    case GLUT_KEY_UP:
        shapes[current].y += 0.2f;
        if (shapes[current].y > 10) shapes[current].y = 10;
        applySmartSnap();
        break;

    case GLUT_KEY_DOWN:
        shapes[current].y -= 0.2f;
        if (shapes[current].y < -10) shapes[current].y = -10;
        applySmartSnap();
        break;
    }

    glutPostRedisplay(); // Update the Screen
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-10, 10, -10, 10);

    srand(time(0));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);   //  Specify the display Mode – RGB, RGBA or color //  Index, single or double Buffer
    glutInitWindowSize(win_width, win_hight);       // Set the window's initial width & height
    glutInitWindowPosition(900, 700);                 // Position the window's initial top-left corner
    glutCreateWindow("CG - Group Project (1) - Shapes pazzle ");    // My Group number 
    init();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);                        // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeFun);
    glutSpecialFunc(special);                        // to connect the arrows 
    glutKeyboardFunc(keyboard);                      // to connect the KeyBored 
    glutMainLoop();                                  // Enter the infinitely event-processing loop
    return 0;
}