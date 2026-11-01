# Escritório 3D — OpenGL

Cena 3D interativa de um quarto/escritório, feita em **C++ com OpenGL (pipeline fixo) e GLUT**.
O usuário caminha pela cena com teclado e mouse e interage com objetos (porta, gaveta, TV, monitor,
cortina, luzes e cadeira). Não usa motor gráfico nem modelos importados: todos os objetos são
modelados no código.

**Equipe:** Felipe Lima Aragão · Lucas Trindade de Souza 
**Disciplina:** [Computação Gráfica] — [UFAL]

---

## Como compilar

### Dependências

| Sistema | O que instalar |
|---|---|
| Linux (Debian/Ubuntu) | `sudo apt install g++ make freeglut3-dev` |
| Windows | MinGW-w64 com freeglut. Exemplo com MSYS2 (terminal *MINGW64*): `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut` |

O código usa C++11 (há uma lambda em `quadros.cpp`).

### Opção 1 — Makefile

```bash
make                 # Linux
mingw32-make         # Windows (MinGW)
```

O executável é gerado na raiz: `cena` (Linux) ou `cena.exe` (Windows). `make clean` apaga os arquivos gerados.

### Opção 2 — g++ direto

```bash
# Linux
g++ -std=c++11 -Isrc main.cpp src/*.cpp src/objetos/*.cpp -o cena -lglut -lGLU -lGL

# Windows (MinGW)
g++ -std=c++11 -Isrc main.cpp src/*.cpp src/objetos/*.cpp -o cena.exe -lfreeglut -lopengl32 -lglu32
```

### Opção 3 — CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> No Windows, `freeglut.dll` precisa estar na mesma pasta do `.exe` ou no `PATH`.

## Como executar

Execute **a partir da raiz do projeto**, porque as imagens são lidas por caminho relativo (`texturas/...`):

```bash
./cena          # Linux
cena.exe        # Windows
```

### Pasta `texturas/` (BMP 24 bits, sem compressão)

| Arquivo | Onde é usada |
|---|---|
| `van_gogh.bmp` | quadro grande da parede do fundo e um dos quadros da parede esquerda |
| `fernando_lopes.bmp` | quadro da parede esquerda |
| `death_note.bmp` | capa do livro dentro da gaveta |
| `simpsons.bmp` | tela da TV |
| `adventure_time.bmp` | tela do monitor |
| `algodao.bmp` | travesseiro |
| `blueT.bmp` | colchão |
| `teclado.bmp` | teclado |
| `granito.bmp` | mesa e criado-mudo |
| `redCedar.bmp` | madeira dos móveis, da porta, do armário, da estante e das molduras |
| `cortina.bmp` | cortinas |
| `janela.bmp` | vidro da janela |
| `leather.bmp` | estofado da cadeira |
| `moldura.bmp` | rack da TV e moldura da janela |
| `steel.bmp` | caneca e hastes da cadeira |

Se um arquivo não for encontrado, o programa continua e escreve o aviso no console. Para algumas
texturas (quadros, TV, monitor, madeira) há uma textura procedural de reserva; nas demais, o objeto
aparece sem textura. O piso, as paredes, o tapete, a colcha e os livros da estante usam texturas
geradas por código e não dependem da pasta.

---

## Comandos

| Tecla / ação | O que faz |
|---|---|
| **W / S** | andar para frente / para trás |
| **A / D** | andar para a esquerda / para a direita |
| **Mouse** (segurar o botão esquerdo e arrastar) | olhar ao redor |
| **E** | interagir com o objeto mais próximo: gaveta, porta, TV ou monitor |
| **F** | perto do criado-mudo: liga/desliga o abajur; longe dele: liga/desliga a luz do teto |
| **C** | abrir/fechar a cortina (perto da janela) |
| **R** | girar a cadeira 45° (perto da cadeira) |
| **H** | mostrar/ocultar a ajuda na tela |
| **ESC** | sair |

As interações dependem da proximidade (distância no plano do chão, em unidades da cena):

| Interação | Alcance |
|---|---|
| Gaveta do criado-mudo | até 3,0 |
| Abajur (tecla F) | até 3,0 |
| Porta | até 4,0 |
| TV | até 4,0 |
| Monitor | até 3,5 |
| Cortina | até 4,0 |
| Cadeira | até 2,5 |

A câmera não atravessa paredes nem móveis (colisão com caixas em planta). Porta, gaveta, cortina e
cadeira se movem de forma suave (animação por interpolação), e a luz do teto também projeta sombras no chão.

---

## Requisitos do trabalho e onde estão no código

| Requisito | Onde |
|---|---|
| Pelo menos 8 objetos modelados pela equipe | `src/objetos/` — 15 arquivos (mesa, monitor, teclado/mouse/caneca, cadeira, cama, criado-mudo com gaveta e abajur, estante com livros, janela e cortinas, quadros, porta, rack e TV, armário, lâmpada, relógio, sala e tapete) |
| Translação, rotação e escala | em todos os objetos; hierarquias na porta (dobradiça), gaveta, abajur e cadeira |
| Navegação com W/A/S/D e mouse | `src/entrada.cpp` |
| Pelo menos 3 interações | porta, gaveta, TV, monitor, cortina, luz do teto, abajur e cadeira (`src/entrada.cpp`) |
| Iluminação com materiais distintos | `src/iluminacao.cpp` (3 luzes) e `src/materiais.cpp` (8 materiais) |
| Mapeamento de textura | `src/texturas.cpp` e `src/bmp_loader.cpp` |
| Sem motores e sem modelos importados | tudo é modelado com primitivas do GLUT/GLU e geometria própria; as imagens são usadas apenas como textura |

## Estrutura do projeto

```
main.cpp                 janela, display, init e laço principal
Makefile, CMakeLists.txt
src/
  comum.h                includes compartilhados
  estado.*               variáveis globais (câmera, estados das interações, alvos da animação)
  materiais.*            material() e presets (madeira, metal, plástico, cerâmica, vidro, tecido, parede, couro)
  texturas.*             texturas procedurais e mapeamentos (planar, cúbico, cilíndrico, esférico)
  bmp_loader.*           carregador de BMP 24 bits
  iluminacao.*           LIGHT0 (teto), LIGHT1 (abajur, spot), LIGHT2 (TV)
  sombras.*              sombra projetada planar com stencil
  colisao.*              caixas AABB e movimento com deslize nas paredes
  animacao.*             interpolação suave e timer
  hud.*                  ajuda de comandos na tela
  entrada.*              callbacks de mouse e teclado
  cena.h                 declaração das funções desenhaXxx()
  objetos/               um arquivo por objeto
```

Para adicionar um objeto: crie `src/objetos/novo.cpp` (com `#include "comum.h"`), declare a função em
`src/cena.h` e chame-a no `display()` de `main.cpp`. O Makefile e o CMake incluem o arquivo novo sozinhos.

## Relatório

As técnicas implementadas estão descritas no relatório que acompanha este repositório.