// sombras.h - sombra projetada planar com stencil
#ifndef SOMBRAS_H
#define SOMBRAS_H

extern bool sombrasAtivas;

void calculaMatrizSombra(const float L[4], float yPlano, float m[16]);
void desenhaSombras();
void initSombras();

#endif
