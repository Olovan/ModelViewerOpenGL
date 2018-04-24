typedef struct IntBuffer {
    int size;
    int max_size;
    int* data;
} IntBuffer;

typedef struct DoubleBuffer {
    int size;
    int max_size;
    double* data;
} DoubleBuffer;

typedef struct CharBuffer {
    int size;
    int max_size;
    unsigned char* data;
} CharBuffer;

void append_to_intbuffer(IntBuffer *b, int i); 
void append_to_doublebuffer(DoubleBuffer *b, double); 
void append_to_charbuffer(CharBuffer *b, unsigned char); 
