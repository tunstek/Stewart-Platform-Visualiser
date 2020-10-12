
#include <stdio.h>
#include <cmath>
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


// Globals
float cameraX = -1.0, cameraY = 0.5, cameraZ = 1500;
float cameraRoll = 0.0, cameraPitch = -43.0;
int lastMouseX = 0, lastMouseY = 0;
int current_button;

const float bottom_link_1[3] = BOTTOM_LINK_1;
const float top_link_1[3] = TOP_LINK_1;
const float bottom_link_2[3] = BOTTOM_LINK_2;
const float top_link_2[3] = TOP_LINK_2;
const float bottom_link_3[3] = BOTTOM_LINK_3;
const float top_link_3[3] = TOP_LINK_3;
const float bottom_link_4[3] = BOTTOM_LINK_4;
const float top_link_4[3] = TOP_LINK_4;
const float bottom_link_5[3] = BOTTOM_LINK_5;
const float top_link_5[3] = TOP_LINK_5;
const float bottom_link_6[3] = BOTTOM_LINK_6;
const float top_link_6[3] = TOP_LINK_6;

float link_1_length;
float link_2_length;
float link_3_length;
float link_4_length;
float link_5_length;
float link_6_length;

// Calculated new platform (top link) vertices after transformations
GLfloat v_1_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_2_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_3_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_4_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_5_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_6_new[4] = { 0.0, 0.0, 0.0, 0.0 };

// memory mapping
struct SPageFileInputs {
    float roll; // in radians
    float pitch; // in radians
    float yaw; // in radians
    float surge; // x; // mm
    float sway; // y; // mm
    float heave; // z; // mm
};
memMapping<SPageFileInputs> inputs;
SPageFileInputs* inputsPtr;



void link_end(GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLint slices, GLint stacks) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutWireSphere(r, slices, stacks);
    glPopMatrix();
}
void link(GLdouble x_1, GLdouble y_1, GLdouble z_1, GLdouble x_2, GLdouble y_2, GLdouble z_2) {
    glBegin(GL_LINES);
    glVertex3f(x_1, y_1, z_1);
    glVertex3f(x_2, y_2, z_2);
    glEnd();
}


void draw_base(GLdouble x_1, GLdouble y_1, GLdouble z_1,
          GLdouble x_2, GLdouble y_2, GLdouble z_2,
          GLdouble x_3, GLdouble y_3, GLdouble z_3,
          GLdouble x_4, GLdouble y_4, GLdouble z_4,
          GLdouble x_5, GLdouble y_5, GLdouble z_5,
          GLdouble x_6, GLdouble y_6, GLdouble z_6) {
    glBegin(GL_LINES);
    glVertex3f(x_1, y_1, z_1);
    glVertex3f(x_2, y_2, z_2);

    glVertex3f(x_2, y_2, z_2);
    glVertex3f(x_3, y_3, z_3);

    glVertex3f(x_3, y_3, z_3);
    glVertex3f(x_4, y_4, z_4);

    glVertex3f(x_4, y_4, z_4);
    glVertex3f(x_5, y_5, z_5);

    glVertex3f(x_5, y_5, z_5);
    glVertex3f(x_6, y_6, z_6);

    glVertex3f(x_6, y_6, z_6);
    glVertex3f(x_1, y_1, z_1);
    glEnd();
}


void draw_platform(GLfloat x_1, GLfloat y_1, GLfloat z_1,
                   GLfloat x_2, GLfloat y_2, GLfloat z_2,
                   GLfloat x_3, GLfloat y_3, GLfloat z_3,
                   GLfloat x_4, GLfloat y_4, GLfloat z_4,
                   GLfloat x_5, GLfloat y_5, GLfloat z_5,
                   GLfloat x_6, GLfloat y_6, GLfloat z_6) {

    GLfloat v_1[4] = { x_1, y_1, z_1, 1.0 };
    GLfloat v_2[4] = { x_2, y_2, z_2, 1.0 };
    GLfloat v_3[4] = { x_3, y_3, z_3, 1.0 };
    GLfloat v_4[4] = { x_4, y_4, z_4, 1.0 };
    GLfloat v_5[4] = { x_5, y_5, z_5, 1.0 };
    GLfloat v_6[4] = { x_6, y_6, z_6, 1.0 };


    glPushMatrix();

    GLfloat camera_matrix[16]; 
    GLfloat matrix[16]; // translation and rotation for both object and camera
    GLfloat translation_rotation_matrix[16]; // this is the resultant translation and rotation matrix for this object

    // get camera matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);

    glTranslatef(inputsPtr->sway, inputsPtr->surge, inputsPtr->heave);
    glRotatef((GLfloat)inputsPtr->roll * (180/PI), 0.0, 1.0, 0.0);
    glRotatef((GLfloat)inputsPtr->pitch * (180 / PI), 1.0, 0.0, 0.0);
    glRotatef((GLfloat)inputsPtr->yaw * (180 / PI), 0.0, 0.0, 1.0);

    // get new matrix after transformations
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);


    // Get the transformation component via camera_matrix * inverse(matrix)
    get_object_translation_rotation_matrix(camera_matrix, matrix, translation_rotation_matrix);
    // translation_matrix is now the translation for everything else
    gluInvertMatrix(translation_rotation_matrix, translation_rotation_matrix); // invert to get the translation for the platform
    // opengl puts the translations on the last row, 'fix' by copying them to the last column instead
    translation_rotation_matrix[3] = -translation_rotation_matrix[12];
    translation_rotation_matrix[7] = -translation_rotation_matrix[13];
    translation_rotation_matrix[11] = -translation_rotation_matrix[14];

    // get the new vertex coordinates
    matrix_mult_16_4(translation_rotation_matrix, v_1, v_1_new);
    matrix_mult_16_4(translation_rotation_matrix, v_2, v_2_new);
    matrix_mult_16_4(translation_rotation_matrix, v_3, v_3_new);
    matrix_mult_16_4(translation_rotation_matrix, v_4, v_4_new);
    matrix_mult_16_4(translation_rotation_matrix, v_5, v_5_new);
    matrix_mult_16_4(translation_rotation_matrix, v_6, v_6_new);

    // inverse kinematics
    link_1_length = sqrt(pow(bottom_link_1[0] - v_1_new[0], 2) + pow(bottom_link_1[1] - v_1[1], 2) + pow(bottom_link_1[2] - v_1_new[2], 2));
    link_2_length = sqrt(pow(bottom_link_2[0] - v_2_new[0], 2) + pow(bottom_link_2[1] - v_2[1], 2) + pow(bottom_link_2[2] - v_2_new[2], 2));
    link_3_length = sqrt(pow(bottom_link_3[0] - v_3_new[0], 2) + pow(bottom_link_3[1] - v_3[1], 2) + pow(bottom_link_3[2] - v_3_new[2], 2));
    link_4_length = sqrt(pow(bottom_link_4[0] - v_4_new[0], 2) + pow(bottom_link_4[1] - v_4[1], 2) + pow(bottom_link_4[2] - v_4_new[2], 2));
    link_5_length = sqrt(pow(bottom_link_5[0] - v_5_new[0], 2) + pow(bottom_link_5[1] - v_5[1], 2) + pow(bottom_link_5[2] - v_5_new[2], 2));
    link_6_length = sqrt(pow(bottom_link_6[0] - v_6_new[0], 2) + pow(bottom_link_6[1] - v_6[1], 2) + pow(bottom_link_6[2] - v_6_new[2], 2));


    glBegin(GL_TRIANGLES);   
    // use the old vertex coordinates here and let opengl handle translation and rotation
    glVertex3f(v_1[0], v_1[1], v_1[2]);
    glVertex3f(v_2[0], v_2[1], v_2[2]);
    glVertex3f(v_3[0], v_3[1], v_3[2]);

    glVertex3f(v_3[0], v_3[1], v_3[2]);
    glVertex3f(v_4[0], v_4[1], v_4[2]);
    glVertex3f(v_5[0], v_5[1], v_5[2]);

    glVertex3f(v_5[0], v_5[1], v_5[2]);
    glVertex3f(v_6[0], v_6[1], v_6[2]);
    glVertex3f(v_1[0], v_1[1], v_1[2]);

    glVertex3f(v_1[0], v_1[1], v_1[2]);
    glVertex3f(v_3[0], v_3[1], v_3[2]);
    glVertex3f(v_5[0], v_5[1], v_5[2]);

    glEnd();

    
    glPopMatrix();
}


void output(GLfloat x, GLfloat y, std::string text, ...) {
    char* cstr = &text[0];

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef((GLfloat)180, 1.0, 0.0, 0.0);
    glScalef(0.1f, 0.1f, 0.1f);
    for (char* p = cstr; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //remove transforms from previous display() call

    glPushMatrix();
    glTranslatef(-cameraX, -cameraY, -cameraZ); //move objects negative = move camera positive
    glRotatef((GLfloat)cameraRoll, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)cameraPitch, 1.0, 0.0, 0.0);

    GLfloat camera_matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);
    //printf("Camera Matrix: \n");
    //print_matrix_16(camera_matrix);

    glColor3ub(PLATFORM_COLOR);
   
    draw_platform(top_link_1[0], top_link_1[1], top_link_1[2],
        top_link_2[0], top_link_2[1], top_link_2[2],
        top_link_3[0], top_link_3[1], top_link_3[2],
        top_link_4[0], top_link_4[1], top_link_4[2],
        top_link_5[0], top_link_5[1], top_link_5[2],
        top_link_6[0], top_link_6[1], top_link_6[2]);
    
    glColor3ub(LINK_COLOR);

    link(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], v_1_new[0], v_1_new[1], v_1_new[2]);
    link_end(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], 10, 5, 5);
    link_end(v_1_new[0], v_1_new[1], v_1_new[2], 10, 5, 5);

    link(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], v_2_new[0], v_2_new[1], v_2_new[2]);
    link_end(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], 10, 5, 5);
    link_end(v_2_new[0], v_2_new[1], v_2_new[2], 10, 5, 5);
    
    link(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], v_3_new[0], v_3_new[1], v_3_new[2]);
    link_end(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], 10, 5, 5);
    link_end(v_3_new[0], v_3_new[1], v_3_new[2], 10, 5, 5);
    
    link(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], v_4_new[0], v_4_new[1], v_4_new[2]);
    link_end(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], 10, 5, 5);
    link_end(v_4_new[0], v_4_new[1], v_4_new[2], 10, 5, 5);
    
    link(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], v_5_new[0], v_5_new[1], v_5_new[2]);
    link_end(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], 10, 5, 5);
    link_end(v_5_new[0], v_5_new[1], v_5_new[2], 10, 5, 5);
    
    link(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], v_6_new[0], v_6_new[1], v_6_new[2]);
    link_end(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], 10, 5, 5);
    link_end(v_6_new[0], v_6_new[1], v_6_new[2], 10, 5, 5);


    draw_base(-360, 625, 0,
         360, 625, 0,
         721, 0, 0,
         360, -625, 0,
         -360, -625, 0,
         -721, 0, 0);

    
    glPopMatrix();



    // Start 2d
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);

    glColor3f(GUI_BOX_COLOR);
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(100.0, 0.0);
    glVertex2f(100.0, 190.0);
    glVertex2f(0.0, 190.0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    output(10, 20,  std::string("Roll:  ") + float_to_str(inputsPtr->roll * (180/PI), 2));
    output(10, 50,  std::string("Pitch: ") + float_to_str(inputsPtr->pitch * (180 / PI), 2));
    output(10, 80,  std::string("Yaw:  ") + float_to_str(inputsPtr->yaw * (180 / PI), 2));
    output(10, 110, std::string("Surge: ") + float_to_str(inputsPtr->surge, 2));
    output(10, 140, std::string("Sway: ") + float_to_str(inputsPtr->sway, 2));
    output(10, 170, std::string("Heave: ") + float_to_str(inputsPtr->heave, 2));

#ifdef DISPLAY_INVERSE_KINEMATICS
    glColor3f(GUI_BOX_COLOR);
    glBegin(GL_QUADS);
    glVertex2f(SCREEN_WIDTH-120, 0.0);
    glVertex2f(SCREEN_WIDTH, 0.0);
    glVertex2f(SCREEN_WIDTH, 190.0);
    glVertex2f(SCREEN_WIDTH-120, 190.0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    output(SCREEN_WIDTH-110, 20, std::string("Link 1:  ") + float_to_str(link_1_length, 2));
    output(SCREEN_WIDTH-110, 50, std::string("Link 2: ") + float_to_str(link_2_length, 2));
    output(SCREEN_WIDTH-110, 80, std::string("Link 3:  ") + float_to_str(link_3_length, 2));
    output(SCREEN_WIDTH-110, 110, std::string("Link 4: ") + float_to_str(link_4_length, 2));
    output(SCREEN_WIDTH-110, 140, std::string("Link 5: ") + float_to_str(link_5_length, 2));
    output(SCREEN_WIDTH-110, 170, std::string("Link 6: ") + float_to_str(link_6_length, 2));
#endif // DISPLAY_INVERSE_KINEMATICS


    // Make sure we can render 3d again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();


    // *** TODO: Limit this call
    glutPostRedisplay(); 
}


void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 4000.0);
}


void drag(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;
    
    if (current_button == GLUT_LEFT_BUTTON) {
        const float speed = 0.1f;
        cameraX -= dx * speed; //or -=, depending on which direction feels more natural to you
        cameraY += dy * speed; //-= as mouse origin is top left, so +y is moving down
    }
    else if (current_button == GLUT_MIDDLE_BUTTON) {
        const float speed = 0.1f;
        cameraRoll += dx * speed;
        cameraPitch += dy * speed;
    }
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
    lastMouseX = x;
    lastMouseY = y;

    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) // It's a wheel event
    {
        // scroll
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        const int speed = 5;
        if (button == 3) {
            cameraZ -= speed;
        }
        else if (button == 4) {
            cameraZ += speed;
        }
        glutPostRedisplay();
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
        current_button = GLUT_MIDDLE_BUTTON;
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        current_button = GLUT_RIGHT_BUTTON;
    }
    else if (button == GLUT_LEFT_BUTTON) {
        current_button = GLUT_LEFT_BUTTON;
        if (state == GLUT_UP) {
            printf("Current camera pos: (%f, %f, %f)\n", cameraX, cameraY, cameraZ);
            printf("Current camera rotation: Pitch: %f Roll: %f\n", cameraPitch, cameraRoll);
        }
    }
    else {  // normal button event
        printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    }
}


void init() {
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutMouseFunc(mouse);
    glutMotionFunc(drag);

    // init the mmf pointers
    inputsPtr = inputs.init(MEM_MAP_FILEPATH);
    inputsPtr->roll = 0.3;
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Steward Platform Visualizer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
}