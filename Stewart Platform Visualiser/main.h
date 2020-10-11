
#include <stdio.h>
#include <cmath>
#include <GL/glut.h>

#define PI 3.14159

// TEST
float roll = 10.0, pitch = 0.0, yaw = 0.0; // degrees
float surge = 50.0, sway = 0.0, heave = 0.0; // mm

// Globals
float cameraX = -1.0, cameraY = 0.5, cameraZ = 1500;
float cameraRoll = 0.0, cameraPitch = -43.0;
int lastMouseX = 0, lastMouseY = 0;

// Calculated new platform vertices after transformations
GLfloat v_1_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_2_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_3_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_4_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_5_new[4] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat v_6_new[4] = { 0.0, 0.0, 0.0, 0.0 };


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


bool gluInvertMatrix(const float m[16], float invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}


void matrix_mult_16_16(GLfloat a[16], GLfloat b[16], GLfloat result[16]) {
    // multiplication for 4x4 matrices
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        }
    }
}
void matrix_mult_16_4(GLfloat a[16], GLfloat b[4], GLfloat result[4]) {
    // multiplication for 4x4 and 4x1 matrices
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 1; j++) {
            result[i + j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i + j] += a[i * 4 + k] * b[k + j];
            }
        }
    }
}

void get_object_translation_rotation_matrix(GLfloat camera_matrix[16], GLfloat object_matrix[16], GLfloat res[16]) {
    GLfloat camera_inverse[16];
    gluInvertMatrix(camera_matrix, camera_inverse);
    matrix_mult_16_16(object_matrix, camera_inverse, res);
}

void print_matrix_16(GLfloat m[16]) {
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]);
}
void print_matrix_4(GLfloat m[4]) {
    printf("%f %f %f %f\n",
        m[0], m[1], m[2], m[3]);
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
    
    glTranslatef(sway, surge, heave);
    glRotatef((GLfloat)roll, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)pitch, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)yaw, 0.0, 0.0, 1.0);

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


    printf("Translation Rotation matrix: \n");
    print_matrix_16(translation_rotation_matrix);

    

    
    printf("Old v_1 co-ordinates: ");
    print_matrix_4(v_1);

    matrix_mult_16_4(translation_rotation_matrix, v_1, v_1_new);
    matrix_mult_16_4(translation_rotation_matrix, v_2, v_2_new);
    matrix_mult_16_4(translation_rotation_matrix, v_3, v_3_new);
    matrix_mult_16_4(translation_rotation_matrix, v_4, v_4_new);
    matrix_mult_16_4(translation_rotation_matrix, v_5, v_5_new);
    matrix_mult_16_4(translation_rotation_matrix, v_6, v_6_new);

    printf("New v_1 co-ordinates: ");
    print_matrix_4(v_1_new);
    


    glBegin(GL_TRIANGLES);   

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

    float bottom_link_1[3] = { -530.0, 233.0, 0 };
    float top_link_1[3] = { -530.0, -233.0, 500 };
    float bottom_link_2[3] = { -466.0, 341, 0 };
    float top_link_2[3] = { -62.0, 575.0, 500 };
    float bottom_link_3[3] = { 466.0f, 341.0f, 0.0f };
    float top_link_3[3] = { 62.0f, 575.0f, 500.0f };
    float bottom_link_4[3] = { 529.0f, 233.0f, 0.0f };
    float top_link_4[3] = { 529.0f, -233.0f, 500.0f };
    float bottom_link_5[3] = { 62.0f, -575.0f, 0.0f };
    float top_link_5[3] = { 466.0f, -341.0f, 500.0f };
    float bottom_link_6[3] = { -62.0f, -575.0f, 0.0f };
    float top_link_6[3] = { -466.0f, -341.0f, 500.0f };



    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //remove transforms from previous display() call

    glPushMatrix();
    glTranslatef(-cameraX, -cameraY, -cameraZ); //move objects negative = move camera positive
    glRotatef((GLfloat)cameraRoll, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)cameraPitch, 1.0, 0.0, 0.0);

    GLfloat camera_matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);
    printf("Camera Matrix: \n");
    print_matrix_16(camera_matrix);

   
    draw_platform(top_link_1[0], top_link_1[1], top_link_1[2],
        top_link_2[0], top_link_2[1], top_link_2[2],
        top_link_3[0], top_link_3[1], top_link_3[2],
        top_link_4[0], top_link_4[1], top_link_4[2],
        top_link_5[0], top_link_5[1], top_link_5[2],
        top_link_6[0], top_link_6[1], top_link_6[2]);
    
    
    
    //printf("\nLink 1\n");
    
    link(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], v_1_new[0], v_1_new[1], v_1_new[2]);
    link_end(bottom_link_1[0], bottom_link_1[1], bottom_link_1[2], 10, 5, 5);
    link_end(v_1_new[0], v_1_new[1], v_1_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_1[0], bottom_link_1[1], top_link_1[0], top_link_1[1], top_link_1[2]);

    //printf("\nLink 2\n");
    
    link(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], v_2_new[0], v_2_new[1], v_2_new[2]);
    link_end(bottom_link_2[0], bottom_link_2[1], bottom_link_2[2], 10, 5, 5);
    link_end(v_2_new[0], v_2_new[1], v_2_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_2[0], bottom_link_2[1], top_link_2[0], top_link_2[1], top_link_2[2]);

    //printf("\nLink 3\n");
    
    link(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], v_3_new[0], v_3_new[1], v_3_new[2]);
    link_end(bottom_link_3[0], bottom_link_3[1], bottom_link_3[2], 10, 5, 5);
    link_end(v_3_new[0], v_3_new[1], v_3_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_3[0], bottom_link_3[1], top_link_3[0], top_link_3[1], top_link_3[2]);

    //printf("\nLink 4\n");
    
    link(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], v_4_new[0], v_4_new[1], v_4_new[2]);
    link_end(bottom_link_4[0], bottom_link_4[1], bottom_link_4[2], 10, 5, 5);
    link_end(v_4_new[0], v_4_new[1], v_4_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_4[0], bottom_link_4[1], top_link_4[0], top_link_4[1], top_link_4[2]);

    //printf("\nLink 5\n");
    
    link(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], v_5_new[0], v_5_new[1], v_5_new[2]);
    link_end(bottom_link_5[0], bottom_link_5[1], bottom_link_5[2], 10, 5, 5);
    link_end(v_5_new[0], v_5_new[1], v_5_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_5[0], bottom_link_5[1], top_link_5[0], top_link_5[1], top_link_5[2]);

    //printf("\nLink 6\n");
    
    link(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], v_6_new[0], v_6_new[1], v_6_new[2]);
    link_end(bottom_link_6[0], bottom_link_6[1], bottom_link_6[2], 10, 5, 5);
    link_end(v_6_new[0], v_6_new[1], v_6_new[2], 10, 5, 5);
    //extendable_link(50.0, 50.0, bottom_link_6[0], bottom_link_6[1], top_link_6[0], top_link_6[1], top_link_6[2]);

    draw_base(-360, 625, 0,
         360, 625, 0,
         721, 0, 0,
         360, -625, 0,
         -360, -625, 0,
         -721, 0, 0);

    
    
    glPopMatrix();
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