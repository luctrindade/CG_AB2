// texturas_avancadas.h
// Incluir DEPOIS do extras.h. Chamar initTexAvancadas() no fim de initExtras().
//
// Conteudo (relacionado aos slides de Mapeamento de Textura):
//  - Strip texture (sin(pi*x/w) > 0)           -> geraListras()
//  - Textura xadrez (exemplo da esfera)         -> geraXadrez()
//  - Mapeamento CUBICO (6 projecoes planas)     -> caixaTex()
//  - Mapeamento CILINDRICO (u = 2*pi*s, z = t)  -> cilindroTex()
//  - Mapeamento ESFERICO (theta=2*pi*s, phi=pi*t)-> esferaTex()
//  - Aproximacao de BUMP MAPPING (normais perturbadas) -> planoBump()
#ifndef TEXTURAS_AVANCADAS_H
#define TEXTURAS_AVANCADAS_H

GLuint texListras, texXadrez, texMovel;
static GLUquadric* qTex = NULL;

// ---------- mapas de textura ----------

// Strip texture dos slides: duas cores alternadas por funcao oscilatoria
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

// Xadrez vermelho/azul (mesmo exemplo da esfera dos slides)
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

// ---------- MAPEAMENTO CUBICO ----------
// Caixa centrada na origem com dimensoes sx,sy,sz. Cada uma das 6 faces recebe uma
// projecao plana da textura. As coordenadas (u,v) sao proporcionais ao tamanho real
// da face, entao a textura NAO estica mesmo em pecas finas (tampo de mesa, porta).
// Substitui: glPushMatrix(); glScalef(sx,sy,sz); glutSolidCube(1); glPopMatrix();
void caixaTex(GLuint tex, float sx, float sy, float sz, float tile = 1.0f) {
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
// exatamente a parametrizacao do slide (u = 2*pi*s, z = t). Eixo do cilindro = Z local.
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
// a parametrizacao do slide (theta = 2*pi*s, phi = pi*t). Polos no eixo Z local.
void esferaTex(GLuint tex, float raio) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluSphere(qTex, raio, 32, 24);
    glDisable(GL_TEXTURE_2D);
}

// ---------- APROXIMACAO DE BUMP MAPPING ----------
// Ideia dos slides: nao mexer na geometria, so perturbar as NORMAIS.
// Aqui a perturbacao e feita por vertice (ruido pseudoaleatorio), o que da aspecto
// rugoso quando a luz passa raspando. Bump mapping real (por pixel, lendo um mapa de
// alturas) exigiria shaders ou extensoes; esta e a versao possivel no pipeline fixo.
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

// ---------- inicializacao ----------
void initTexAvancadas() {
    qTex = gluNewQuadric();
    gluQuadricTexture(qTex, GL_TRUE);          // liga a geracao de coordenadas (s,t)
    gluQuadricNormals(qTex, GLU_SMOOTH);
    texListras = geraListras();
    texXadrez  = geraXadrez();
    texMovel   = geraMadeiraMovel();
}

#endif