
#include <stdio.h>
#include <cmath>
#include <GL/glut.h>

#define PI 3.14159

// Globals
float cameraX = -1.0, cameraY = 0.5, cameraZ = 1500;
float cameraRoll = 0.0, cameraPitch = -43.0;
int lastMouseX = 0, lastMouseY = 0;


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


void base(GLdouble x_1, GLdouble y_1, GLdouble z_1,
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

void platform(GLdouble x_1, GLdouble y_1, GLdouble z_1, 
              GLdouble x_2, GLdouble y_2, GLdouble z_2, 
              GLdouble x_3, GLdouble y_3, GLdouble z_3, 
              GLdouble x_4, GLdouble y_4, GLdouble z_4, 
              GLdouble x_5, GLdouble y_5, GLdouble z_5, 
              GLdouble x_6, GLdouble y_6, GLdouble z_6) {
    glBegin(GL_TRIANGLES);   

    glVertex3f(x_1, y_1, z_1);
    glVertex3f(x_2, y_2, z_2);
    glVertex3f(x_3, y_3, z_3);

    glVertex3f(x_3, y_3, z_3);
    glVertex3f(x_4, y_4, z_4);
    glVertex3f(x_5, y_5, z_5);

    glVertex3f(x_5, y_5, z_5);
    glVertex3f(x_6, y_6, z_6);
    glVertex3f(x_1, y_1, z_1);

    glVertex3f(x_1, y_1, z_1);
    glVertex3f(x_3, y_3, z_3);
    glVertex3f(x_5, y_5, z_5);

    glEnd();
}

// WiP, Almost but not quite..
void extendable_link(GLdouble width, GLdouble height, GLdouble bottom_x, GLdouble bottom_y, GLdouble top_x, GLdouble top_y, GLdouble top_z) {
    // a^2 = b^2+c^2
    float H_1 = sqrt(pow(bottom_x - top_x, 2) + pow(bottom_y - top_y, 2));
    float H_2 = sqrt(pow(H_1, 2) + pow(500, 2)); // actual length of link
    float tan_theta = 500 / H_1;
    float theta = atan(tan_theta);
    //theta = theta - (0.5 * PI);
    if (top_x != bottom_x) {
        theta = theta * -1;
    }
    theta = theta * (180 / PI);
    if (theta != 0) {
        //theta = abs(theta - 90);
    }

    float tan_alpha = abs(top_x - bottom_x) / abs(top_y - bottom_y);
    float alpha = atan(tan_alpha);
    if (top_x < bottom_x) {
        alpha = -1 * alpha;
    }
    alpha = alpha * (180 / PI);
    if (alpha != 0) {
        alpha = abs(alpha - 90);
    }

    //printf("top_x: %f top_y: %f bottom_x: %f bottom_y: %f\n", top_x, top_y, bottom_x, bottom_y);
    //printf("alpha: %f\n", alpha);
    //printf("theta: %f\n", theta);

    glPushMatrix();
    glTranslatef(bottom_x, bottom_y, 0.0);
    glRotatef((GLfloat)theta, 1.0, 0.0, 0.0);
    //glRotatef((GLfloat)alpha, 1.0, 0.0, 0.0);

    glScalef(width, height, H_2);
    glutWireCube(1.0);
    glPopMatrix();
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity(); //remove transforms from previous display() call
    glTranslatef(-cameraX, -cameraY, -cameraZ); //move objects negative = move camera positive
    glRotatef((GLfloat)cameraRoll, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)cameraPitch, 1.0, 0.0, 0.0);
    
    //printf("\nLink 1\n");
    float bottom_link_1[3] = { -530.0, 233.0, 0 };
    float top_link_1[3] = { -530.0, -233.0, 500 };
    link(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], top_link_1[0], top_link_1[1], top_link_1[2]);
    link_end(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], 10, 5, 5);
    link_end(top_link_1[0], top_link_1[1], top_link_1[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_1[0], bottom_link_1[1], top_link_1[0], top_link_1[1], top_link_1[2]);

    //printf("\nLink 2\n");
    float bottom_link_2[3] = { -466.0, 341, 0 };
    float top_link_2[3] = { -62.0, 575.0, 500 };
    link(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], top_link_2[0], top_link_2[1], top_link_2[2]);
    link_end(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], 10, 5, 5);
    link_end(top_link_2[0], top_link_2[1], top_link_2[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_2[0], bottom_link_2[1], top_link_2[0], top_link_2[1], top_link_2[2]);

    //printf("\nLink 3\n");
    float bottom_link_3[3] = { 466.0f, 341.0f, 0.0f };
    float top_link_3[3] = { 62.0f, 575.0f, 500.0f };
    link(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], top_link_3[0], top_link_3[1], top_link_3[2]);
    link_end(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], 10, 5, 5);
    link_end(top_link_3[0], top_link_3[1], top_link_3[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_3[0], bottom_link_3[1], top_link_3[0], top_link_3[1], top_link_3[2]);

    //printf("\nLink 4\n");
    float bottom_link_4[3] = { 529.0f, 233.0f, 0.0f };
    float top_link_4[3] = { 529.0f, -233.0f, 500.0f };
    link(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], top_link_4[0], top_link_4[1], top_link_4[2]);
    link_end(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], 10, 5, 5);
    link_end(top_link_4[0], top_link_4[1], top_link_4[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_4[0], bottom_link_4[1], top_link_4[0], top_link_4[1], top_link_4[2]);

    //printf("\nLink 5\n");
    float bottom_link_5[3] = { 62.0f, -575.0f, 0.0f };
    float top_link_5[3] = { 466.0f, -341.0f, 500.0f };
    link(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], top_link_5[0], top_link_5[1], top_link_5[2]);
    link_end(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], 10, 5, 5);
    link_end(top_link_5[0], top_link_5[1], top_link_5[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_5[0], bottom_link_5[1], top_link_5[0], top_link_5[1], top_link_5[2]);

    //printf("\nLink 6\n");
    float bottom_link_6[3] = { -62.0f, -575.0f, 0.0f };
    float top_link_6[3] = { -466.0f, -341.0f, 500.0f };
    link(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], top_link_6[0], top_link_6[1], top_link_6[2]);
    link_end(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], 10, 5, 5);
    link_end(top_link_6[0], top_link_6[1], top_link_6[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_6[0], bottom_link_6[1], top_link_6[0], top_link_6[1], top_link_6[2]);

    base(-360, 625, 0,
         360, 625, 0,
         721, 0, 0,
         360, -625, 0,
         -360, -625, 0,
         -721, 0, 0);

    platform(top_link_1[0], top_link_1[1], top_link_1[2],
        top_link_2[0], top_link_2[1], top_link_2[2],
        top_link_3[0], top_link_3[1], top_link_3[2],
        top_link_4[0], top_link_4[1], top_link_4[2],
        top_link_5[0], top_link_5[1], top_link_5[2],
        top_link_6[0], top_link_6[1], top_link_6[2]);

    glFlush();
}


void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 4000.0);
}


int current_button;
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
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Steward Platform Visualizer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
}