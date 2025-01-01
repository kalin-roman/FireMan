
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

#include "Game.h"
#include "../Logger/Logger.h"

#include "../Components/Transformcomponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../ECS/ECS.h"
#include "../System/RenderSystem.h"
#include "../System/AnimationSystem.h"


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

    camera.x = 0;
    camera.y = 0;
    camera.w = displayWidth;
    camera.h = displayHeight;
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
    assetstore->AddTexture(renderer, "hammer-image", "./assets/images/Hammer.png");
    assetstore->AddTexture(renderer, "gates-image", "./assets/images/Entry.png");
    assetstore->AddTexture(renderer, "platform-image", "./assets/images/Platform.png");
    assetstore->AddTexture(renderer, "chest-image", "./assets/images/Chest.png");
    assetstore->AddTexture(renderer, "screen1-image", "./assets/images/Screen1.png");
    assetstore->AddTexture(renderer, "screen2-image", "./assets/images/Screen2.png");

    std::ifstream file("./assets/tilemaps/Tiles.map");
    if (!file.is_open()) {
        Logger::Log("Error opening file!");
        return;
    }
    std::string line;
    int row = 0;
    int col = 0;
    float scaleT = 2.5;
    int sizeOfPic = 32;

    std::string delimiter = ",";
    while (getline(file, line)) {
        size_t pos = 0;
        col = 0;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            int n = std::stoi(line.substr(0, pos)) - 1;
            int y = (int)floor(n/10);
            int x = n - y * 10;

            if (n >= 0){
                Entity tile = registry->CreateEntity();
                tile.Group("tiles");
                tile.AddComponent<SpriteComponent>("Tiles", sizeOfPic, sizeOfPic, 0, false, x * sizeOfPic, y * sizeOfPic);
                tile.AddComponent<Transformcomponent>(glm::vec2(col * sizeOfPic * scaleT, row * sizeOfPic * scaleT), glm::vec2(scaleT, scaleT), 0.0);
            }
            line.erase(0, pos + delimiter.length());
            col++;
            /* code */
        }
        row++;
    }
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();

    Entity hammer = registry->CreateEntity();
    hammer.AddComponent<SpriteComponent>("hammer-image", 32, 64, 1);
    hammer.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    hammer.AddComponent<Transformcomponent>(glm::vec2(320 * scaleT, 192 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    hammer.AddComponent<AnimationComponent>(8, 8, true);

    Entity hammer2 = registry->CreateEntity();
    hammer2.AddComponent<SpriteComponent>("hammer-image", 32, 64, 1);
    hammer2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    hammer2.AddComponent<Transformcomponent>(glm::vec2(384 * scaleT, 192 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    hammer2.AddComponent<AnimationComponent>(8, 8, true, 3);

    Entity gate = registry->CreateEntity();
    gate.AddComponent<SpriteComponent>("gates-image", 32, 64, 1);
    gate.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    gate.AddComponent<Transformcomponent>(glm::vec2(128 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    gate.AddComponent<AnimationComponent>(8, 8, true);

    Entity platform = registry->CreateEntity();
    platform.AddComponent<SpriteComponent>("platform-image", 32, 32, 1);
    platform.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    platform.AddComponent<Transformcomponent>(glm::vec2(160 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    platform.AddComponent<AnimationComponent>(8, 4, true);

    Entity chest = registry->CreateEntity();
    chest.AddComponent<SpriteComponent>("chest-image", 32, 32, 1);
    chest.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chest.AddComponent<Transformcomponent>(glm::vec2(192 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    chest.AddComponent<AnimationComponent>(8, 7, true);

    Entity screen1 = registry->CreateEntity();
    screen1.AddComponent<SpriteComponent>("screen1-image", 32, 32, 1);
    screen1.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    screen1.AddComponent<Transformcomponent>(glm::vec2(224 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    screen1.AddComponent<AnimationComponent>(4, 7, true);

    Entity screen2 = registry->CreateEntity();
    screen2.AddComponent<SpriteComponent>("screen2-image", 32, 42, 1);
    screen2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    screen2.AddComponent<Transformcomponent>(glm::vec2(256 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    screen2.AddComponent<AnimationComponent>(4, 7, true);
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
    registry->GetSystem<RenderSystem>().Update(renderer, assetstore, camera);

    SDL_RenderPresent(renderer);
}

void Game::Update(){
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - milliSecondsPreviousFrame);
    if(timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME){
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - milliSecondsPreviousFrame)/1000.0;
    milliSecondsPreviousFrame = SDL_GetTicks();

    registry->Update();

    registry->GetSystem<AnimationSystem>().Update();

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