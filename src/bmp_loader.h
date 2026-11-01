// bmp_loader.h - carregador de texturas BMP 24 bits (codigo proprio, sem bibliotecas)
//   arquivo .bmp -> vetor de bytes RGB (leBMP) -> gluBuild2DMipmaps -> textura
#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <GL/glut.h>

unsigned char* leBMP(const char* arq, int* w, int* h);
GLuint carregaTexturaBMP(const char* arq, GLuint reserva);
void carregaImagens();        // carrega as imagens da pasta texturas/

#endif
