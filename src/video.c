#include "../include/type.h"
#include "../include/video.h"
#include "../include/io.h"

void write(const char* str,size_t length) {
    
    char* video = (char*) 0xb8000;
    int posicion = 0;
    
    while (length--) {
        video[posicion*2] = str[posicion];
        posicion++;
    }
}

void updateCursor(int row, int col) {

    unsigned short position=(row*80) + col;
 
    // cursor LOW port to vga INDEX register
    outB(0x3D4, 0x0F);
    outB(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outB(0x3D4, 0x0E);
    outB(0x3D5, (unsigned char )((position>>8)&0xFF));
 }