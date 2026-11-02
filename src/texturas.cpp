// texturas.cpp - texturas procedurais e mapeamentos (cubico, cilindrico, esferico, planar)
#include <GL/glut.h>
#include <math.h>
#include "texturas.h"

GLuint texChao, texParede, texTV, texMonitor;
GLuint texQuadroPaisagem, texCaneca, texCama, texAba, texQuadroAbstrato, texMoldura, texCouro, texJanela, texTapete, texCortina, texColcha, texMesa, texLivro, texDN,texTeclado,texAlgodao,texMadeira;
GLuint texListras, texXadrez, texMovel;
GLUquadric* qTex = NULL;

// ======================= TEXTURAS PROCEDURAIS =======================
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

// Textura procedural de couro/tecido em tons de cinza para os livros
GLuint geraCapaLivro() {
    const int W = 64, H = 64;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            // Gera um ruído acinzentado (170 a 220) para manter a cor do material viva
            int n = 170 + (int)((((unsigned)x * 73856093u ^ (unsigned)y * 19349663u) >> 4) & 63);
            
            // Cria uma "lombada/costura" escurecendo as bordas
            if (x < 3 || x > 60 || y < 3 || y > 60) n -= 50; 
            
            px(d, W, x, y, n, n, n);
        }
    }
    return criaTextura(W, H, d);
}

// ---------- mapas de textura ----------

// Strip texture: duas cores alternadas por funcao oscilatoria
GLuint geraListras() {
    const int W = 64, H = 64;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            bool c0 = sinf(3.14159265f * x / 8.0f) > 0.0f;     // w = 8 texels
            if (c0) px(d, W, x, y, 210, 50, 50);
            else    px(d, W, x, y, 245, 245, 240);
        }
    return criaTextura(W, H, d);
}

// Xadrez vermelho/azul 
GLuint geraXadrez() {
    const int W = 64, H = 64;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            if (((x / 8) + (y / 8)) % 2) px(d, W, x, y, 200, 30, 30);
            else                         px(d, W, x, y, 30, 30, 200);
        }
    return criaTextura(W, H, d);
}

// Madeira de movel: veios ondulados
GLuint geraMadeiraMovel() {
    const int W = 128, H = 128;
    static unsigned char d[W * H * 3];
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            float g = 0.82f + 0.18f * sinf(x * 0.35f + 2.0f * sinf(y * 0.05f));
            int n = (int)((((unsigned)x * 73856093u ^ (unsigned)y * 19349663u) >> 5) & 7);
            px(d, W, x, y, (int)(150 * g) + n, (int)(95 * g) + n, (int)(55 * g));
        }
    return criaTextura(W, H, d);
}

// ======================= MAPEAMENTOS =======================
void cuboTex() {
    float h = 0.5f;
    glBegin(GL_QUADS);
        // Frente
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-h, -h,  h);
        glTexCoord2f(1, 0); glVertex3f( h, -h,  h);
        glTexCoord2f(1, 1); glVertex3f( h,  h,  h);
        glTexCoord2f(0, 1); glVertex3f(-h,  h,  h);
        // Trás
        glNormal3f(0, 0, -1);
        glTexCoord2f(1, 0); glVertex3f(-h, -h, -h);
        glTexCoord2f(1, 1); glVertex3f(-h,  h, -h);
        glTexCoord2f(0, 1); glVertex3f( h,  h, -h);
        glTexCoord2f(0, 0); glVertex3f( h, -h, -h);
        // Topo
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-h,  h, -h);
        glTexCoord2f(0, 0); glVertex3f(-h,  h,  h);
        glTexCoord2f(1, 0); glVertex3f( h,  h,  h);
        glTexCoord2f(1, 1); glVertex3f( h,  h, -h);
        // Fundo
        glNormal3f(0, -1, 0);
        glTexCoord2f(0, 0); glVertex3f(-h, -h, -h);
        glTexCoord2f(1, 0); glVertex3f( h, -h, -h);
        glTexCoord2f(1, 1); glVertex3f( h, -h,  h);
        glTexCoord2f(0, 1); glVertex3f(-h, -h,  h);
        // Direita
        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f( h, -h, -h);
        glTexCoord2f(1, 0); glVertex3f( h,  h, -h);
        glTexCoord2f(1, 1); glVertex3f( h,  h,  h);
        glTexCoord2f(0, 1); glVertex3f( h, -h,  h);
        // Esquerda
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-h, -h, -h);
        glTexCoord2f(1, 0); glVertex3f(-h, -h,  h);
        glTexCoord2f(1, 1); glVertex3f(-h,  h,  h);
        glTexCoord2f(0, 1); glVertex3f(-h,  h, -h);
    glEnd();
}

// ---------- MAPEAMENTO CUBICO ----------
// Caixa centrada na origem com dimensoes sx,sy,sz. Cada uma das 6 faces recebe uma
// projecao plana da textura. As coordenadas (u,v) sao proporcionais ao tamanho real
// da face, entao a textura nao estica mesmo em pecas finas (tampo de mesa, porta).
// Substitui: glPushMatrix(); glScalef(sx,sy,sz); glutSolidCube(1); glPopMatrix();

void caixaTex(GLuint tex, float sx, float sy, float sz, float tile) {   // valor padrao (1.0) declarado em texturas.h
    float x = sx / 2, y = sy / 2, z = sz / 2;
    float U = sx * tile, V = sy * tile, W = sz * tile;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);    // frente (+Z)
        glTexCoord2f(0, 0); glVertex3f(-x, -y,  z);
        glTexCoord2f(U, 0); glVertex3f( x, -y,  z);
        glTexCoord2f(U, V); glVertex3f( x,  y,  z);
        glTexCoord2f(0, V); glVertex3f(-x,  y,  z);

        glNormal3f(0, 0, -1);   // tras (-Z)
        glTexCoord2f(0, 0); glVertex3f( x, -y, -z);
        glTexCoord2f(U, 0); glVertex3f(-x, -y, -z);
        glTexCoord2f(U, V); glVertex3f(-x,  y, -z);
        glTexCoord2f(0, V); glVertex3f( x,  y, -z);

        glNormal3f(1, 0, 0);    // direita (+X)
        glTexCoord2f(0, 0); glVertex3f( x, -y,  z);
        glTexCoord2f(W, 0); glVertex3f( x, -y, -z);
        glTexCoord2f(W, V); glVertex3f( x,  y, -z);
        glTexCoord2f(0, V); glVertex3f( x,  y,  z);

        glNormal3f(-1, 0, 0);   // esquerda (-X)
        glTexCoord2f(0, 0); glVertex3f(-x, -y, -z);
        glTexCoord2f(W, 0); glVertex3f(-x, -y,  z);
        glTexCoord2f(W, V); glVertex3f(-x,  y,  z);
        glTexCoord2f(0, V); glVertex3f(-x,  y, -z);

        glNormal3f(0, 1, 0);    // topo (+Y)
        glTexCoord2f(0, 0); glVertex3f(-x,  y,  z);
        glTexCoord2f(U, 0); glVertex3f( x,  y,  z);
        glTexCoord2f(U, W); glVertex3f( x,  y, -z);
        glTexCoord2f(0, W); glVertex3f(-x,  y, -z);

        glNormal3f(0, -1, 0);   // base (-Y)
        glTexCoord2f(0, 0); glVertex3f(-x, -y, -z);
        glTexCoord2f(U, 0); glVertex3f( x, -y, -z);
        glTexCoord2f(U, W); glVertex3f( x, -y,  z);
        glTexCoord2f(0, W); glVertex3f(-x, -y,  z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// ---------- MAPEAMENTO CILINDRICO ----------
// gluQuadricTexture gera: s = angulo/(2*pi) ao redor do eixo Z e t = altura/h,
void cilindroTex(GLuint tex, float rBase, float rTopo, float altura) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluCylinder(qTex, rBase, rTopo, altura, 24, 4);
    glPushMatrix();                       // fundo fechado, virado para baixo
        glRotatef(180.0f, 1, 0, 0);
        gluDisk(qTex, 0.0, rBase, 24, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

// ---------- MAPEAMENTO ESFERICO ----------
// gluSphere gera: s = theta/(2*pi) (longitude) e t = phi/pi (do polo ao polo),
void esferaTex(GLuint tex, float raio) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluSphere(qTex, raio, 32, 24);
    glDisable(GL_TEXTURE_2D);
}

// ---------- APROXIMACAO DE BUMP MAPPING ----------
// Nao mexer na geometria, so perturbar as normais.
// Perturbacao e feita por vertice (ruido pseudoaleatorio), o que da aspecto
// rugoso quando a luz passa raspando. 
static float ruido(int i, int j) {
    unsigned h = (unsigned)i * 73856093u ^ (unsigned)j * 19349663u;
    h *= 2654435761u;
    return ((h >> 8) & 1023) / 1023.0f * 2.0f - 1.0f;     // -1..1
}

void planoBump(GLuint tex,
               float ox, float oy, float oz,
               float ux, float uy, float uz,
               float vx, float vy, float vz,
               float nx, float ny, float nz,
               float repU, float repV, int dU, int dV, float amp) {
    float ul = sqrtf(ux*ux + uy*uy + uz*uz), vl = sqrtf(vx*vx + vy*vy + vz*vz);
    float tux = ux / ul, tuy = uy / ul, tuz = uz / ul;
    float tvx = vx / vl, tvy = vy / vl, tvz = vz / vl;

    auto emite = [&](int i, int j) {
        float a = (float)i / dU, b = (float)j / dV;
        float r1 = ruido(i, j), r2 = ruido(j + 31, i + 17);
        glNormal3f(nx + amp * (r1 * tux + r2 * tvx),
                   ny + amp * (r1 * tuy + r2 * tvy),
                   nz + amp * (r1 * tuz + r2 * tvz));      // GL_NORMALIZE renormaliza
        glTexCoord2f(a * repU, b * repV);
        glVertex3f(ox + ux*a + vx*b, oy + uy*a + vy*b, oz + uz*a + vz*b);
    };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    for (int i = 0; i < dU; i++)
        for (int j = 0; j < dV; j++) {
            emite(i, j); emite(i + 1, j); emite(i + 1, j + 1); emite(i, j + 1);
        }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// ======================= PLANO TEXTURIZADO SUBDIVIDIDO =======================
// Origem O, vetores U e V (arestas), normal N, repeticoes de textura e subdivisoes.
// A subdivisao melhora a iluminacao por vertice.
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
    if (tex) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, tex); }
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

// ======================= INICIALIZACAO =======================
// Gera as texturas procedurais usadas pela cena
void initTexturas() {
    texChao = geraMadeira();         texParede = geraParede();
    texTV = geraTelaTV();            texMonitor = geraTelaMonitor();
    texQuadroPaisagem = geraPaisagem(); texQuadroAbstrato = geraAbstrato();
    texTapete = geraTapete();        texColcha = geraColcha();
    texLivro = geraCapaLivro();
}

// ---------- inicializacao ----------
void initTexAvancadas() {
    qTex = gluNewQuadric();
    gluQuadricTexture(qTex, GL_TRUE);          // liga a geracao de coordenadas (s,t)
    gluQuadricNormals(qTex, GLU_SMOOTH);
    texListras = geraListras();
    texXadrez  = geraXadrez();
    texMovel   = geraMadeiraMovel();
}
