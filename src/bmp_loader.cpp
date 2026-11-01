#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_loader.h"
#include "texturas.h"

// Le um BMP de 24 bits sem compressao. Devolve RGB.
// Retorna NULL se o arquivo nao existir ou nao for BMP 24 bits.
unsigned char* leBMP(const char* arq, int* w, int* h) {
    FILE* f = fopen(arq, "rb");
    if (!f) return NULL;

    unsigned char cab[54];
    if (fread(cab, 1, 54, f) != 54 || cab[0] != 'B' || cab[1] != 'M') { fclose(f); return NULL; }

    int offset, largura, altura, compressao;
    short bpp;
    memcpy(&offset,     cab + 10, 4);
    memcpy(&largura,    cab + 18, 4);
    memcpy(&altura,     cab + 22, 4);
    memcpy(&bpp,        cab + 28, 2);
    memcpy(&compressao, cab + 30, 4);
    if (bpp != 24 || compressao != 0 || largura <= 0) { fclose(f); return NULL; }

    bool topDown = altura < 0;          // altura negativa = linhas gravadas de cima para baixo
    if (altura < 0) altura = -altura;

    int linha = (largura * 3 + 3) & ~3; // cada linha do BMP e preenchida ate multiplo de 4 bytes
    unsigned char* bruto = (unsigned char*)malloc((size_t)linha * altura);
    unsigned char* rgb   = (unsigned char*)malloc((size_t)largura * altura * 3);
    if (!bruto || !rgb) { fclose(f); free(bruto); free(rgb); return NULL; }

    fseek(f, offset, SEEK_SET);
    size_t lidos = fread(bruto, 1, (size_t)linha * altura, f);
    fclose(f);
    if (lidos != (size_t)linha * altura) { free(bruto); free(rgb); return NULL; }

    for (int y = 0; y < altura; y++) {
        int ty = topDown ? (altura - 1 - y) : y;
        for (int x = 0; x < largura; x++) {
            unsigned char* s = bruto + (size_t)y * linha + x * 3;      // B, G, R
            unsigned char* d = rgb + ((size_t)ty * largura + x) * 3;   // R, G, B
            d[0] = s[2]; d[1] = s[1]; d[2] = s[0];
        }
    }
    free(bruto);
    *w = largura; *h = altura;
    return rgb;
}

// Cria a textura a partir do BMP. Se falhar, devolve a textura de reserva (procedural).
GLuint carregaTexturaBMP(const char* arq, GLuint reserva) {
    int w, h;
    unsigned char* rgb = leBMP(arq, &w, &h);
    if (!rgb) { printf("Imagem nao carregada (usando textura de reserva): %s\n", arq); return reserva; }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // pixels empacotados, sem preenchimento de linha
    GLuint id = criaTextura(w, h, rgb);      // gluBuild2DMipmaps aceita tamanhos que nao sao potencia de 2
    free(rgb);
    return id;
}

//24 bits
void carregaImagens() {
    texQuadroPaisagem = carregaTexturaBMP("texturas/van_gogh.bmp",  texQuadroPaisagem);
    texQuadroAbstrato = carregaTexturaBMP("texturas/fernando_lopes.bmp", texQuadroAbstrato);
    texDN = carregaTexturaBMP("texturas/death_note.bmp", 0);
    texTV = carregaTexturaBMP("texturas/simpsons.bmp", texTV);
    texMonitor = carregaTexturaBMP("texturas/adventure_time.bmp", texMonitor);
    texAlgodao = carregaTexturaBMP("texturas/algodao.bmp", 0);
    texTeclado = carregaTexturaBMP("texturas/teclado.bmp", 0);
    texMesa = carregaTexturaBMP("texturas/granito.bmp", 0);
    texMadeira = carregaTexturaBMP("texturas/redCedar.bmp", texMovel);
    texCortina = carregaTexturaBMP("texturas/cortina.bmp", 0);
    texJanela = carregaTexturaBMP("texturas/janela.bmp", 0);
    texCouro = carregaTexturaBMP("texturas/leather.bmp", 0);
    texMoldura = carregaTexturaBMP("texturas/moldura.bmp", 0);
    texCaneca = carregaTexturaBMP("texturas/steel.bmp",0 );
    texCama = carregaTexturaBMP("texturas/blueT.bmp",0);
}
