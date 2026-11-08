# Escritório 3D — OpenGL

Cena 3D interativa de um quarto/escritório, feita em **C++ com OpenGL (pipeline fixo) e GLUT**.
O usuário caminha pela cena com teclado e mouse e interage com objetos (porta, gaveta, TV, monitor,
armário, rack, cortina, luzes e cadeira). Não usa motor gráfico nem modelos importados: todos os objetos
são modelados no código.

**Equipe:** Felipe Lima Aragão · Lucas Trindade de Souza
**Disciplina:** Computação Gráfica (COMP 381) — UFAL

---

## Como compilar

### Dependências

| Sistema | O que instalar |
|---|---|
| Linux (Debian/Ubuntu) | `sudo apt install g++ make freeglut3-dev` |
| Windows | MinGW-w64 com freeglut. Exemplo com MSYS2 (terminal *MINGW64*): `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut` |

O código usa C++11 (há lambdas em `quadros.cpp` e `texturas.cpp`).

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

Execute **a partir da raiz do projeto**, porque as imagens são lidas por caminho relativo (`texturas/...`).
Com o CMake o executável fica em `build/`; copie-o para a raiz ou rode `./build/cena` a partir da raiz.

```bash
./cena          # Linux
cena.exe        # Windows
```

---

## Comandos

| Tecla / ação | O que faz |
|---|---|
| **W / S** | andar para frente / para trás |
| **A / D** | andar para a esquerda / para a direita |
| **Mouse** (segurar o botão esquerdo e arrastar) | olhar ao redor |
| **E** | interagir com o objeto mais próximo: gaveta, porta, TV, monitor ou armário (abre/fecha as duas portas) |
| **1 / 2** | perto do armário: abre/fecha a porta individual (1 e 2) |
| **1 / 2 / 3** | perto do rack da TV: abre/fecha cada uma das três portas basculantes |
| **F** | perto do criado-mudo: liga/desliga o abajur; longe dele: liga/desliga a luz do teto |
| **C** | abrir/fechar a cortina (perto da janela) |
| **R** | girar a cadeira 45° (perto da cadeira) |
| **H** | mostrar/ocultar a ajuda na tela |
| **ESC** | sair |

As interações dependem da proximidade (distância no plano do chão, em unidades da cena). Quando mais de um
objeto da tecla **E** está ao alcance, vale o mais próximo.

| Interação | Tecla | Alcance |
|---|---|---|
| Gaveta do criado-mudo | E | 3,0 |
| Porta | E | 4,0 |
| TV | E | 4,0 |
| Monitor | E | 3,5 |
| Armário (as duas portas) | E | 4,0 |
| Armário (porta individual) | 1 / 2 | 4,0 |
| Rack da TV (3 portas) | 1 / 2 / 3 | 4,5 |
| Abajur | F | 3,0 |
| Cortina | C | 4,0 |
| Cadeira | R | 2,5 |

A câmera não atravessa paredes nem móveis (colisão com caixas em planta; a cadeira não tem colisão).
Porta, gaveta, cortina, cadeira, portas do armário e portas do rack se movem de forma suave (animação por
interpolação). Relógio, rolagem da imagem da TV e o LED do modem são animados pelo tempo de execução.
A luz do teto também projeta sombras no chão.

---

## Pasta `texturas/` (BMP 24 bits, sem compressão)

O programa carrega 17 imagens:

| Arquivo | Onde é usada |
|---|---|
| `van_gogh.bmp` | quadro grande da parede do fundo |
| `fernando_lopes.bmp` | quadro 1 da parede esquerda |
| `abaporu.bmp` | quadro 2 da parede esquerda |
| `death_note.bmp` | capa do livro dentro da gaveta |
| `simpsons.bmp` | tela da TV |
| `desktop.bmp` | tela do monitor |
| `algodao.bmp` | travesseiro e roupas do armário |
| `blueT.bmp` | colchão |
| `teclado.bmp` | teclado |
| `granito.bmp` | tampo da mesa e gaveta do criado-mudo |
| `redCedar.bmp` | madeira dos móveis, porta, armário, estante, molduras dos quadros e base da cama |
| `oak.bmp` | pés, base e detalhe do rack da TV |
| `moldura.bmp` | corpo do rack da TV e moldura da janela |
| `cortina.bmp` | cortinas |
| `janela.bmp` | vidro da janela |
| `leather.bmp` | estofado da cadeira e casaco do armário |
| `steel.bmp` | caneca e hastes da cadeira |

`adventure_time.bmp` e `couro.bmp` estão na pasta, mas não são usadas pelo código.

Se um arquivo não for encontrado, o programa continua e escreve o aviso no console. Quase todas as texturas têm
uma textura procedural de reserva; só a capa do livro, o teclado e o vidro da janela ficam sem textura.
O piso, as paredes, o tapete, a colcha, os livros da estante e as listras/xadrez das roupas usam texturas geradas
por código e não dependem da pasta.

---

## Requisitos do trabalho e onde estão no código

| Requisito | Onde |
|---|---|
| Pelo menos 8 objetos modelados pela equipe | `src/objetos/` — 15 arquivos (sala e tapete, mesa, monitor, teclado/mouse/caneca, cadeira, cama, criado-mudo com gaveta e abajur, estante com livros, janela e cortinas, quadros, porta, rack e TV, armário, lâmpada, relógio) |
| Translação, rotação e escala | em todos os objetos; hierarquias na porta (dobradiça), gaveta, abajur, cadeira, armário e rack; escala animada nas cortinas |
| Navegação com W/A/S/D e mouse | `src/entrada.cpp` |
| Pelo menos 3 interações | porta, gaveta, TV, monitor, armário, rack, cortina, luz do teto, abajur e cadeira (`src/entrada.cpp`) |
| Iluminação com materiais distintos | `src/iluminacao.cpp` (3 luzes) e `src/materiais.cpp` (8 materiais) |
| Mapeamento de textura | `src/texturas.cpp` (planar, cúbico, cilíndrico, esférico) e `src/bmp_loader.cpp` |
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
texturas/                imagens BMP
```

Para adicionar um objeto: crie `src/objetos/novo.cpp` (com `#include "comum.h"`), declare a função em
`src/cena.h` e chame-a no `display()` de `main.cpp`. O Makefile e o CMake incluem o arquivo novo sozinhos.

## Relatório e vídeo

As técnicas implementadas estão descritas no relatório que acompanha este repositório (`Relatório - CG.pdf`).

Vídeo: https://drive.google.com/file/d/1dCo3XKSdZ8ce9PQSNzOiL6s3cAG6rR3t/view?usp=sharing
