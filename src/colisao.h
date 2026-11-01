// colisao.h - colisao da camera com paredes e moveis (caixas AABB em planta)
#ifndef COLISAO_H
#define COLISAO_H

bool livre(float x, float z);
void mover(float dx, float dz);     // move testando cada eixo separado (desliza nas paredes)

#endif
