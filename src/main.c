#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <model.h>

#define FAR_PLANE 100.0

char *help_message = "\
modelviewer [OPTIONS...]\n\
\n\
Displays an OBJ file using OpenGL.\n\
\n\
Example:\n\
  modelviewer -file assets/bunny.obj \\\n\
              -offset 0,2,4 \\\n\
              -target 0,2,0 \\\n\
              -spin   0,1,0 \n\
\n\
Options:\n\
  -file FILENAME    Select obj file to view\n\
  -offset X,Y,Z     Set camera offset\n\
  -target X,Y,Z     Set coordinates that camera looks at\n\
  -spin X,Y,Z       Set model rotatation speed around each axis\n\
\n\
Commands:\n\
  -help             Display this help message\n\
";


//Struct used to track our settings which we modify with params
typedef struct Settings {
    char *filename;
    bool lighting;
    double xoffset;
    double yoffset;
    double zoffset;
    double xtarget;
    double ytarget;
    double ztarget;
    double xspin;
    double yspin;
    double zspin;
} Settings;

Settings default_settings() {
    Settings s;
    s.filename = "assets/bunny.obj";
    s.lighting = true;
    s.xoffset = 0;
    s.yoffset = 0;
    s.zoffset = 4;
    s.xtarget = 0;
    s.ytarget = 0;
    s.ztarget = 0;
    s.xspin = 0.3;
    s.yspin = 1;
    s.zspin = 0.7;
    return s;
}

//Function to parse any command line arguments
void parse_args(Settings *s, int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-file") == 0) {
            s->filename = argv[++i];
        } 
        else if(strcmp(argv[i], "-offset") == 0) {
            char *token = strtok(argv[++i], ",");
            s->xoffset = atof(token);
            s->yoffset = atof(strtok(NULL, ","));
            s->zoffset = atof(strtok(NULL, ","));
        }
        else if(strcmp(argv[i], "-target") == 0) {
            char *token = strtok(argv[++i], ",");
            s->xtarget = atof(token);
            s->ytarget = atof(strtok(NULL, ","));
            s->ztarget = atof(strtok(NULL, ","));
        }
        else if(strcmp(argv[i], "-spin") == 0) {
            char *token = strtok(argv[++i], ",");
            s->xspin = atof(token);
            s->yspin = atof(strtok(NULL, ","));
            s->zspin = atof(strtok(NULL, ","));
        }
        else if(strcmp(argv[i], "-nolight") == 0) {
            s->lighting = false;
        }
        else if(strcmp(argv[i], "-help") == 0) {
            printf("%s\n", help_message);
            exit(0);
        }
        else {
            printf("Unrecongized parameter %s\n", argv[i]);
            exit(-1);
        }
    }
}

//Set up all of OpenGL's little settings
void opengl_startup_chores(Settings s) {
    //Turn on the relevant features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    if(s.lighting) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    //Set background color to grey
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    //Set up rendering matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.6, 0.01, FAR_PLANE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(s.xoffset, s.yoffset, s.zoffset, s.xtarget, s.ytarget, s.ztarget, 0.0, 1.0, 0.0);

    //Lighting Settings
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat position[] = {0.0f, 5.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float[]){0.3f, 0.2f, 0.2f, 0.4f});
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float[]){0.5f, 0.5f, 0.5f, 1.0f});
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
}

//Function called every time the window is resized
void window_size_callback(GLFWwindow* win, int width, int height) {
    double aspect = (double)width/height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, aspect, 0.01, FAR_PLANE);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char* argv[]) {

    //Parse Arguments and Load Model
    Settings s = default_settings();
    parse_args(&s, argc, argv);
    ModelData m = create_empty_modeldata();
    printf("Opening %s\n", s.filename);                 //DEBUG
    load_from_file(&m, s.filename);
    printf("Loaded %d vertices!\n", m.verts.size / 3);  //DEBUG
    printf("Loaded %d faces!\n", m.index.size / 3);     //DEBUG
    printf("Loaded %d normals!\n", m.normal.size / 3);  //DEBUG

    //Open Window and get it ready to render
    if(!glfwInit()) {
        printf("GLFW FAILED TO LOAD!\n");
        exit(-1);
    }
    GLFWwindow* win;
    win = glfwCreateWindow(1000, 625, "Test", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetWindowSizeCallback(win, window_size_callback);
    opengl_startup_chores(s);

    double xangle=0,yangle=0,zangle=0;

    //Render Loop
    while(!glfwWindowShouldClose(win)) {
        glfwSwapBuffers(win);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        xangle = fmod((xangle + s.xspin) ,360.0);
        yangle = fmod((yangle + s.yspin) ,360.0);
        zangle = fmod((zangle + s.zspin) ,360.0);
        glPushMatrix();
        glRotatef(xangle, 1.f, 0.f, 0.f);
        glRotatef(yangle, 0.f, 1.f, 0.f);
        glRotatef(zangle, 0.f, 0.f, 1.f);
        draw_model(&m);
        glPopMatrix();
    }

    //Shutdown and Cleanup
    glfwTerminate();
}
