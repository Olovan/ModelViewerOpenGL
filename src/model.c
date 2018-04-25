#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <model.h>

void load_from_file(ModelData *m, char* file_path) {
    const int MAX_LINE_LENGTH = 256;
    char nextline[MAX_LINE_LENGTH];
    FILE *file = fopen(file_path, "r");

    if(file == NULL) {
        printf("Could not open file %s\n", file_path);
        exit(-1);
    }

    while(fgets(nextline, MAX_LINE_LENGTH, file) != NULL) {
        char *token = strtok(nextline, " ");
        if(strcmp(token, "v") == 0) {
            double x, y, z;
            x = atof(strtok(NULL, " "));
            y = atof(strtok(NULL, " "));
            z = atof(strtok(NULL, " "));

            append_to_doublebuffer(&m->verts, x);
            append_to_doublebuffer(&m->verts, y);
            append_to_doublebuffer(&m->verts, z);
            //Add Color
            append_to_charbuffer(&m->color, 120); //R
            append_to_charbuffer(&m->color, 60); //G
            append_to_charbuffer(&m->color, 60); //B
        } else if(strcmp(token, "f") == 0) {
            //TODO Implement loading faces
            int x, y, z;  //Index numbers for vertices
            x = atoi(strtok(NULL, " "));
            y = atoi(strtok(NULL, " "));
            z = atoi(strtok(NULL, " "));
            append_to_intbuffer(&m->index, x);
            append_to_intbuffer(&m->index, y);
            append_to_intbuffer(&m->index, z);
        } else if(strcmp(token, "vn") == 0){
            double x, y, z;
            x = atof(strtok(NULL, " "));
            y = atof(strtok(NULL, " "));
            z = atof(strtok(NULL, " "));

            append_to_doublebuffer(&m->normal, x);
            append_to_doublebuffer(&m->normal, y);
            append_to_doublebuffer(&m->normal, z);
        } else {
            //Do nothing
        }
    } //Finished loading file
    //Fix Indices because OBJ file format is not 0 indexed
    for(int i = 0; i < m->index.size; i++) {
        if(m->index.data[i] > 0) {
            m->index.data[i] -= 1;
        } else {
            m->index.data[i] = m->index.size - m->index.data[i];
        }
    }
}

ModelData create_empty_modeldata() {
    ModelData m;
    m.verts.size = 0;
    m.verts.max_size = 0;
    m.verts.data = 0;
    m.index.size = 0;
    m.index.max_size = 0;
    m.index.data = 0;
    m.color.size = 0;
    m.color.max_size = 0;
    m.color.data = 0;
    m.normal.size = 0;
    m.normal.max_size = 0;
    m.normal.data = 0;
    return m;
}

void draw_model(ModelData *m) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, (void*)m->verts.data);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, (void*)m->color.data);
    glNormalPointer(GL_DOUBLE, 0, (void*)m->normal.data);
    glDrawElements(GL_TRIANGLES, m->index.size, GL_UNSIGNED_INT, m->index.data);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
