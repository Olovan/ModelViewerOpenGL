#include <databuffers.h>

typedef struct ModelData {
    DoubleBuffer verts;
    DoubleBuffer normal;
    IntBuffer index;
    CharBuffer color;
} ModelData;


void load_from_file(ModelData*, char*);
ModelData create_empty_modeldata();
void draw_model(ModelData *m); 
