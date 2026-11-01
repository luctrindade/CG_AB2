// colisao.cpp
#include <GL/glut.h>
#include "colisao.h"
#include "estado.h"

// ======================= COLISAO =======================
struct Caixa { float x0, x1, z0, z1; };
Caixa obstaculos[] = {
    {-2.0f,  2.0f, -7.0f, -5.0f},   // mesa
    { 4.0f,  7.0f, -7.0f, -1.0f},   // cama
    { 2.25f, 3.75f,-6.75f,-5.25f},  // criado-mudo
    {-7.5f, -5.4f, -8.0f, -6.9f},   // estante
    {-8.0f, -6.4f,  1.5f,  4.5f},   // armario
    { 4.0f,  7.0f,  6.3f,  8.0f}    // rack da TV
};

bool livre(float x, float z) {
    const float r = 0.4f;
    if (x < -7.5f || x > 7.5f || z < -7.5f || z > 7.5f) return false;
    for (unsigned i = 0; i < sizeof(obstaculos) / sizeof(Caixa); i++) {
        Caixa& c = obstaculos[i];
        if (x > c.x0 - r && x < c.x1 + r && z > c.z0 - r && z < c.z1 + r) return false;
    }
    return true;
}

// Move testando cada eixo separado 
void mover(float dx, float dz) {
    if (livre(camX + dx, camZ)) camX += dx;
    if (livre(camX, camZ + dz)) camZ += dz;
}
