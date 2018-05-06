#include <databuffers.h>
#include <stdio.h>
#include <stdlib.h>

#define NEW_CHUNK_SIZE 100

/********************************************************************************
 * Adds data to the end of the buffer. If the buffer needs to be resized to 
 * accomodate the new data the buffer's size will be increased by NEW_CHUNK_SIZE
 * number of elements
 * ******************************************************************************/
void append_to_intbuffer(IntBuffer *b, int i) {
    if(b->size == b->max_size) {
        int *new_verts = realloc(b->data, (b->max_size + NEW_CHUNK_SIZE) * sizeof(int));
        if(new_verts == NULL) {
            printf("MEMORY ERROR: We could not realloc ModelData verts!");
            exit(-1);
        }
        b->data = new_verts;
        b->max_size = b->max_size + NEW_CHUNK_SIZE;
    }
    b->data[b->size] = i;
    b->size += 1;
}

/********************************************************************************
 * Adds data to the end of the buffer. If the buffer needs to be resized to 
 * accomodate the new data the buffer's size will be increased by NEW_CHUNK_SIZE
 * number of elements
 * ******************************************************************************/
void append_to_doublebuffer(DoubleBuffer *b, double d) {
    if(b->size == b->max_size) {
        double *new_verts = realloc(b->data, (b->max_size + NEW_CHUNK_SIZE) * sizeof(double));
        if(new_verts == NULL) {
            printf("MEMORY ERROR: We could not realloc ModelData verts!");
            exit(-1);
        }
        b->data = new_verts;
        b->max_size = b->max_size + NEW_CHUNK_SIZE;
    }
    b->data[b->size] = d;
    b->size += 1;
}

/********************************************************************************
 * Adds data to the end of the buffer. If the buffer needs to be resized to 
 * accomodate the new data the buffer's size will be increased by NEW_CHUNK_SIZE
 * number of elements
 * ******************************************************************************/
void append_to_charbuffer(CharBuffer *b, unsigned char d) {
    if(b->size == b->max_size) {
        unsigned char *new_verts = realloc(b->data, (b->max_size + NEW_CHUNK_SIZE) * sizeof(unsigned char));
        if(new_verts == NULL) {
            printf("MEMORY ERROR: We could not realloc ModelData verts!");
            exit(-1);
        }
        b->data = new_verts;
        b->max_size = b->max_size + NEW_CHUNK_SIZE;
    }
    b->data[b->size] = d;
    b->size += 1;
}
