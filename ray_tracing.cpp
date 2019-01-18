#include <bits/stdc++.h>
#include "vector_3d.h"
#include "point.h"
#include "sphere.h"
#include "pyramid.h"

#include <GL/glut.h>

using namespace std;

int draw_grid;
int draw_axes;

const int CAMERA_MOVEMENT = 5;

double near, far, fov_y, aspect_ratio;
int level_of_recursion, screen_size ;
double checker_board_width;
double ambient_co_eff, diffuse_co_eff, reflection_co_eff;
double no_of_objects;
double no_of_light_sources;

vector<sphere> spheres;
vector<pyramid> pyramids;
vector<point> light_sources;

struct camera
{
    point pos;
    vector_3d u, r, l;

    camera()
    {
        init_camera();
    }

    void init_camera() {
        pos = point(50, -50, 150);


        l = vector_3d(0, 1, 0);
        u = vector_3d(0, 0, 1);
        r = vector_3d(1, 0, 0);
    }

} my_camera;

void drawAxes()
{
    if (draw_axes == 1)
    {
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f(1000, 0, 0);
            glVertex3f(-1000, 0, 0);

            glVertex3f(0, -1000, 0);
            glVertex3f(0, 1000, 0);

            glVertex3f(0, 0, 1000);
            glVertex3f(0, 0, -1000);
        }
        glEnd();
    }
}

void drawGrid()
{
    int i;
    if (draw_grid == 1)
    {
        glColor3f(0.6, 0.6, 0.6); //grey
        glBegin(GL_LINES);
        {
            for (i = -80; i <= 80; i++)
            {

                if (i == 0)
                    continue; //SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i * 10, -900, 0);
                glVertex3f(i * 10, 900, 0);

                //lines parallel to X-axis
                glVertex3f(-900, i * 10, 0);
                glVertex3f(900, i * 10, 0);
            }
        }
        glEnd();
    }
}

void drawSphere(double radius, int slices, int stacks)
{
    struct point points[100][100];
    int i, j;
    double h, r;
    //generate points
    for (i = 0; i <= stacks; i++)
    {
        h = radius * sin(((double)i / (double)stacks) * (PI / 2));
        r = radius * cos(((double)i / (double)stacks) * (PI / 2));
        for (j = 0; j <= slices; j++)
        {
            points[i][j].x = r * cos(((double)j / (double)slices) * 2 * PI);
            points[i][j].y = r * sin(((double)j / (double)slices) * 2 * PI);
            points[i][j].z = h;
        }
    }
    //draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        glColor3f((double)i / (double)stacks, (double)i / (double)stacks, (double)i / (double)stacks);
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
            glEnd();
        }
    }
}


void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {

    case '1':
        my_camera.r = my_camera.r.rotate_3d(CAMERA_MOVEMENT, my_camera.u);
        my_camera.l = my_camera.l.rotate_3d(CAMERA_MOVEMENT, my_camera.u);
        break;
    case '2':
        my_camera.r = my_camera.r.rotate_3d(-CAMERA_MOVEMENT, my_camera.u);
        my_camera.l = my_camera.l.rotate_3d(-CAMERA_MOVEMENT, my_camera.u);
        break;
    case '3':
        my_camera.u = my_camera.u.rotate_3d(CAMERA_MOVEMENT, my_camera.r);
        my_camera.l = my_camera.l.rotate_3d(CAMERA_MOVEMENT, my_camera.r);
        break;
    case '4':
        my_camera.u = my_camera.u.rotate_3d(-CAMERA_MOVEMENT, my_camera.r);
        my_camera.l = my_camera.l.rotate_3d(-CAMERA_MOVEMENT, my_camera.r);
        break;
    case '5':
        my_camera.u = my_camera.u.rotate_3d(-CAMERA_MOVEMENT, my_camera.l);
        my_camera.r = my_camera.r.rotate_3d(-CAMERA_MOVEMENT, my_camera.l);
        break;
    case '6':
        my_camera.u = my_camera.u.rotate_3d(CAMERA_MOVEMENT, my_camera.l);
        my_camera.r = my_camera.r.rotate_3d(CAMERA_MOVEMENT, my_camera.l);
        break;
    default:
        break;
    }
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN: //down arrow key
        my_camera.pos = my_camera.pos.add_vector(my_camera.l.scale(-1));
        break;
    case GLUT_KEY_UP: // up arrow key
        my_camera.pos = my_camera.pos.add_vector(my_camera.l);
        break;

    case GLUT_KEY_RIGHT:
        my_camera.pos = my_camera.pos.add_vector(my_camera.r);
        break;
    case GLUT_KEY_LEFT:
        my_camera.pos = my_camera.pos.add_vector(my_camera.r.scale(-1));
        break;

    case GLUT_KEY_PAGE_UP:
        my_camera.pos = my_camera.pos.add_vector(my_camera.u);
        break;
    case GLUT_KEY_PAGE_DOWN:
        my_camera.pos = my_camera.pos.add_vector(my_camera.u.scale(-1));
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        my_camera.init_camera();
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}

void mouseListener(int button, int state, int x, int y)
{ //x, y is the x-y of the screen (2D
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        { // 2 times?? in ONE click? -- solution is checking DOWN or UP
            //draw_axes = 1 - draw_axes;

        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        if (state == GLUT_DOWN)
        { // 2 times?? in ONE click? -- solution is checking DOWN or UP
            //draw_grid = 1 - draw_grid;

        }
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display()
{
    {
        //clear the display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1, 1); //color black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************
	    / set-up camera here
	    ********************/
        //load the correct matrix -- MODEL-VIEW matrix
        glMatrixMode(GL_MODELVIEW);

        //initialize the matrix
        glLoadIdentity();

        //now give three info
        //1. where is the camera (viewer)?
        //2. where is the camera looking?
        //3. Which direction is the camera's UP direction?

        //gluLookAt(50, 50, -100, 0, 0, 0, 1, 0, 0);
        //gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
        //gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
        point p = my_camera.pos.add_vector(my_camera.l);
        gluLookAt(my_camera.pos.x, my_camera.pos.y, my_camera.pos.z,
                  p.x, p.y, p.z, my_camera.u.i, my_camera.u.j, my_camera.u.k);

        //again select MODEL-VIEW
        glMatrixMode(GL_MODELVIEW);
    }
    /****************************
	/ Add your objects from here
	****************************/
    //add objects

    drawAxes();
    drawGrid();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate()
{

    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    draw_grid = 1;
    draw_axes = 1;

    //clear the screen
    //glClearColor(0, 0, 0, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    /************************
	/ set-up projection here
	************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
//    gluPerspective(80, 1, 1, 1500.0);
    gluPerspective(fov_y, aspect_ratio, near, far);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

void take_input() {
    ifstream description;
    description.open("description.txt");
    if (!description.is_open()) {
        cerr << "Cannot find description file" << endl;
        exit(EXIT_FAILURE);
    }

    description >> near >> far >> fov_y >> aspect_ratio;
    description >> level_of_recursion >> screen_size;
    description >> checker_board_width;
    description >> ambient_co_eff >> diffuse_co_eff >> reflection_co_eff;
    description >> no_of_objects;

    for (int i = 0; i < no_of_objects; ++i) {
        string type;
        description >> type;
        if (type == "sphere") {
            sphere sp;
            description >> sp;
            spheres.push_back(sp);
        } else if (type == "pyramid") {
            pyramid py;
            description >> py;
            py.complete_pyramid();
            pyramids.push_back(py);
        } else {
            cerr << "Unknown object\n";
            exit(EXIT_FAILURE);
        }
    }

    description >> no_of_light_sources;

    for (int j = 0; j < no_of_light_sources; ++j) {
        point p;
        description >> p;
        light_sources.push_back(p);
    }
    cout << "Done taking input" << endl;
}

int main(int argc, char **argv)
{
    take_input();

    glutInit(&argc, argv);
    glutInitWindowSize(screen_size, screen_size);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //Depth, Double buffer, RGB color

    glutCreateWindow("RAY");

    init();

    glEnable(GL_DEPTH_TEST); //enable Depth Testing

    glutDisplayFunc(display); //display callback function
    glutIdleFunc(animate);    //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop(); //The main loop of OpenGL

    return 0;
}
