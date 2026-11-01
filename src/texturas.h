// texturas.h - mapas de textura (procedurais) e funcoes de mapeamento
#ifndef TEXTURAS_H
#define TEXTURAS_H

#include <GL/glut.h>

// identificadores das texturas
extern GLuint texChao, texParede, texTV, texMonitor;
extern GLuint texQuadroPaisagem, texCaneca, texCama, texQuadroAbstrato, texMoldura, texCouro,
              texJanela, texTapete, texCortina, texColcha, texMesa, texLivro, texDN,
              texTeclado, texAlgodao, texMadeira;
extern GLuint texListras, texXadrez, texMovel;

// quadric com coordenadas de textura (usada no mapeamento cilindrico/esferico)
extern GLUquadric* qTex;

GLuint criaTextura(int w, int h, unsigned char* data);

// mapeamentos
void cuboTex();
void caixaTex(GLuint tex, float sx, float sy, float sz, float tile = 1.0f);   // mapeamento cubico
void cilindroTex(GLuint tex, float rBase, float rTopo, float altura);         // mapeamento cilindrico
void esferaTex(GLuint tex, float raio);                                        // mapeamento esferico
void planoTex(GLuint tex,
              float ox, float oy, float oz,
              float ux, float uy, float uz,
              float vx, float vy, float vz,
              float nx, float ny, float nz,
              float repU, float repV, int dU, int dV);
void planoBump(GLuint tex,
               float ox, float oy, float oz,
               float ux, float uy, float uz,
               float vx, float vy, float vz,
               float nx, float ny, float nz,
               float repU, float repV, int dU, int dV, float amp);

// inicializacao (gera as texturas procedurais)
void initTexturas();
void initTexAvancadas();

#endif
