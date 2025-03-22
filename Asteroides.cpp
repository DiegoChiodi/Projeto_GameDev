#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <memory> // Para std::unique_ptr

const int widthJan = 800;
const int heightJan = 600;

struct Vector
{
    float x;
    float y;
};

bool colision (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    if (ax + aw < bx) return false;
    else if (bx + bw < ax) return false;
    else if (ay + ah < by) return false;
    else if (by + bh < ay) return false;

    return true;
}

int sign(float valor) 
{
    return (valor > 0) - (valor < 0);
}

using Ponto = Vector;

class Movel
{
protected:
    SDL_Rect rect;
    Ponto position;
    float speed;
public:
    Movel(int x, int y, int width, int height, float speed)
    : speed(speed), position{static_cast<float>(x), static_cast<float>(y)}
    {   this->rect = {   x, y, width, height   };   };

    void atualizar (const float dt, const int dir)
    {
        this->position.x -= (this->speed * dir) * dt;
        this->rect.x = static_cast<int>(this->position.x);
    }

    virtual int objectIndex()
    {
        return 0;
    }

    SDL_Rect get_rect() {    return this->rect;    }

    Ponto get_pos() {   return this->position;    }

    void desenhar(SDL_Renderer* renderer, SDL_Color cor) 
    {
        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
        SDL_RenderFillRect(renderer, &this->rect);
    }

    virtual ~Movel() {}
};

class Meteor : public Movel
{
public:
    using Movel::Movel; 

    bool limpar (std::list<std::unique_ptr<Meteor>>& meteors)
    {
        if (this->position.x < 0)
        {
            return true;
        }
        return false;
    }

    int objectIndex() override
    {
        return 2;
    }
};

class Project : public Movel
{
public:
    using Movel::Movel;

    int objectIndex() override
    {
        return 1;
    }
};

class Spacenave 
{
private:
    SDL_Rect rect; //é uma struct
    Ponto position;
    Vector speed;
    float acceleration, deceleration, max_speed;

public:
    Spacenave(int x, int y, int width, int height, float acceleration, float deceleration, float max_speed)
    : acceleration(acceleration), deceleration(deceleration), max_speed(max_speed), position{static_cast<float>(x),static_cast<float>(y)}  // Inicializa pos_y com y
    {   this->rect = { x, y, width, height};    }

    Ponto get_pos()
    {
        return this->position;
    }

    SDL_Rect get_rect()
    {
        return this->rect;
    }

    void move(float dt) 
    {
        this->speed.x = std::clamp(this->speed.x, -this->max_speed, this->max_speed);
        this->speed.y = std::clamp(this->speed.y, -this->max_speed, this->max_speed);

	    this->position.x = std::clamp(this->position.x + (this->speed.x * dt), 0.0f, static_cast<float>(widthJan - this->rect.w));
        this->rect.x = static_cast<int>(this->position.x);

        this->position.y = std::clamp(this->position.y + (this->speed.y * dt), 0.0f, static_cast<float>(heightJan - this->rect.h));
        this->rect.y = static_cast<int>(this->position.y);  // Atualiza a posição inteira para desenhar
    }

    void atualizar(const Uint8* stat, float dt)
    {
		//MOVIMENTAÇÃO HORIZONTAL
		if (stat[SDL_SCANCODE_A])	//verifica no array se "a" está pressionado
		{
			this->speed.x -= this->acceleration;
		}
		if (stat[SDL_SCANCODE_D])
		{
			this->speed.x += this->acceleration;
		}

        if (!(stat[SDL_SCANCODE_A]) && !(stat[SDL_SCANCODE_D]))
        {
            this->speed.x -= this->deceleration * sign(this->speed.x);
            if (this->speed.x < this->deceleration)
            {
                this->speed.x = 0.0f;
            }
        }
		//MOVIMENTAÇÃO VERTICAL
    	if (stat[SDL_SCANCODE_W]) 
        { 
            this->speed.y -= this->acceleration;
        }
        if (stat[SDL_SCANCODE_S])
        {
            this->speed.y += this->acceleration;
        }

        if (!(stat[SDL_SCANCODE_W]) && !(stat[SDL_SCANCODE_S]))
        {
            this->speed.y -= this->deceleration * sign(this->speed.y);
            if (this->speed.y < this->deceleration)
            {
                this->speed.y = 0.0f;
            }
        }

        this->move(dt);//Movimenta
    }

    void project_create (std::list<std::unique_ptr<Movel>>& list)
    {
        list.push_back(std::make_unique<Project>(this->position.x + this->rect.w, this->position.y - 5, 20, 10, 100.0f));
    }
    
    void desenhar(SDL_Renderer* renderer, SDL_Color cor) 
    {
        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
        SDL_RenderFillRect(renderer, &this->rect);
    }
};

class Jogo 
{
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

    ~Jogo() 
    {
        if (this->soundEffect) Mix_FreeChunk(this->soundEffect);
        if (this->music) Mix_FreeMusic(this->music);
        if (this->renderer) SDL_DestroyRenderer(this->renderer);
        if (this->window) SDL_DestroyWindow(this->window);

        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
    }

    bool inicializar() 
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            std::cerr << "Erro ao inicializar SDL_Mixer: " << Mix_GetError() << std::endl;
            return false;
        }

        this->window = SDL_CreateWindow("Ping pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->largura, this->altura, SDL_WINDOW_SHOWN);
        if (!this->window) {
            std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
            return false;
        }

        this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
        if (!this->renderer) {
            std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
            return false;
        }

        this->music = Mix_LoadMUS("music.mp3");
        if (!this->music) {
            std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
            return false;
        }

        this->soundEffect = Mix_LoadWAV("effect.mp3");
        if (!this->soundEffect) {
            std::cerr << "Erro ao carregar efeito sonoro: " << Mix_GetError() << std::endl;
            return false;
        }

        Mix_PlayMusic(this->music, -1);
        return true;
    }

    void meteor_create (std::list<std::unique_ptr<Movel>>& meteors, int row)
    {
        meteors.push_back(std::make_unique<Meteor>(550, 100 * row, 20, 20, 50.0f));
    }

    void executar() 
    {
        //Criar
        std::list<std::unique_ptr<Movel>> movels;
        Spacenave spacenave (widthJan / 2, heightJan / 2, 20, 20, 30.0f, 40.0f, 150.0f);
        SDL_Color blue = {0,0,255};
        SDL_Color white = {200,200,200};
        SDL_Color red = {255,0,0};
        int previous_time, actual_time;
        float dt;
        int meteor_alarm = 170;
        
        while (this->running) 
        {
            while (SDL_PollEvent(&this->event)) 
            {
                if (this->event.type == SDL_QUIT)
                {
                    this->running = false;
                }

                if (this->event.type == SDL_KEYDOWN && this->event.key.keysym.sym == SDLK_SPACE) {
                    spacenave.project_create(movels);
                }
            }
            //LOOPIN
			const Uint8* stat = SDL_GetKeyboardState(nullptr);

            actual_time = SDL_GetTicks();
            dt = ((actual_time - previous_time) / 1000.0f);
            previous_time = actual_time;
            
			spacenave.atualizar(stat, dt);

            if (!movels.empty()) // Se a lista não está vazia
            {
                for (auto it = movels.begin(); it != movels.end();)
                {
                    // Verifica se o objeto é do tipo Project
                    int obj_id = (*it)->objectIndex();
                    switch (obj_id)
                    {
                        case 1: // Projétil
                            (*it)->atualizar(dt, -1);           

                            for (auto it_m = movels.begin(); it_m != movels.end();)
                            {
                                int obj_id_m = (*it_m)->objectIndex();
                                if (obj_id_m == 2) // Meteoro
                                {
                                    SDL_Rect project_rect = (*it)->get_rect();
                                    Ponto project_pos = (*it)->get_pos();           

                                    SDL_Rect meteor_rect = (*it_m)->get_rect();
                                    Ponto meteor_pos = (*it_m)->get_pos();          

                                    if (colision(project_pos.x, project_pos.y, project_rect.w, project_rect.h,
                                        meteor_pos.x, meteor_pos.y, meteor_rect.w, meteor_rect.h))
                                    {
                                        it = movels.erase(it);   // Remove o projétil
                                        it_m = movels.erase(it_m); // Remove o meteoro
                                        break; // Sai do loop interno após a remoção
                                    }
                                    it_m++;
                                }
                                else
                                {
                                    it_m++;
                                }
                            }
                            it++;
                            break;          

                        case 2: // Meteoro
                            (*it)->atualizar(dt, 1);
                            Ponto space_pos = spacenave.get_pos();
                            SDL_Rect space_rect = spacenave.get_rect();
                            Ponto meteor_pos = (*it)->get_pos();
                            SDL_Rect meteor_rect = (*it)->get_rect();           

                            if (colision(space_pos.x, space_pos.y, space_rect.w, space_rect.h,
                                meteor_pos.x, meteor_pos.y, meteor_rect.w, meteor_rect.h))
                            {
                                this->running = false; // Fim do jogo
                            }
                            it++;
                            break;
                    }
                }
            }

            meteor_alarm ++;

            if (meteor_alarm >= 200)
            {
                for (int row = 1; row <= 5; row++)
                {
                    this->meteor_create(movels,row);
                }
                meteor_alarm = 0;
            }

			//RENDERIZAR
			SDL_SetRenderDrawColor(this->renderer, 25, 25, 25, 255); //cor do fundo
			SDL_RenderClear(this->renderer); //limpa o fundo

            spacenave.desenhar(this->renderer, blue); //azul

            //Loop para desenhar os projeteis
            if (!movels.empty())//se lista não está vazia
            {
                for (auto it = movels.begin(); it != movels.end();)
                {
                    (*it)->desenhar(this->renderer, white);
                    it ++;
                }
            }

            SDL_RenderPresent(this->renderer);
			
			SDL_Delay(30); // Pequeno atraso para controlar a velocidade do loop
        }
    }
};

int main(int argc, char* argv[]) 
{
    Jogo jogo(widthJan, heightJan);

    if (!jogo.inicializar()) 
    {
        std::cerr << "Erro ao inicializar o jogo." << std::endl;
        return 1;
    }

    jogo.executar();
    return 0;
}