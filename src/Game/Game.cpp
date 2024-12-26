#include "Game.h"
#include "../Logger/Logger.h"

#include "../ECS/Entity.h"
#include "../Components/Transformcomponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <fstream>


int Game::displayWidth;
int Game::displayHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game(){
    isRunning = false;
    // oYel = false;
    registry = std::make_unique<Registry>();
    assetstore = std::make_unique<Assetstore>();
    // eventBus = std::make_unique<EventBus>();
    Logger::Log("Game constructor called:");

} 
Game::~Game(){
    Logger::Log("Game destructor called:");
}

void Game::Initialize(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error inirializing SDL.");
        return;
    }
    // if (TTF_Init() != 0)
    // {
    //     Logger::Err("Error initializing SDL TTF. ");
    //     return;
    // }
    
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    displayWidth = displayMode.w;
    displayHeight = displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        displayWidth,
        displayHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        Logger::Err("Error creeating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if (!renderer){
        Logger::Err("Error creating SDL renderer.");
        return;
    }
    // IMGUI_CHECKVERSION();

    // ImGui::CreateContext();

    // ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);

    // ImGui_ImplSDLRenderer2_Init(renderer);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;

    // camera.x = 0;
    // camera.y = 0;
    // camera.w = displayWidth;
    // camera.h = displayHeight;
}

void Game::Run(){

    Setup();
    while(isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::LevelLoad(int LevelNum){
    assetstore->AddTexture(renderer, "Tiles", "./assets/tilemaps/Tiles.png");

    // std::ifstream file("./assets/tilemaps/Tiles.map");
    // if (!file.is_open()) {
    //     Logger::Log("Error opening file!");
    //     return;
    // }

    Entity tile = registry->CreateEntity();
    tile.Group("tiles");
    tile.AddComponent<SpriteComponent>("Tiles", 32, 32, 0, false, 0, 0);
    tile.AddComponent<Transformcomponent>(glm::vec2(0 , 0), glm::vec2(3.0, 3.0), 0.0);
    // line.erase(0, pos + delimiter.length());
    return;
}
void Game::Setup(){
    LevelLoad(1);
}

void Game::ProcessInput(){
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent)){
        // ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        // ImGuiIO& io = ImGui::GetIO();
        // int mouseX, mouseY;
        // const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        // io.MousePos = ImVec2(mouseX, mouseY);
        // io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        // io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        switch(sdlEvent.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                    isRunning = false;
                }
                // eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);

                // oYel = sdlEvent.key.keysym.sym == SDLK_d && !oYel;
                break;
            
        }
    }
    return;
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer,21,21,21,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return;
}

void Game::Update(){
    return;
}

void Game::Destroy(){
    // ImGui_ImplSDLRenderer2_Shutdown();
    // ImGui_ImplSDL2_Shutdown();
    // ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}