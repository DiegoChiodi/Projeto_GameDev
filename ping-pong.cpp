#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <algorithm>

class Racket {
private:
    SDL_Rect racket;
    float pos_y;  // Posição Y como float
    float velocidade;  // Velocidade como float

public:
    Racket(int x, int y, int largura, int altura, float velocidade)
        : velocidade(velocidade), pos_y(y)  // Inicializa pos_y com y
    {   racket = { x, y, largura, altura };   }

    SDL_Rect get_rect () { return racket; }

    void mover(float hspd, int limite) {
        pos_y = std::clamp(pos_y + hspd, 0.0f, static_cast<float>(limite - racket.h));
        racket.y = static_cast<int>(pos_y);  // Atualiza a posição inteira para desenhar
    }

    void atualizar(const Uint8* state, bool player, int altura) { //state retorna 1 se a tecla tiver pressionada e 0 se não
        if (player) {
            if (state[SDL_SCANCODE_W]) { //verifica no array se w está pressionado
                mover(-velocidade, altura);
            }
            if (state[SDL_SCANCODE_S]) {
                mover(velocidade, altura);
            }
        } else {
            if (state[SDL_SCANCODE_UP]) {
                mover(-velocidade, altura);
            }
            if (state[SDL_SCANCODE_DOWN]) {
                mover(velocidade, altura);
            }
        }
    }
    
    void desenhar(SDL_Renderer* renderer, SDL_Color cor) {
        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
        SDL_RenderFillRect(renderer, &racket);
    }
};

class Ball {
private:
    SDL_Rect ball; //é uma struct
    float pos_x;
    float pos_y;  // Posição Y como float 
    float velocidade;  // Velocidade como float

public:
    Ball(int x, int y, int tamanho, float velocidade)
        : velocidade(velocidade), pos_x(x), pos_y(y)  // Inicializa pos_y com y
    {   ball = { x, y, tamanho, tamanho };    } //pra compactar

    SDL_Rect get_rect () { return ball; }

    bool colision_obj (float wspd, float hspd, Racket& racket_obj, Ball& ball_obj)
    {   
        SDL_Rect racket = racket_obj.get_rect();
        SDL_Rect ball = ball_obj.get_rect();

        int a1 = racket.x;
        int a2 = racket.x + racket.w;
        
        int b1 = ball.x;
        int b2 = ball.x + ball.w + wspd;

        int c1 = racket.y;
        int c2 = racket.y + racket.h;
        
        int d1 = ball.y;
        int d2 = ball.y + ball.h + hspd;
        

        if ((a2 < b1 or b2 < a1) || (c2 < d1 or d2 < c1)) //se nao colido
        {
            return false;
        }
        return true;
    }

    bool colision_lim (float spd, int medida_ball, int pos_ball, int medida_room)
    {
        if (pos_ball + medida_ball + spd >= medida_room || pos_ball + spd <= 0)
        {
            return true;
        }
        return false;
    }

    void mover(float wspd, float hspd, int limite_x, int limite_y) {
        pos_x = std::clamp(pos_x + wspd, 0.0f, static_cast<float>(limite_x - ball.w)); // static cast é um conversor
        ball.x = static_cast<int>(pos_x);  // Atualiza a posição inteira para desenhar
        pos_y = std::clamp(pos_y + hspd, 0.0f, static_cast<float>(limite_y - ball.h)); // static cast é um conversor
        ball.y = static_cast<int>(pos_y);  // Atualiza a posição inteira para desenhar
    }
    
    void desenhar(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball);
    }
};


class Jogo {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* music;
    Mix_Chunk* soundEffect;
    bool running;
    int largura, altura;
    SDL_Event event;

public:
    Jogo(int largura, int altura)
        : window(nullptr), renderer(nullptr), music(nullptr), soundEffect(nullptr), running(true), largura(largura), altura(altura) {}

    ~Jogo() {
        if (soundEffect) Mix_FreeChunk(soundEffect);  // Libera a memória
        if (music) Mix_FreeMusic(music);        // Libera a memória
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        Mix_Quit();
    }

    bool inicializar() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            std::cerr << "Erro ao inicializar SDL_Mixer: " << Mix_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow("Ping pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largura, altura, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
            return false;
        }

        music = Mix_LoadMUS("music.mp3");
        if (!music) {
            std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
            return false;
        }

        soundEffect = Mix_LoadWAV("effect.mp3");
        if (!soundEffect) {
            std::cerr << "Erro ao carregar efeito sonoro: " << Mix_GetError() << std::endl;
            return false;
        }

        Mix_PlayMusic(music, -1);  // Tocar música em loop
        return true;
    }

    

    void executar() {

        //Criar
        Racket racket(30, 300, 20, 100, 0.2f);   // Velocidade ajustada
        Racket racket_2(largura - 50, 300, 20, 100, 0.2f); // Outra 
        Ball ball(largura / 2, altura / 2, 15, 4);

        SDL_Color blue = {0,0,255};
        SDL_Color red = {255,0,0};
        float wspd = 0.1;
        float hspd = 0.1;

        //Executando
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            const Uint8* state = SDL_GetKeyboardState(nullptr);

            wspd += 0.00004 * wspd;
            hspd += 0.00004 * hspd;

            racket.atualizar(state, true,altura);
            racket_2.atualizar(state, false,altura);
            ball.mover(wspd,hspd,800,600);

            //se ball colidi com raquete
            if (ball.colision_obj(wspd, hspd, racket, ball) || ball.colision_obj(wspd, hspd, racket_2, ball))
            {
                wspd *= -1;
                Mix_PlayChannel(-1, soundEffect, 0); // Tocar som
            }

            SDL_Rect ball_rect = ball.get_rect();

            if (ball.colision_lim(wspd, ball_rect.w, ball_rect.x, largura)) {
                ball = Ball(largura / 2, altura / 2, 15, 4);  // Reinicia a bola
                wspd = 0.1;
                hspd = 0.1;
            }

            if (ball.colision_lim(hspd, ball_rect.h, ball_rect.y, altura)) {

                hspd *= -1;
                Mix_PlayChannel(-1, soundEffect, 0); // Tocar som
            }
            
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde para fundo
            SDL_RenderClear(renderer);

            racket.desenhar(renderer, blue); //azul
            racket_2.desenhar(renderer, red); //vermelho
            ball.desenhar(renderer);

            // Linha no meio mais grossa
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco
            SDL_RenderDrawLine(renderer, largura / 2, 0, largura / 2, altura);

            SDL_RenderPresent(renderer);
        }
    }
};

int main(int argc, char* argv[]) {
    Jogo jogo(800, 600);

    if (!jogo.inicializar()) {
        std::cerr << "Erro ao inicializar o jogo." << std::endl;
        return 1;
    }

    jogo.executar();
    return 0;
}