// extras.h - texturas, sala texturizada, colisao, animacao suave, cortinas, luz da TV e HUD
// Incluir no main.cpp DEPOIS das variaveis globais e das funcoes material()/matXxx().
#ifndef EXTRAS_H
#define EXTRAS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SEPARATE_SPECULAR_COLOR   0x81FA
#endif

// ---------------- estados animados (valor atual x alvo) ----------------
float anguloPortaAlvo   = 0.0f;
float gavetaAlvo        = 0.0f;
float cortinaPos        = 0.0f;   // 0 = fechada, 1 = aberta
float cortinaAlvo       = 0.0f;
float anguloCadeiraAlvo = 0.0f;
bool  mostraHUD         = true;

GLuint texChao, texParede, texTV, texMonitor;
GLuint texQuadroPaisagem, texQuadroAbstrato, texTapete, texColcha;

// ======================= TEXTURAS PROCEDURAIS =======================
static void px(unsigned char* d, int w, int x, int y, int r, int g, int b) {
    int i = (y * w + x) * 3;
    d[i] = (unsigned char)r; d[i+1] = (unsigned char)g; d[i+2] = (unsigned char)b;
}

GLuint criaTextura(int w, int h, unsigned char* data) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
    return id;
}

// Piso de tabuas
GLuint geraMadeira() {
    const int W = 256, H = 256;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++) {
        int tabua = y / 32;
        float tom = 0.75f + 0.25f * ((tabua * 37) % 10) / 10.0f;
        for (int x = 0; x < W; x++) {
            float veio = 0.9f + 0.1f * sinf(x * 0.15f + tabua * 3.0f);
            bool junta = (y % 32) < 2;
            float f = junta ? 0.4f : tom * veio;
            px(d, W, x, y, (int)(160 * f), (int)(105 * f), (int)(60 * f));
        }
    }
    return criaTextura(W, H, d);
}

// Parede: reboco bege com granulado
GLuint geraParede() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            int n = (int)((((unsigned)x * 73856093u ^ (unsigned)y * 19349663u) >> 4) & 7);
            px(d, W, x, y, 205 + n, 196 + n, 178 + n);
        }
    return criaTextura(W, H, d);
}

// Tela da TV: barras de cor + rampa de cinza
GLuint geraTelaTV() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    int barras[7][3] = {{255,255,255},{255,255,0},{0,255,255},{0,255,0},{255,0,255},{255,0,0},{0,0,255}};
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            if (y < H / 4) { int g = x * 255 / W; px(d, W, x, y, g, g, g); }
            else { int b = x * 7 / W; px(d, W, x, y, barras[b][0], barras[b][1], barras[b][2]); }
        }
    return criaTextura(W, H, d);
}

// Tela do monitor: area de trabalho com janela e barra de tarefas
GLuint geraTelaMonitor() {
    const int W = 256, H = 160;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            int r = 30 + x / 4, g = 60 + y / 3, b = 150 + y / 4;      // papel de parede
            if (y < 14) { r = 25; g = 25; b = 35; }                    // barra de tarefas
            else if (x > 40 && x < 190 && y > 40 && y < 135) {         // janela
                if (y > 122) { r = 30; g = 60; b = 170; }              // titulo
                else         { r = 235; g = 235; b = 240; }            // corpo
            }
            px(d, W, x, y, r, g, b);
        }
    return criaTextura(W, H, d);
}

// Quadro 1: paisagem (ceu, sol, montanhas, lago)
GLuint geraPaisagem() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            int r = 90 + y / 2, g = 150 + y / 3, b = 230;              // ceu
            float dx = x - 90.0f, dy = y - 95.0f;
            if (dx * dx + dy * dy < 150.0f) { r = 255; g = 230; b = 80; }   // sol
            float h = 55.0f + 25.0f * sinf(x * 0.07f) + 10.0f * sinf(x * 0.19f);
            if (y < h) { r = 90; g = 105; b = 115; }                   // montanhas
            if (y < 40) { r = 40; g = 90 + (x % 7) * 2; b = 150; }     // lago
            px(d, W, x, y, r, g, b);
        }
    return criaTextura(W, H, d);
}

// Quadro 2: aneis concentricos
GLuint geraAbstrato() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    int pal[4][3] = {{200,60,50},{240,190,60},{40,110,150},{240,235,220}};
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            float dist = sqrtf((x - 64.0f) * (x - 64.0f) + (y - 64.0f) * (y - 64.0f));
            int c = ((int)(dist / 10.0f)) % 4;
            px(d, W, x, y, pal[c][0], pal[c][1], pal[c][2]);
        }
    return criaTextura(W, H, d);
}

// Tapete com borda e losangos
GLuint geraTapete() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            int bx = x < W - 1 - x ? x : W - 1 - x;
            int by = y < H - 1 - y ? y : H - 1 - y;
            int m = bx < by ? bx : by;
            int r, g, b;
            if (m < 8)       { r = 150; g = 30;  b = 30;  }
            else if (m < 12) { r = 230; g = 200; b = 120; }
            else if (m < 28) { r = 40;  g = 50;  b = 110; }
            else {
                int k = ((abs(x - 64) + abs(y - 64)) / 10) % 2;
                if (k) { r = 150; g = 30; b = 30; } else { r = 200; g = 170; b = 100; }
            }
            px(d, W, x, y, r, g, b);
        }
    return criaTextura(W, H, d);
}

// Colcha xadrez
GLuint geraColcha() {
    const int W = 64, H = 64;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            bool k = ((x / 16) + (y / 16)) % 2;
            bool costura = (x % 16 == 0) || (y % 16 == 0);
            if (costura) px(d, W, x, y, 50, 70, 130);
            else if (k)  px(d, W, x, y, 70, 95, 170);
            else         px(d, W, x, y, 100, 125, 200);
        }
    return criaTextura(W, H, d);
}

// ======================= PLANO TEXTURIZADO SUBDIVIDIDO =======================
// Origem O, vetores U e V (arestas), normal N, repeticoes de textura e subdivisoes.
// A subdivisao melhora a iluminacao por vertice (brilho especular aparece em planos grandes).
void planoTex(GLuint tex,
              float ox, float oy, float oz,
              float ux, float uy, float uz,
              float vx, float vy, float vz,
              float nx, float ny, float nz,
              float repU, float repV, int dU, int dV) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glNormal3f(nx, ny, nz);
    glBegin(GL_QUADS);
    for (int i = 0; i < dU; i++)
        for (int j = 0; j < dV; j++) {
            float a0 = (float)i / dU, a1 = (float)(i + 1) / dU;
            float b0 = (float)j / dV, b1 = (float)(j + 1) / dV;
            glTexCoord2f(a0 * repU, b0 * repV); glVertex3f(ox + ux*a0 + vx*b0, oy + uy*a0 + vy*b0, oz + uz*a0 + vz*b0);
            glTexCoord2f(a1 * repU, b0 * repV); glVertex3f(ox + ux*a1 + vx*b0, oy + uy*a1 + vy*b0, oz + uz*a1 + vz*b0);
            glTexCoord2f(a1 * repU, b1 * repV); glVertex3f(ox + ux*a1 + vx*b1, oy + uy*a1 + vy*b1, oz + uz*a1 + vz*b1);
            glTexCoord2f(a0 * repU, b1 * repV); glVertex3f(ox + ux*a0 + vx*b1, oy + uy*a0 + vy*b1, oz + uz*a0 + vz*b1);
        }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// ======================= SALA (chao, paredes, teto) =======================
void desenhaSala() {
    // Chao de madeira envernizada
    material(1, 1, 1, 0.35f, 30.0f);
    planoTex(texChao, -8,0,-8,  16,0,0,  0,0,16,  0,1,0,  4,4, 16,16);

    // Paredes
    matParede(1, 1, 1);
    planoTex(texParede, -8,0,-8,  16,0,0,  0,6,0,  0,0,1,   4,1.5f, 16,6);    // frente
    planoTex(texParede, -8,0,-8,  0,0,16,  0,6,0,  1,0,0,   4,1.5f, 16,6);    // esquerda
    planoTex(texParede,  8,0,-8,  0,0,16,  0,6,0, -1,0,0,   4,1.5f, 16,6);    // direita
    // parede de tras, com vao da porta
    planoTex(texParede, -8,0,8,    1.5f,0,0, 0,6,0, 0,0,-1,  0.4f,1.5f, 2,6);
    planoTex(texParede, -6.5f,4,8, 2,0,0,    0,2,0, 0,0,-1,  0.5f,0.5f, 2,2);
    planoTex(texParede, -4.5f,0,8, 12.5f,0,0,0,6,0, 0,0,-1,  3,1.5f, 12,6);

    // Teto
    matParede(0.9f, 0.9f, 0.9f);
    planoTex(texParede, -8,6,-8,  16,0,0,  0,0,16,  0,-1,0,  4,4, 16,16);
}

// ======================= TAPETE, COLCHA, TELAS =======================
void desenhaTapete() {
    matTecido(1, 1, 1);
    planoTex(texTapete, -2.5f,0.02f,-4.8f,  5,0,0,  0,0,4,  0,1,0,  1,1, 4,4);
}

// chamar dentro da matriz da cama, depois do colchao
void desenhaColcha() {
    matTecido(1, 1, 1);
    planoTex(texColcha, -1.4f,1.205f,-2.9f,  2.8f,0,0,  0,0,5.8f,  0,1,0,  3,6, 4,8);
}

// Tela da TV (coordenadas locais do painel). Barras rolam com o tempo.
void telaTV() {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(glutGet(GLUT_ELAPSED_TIME) * 0.0002f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    planoTex(texTV, 0.056f,-0.51f,1.01f,  0,0,-2.02f,  0,1.02f,0,  1,0,0,  1,1, 4,4);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

// Tela do monitor (coordenadas locais do painel)
void telaMonitor() {
    planoTex(texMonitor, -0.95f,-0.55f,0.121f,  1.9f,0,0,  0,1.1f,0,  0,0,1,  1,1, 4,4);
}

// ======================= CORTINAS (transparentes, desenhadas por ultimo) =======================
void desenhaCortinas() {
    float esc = 1.6f + (0.3f - 1.6f) * cortinaPos;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glPushMatrix();
        glTranslatef(4.0f, 3.5f, -7.9f);
        glColor4f(0.8f, 0.7f, 0.5f, 0.7f);
        float zero[] = {0, 0, 0, 1};
        glMaterialfv(GL_FRONT, GL_SPECULAR, zero);

        glPushMatrix();   // esquerda
            glTranslatef(-1.6f + esc / 2.0f, 0.0f, 0.1f);
            glScalef(esc, 2.2f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
        glPushMatrix();   // direita
            glTranslatef(1.6f - esc / 2.0f, 0.0f, 0.1f);
            glScalef(esc, 2.2f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

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

// Move testando cada eixo separado (desliza ao longo da parede)
void mover(float dx, float dz) {
    if (livre(camX + dx, camZ)) camX += dx;
    if (livre(camX, camZ + dz)) camZ += dz;
}

// ======================= ANIMACAO SUAVE =======================
float aproxima(float a, float alvo, float vel) {
    if (fabsf(alvo - a) <= vel) return alvo;
    return a + (alvo > a ? vel : -vel);
}

void atualiza(int) {
    anguloPorta   = aproxima(anguloPorta,   anguloPortaAlvo,   3.0f);
    gavetaX       = aproxima(gavetaX,       gavetaAlvo,        0.04f);
    cortinaPos    = aproxima(cortinaPos,    cortinaAlvo,       0.03f);
    anguloCadeira = aproxima(anguloCadeira, anguloCadeiraAlvo, 3.0f);
    glutPostRedisplay();
    glutTimerFunc(16, atualiza, 0);
}

// ======================= HUD (ajuda na tela) =======================
static void texto(float x, float y, const char* s) {
    glRasterPos2f(x, y);
    while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s++);
}

void desenhaHUD() {
    if (!mostraHUD) return;
    int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();

    glColor3f(1.0f, 1.0f, 0.6f);
    texto(10, h - 18, "W/A/S/D: mover | Mouse (segurar botao esquerdo): olhar");
    texto(10, h - 34, "E: porta / gaveta / TV / monitor | F: luzes | C: cortina | R: cadeira | H: ocultar ajuda");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// ======================= INICIALIZACAO =======================
void initExtras() {
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);  // brilho sobre a textura

    texChao = geraMadeira();         texParede = geraParede();
    texTV = geraTelaTV();            texMonitor = geraTelaMonitor();
    texQuadroPaisagem = geraPaisagem(); texQuadroAbstrato = geraAbstrato();
    texTapete = geraTapete();        texColcha = geraColcha();

    // Luz 2: brilho azulado da TV (ligada junto com a TV)
    float amb[] = {0, 0, 0, 1}, dif[] = {0.25f, 0.4f, 0.9f, 1}, esp[] = {0.2f, 0.3f, 0.6f, 1};
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, esp);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.10f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.05f);

    glutTimerFunc(16, atualiza, 0);
}

#endif