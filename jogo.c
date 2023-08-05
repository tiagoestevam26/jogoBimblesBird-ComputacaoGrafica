#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define START_BUTTON_Y 150
#define INSTRUCTIONS_BUTTON_Y 80
#define BACK_BUTTON_Y 50
#define MENU 0
#define INSTRUCOES 1
#define JOGANDO 2
#define GAMEOVER 3
#define VITORIA 4
#define LOADING 5
#define ZEROU 6

GLuint idTexturaMontanha;
GLuint idTexturaAgua;
GLuint idTexturaNuvens;
GLuint idTexturaLava;
GLuint idTexturaArena;
GLuint idTexturaBarcos;
GLuint idTexturaRua;
GLuint idTexturaCidade;
GLuint idTexturaCeu;
GLuint idTexturaCarro;
GLuint idTexturaInimigo;
GLuint idTexturaCoracaoCheio;
GLuint idTexturaCoracaoVazio;
GLuint idTexturaHero[5];
GLuint idTexturaVilao[8];
GLuint idTexturaLogo;
GLuint idTexturaButton;
GLuint idTexturaInstrucoes;
GLuint idTexturaGameover;
GLuint idTexturaLoading;
GLuint idTexturaNext;
GLuint idTexturaZerou;
GLuint idTexturaSpikes;

int faseAtual = 0;
int danoAux = 0;
int auxiliar = 0;
int j = -3, l = -3;
float mousex, mousey;
int spriteAtualHero = 0;
int spriteAtualVilao = 0;
int movimentoX = 0;
int vidas = 3;
double movimentoY = 0;
bool viradoEsquerda = true;
int currentMouseX = 0, currentMouseY = 0;
int windowWidth = 768, windowHeight = 500;
int buttonX = 0;
int estado = MENU;
int mostrarInstrucoes = 0;
int contadorQuadrosHero = 0;
int contadorQuadrosVilao = 0;

struct Coordenadas
{
    double x;
    double y;
};

struct Inimigos
{
    struct Coordenadas pontoInicial;
    struct Coordenadas pontoFinal;
    struct Coordenadas direcao;
};

struct Rectangle
{
    struct Coordenadas min; // canto inferior esquerdo
    struct Coordenadas max; // canto superior direito
};

struct Coordenadas montanha, agua, nuvens, lava, arena, barco1, barco2, barco3, ceu, cidade, rua, carro, inimigo, inimigo2, direcao, pontoInicialAux, pontoFinalAux, jogador, vilao;
struct Inimigos inimigosFase1[90];
struct Inimigos inimigosFase1b[36];
struct Inimigos inimigosFase2[90];
struct Inimigos inimigosFase2b[36];
struct Inimigos inimigosFase2c[36];
struct Inimigos inimigosFase2d[90];
struct Inimigos inimigosFase3[90];
struct Inimigos inimigosFase3b[36];
struct Inimigos inimigosFase3c[90];
struct Inimigos inimigosFase3d[36];

void dano()
{
    vidas--;
}

struct Coordenadas gap(struct Coordenadas posicaoVilao, struct Coordenadas posicaoHeroi, int repulsao)
{
    struct Coordenadas gapTotal;

    gapTotal.x = 0;
    gapTotal.y = 0;

    if (posicaoVilao.x > posicaoHeroi.x && posicaoHeroi.x - repulsao >= 0)
    {
        gapTotal.x -= repulsao;
    }
    else if (posicaoVilao.x > posicaoHeroi.x && posicaoHeroi.x - repulsao < 0)
    {
        if (posicaoHeroi.y + repulsao <= 500)
        {
            gapTotal.y += repulsao;
        }
        else
        {
            gapTotal.y -= repulsao;
        }
    }
    else if (posicaoVilao.x < posicaoHeroi.x && posicaoHeroi.x + repulsao <= 768)
    {
        gapTotal.x += repulsao;
    }
    else if (posicaoVilao.x < posicaoHeroi.x && posicaoHeroi.x + repulsao > 768)
    {
        if (posicaoHeroi.y + repulsao <= 500)
        {
            gapTotal.y += repulsao;
        }
        else
        {
            gapTotal.y -= repulsao;
        }
    }
    else if (posicaoVilao.y < posicaoHeroi.y && posicaoHeroi.y + repulsao <= 500)
    {
        gapTotal.y += repulsao;
    }
    else if (posicaoVilao.x < posicaoHeroi.x && posicaoHeroi.y + repulsao > 500)
    {
        if (posicaoHeroi.x + repulsao <= 760)
        {
            gapTotal.x += repulsao;
        }
        else
        {
            gapTotal.x -= repulsao;
        }
    }
    else if (posicaoVilao.y > posicaoHeroi.y && posicaoHeroi.y - repulsao >= 0)
    {
        gapTotal.y -= repulsao;
    }
    else if (posicaoVilao.x > posicaoHeroi.x && posicaoHeroi.y - repulsao < 0)
    {
        if (posicaoHeroi.x + repulsao <= 760)
        {
            gapTotal.x += repulsao;
        }
        else
        {
            gapTotal.x -= repulsao;
        }
    }

    return gapTotal;
}

void atualizaVilao()
{
    double dx = jogador.x - vilao.x;
    double dy = jogador.y - vilao.y;
    double dist = sqrt(dx * dx + dy * dy);

    if (dist != 0)
    {
        double velocidade = 1.8; // Velocidade do vilão

        vilao.x += dx / dist * velocidade;
        vilao.y += dy / dist * velocidade;
    }
}

double randomX(int aux)
{
    double result;

    if (aux == 1)
    {
        double range = 0.0 - 100.0;
        result = ((double)rand() / RAND_MAX) * range + (-40);
    }
    else if (aux == 2)
    {
        double range = 810.0 - 768.0;
        result = ((double)rand() / RAND_MAX) * range + 768.0;
    }
    else
    {
        double range = 768.0;
        result = ((double)rand() / RAND_MAX) * range;
    }

    return result;
}

double randomY(int aux)
{
    double result;

    if (aux == 1)
    {
        double range = 0.0 - 100;
        result = ((double)rand() / RAND_MAX) * range + (-40);
    }
    else if (aux == 2)
    {
        double range = 600 - 500.0;
        result = ((double)rand() / RAND_MAX) * range + 500.0;
    }
    else
    {
        double range = 600;
        result = ((double)rand() / RAND_MAX) * range;
    }

    return result;
}

struct Coordenadas calculaDirecaoFase1()
{
    double length;

    int sentido = rand() % 2 + 1;

    pontoInicialAux.x = randomX(sentido);
    pontoFinalAux.x = randomX((sentido == 1) ? 2 : 1);
    pontoInicialAux.y = randomY(3);
    pontoFinalAux.y = randomY(3);

    direcao.x = pontoFinalAux.x - pontoInicialAux.x;
    direcao.y = pontoFinalAux.y - pontoInicialAux.y;

    // Normalize
    length = sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
    direcao.x /= length;
    direcao.y /= length;

    return direcao;
}

struct Coordenadas calculaDirecaoFase2()
{
    double length;

    int sentido = rand() % 2 + 1;

    pontoInicialAux.x = randomX(3);
    pontoFinalAux.x = randomX(3);
    pontoInicialAux.y = randomY(sentido);
    pontoFinalAux.y = randomY((sentido == 1) ? 2 : 1);

    direcao.x = pontoFinalAux.x - pontoInicialAux.x;
    direcao.y = pontoFinalAux.y - pontoInicialAux.y;

    // Normalize
    length = sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
    direcao.x /= length;
    direcao.y /= length;

    return direcao;
}

struct Coordenadas calculaDirecaoFase3()
{
    double length;

    int sentido = rand() % 2 + 1;

    pontoInicialAux.x = randomX(3);
    pontoFinalAux.x = 384; // randomX(3);
    pontoInicialAux.y = randomY(sentido);
    pontoFinalAux.y = 250; // randomY((sentido == 1) ? 2 : 1);

    direcao.x = pontoFinalAux.x - pontoInicialAux.x;
    direcao.y = pontoFinalAux.y - pontoInicialAux.y;

    // Normalize
    length = sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
    direcao.x /= length;
    direcao.y /= length;

    return direcao;
}

struct Coordenadas calculaDirecaoFase3b()
{
    double length;

    int sentido = rand() % 2 + 1;

    pontoInicialAux.x = randomX(sentido);
    pontoFinalAux.x = 384; // randomX(3);
    pontoInicialAux.y = randomY(3);
    pontoFinalAux.y = 250; // randomY((sentido == 1) ? 2 : 1);

    direcao.x = pontoFinalAux.x - pontoInicialAux.x;
    direcao.y = pontoFinalAux.y - pontoInicialAux.y;

    // Normalize
    length = sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
    direcao.x /= length;
    direcao.y /= length;

    return direcao;
}

void geraInimigos()
{

    for (int i = 0; i < 90; i++)
    {
        direcao = calculaDirecaoFase1();
        inimigosFase1[i].direcao = direcao;
        inimigosFase1[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase1[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase1[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase1[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 36; i++)
    {
        direcao = calculaDirecaoFase1();
        inimigosFase1b[i].direcao = direcao;
        inimigosFase1b[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase1b[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase1b[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase1b[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 90; i++)
    {
        direcao = calculaDirecaoFase2();
        inimigosFase2[i].direcao = direcao;
        inimigosFase2[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase2[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase2[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase2[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 36; i++)
    {
        direcao = calculaDirecaoFase1();
        inimigosFase2b[i].direcao = direcao;
        inimigosFase2b[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase2b[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase2b[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase2b[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 90; i++)
    {
        direcao = calculaDirecaoFase2();
        inimigosFase2d[i].direcao = direcao;
        inimigosFase2d[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase2d[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase2d[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase2d[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 36; i++)
    {
        direcao = calculaDirecaoFase1();
        inimigosFase2c[i].direcao = direcao;
        inimigosFase2c[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase2c[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase2c[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase2c[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 90; i++)
    {
        direcao = calculaDirecaoFase3();
        inimigosFase3[i].direcao = direcao;
        inimigosFase3[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase3[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase3[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase3[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 36; i++)
    {
        direcao = calculaDirecaoFase3b();
        inimigosFase3b[i].direcao = direcao;
        inimigosFase3b[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase3b[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase3b[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase3b[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 90; i++)
    {
        direcao = calculaDirecaoFase3();
        inimigosFase3c[i].direcao = direcao;
        inimigosFase3c[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase3c[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase3c[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase3c[i].pontoFinal.y = pontoFinalAux.y;
    }
    for (int i = 0; i < 36; i++)
    {
        direcao = calculaDirecaoFase3b();
        inimigosFase3d[i].direcao = direcao;
        inimigosFase3d[i].pontoInicial.x = pontoInicialAux.x;
        inimigosFase3d[i].pontoFinal.x = pontoFinalAux.x;
        inimigosFase3d[i].pontoInicial.y = pontoInicialAux.y;
        inimigosFase3d[i].pontoFinal.y = pontoFinalAux.y;
    }
}

void atualizaJogador()
{
    double velocidade = 5.0;

    jogador.x += movimentoX * velocidade;
    jogador.y += movimentoY * velocidade;

    double minX = 0.0;
    double maxX = 700.0;
    double minY = 0.0;
    double maxY = 450.0;

    if (jogador.x < minX)
        jogador.x = minX;
    else if (jogador.x > maxX)
        jogador.x = maxX;

    if (faseAtual == 0)
    {
        minY = -60.0;
        if (jogador.y < minY)
        {
            estado = GAMEOVER;
            j = -3;
            l = -3;
            geraInimigos();
            faseAtual = 0;
            vidas = 3;
            auxiliar = 0;
            jogador.y = maxY / 2;
        }
        else if (jogador.y > maxY)
        {
            jogador.y = maxY;
        }
    }

    if (jogador.y < minY)
    {
        estado = GAMEOVER;
        j = -3;
        l = -3;
        geraInimigos();
        faseAtual = 0;
        auxiliar = 0;
        vidas = 3;
        jogador.y = maxY;
    }
    else if (jogador.y > maxY)
    {
        jogador.y = maxY;
    }
}

void confereArestas()
{

    struct Coordenadas gapTotal;
    struct Rectangle A;
    struct Rectangle B;
    struct Rectangle C[90];
    struct Rectangle D[36];
    struct Rectangle E[90];
    struct Rectangle F[36];
    struct Rectangle G[90];
    struct Rectangle H[36];
    struct Rectangle I[90];
    struct Rectangle J[36];
    struct Rectangle K[36];
    struct Rectangle L[90];

    A.min = jogador;
    A.max.x = A.min.x + 40;
    A.max.y = A.min.y + 40;

    B.min = vilao;
    B.max.x = B.min.x + 40;
    B.max.y = B.min.y + 40;

    for (int i = 0; i < 90; i++)
    {
        C[i].min = inimigosFase3[i].pontoInicial;
        C[i].max.x = inimigosFase3[i].pontoInicial.x + 20;
        C[i].max.y = inimigosFase3[i].pontoInicial.y + 20;

        E[i].min = inimigosFase3c[i].pontoInicial;
        E[i].max.x = inimigosFase3c[i].pontoInicial.x + 20;
        E[i].max.y = inimigosFase3c[i].pontoInicial.y + 20;

        G[i].min = inimigosFase1[i].pontoInicial;
        G[i].max.x = inimigosFase1[i].pontoInicial.x + 20;
        G[i].max.y = inimigosFase1[i].pontoInicial.y + 20;

        I[i].min = inimigosFase2[i].pontoInicial;
        I[i].max.x = inimigosFase2[i].pontoInicial.x + 20;
        I[i].max.y = inimigosFase2[i].pontoInicial.y + 20;

        L[i].min = inimigosFase2d[i].pontoInicial;
        L[i].max.x = inimigosFase2d[i].pontoInicial.x + 20;
        L[i].max.y = inimigosFase2d[i].pontoInicial.y + 20;
    }

    for (int i = 0; i < 36; i++)
    {
        D[i].min = inimigosFase3b[i].pontoInicial;
        D[i].max.x = inimigosFase3b[i].pontoInicial.x + 20;
        D[i].max.y = inimigosFase3b[i].pontoInicial.y + 20;

        F[i].min = inimigosFase3d[i].pontoInicial;
        F[i].max.x = inimigosFase3d[i].pontoInicial.x + 20;
        F[i].max.y = inimigosFase3d[i].pontoInicial.y + 20;

        H[i].min = inimigosFase1b[i].pontoInicial;
        H[i].max.x = inimigosFase1b[i].pontoInicial.x + 20;
        H[i].max.y = inimigosFase1b[i].pontoInicial.y + 20;

        J[i].min = inimigosFase2b[i].pontoInicial;
        J[i].max.x = inimigosFase2b[i].pontoInicial.x + 20;
        J[i].max.y = inimigosFase2b[i].pontoInicial.y + 20;

        K[i].min = inimigosFase2c[i].pontoInicial;
        K[i].max.x = inimigosFase2c[i].pontoInicial.x + 20;
        K[i].max.y = inimigosFase2c[i].pontoInicial.y + 20;
    }

    if (faseAtual == 0)
    {
        // INIMIGOS FASE 1
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, G[i].min, 100);
            if (!(A.max.x < G[i].min.x || A.min.x > G[i].max.x || A.max.y < G[i].min.y || A.min.y > G[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
        // INIMIGOS FASE 1b
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, H[i].min, 100);
            if (!(A.max.x < H[i].min.x || A.min.x > H[i].max.x || A.max.y < H[i].min.y || A.min.y > H[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
    }
    if (faseAtual == 1)
    {
        // INIMIGOS FASE 2
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, I[i].min, 100);
            if (!(A.max.x < I[i].min.x || A.min.x > I[i].max.x || A.max.y < I[i].min.y || A.min.y > I[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
        // INIMIGOS FASE 2d
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, L[i].min, 100);
            if (!(A.max.x < L[i].min.x || A.min.x > L[i].max.x || A.max.y < L[i].min.y || A.min.y > L[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
        // INIMIGOS FASE 2b
        for (int i = 0; i < 36; i++)
        {
            gapTotal = gap(jogador, J[i].min, 100);
            if (!(A.max.x < J[i].min.x || A.min.x > J[i].max.x || A.max.y < J[i].min.y || A.min.y > J[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
        // INIMIGOS FASE 2c
        for (int i = 0; i < 36; i++)
        {
            gapTotal = gap(jogador, K[i].min, 100);
            if (!(A.max.x < K[i].min.x || A.min.x > K[i].max.x || A.max.y < K[i].min.y || A.min.y > K[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
    }
    if (faseAtual == 2)
    {
        // INIMIGOS FASE 3
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, C[i].min, 100);
            if (!(A.max.x < C[i].min.x || A.min.x > C[i].max.x || A.max.y < C[i].min.y || A.min.y > C[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }

        // INIMIGOS FASE 3b
        for (int i = 0; i < 36; i++)
        {
            gapTotal = gap(jogador, D[i].min, 100);
            if (!(A.max.x < D[i].min.x || A.min.x > D[i].max.x || A.max.y < D[i].min.y || A.min.y > D[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }

        // INIMIGOS FASE 3c
        for (int i = 0; i < 90; i++)
        {
            gapTotal = gap(jogador, E[i].min, 100);
            if (!(A.max.x < E[i].min.x || A.min.x > E[i].max.x || A.max.y < E[i].min.y || A.min.y > E[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }

        // INIMIGOS FASE 3d
        for (int i = 0; i < 36; i++)
        {
            gapTotal = gap(jogador, F[i].min, 100);
            if (!(A.max.x < F[i].min.x || A.min.x > F[i].max.x || A.max.y < F[i].min.y || A.min.y > F[i].max.y))
            {
                if (danoAux == 0)
                {
                    dano();
                    danoAux = 1;
                    jogador.x += gapTotal.x;
                    jogador.y += gapTotal.y;
                }
            }
            else
            {
                danoAux = 0;
            }
        }
        if (!(A.max.x < B.min.x || A.min.x > B.max.x || A.max.y < B.min.y || A.min.y > B.max.y))
        {
            if (danoAux == 0)
            {
                dano();
                danoAux = 1;
                if (vilao.x > jogador.x && jogador.x - 50 >= 0)
                {
                    jogador.x -= 50;
                }
                else if (vilao.x < jogador.x && jogador.x + 50 <= 768)
                {
                    jogador.x += 50;
                }
                else if (vilao.x > jogador.x && jogador.x - 50 < 0)
                {
                    jogador.y += 50;
                }
                else if (vilao.x < jogador.x && jogador.x + 50 > 768)
                {
                    jogador.y += 50;
                }
            }
        }
        else
        {
            danoAux = 0;
        }
    }

    if (vidas <= 0)
    {
        estado = GAMEOVER;
        j = -3;
        l = -3;
        geraInimigos();
        faseAtual = 0;
        vidas = 3;
        auxiliar = 0;
    }
}

void mudaJ()
{

    if (estado == JOGANDO)
    {
        j++;
        // printf("ta mudandooooo  %lf",  jogador.y);
    }
    if (j >= 8)
    {

        if (faseAtual < 2)
        {
            j = -3;
            l = -3;
            estado = VITORIA;
            geraInimigos();
            faseAtual++;
            vidas = 3;
            jogador.x = 384;
            jogador.y = 250;
            auxiliar = 0;
        }
        else if (faseAtual == 2)
        {
            j = -3;
            l = -3;
            estado = ZEROU;
            geraInimigos();
            faseAtual = 0;
            vidas = 3;
            jogador.x = 384;
            jogador.y = 250;
            auxiliar = 0;
        }
    }

    if (estado == JOGANDO)
    {
        glutTimerFunc(4000, mudaJ, 0);
    }
}

void mudaL()
{
    if (estado == JOGANDO)
    {
        l++;
    }

    if (estado == JOGANDO)
    {
        glutTimerFunc(6000, mudaL, 0);
    }
}

void mudaEstado(int est)
{
    estado = est;
    glutTimerFunc(0, mudaJ, 0);
    glutTimerFunc(0, mudaL, 0);
}

void mudaValorInimigo()
{

    if (estado == JOGANDO)
    {
        inimigosFase1[j].pontoInicial.x = inimigosFase1[j].pontoInicial.x + ((inimigosFase1[j].direcao.x * 8));
        inimigosFase1[j].pontoInicial.y = inimigosFase1[j].pontoInicial.y + ((inimigosFase1[j].direcao.y * 8));
        inimigosFase1b[l].pontoInicial.x = inimigosFase1b[l].pontoInicial.x + ((inimigosFase1b[l].direcao.x * 8));
        inimigosFase1b[l].pontoInicial.y = inimigosFase1b[l].pontoInicial.y + ((inimigosFase1b[l].direcao.y * 8));
        inimigosFase2[j].pontoInicial.x = inimigosFase2[j].pontoInicial.x + ((inimigosFase2[j].direcao.x * 10));
        inimigosFase2[j].pontoInicial.y = inimigosFase2[j].pontoInicial.y + ((inimigosFase2[j].direcao.y * 10));
        inimigosFase2b[l].pontoInicial.x = inimigosFase2b[l].pontoInicial.x + ((inimigosFase2b[l].direcao.x * 8));
        inimigosFase2b[l].pontoInicial.y = inimigosFase2b[l].pontoInicial.y + ((inimigosFase2b[l].direcao.y * 8));
        inimigosFase2c[l].pontoInicial.x = inimigosFase2c[l].pontoInicial.x + ((inimigosFase2c[l].direcao.x * 8));
        inimigosFase2c[l].pontoInicial.y = inimigosFase2c[l].pontoInicial.y + ((inimigosFase2c[l].direcao.y * 8));
        inimigosFase2d[l].pontoInicial.x = inimigosFase2d[l].pontoInicial.x + ((inimigosFase2d[l].direcao.x * 8));
        inimigosFase2d[l].pontoInicial.y = inimigosFase2d[l].pontoInicial.y + ((inimigosFase2d[l].direcao.y * 8));
        inimigosFase3[j].pontoInicial.x = inimigosFase3[j].pontoInicial.x + ((inimigosFase3[j].direcao.x * 5));
        inimigosFase3[j].pontoInicial.y = inimigosFase3[j].pontoInicial.y + ((inimigosFase3[j].direcao.y * 5));
        inimigosFase3b[l].pontoInicial.x = inimigosFase3b[l].pontoInicial.x + ((inimigosFase3b[l].direcao.x * 5));
        inimigosFase3b[l].pontoInicial.y = inimigosFase3b[l].pontoInicial.y + ((inimigosFase3b[l].direcao.y * 5));
        inimigosFase3c[j].pontoInicial.x = inimigosFase3c[j].pontoInicial.x + ((inimigosFase3c[j].direcao.x * 5));
        inimigosFase3c[j].pontoInicial.y = inimigosFase3c[j].pontoInicial.y + ((inimigosFase3c[j].direcao.y * 5));
        inimigosFase3d[l].pontoInicial.x = inimigosFase3d[l].pontoInicial.x + ((inimigosFase3d[l].direcao.x * 5));
        inimigosFase3d[l].pontoInicial.y = inimigosFase3d[l].pontoInicial.y + ((inimigosFase3d[l].direcao.y * 5));
        confereArestas();
    }
}

void mudaValorLava()
{
    if (lava.x > -3072)
    {
        lava.x = lava.x - 1;
    }
    else
    {
        lava.x = 0;
    }
    glutPostRedisplay();
}

void mudaValorCeu()
{
    if (ceu.x > -3072)
    {
        ceu.x = ceu.x - 1;
    }
    else
    {
        ceu.x = 0;
    }
    glutPostRedisplay();
}

void mudaValor()
{

    if (agua.x > -3072)
    {
        agua.x = agua.x - 3;
    }
    else
    {
        agua.x = 0;
    }

    if (nuvens.x > -3072)
    {
        nuvens.x = nuvens.x - 1;
    }
    else
    {
        nuvens.x = 0;
    }

    if (montanha.x > -3072)
    {
        montanha.x = montanha.x - 2;
    }
    else
    {
        montanha.x = 0;
    }

    if (lava.x > -3072)
    {
        lava.x = lava.x - 1;
    }
    else
    {
        lava.x = 0;
    }
    if (barco1.x > -200)
    {
        barco1.x = barco1.x - 1.5;
    }
    else
    {
        barco1.x = 768;
    }
    if (barco2.x > -200)
    {
        barco2.x = barco2.x - 2;
    }
    else
    {
        barco2.x = 768;
    }
    if (barco3.x < 768)
    {
        barco3.x = barco3.x + 1;
    }
    else
    {
        barco3.x = -200;
    }
    if (ceu.x > -3072)
    {
        ceu.x = ceu.x - 1;
    }
    else
    {
        ceu.x = 0;
    }
    if (cidade.x > -3072)
    {
        cidade.x = cidade.x - 2;
    }
    else
    {
        cidade.x = 0;
    }
    if (rua.x > -3072)
    {
        rua.x = rua.x - 7;
    }
    else
    {
        rua.x = 0;
    }
    if (carro.x < 918)
    {
        carro.x = carro.x + 2;
    }
    else
    {
        carro.x = -350;
    }

    contadorQuadrosHero++;
    contadorQuadrosVilao++;

    // Só muda a sprite do herói a cada 10 quadros
    if (contadorQuadrosHero >= 3)
    {
        spriteAtualHero = (spriteAtualHero + 1) % 5;
        contadorQuadrosHero = 0; // Reinicia o contador
    }

    // Só muda a sprite do vilão a cada 15 quadros
    if (contadorQuadrosVilao >= 8)
    {
        spriteAtualVilao = (spriteAtualVilao + 1) % 8;
        contadorQuadrosVilao = 0; // Reinicia o contador
    }

    atualizaJogador();

    if (faseAtual == 2 && estado == JOGANDO)
    {
        atualizaVilao();
    }

    confereArestas();

    glutPostRedisplay();
}

void desenhaLogo(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 400, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 400, y + 200);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 200);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaSpikes(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 784, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 784, y + 500);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 500);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaCenario(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 3840, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 3840, y + 500);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 500);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaCenarioArena(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 500, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 500, y + 500);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 500);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaCenarioBarco(GLuint idTextura, int x, int y, double tex1, double tex2, double tex1b)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(tex1, 0);
    glVertex2f(x, y);
    glTexCoord2f(tex1b, 0);
    glVertex2f(x + 150, y);
    glTexCoord2f(tex1b, tex2);
    glVertex2f(x + 150, y + 250);
    glTexCoord2f(tex1, tex2);
    glVertex2f(x, y + 250);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaCenarioCarro(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 150, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 150, y + 150);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 150);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaPersonagem()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaHero[spriteAtualHero]);
    glBegin(GL_POLYGON);
    if (viradoEsquerda)
    {
        glTexCoord2f(0.87, 0.12);
        glVertex2f(jogador.x, jogador.y);
        glTexCoord2f(0.12, 0.12);
        glVertex2f(jogador.x + 80, jogador.y);
        glTexCoord2f(0.12, 0.87);
        glVertex2f(jogador.x + 80, jogador.y + 80);
        glTexCoord2f(0.87, 0.87);
        glVertex2f(jogador.x, jogador.y + 80);
    }
    else
    {
        glTexCoord2f(0.12, 0.12);
        glVertex2f(jogador.x, jogador.y);
        glTexCoord2f(0.87, 0.12);
        glVertex2f(jogador.x + 80, jogador.y);
        glTexCoord2f(0.87, 0.87);
        glVertex2f(jogador.x + 80, jogador.y + 80);
        glTexCoord2f(0.12, 0.87);
        glVertex2f(jogador.x, jogador.y + 80);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaVilao()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaVilao[spriteAtualVilao]);
    if (vilao.x < jogador.x)
    {
        glBegin(GL_POLYGON);
        glTexCoord2f(0.80, 0.20);
        glVertex2f(vilao.x, vilao.y);
        glTexCoord2f(0.20, 0.25);
        glVertex2f(vilao.x + 100, vilao.y);
        glTexCoord2f(0.20, 0.80);
        glVertex2f(vilao.x + 100, vilao.y + 100);
        glTexCoord2f(0.80, 0.80);
        glVertex2f(vilao.x, vilao.y + 100);
        glEnd();
    }
    else
    {
        glBegin(GL_POLYGON);
        glTexCoord2f(0.20, 0.25);
        glVertex2f(vilao.x, vilao.y);
        glTexCoord2f(0.80, 0.20);
        glVertex2f(vilao.x + 100, vilao.y);
        glTexCoord2f(0.80, 0.80);
        glVertex2f(vilao.x + 100, vilao.y + 100);
        glTexCoord2f(0.20, 0.80);
        glVertex2f(vilao.x, vilao.y + 100);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

void desenhaCoracao(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);

    // Primeiro coração
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 35, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 35, y + 35);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 35);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void desenhaInimigo1(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0.85);
    glVertex2f(x, y);
    glTexCoord2f(0.15, 0.85);
    glVertex2f(x + 40, y);
    glTexCoord2f(0.15, 1);
    glVertex2f(x + 40, y + 40);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + 40);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaInimigo2(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.85, 0.85);
    glVertex2f(x, y);
    glTexCoord2f(1, 0.85);
    glVertex2f(x + 40, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + 40, y + 40);
    glTexCoord2f(0.85, 1);
    glVertex2f(x, y + 40);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaInimigo3(GLuint idTextura, int x, int y)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.85, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + 40, y);
    glTexCoord2f(1, 0.15);
    glVertex2f(x + 40, y + 40);
    glTexCoord2f(0.85, 0.15);
    glVertex2f(x, y + 40);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

GLuint carregaTextura(const char *arquivo)
{
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y);

    if (idTextura == 0)
    {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

void desenhaTexto(float x, float y, void *font, const char *string)
{
    glRasterPos2f(x, y);
    while (*string)
    {
        glutBitmapCharacter(font, *string++);
    }
}

void drawButton(GLuint idTextura, int y, const char *text, int hover)
{
    if (hover)
    {
        glColor3f(1.0, 0.5, 0.0);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(buttonX, y);
    glTexCoord2f(1, 0);
    glVertex2i(buttonX + BUTTON_WIDTH, y);
    glTexCoord2f(1, 1);
    glVertex2i(buttonX + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glTexCoord2f(0, 1);
    glVertex2i(buttonX, y + BUTTON_HEIGHT);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Centralizar o texto
    int text_width = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)text);
    int text_x = buttonX + (BUTTON_WIDTH - text_width) / 2;
    int text_y = y + (BUTTON_HEIGHT - 18) / 2; // 18 é a altura da fonte GLUT_BITMAP_HELVETICA_18

    glColor3f(0.0, 0.0, 0.0); // cor do texto
    glRasterPos2i(text_x, text_y);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)text);
}

void drawInstructions()
{
    if (mostrarInstrucoes == 1)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaInstrucoes);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(209, 120);
        glTexCoord2f(1, 0);
        glVertex2f(209 + 350, 120);
        glTexCoord2f(1, 1);
        glVertex2f(209 + 350, 120 + 350);
        glTexCoord2f(0, 1);
        glVertex2f(209, 120 + 350);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

// DESENHA
void desenhaMinhaCena()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (estado == MENU)
    {
        glColor3f(1.0, 1.0, 1.0);

        int startButtonHover = currentMouseX >= buttonX && currentMouseX <= buttonX + BUTTON_WIDTH &&
                               currentMouseY >= START_BUTTON_Y && currentMouseY <= START_BUTTON_Y + BUTTON_HEIGHT;
        int instructionsButtonHover = currentMouseX >= buttonX && currentMouseX <= buttonX + BUTTON_WIDTH &&
                                      currentMouseY >= INSTRUCTIONS_BUTTON_Y && currentMouseY <= INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT;

        desenhaCenario(idTexturaCeu, ceu.x, 0);
        mudaValorCeu();
        drawButton(idTexturaButton, START_BUTTON_Y, "Iniciar", startButtonHover);
        drawButton(idTexturaButton, INSTRUCTIONS_BUTTON_Y, "Como jogar", instructionsButtonHover);
        glColor3f(1.0, 1.0, 1.0);
        desenhaLogo(idTexturaLogo, 184, 250);

        /*const char* nomeTexto = "Bimbles Bird";
        int nomeTexto_width = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)nomeTexto);
        int nomeTexto_x = (windowWidth - nomeTexto_width) / 2;
        int nomeTexto_y = INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT + 40; // 20 pixels acima do botão superior

        glColor3f(1.0f, 1.0f, 1.0f);  // cor do texto
        desenhaTexto(nomeTexto_x, nomeTexto_y, GLUT_BITMAP_TIMES_ROMAN_24, nomeTexto);*/
    }
    else if (estado == INSTRUCOES)
    {
        glColor3f(1.0, 1.0, 1.0);

        desenhaCenario(idTexturaCeu, ceu.x, 0);
        mudaValorCeu();

        mostrarInstrucoes = 1;

        drawInstructions();
        int backButtonHover = currentMouseX >= buttonX && currentMouseX <= buttonX + BUTTON_WIDTH &&
                              currentMouseY >= BACK_BUTTON_Y && currentMouseY <= BACK_BUTTON_Y + BUTTON_HEIGHT;

        drawButton(idTexturaButton, BACK_BUTTON_Y, "Retornar ao menu", backButtonHover);
    }
    else if (estado == JOGANDO)
    {
        glColor3f(1.0, 1.0, 1.0);
        if (faseAtual == 0)
        {
            glClearColor(0.322, 0.686, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            desenhaCenario(idTexturaMontanha, montanha.x, montanha.y);
            desenhaCenario(idTexturaAgua, agua.x, agua.y);
            desenhaCenario(idTexturaNuvens, nuvens.x, nuvens.y);
            desenhaPersonagem();

            if (vidas == 3)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 720, 450);
            }
            else if (vidas == 2)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else if (vidas == 1)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else
            {
                desenhaCoracao(idTexturaCoracaoVazio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }

            for (int i = 0; i < 90; i++)
            {
                desenhaInimigo1(idTexturaInimigo, inimigosFase1[i].pontoInicial.x, inimigosFase1[i].pontoInicial.y);
            }
            for (int i = 0; i < 36; i++)
            {
                desenhaInimigo1(idTexturaInimigo, inimigosFase1b[i].pontoInicial.x, inimigosFase1b[i].pontoInicial.y);
            }
            // desenhaInimigo1(idTexturaInimigo, pontoInicial.x, pontoInicial.y);
            mudaValor();
            mudaValorInimigo();
        }
        if (faseAtual == 2)
        {

            desenhaCenario(idTexturaLava, lava.x, lava.y);
            desenhaCenarioBarco(idTexturaBarcos, barco1.x, -50, 0, 1, 0.33);    // barco1
            desenhaCenarioBarco(idTexturaBarcos, barco2.x, 300, 0.33, 1, 0.66); // barco2
            desenhaCenarioBarco(idTexturaBarcos, barco3.x, 125, 0.66, 1, 1);    // barco 3
            desenhaCenarioArena(idTexturaArena, arena.x, arena.y);
            desenhaPersonagem();
            desenhaVilao();
            desenhaSpikes(idTexturaSpikes, 0, 0);

            if (vidas == 3)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 720, 450);
            }
            else if (vidas == 2)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else if (vidas == 1)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else
            {
                desenhaCoracao(idTexturaCoracaoVazio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }

            for (int i = 0; i < 90; i++)
            {
                desenhaInimigo3(idTexturaInimigo, inimigosFase3[i].pontoInicial.x, inimigosFase3[i].pontoInicial.y);
                desenhaInimigo3(idTexturaInimigo, inimigosFase3c[i].pontoInicial.x, inimigosFase3c[i].pontoInicial.y);
            }
            for (int i = 0; i < 36; i++)
            {
                desenhaInimigo3(idTexturaInimigo, inimigosFase3b[i].pontoInicial.x, inimigosFase3b[i].pontoInicial.y);
                desenhaInimigo3(idTexturaInimigo, inimigosFase3d[i].pontoInicial.x, inimigosFase3d[i].pontoInicial.y);
            }
            mudaValor();
            mudaValorInimigo();
        }
        if (faseAtual == 1)
        {

            desenhaCenario(idTexturaCeu, ceu.x, 0);
            desenhaCenario(idTexturaCidade, cidade.x, 0);
            desenhaCenario(idTexturaRua, rua.x, 0);
            desenhaCenarioCarro(idTexturaCarro, carro.x, 0);
            desenhaPersonagem();
            desenhaSpikes(idTexturaSpikes, 0, 0);

            if (vidas == 3)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 720, 450);
            }
            else if (vidas == 2)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoCheio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else if (vidas == 1)
            {
                desenhaCoracao(idTexturaCoracaoCheio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }
            else
            {
                desenhaCoracao(idTexturaCoracaoVazio, 640, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 680, 450);
                desenhaCoracao(idTexturaCoracaoVazio, 720, 450);
            }

            for (int i = 0; i < 90; i++)
            {
                desenhaInimigo2(idTexturaInimigo, inimigosFase2[i].pontoInicial.x, inimigosFase2[i].pontoInicial.y);
                desenhaInimigo2(idTexturaInimigo, inimigosFase2d[i].pontoInicial.x, inimigosFase2d[i].pontoInicial.y);
            }
            for (int i = 0; i < 36; i++)
            {
                desenhaInimigo1(idTexturaInimigo, inimigosFase2b[i].pontoInicial.x, inimigosFase2b[i].pontoInicial.y);
                desenhaInimigo1(idTexturaInimigo, inimigosFase2c[i].pontoInicial.x, inimigosFase2c[i].pontoInicial.y);
            }
            mudaValor();
            mudaValorInimigo();
        }
    }
    else if (estado == GAMEOVER)
    {
        glColor3f(1.0, 1.0, 1.0);

        desenhaCenario(idTexturaLava, lava.x, lava.y);
        mudaValorLava();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaGameover);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(184, 200);
        glTexCoord2f(1, 0);
        glVertex2f(184 + 400, 200);
        glTexCoord2f(1, 1);
        glVertex2f(184 + 400, 200 + 200);
        glTexCoord2f(0, 1);
        glVertex2f(184, 200 + 200);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        int backButtonHover = currentMouseX >= buttonX && currentMouseX <= buttonX + BUTTON_WIDTH &&
                              currentMouseY >= BACK_BUTTON_Y && currentMouseY <= BACK_BUTTON_Y + BUTTON_HEIGHT;

        drawButton(idTexturaButton, BACK_BUTTON_Y, "Retornar ao menu", backButtonHover);
    }
    else if (estado == VITORIA)
    {
        glColor3f(1.0, 1.0, 1.0);

        desenhaCenario(idTexturaCeu, ceu.x, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaNext);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(184, 100);
        glTexCoord2f(1, 0);
        glVertex2f(184 + 400, 100);
        glTexCoord2f(1, 1);
        glVertex2f(184 + 400, 100 + 400);
        glTexCoord2f(0, 1);
        glVertex2f(184, 100 + 400);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        mudaValorCeu();
        int backButtonHover = currentMouseX >= buttonX && currentMouseX <= buttonX + BUTTON_WIDTH &&
                              currentMouseY >= BACK_BUTTON_Y && currentMouseY <= BACK_BUTTON_Y + BUTTON_HEIGHT;

        drawButton(idTexturaButton, BACK_BUTTON_Y, "->", backButtonHover);
    }
    else if (estado == LOADING)
    {
        glColor3f(1.0, 1.0, 1.0);
        desenhaCenario(idTexturaCeu, ceu.x, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaLoading);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(184, 50);
        glTexCoord2f(1, 0);
        glVertex2f(184 + 400, 50);
        glTexCoord2f(1, 1);
        glVertex2f(184 + 400, 50 + 400);
        glTexCoord2f(0, 1);
        glVertex2f(184, 50 + 400);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        mudaValorCeu();
    }
    else if (estado == ZEROU)
    {
        glColor3f(1.0, 1.0, 1.0);
        desenhaCenario(idTexturaCeu, ceu.x, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idTexturaZerou);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(184, 50);
        glTexCoord2f(1, 0);
        glVertex2f(184 + 400, 50);
        glTexCoord2f(1, 1);
        glVertex2f(184 + 400, 50 + 400);
        glTexCoord2f(0, 1);
        glVertex2f(184, 50 + 400);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        mudaValorCeu();

        glutTimerFunc(5000, mudaEstado, MENU);
    }

    glutSwapBuffers(); // glflush se tiver so um buffer
}

// TECLA PRESSIONADA
void teclaPressionada(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        movimentoY = 1;
        break;
    case 's':
        movimentoY = -1;
        break;
    case 'a':
        movimentoX = -1;
        spriteAtualHero = 0;   // Define a sprite do herói para o lado esquerdo
        viradoEsquerda = true; // Ativa a flag de virado para a esquerda
        break;
    case 'd':
        movimentoX = 1;
        spriteAtualHero = 0;    // Define a sprite do herói para o lado direito
        viradoEsquerda = false; // Desativa a flag de virado para a esquerda
        break;
    case 'W':
        movimentoY = 1;
        break;
    case 'S':
        movimentoY = -1;
        break;
    case 'A':
        movimentoX = -1;
        spriteAtualHero = 0;   // Define a sprite do herói para o lado esquerdo
        viradoEsquerda = true; // Ativa a flag de virado para a esquerda
        break;
    case 'D':
        movimentoX = 1;
        spriteAtualHero = 0;    // Define a sprite do herói para o lado direito
        viradoEsquerda = false; // Desativa a flag de virado para a esquerda
        break;
    case ' ':
        spriteAtualHero = (spriteAtualHero + 1) % 5;
        break;
    }
    glutPostRedisplay();
}

void teclaLiberada(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 's':
        movimentoY = -0.7;
        break;
    case 'a':
    case 'd':
        movimentoX = 0;
        break;
    case 'W':
    case 'S':
        movimentoY = -0.7;
        break;
    case 'A':
    case 'D':
        movimentoX = 0;
        break;
    }
}

// INICIALIZA
void inicializa()
{
    // define a cor de fundo
    glClearColor(0, 0, 0, 0);
    jogador.x = 384;
    jogador.y = 250;
    vilao.x = -100;
    vilao.y = 250;
    idTexturaMontanha = carregaTextura("imagens/montanha.png");
    idTexturaAgua = carregaTextura("imagens/agua.png");
    idTexturaNuvens = carregaTextura("imagens/nuvens.png");
    idTexturaLava = carregaTextura("imagens/lava.png");
    idTexturaArena = carregaTextura("imagens/arena.png");
    idTexturaBarcos = carregaTextura("imagens/barcos.png");
    idTexturaCarro = carregaTextura("imagens/carro.png");
    idTexturaCeu = carregaTextura("imagens/ceu.png");
    idTexturaCidade = carregaTextura("imagens/cidade.png");
    idTexturaRua = carregaTextura("imagens/rua.png");
    idTexturaInimigo = carregaTextura("imagens/inimigos.png");
    idTexturaCoracaoCheio = carregaTextura("imagens/coracaoCheio.png");
    idTexturaCoracaoVazio = carregaTextura("imagens/coracaoVazio.png");
    idTexturaHero[0] = carregaTextura("imagens/hero0.png");
    idTexturaHero[1] = carregaTextura("imagens/hero1.png");
    idTexturaHero[2] = carregaTextura("imagens/hero2.png");
    idTexturaHero[3] = carregaTextura("imagens/hero3.png");
    idTexturaHero[4] = carregaTextura("imagens/hero4.png");
    idTexturaVilao[0] = carregaTextura("imagens/vilao1.png");
    idTexturaVilao[1] = carregaTextura("imagens/vilao2.png");
    idTexturaVilao[2] = carregaTextura("imagens/vilao3.png");
    idTexturaVilao[3] = carregaTextura("imagens/vilao4.png");
    idTexturaVilao[4] = carregaTextura("imagens/vilao5.png");
    idTexturaVilao[5] = carregaTextura("imagens/vilao6.png");
    idTexturaVilao[6] = carregaTextura("imagens/vilao7.png");
    idTexturaVilao[7] = carregaTextura("imagens/vilao8.png");
    idTexturaLogo = carregaTextura("imagens/logo.png");
    idTexturaButton = carregaTextura("imagens/button.png");
    idTexturaInstrucoes = carregaTextura("imagens/instrucoes.png");
    idTexturaGameover = carregaTextura("imagens/gameover.png");
    idTexturaLoading = carregaTextura("imagens/loading.png");
    idTexturaNext = carregaTextura("imagens/neext.png");
    idTexturaZerou = carregaTextura("imagens/zerou.png");
    idTexturaSpikes = carregaTextura("imagens/spikes.png");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void idle()
{
    if (estado == JOGANDO)
    {
        int gameOver = 0; // Substitua isso pela sua condição de game over
        if (gameOver)
        {
            estado = GAMEOVER;
        }
    }
    else if (estado == GAMEOVER)
    {
        int tempoGameOver = 0; // Substitua isso pelo seu contador de tempo
        if (tempoGameOver > 100)
        { // Substitua 100 pelo número de frames que você quer esperar
            estado = MENU;
        }
    }
    else if (estado == VITORIA)
    {
        int tempoVitoria = 0; // Substitua isso pelo seu contador de tempo
        if (tempoVitoria > 100)
        { // Substitua 100 pelo número de frames que você quer esperar
            estado = MENU;
        }
    }
}

// REDIMENSIONA
void redimensiona(int width, int height)
{

    glViewport(0, 0, 768, 500);
    // pro quadrado redimensionar junto com a janela colocar width e height aqui
    // aqui a unidade de medida é pixel!!!
    // controla o tamanho em pixel do conteudo.

    buttonX = windowWidth / 2 - BUTTON_WIDTH / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 768, 0, 500, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    y = windowHeight - y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (x >= buttonX && x <= buttonX + BUTTON_WIDTH)
        {
            if (estado == MENU)
            {
                if (y >= START_BUTTON_Y && y <= START_BUTTON_Y + BUTTON_HEIGHT)
                {
                    estado = JOGANDO;
                    glutTimerFunc(0, mudaJ, 0);
                    glutTimerFunc(0, mudaL, 0);

                    geraInimigos();
                    mostrarInstrucoes = 0; // Não mostrar instruções
                }
                else if (y >= INSTRUCTIONS_BUTTON_Y && y <= INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT)
                {
                    estado = INSTRUCOES;
                    mostrarInstrucoes = 1; // Mostrar instruções
                }
            }
            else if (estado == INSTRUCOES)
            {
                if (y >= BACK_BUTTON_Y && y <= BACK_BUTTON_Y + BUTTON_HEIGHT)
                {
                    estado = MENU;
                    mostrarInstrucoes = 0; // Não mostrar instruções
                }
            }
            else if (estado == GAMEOVER)
            {
                if (y >= BACK_BUTTON_Y && y <= BACK_BUTTON_Y + BUTTON_HEIGHT)
                {
                    glutTimerFunc(6000, mudaEstado, 0);
                    estado = LOADING;
                    mostrarInstrucoes = 0; // Não mostrar instruções
                }
            }
            else if (estado == VITORIA)
            {
                if (y >= BACK_BUTTON_Y && y <= BACK_BUTTON_Y + BUTTON_HEIGHT)
                {
                    glutTimerFunc(6000, mudaEstado, JOGANDO);
                    estado = LOADING;
                    glutTimerFunc(0, mudaJ, 0);
                    glutTimerFunc(0, mudaL, 0);
                    geraInimigos();
                    mostrarInstrucoes = 0; // Não mostrar instruções
                }
            }
        }
        glutPostRedisplay();
    }
}

void motion(int x, int y)
{

    currentMouseX = (0) + ((768) - (0)) * (x - (0)) / ((768) - (0));
    currentMouseY = (0) + ((500) - (0)) * (y - (500)) / ((0) - (500));

    glutPostRedisplay();
}

// MAIN
int main(int argc, char **argv)
{
    srand(time(0));

    geraInimigos();
    buttonX = windowWidth / 2 - BUTTON_WIDTH / 2;

    montanha.x = 0;
    montanha.y = 0;
    agua.x = 0;
    agua.y = 0;
    nuvens.x = 0;
    nuvens.y = 0;
    lava.x = 0;
    lava.y = 0;
    arena.x = 134;
    arena.y = 0;
    barco1.x = 768;
    barco2.x = 768;
    barco3.x = -200;
    ceu.x = 0;
    cidade.x = 0;
    rua.x = 0;
    carro.x = -150;

    // acordando o GLUT
    glutInit(&argc, argv);

    // definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(768, 500);

    // abre a janela
    glutCreateWindow("Jogo");

    // registro de callbacks de eventos
    glutDisplayFunc(desenhaMinhaCena);
    glutKeyboardFunc(teclaPressionada);
    glutKeyboardUpFunc(teclaLiberada);
    glutReshapeFunc(redimensiona);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutIdleFunc(idle);
    inicializa();
    glutMainLoop();
    return 0;
}