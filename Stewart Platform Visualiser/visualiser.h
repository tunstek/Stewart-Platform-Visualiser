#pragma once

#include <GL/glut.h>
#include "utils.h"
#include "memMapping.h"


// CONFIG
#define DISPLAY_INVERSE_KINEMATICS // comment to hide the link lengths in GUI

// the following are the linkage coordinates {x, y, z}
#define BOTTOM_LINK_1 { -530.0, 233.0, 0 }
#define TOP_LINK_1 { -530.0, -233.0, 500 }

#define BOTTOM_LINK_2 { -466.0, 341, 0 }
#define TOP_LINK_2 { -62.0, 575.0, 500 }

#define BOTTOM_LINK_3 { 466.0f, 341.0f, 0.0f }
#define TOP_LINK_3 { 62.0f, 575.0f, 500.0f }

#define BOTTOM_LINK_4 { 529.0f, 233.0f, 0.0f }
#define TOP_LINK_4 { 529.0f, -233.0f, 500.0f }

#define BOTTOM_LINK_5 { 62.0f, -575.0f, 0.0f }
#define TOP_LINK_5 { 466.0f, -341.0f, 500.0f }

#define BOTTOM_LINK_6 { -62.0f, -575.0f, 0.0f }
#define TOP_LINK_6 { -466.0f, -341.0f, 500.0f }

#define MEM_MAP_FILEPATH "Local\\motion_ctrl_mid_vals"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLATFORM_COLOR 255, 255, 255
#define LINK_COLOR 255, 0, 255
#define GUI_BOX_COLOR 0, 255, 1


#define PI 3.14159

namespace visualiser {


    void link_end(GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLint slices, GLint stacks);

    void link(GLdouble x_1, GLdouble y_1, GLdouble z_1, GLdouble x_2, GLdouble y_2, GLdouble z_2);


    void draw_base(GLdouble x_1, GLdouble y_1, GLdouble z_1,
        GLdouble x_2, GLdouble y_2, GLdouble z_2,
        GLdouble x_3, GLdouble y_3, GLdouble z_3,
        GLdouble x_4, GLdouble y_4, GLdouble z_4,
        GLdouble x_5, GLdouble y_5, GLdouble z_5,
        GLdouble x_6, GLdouble y_6, GLdouble z_6);


    void draw_platform(GLfloat x_1, GLfloat y_1, GLfloat z_1,
        GLfloat x_2, GLfloat y_2, GLfloat z_2,
        GLfloat x_3, GLfloat y_3, GLfloat z_3,
        GLfloat x_4, GLfloat y_4, GLfloat z_4,
        GLfloat x_5, GLfloat y_5, GLfloat z_5,
        GLfloat x_6, GLfloat y_6, GLfloat z_6);

    void output(GLfloat x, GLfloat y, std::string text, ...);


    void display();


    void reshape(GLint w, GLint h);

    void drag(int x, int y);
    void mouse(int button, int state, int x, int y);

    void init();

    int main(int argc, char** argv);

}