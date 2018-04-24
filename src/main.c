#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <model.h>

typedef struct Settings {
    char *filename;
    double xoffset;
    double yoffset;
    double zoffset;
    double xtarget;
    double ytarget;
    double ztarget;
} Settings;

Settings default_settings();
void opengl_startup_chores(Settings);
void parse_args(Settings *s, int argc, char** argv);
void main(int argc, char* argv[]);

Settings default_settings() {
    Settings s;
    s.filename = "models/bunny.obj";
    s.xoffset = 0;
    s.yoffset = 0;
    s.zoffset = 4;
    s.xtarget = 0;
    s.ytarget = 0;
    s.ztarget = 0;
    return s;
}

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
        else {
            printf("Unrecongized parameter %s\n", argv[i]);
            exit(-1);
        }
    }
}

void opengl_startup_chores(Settings s) {
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.6, 0.01, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(s.xoffset, s.yoffset, s.zoffset, s.xtarget, s.ytarget, s.ztarget, 0.0, 1.0, 0.0);
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {0.0f, 5.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float[]){0.3f, 0.2f, 0.2f, 0.2f});
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float[]){0.8f, 0.3f, 0.3f, 1.0f});
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 120.0f);
}

void main(int argc, char* argv[]) {
    char *objfile = "models/bunny.obj";
    Settings s = default_settings();
    parse_args(&s, argc, argv);
    ModelData m = create_empty_modeldata();
    printf("%s\n", s.filename);
    load_from_file(&m, s.filename);
    printf("Loaded %d vertices!\n", m.verts.size / 3);
    printf("Loaded %d faces!\n", m.index.size / 3);
    printf("Loaded %d normals!\n", m.normal.size / 3);
    if(!glfwInit()) {
        printf("GLFW FAILED TO LOAD!\n");
        exit(-1);
    }

    GLFWwindow* win;
    win = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    glfwMakeContextCurrent(win);
    opengl_startup_chores(s);

    while(!glfwWindowShouldClose(win)) {
        glfwSwapBuffers(win);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glRotatef(1.0f, 0.f, 1.f, 0.f);
        draw_model(&m);
    }

    glfwTerminate();
}
