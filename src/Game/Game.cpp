
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
#include "../Components/KeyBoardControlComponent.h"
#include "../Components/WeightComponent.h"


#include "../ECS/ECS.h"
#include "../System/RenderSystem.h"
#include "../System/AnimationSystem.h"
#include "../System/KeyBoardMovementSystem.h"
#include "../System/Movementsystem.h"
#include "../System/WeightSystem.h"



int Game::displayWidth;
int Game::displayHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game(){
    isRunning = false;
    oYel = false;
    registry = std::make_unique<Registry>();
    assetstore = std::make_unique<Assetstore>();
    eventBus = std::make_unique<EventBus>();
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

    SDL_SetWindowFullscreen(window, SDL_WINDOW_BORDERLESS);
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
    assetstore->AddTexture(renderer, "Player", "./assets/images/Player/Punk.png");
    assetstore->AddTexture(renderer, "Tiles", "./assets/tilemaps/Tiles.png");
    assetstore->AddTexture(renderer, "hammer", "./assets/images/Hammer.png");
    assetstore->AddTexture(renderer, "gates", "./assets/images/Entry.png");
    assetstore->AddTexture(renderer, "platform", "./assets/images/Platform.png");
    assetstore->AddTexture(renderer, "chest", "./assets/images/Chest.png");
    assetstore->AddTexture(renderer, "screen1", "./assets/images/Screen1.png");
    assetstore->AddTexture(renderer, "screen2", "./assets/images/Screen2.png");
    assetstore->AddTexture(renderer, "money", "./assets/images/Money.png");
    assetstore->AddTexture(renderer, "card", "./assets/images/Card.png");
    assetstore->AddTexture(renderer, "transporter1", "./assets/images/Transporter1.png");
    assetstore->AddTexture(renderer, "transporter2", "./assets/images/Transporter2.png");
    assetstore->AddTexture(renderer, "transporter3", "./assets/images/Transporter3.png");
    assetstore->AddTexture(renderer, "fence1", "./assets/images/Objects/Fence1.png");
    assetstore->AddTexture(renderer, "fence2", "./assets/images/Objects/Fence2.png");
    assetstore->AddTexture(renderer, "fence3", "./assets/images/Objects/Fence3.png");
    assetstore->AddTexture(renderer, "locker1", "./assets/images/Objects/Locker1.png");
    assetstore->AddTexture(renderer, "ladder1", "./assets/images/Objects/Ladder1.png");
    assetstore->AddTexture(renderer, "ladder2", "./assets/images/Objects/Ladder2.png");
    assetstore->AddTexture(renderer, "ladder3", "./assets/images/Objects/Ladder3.png");


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
    for (auto i : map){
        i.clear();
    }
    map.clear();
    std::string delimiter = ",";
    while (getline(file, line)) {
        size_t pos = 0;
        col = 0;
        std::vector<bool> mapLine;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            int n = std::stoi(line.substr(0, pos)) - 1;
            mapLine.push_back(false);
            if (n >= 0){
                int y = (int)floor(n/10);
                int x = n - y * 10;
                std::vector<bool>::iterator end = mapLine.end() - 1;
                *end = !(x < 3 && y < 6);
                // mapLine[mapLine.size() - 1] = (!(x < 3 && y < 6));
                Entity tile = registry->CreateEntity();
                tile.Group("Map");
                tile.AddComponent<SpriteComponent>("Tiles", sizeOfPic, sizeOfPic, 0, false, x * sizeOfPic, y * sizeOfPic);
                tile.AddComponent<Transformcomponent>(glm::vec2(col * sizeOfPic * scaleT, row * sizeOfPic * scaleT), glm::vec2(scaleT, scaleT), 0.0);
            }
            line.erase(0, pos + delimiter.length());
            col++;
            /* code */
        }
        map.push_back(mapLine);
        row++;
    }
    file.close();

    for (auto i : map){
        for (auto j : i){
            std::cout << j << ", ";
        }
        std::cout << "\n";
    }

    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<KeyBoardMovementSystem>();
    registry->AddSystem<Movementsystem>();
    registry->AddSystem<WeightSystem>();


    Entity player = registry->CreateEntity();
    player.Group("Player");
    player.AddComponent<SpriteComponent>("Player", 48, 48, 8);
    player.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    player.AddComponent<Transformcomponent>(glm::vec2(0 * scaleT, 0 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    player.AddComponent<AnimationComponent>(4, 8, true);//, 0, SDL_FLIP_NONE);
    player.AddComponent<KeyBoardControlComponent>(glm::vec2(0, -100.0), glm::vec2(200, 0), glm::vec2(0, 100), glm::vec2(-200,0));
    player.AddComponent<WeightComponent>(100.0);
    // player.AddComponent<CameraFollowComponent>();


    Entity hammer = registry->CreateEntity();
    hammer.Group("hummer");
    hammer.AddComponent<SpriteComponent>("hammer", 32, 64, 1);
    hammer.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    hammer.AddComponent<Transformcomponent>(glm::vec2(320 * scaleT, 192 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    hammer.AddComponent<AnimationComponent>(8, 8, true);//, 0, SDL_FLIP_NONE);

    Entity hammer2 = registry->CreateEntity();
    hammer2.Group("hummer");
    hammer2.AddComponent<SpriteComponent>("hammer", 32, 64, 1);
    hammer2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    hammer2.AddComponent<Transformcomponent>(glm::vec2(384 * scaleT, 192 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    hammer2.AddComponent<AnimationComponent>(8, 8, true, 3);//, 3, SDL_FLIP_NONE);

    Entity gate = registry->CreateEntity();
    gate.AddComponent<SpriteComponent>("gates", 32, 64, 1);
    gate.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    gate.AddComponent<Transformcomponent>(glm::vec2(576 * scaleT, 192 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    gate.AddComponent<AnimationComponent>(8, 8, true);//, 0, SDL_FLIP_NONE);

    Entity platform = registry->CreateEntity();
    platform.AddComponent<SpriteComponent>("platform", 32, 32, 1);
    platform.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    platform.AddComponent<Transformcomponent>(glm::vec2(512 * scaleT, 256 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    platform.AddComponent<AnimationComponent>(8, 4, true);//, 0, SDL_FLIP_NONE);
    
    Entity platform2 = registry->CreateEntity();
    platform2.AddComponent<SpriteComponent>("platform", 32, 32, 1);
    platform2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    platform2.AddComponent<Transformcomponent>(glm::vec2(480 * scaleT, 256 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    platform2.AddComponent<AnimationComponent>(8, 4, true);//, 0, SDL_FLIP_NONE);

    Entity chest = registry->CreateEntity();
    chest.AddComponent<SpriteComponent>("chest", 32, 32, 1);
    chest.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chest.AddComponent<Transformcomponent>(glm::vec2(256 * scaleT, 320 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    chest.AddComponent<AnimationComponent>(8, 7, true);//, 0, SDL_FLIP_NONE);

    Entity screen1 = registry->CreateEntity();
    screen1.AddComponent<SpriteComponent>("screen1", 32, 32, 1);
    screen1.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    screen1.AddComponent<Transformcomponent>(glm::vec2(120 * scaleT, 180 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    screen1.AddComponent<AnimationComponent>(4, 7, true);//, 0, SDL_FLIP_NONE);

    Entity screen2 = registry->CreateEntity();
    screen2.AddComponent<SpriteComponent>("screen2", 32, 42, 1);
    screen2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    screen2.AddComponent<Transformcomponent>(glm::vec2(544 * scaleT, 216 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    screen2.AddComponent<AnimationComponent>(4, 7, true);//, 0, SDL_FLIP_NONE);

    for (int i = 0; i < 3; i++)
    {
        Entity money = registry->CreateEntity();
        money.AddComponent<SpriteComponent>("money", 24, 24, 3);
        money.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        money.AddComponent<Transformcomponent>(glm::vec2((325 + i * 32) * scaleT, 132 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
        money.AddComponent<AnimationComponent>(6, 9, true);//, i, 0, SDL_FLIP_NONE);
    }

    Entity card = registry->CreateEntity();
    card.AddComponent<SpriteComponent>("card", 24, 24, 3);
    card.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    card.AddComponent<Transformcomponent>(glm::vec2(524 * scaleT, 352 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    card.AddComponent<AnimationComponent>(6, 9, true);//, 0, SDL_FLIP_NONE);

    Entity transporter1 = registry->CreateEntity();
    transporter1.AddComponent<SpriteComponent>("transporter1", 32, 32, 1);
    transporter1.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    transporter1.AddComponent<Transformcomponent>(glm::vec2(256 * scaleT, 256 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    transporter1.AddComponent<AnimationComponent>(4, 9, true);//, 0, SDL_FLIP_NONE);

    for (int i = 0; i < 5; i++)
    {
        Entity transporter2 = registry->CreateEntity();
        transporter2.AddComponent<SpriteComponent>("transporter2", 32, 32, 1);
        transporter2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
        transporter2.AddComponent<Transformcomponent>(glm::vec2((288 + 32 * i) * scaleT, 256 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
        transporter2.AddComponent<AnimationComponent>(4, 9, true);//, 0, SDL_FLIP_NONE);
    }
        

    Entity transporter3 = registry->CreateEntity();
    transporter3.AddComponent<SpriteComponent>("transporter3", 32, 32, 1);
    transporter3.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    transporter3.AddComponent<Transformcomponent>(glm::vec2(448 * scaleT, 256 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    transporter3.AddComponent<AnimationComponent>(4, 9, true);//, 0, SDL_FLIP_NONE);

    Entity fence = registry->CreateEntity();
    fence.AddComponent<SpriteComponent>("fence1", 32, 32, 1);
    fence.AddComponent<Transformcomponent>(glm::vec2(320 * scaleT, 128 * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    
    Entity fence2 = registry->CreateEntity();
    fence2.AddComponent<SpriteComponent>("fence2", 32, 32, 1);
    fence2.AddComponent<Transformcomponent>(glm::vec2(352 * scaleT, 128 * scaleT), glm::vec2(scaleT, scaleT), 0.0);

    Entity fence3 = registry->CreateEntity();
    fence3.AddComponent<SpriteComponent>("fence3", 32, 32, 1);
    fence3.AddComponent<Transformcomponent>(glm::vec2(384 * scaleT, 128 * scaleT), glm::vec2(scaleT, scaleT), 0.0);

    Entity locker = registry->CreateEntity();
    locker.AddComponent<SpriteComponent>("locker1",32,32,1);
    locker.AddComponent<Transformcomponent>(glm::vec2(64 * scaleT, 208 * scaleT), glm::vec2(scaleT, scaleT), 0.0);

    Entity ladder3 = registry->CreateEntity();
    ladder3.AddComponent<SpriteComponent>("ladder3",32,32,1);
    ladder3.AddComponent<Transformcomponent>(glm::vec2(0 * scaleT, 96 * scaleT), glm::vec2(scaleT, scaleT), 0.0);

    Entity ladder4 = registry->CreateEntity();
    ladder4.AddComponent<SpriteComponent>("ladder2",32,32,1);
    ladder4.AddComponent<Transformcomponent>(glm::vec2(0 * scaleT, 128 * scaleT), glm::vec2(scaleT, scaleT), 0.0);


    Entity ladder2 = registry->CreateEntity();
    ladder2.AddComponent<SpriteComponent>("ladder2",32,32,1);
    ladder2.AddComponent<Transformcomponent>(glm::vec2(0 * scaleT, 160 * scaleT), glm::vec2(scaleT, scaleT), 0.0);

    for (int i = 0; i < 2; i++){
        Entity ladder1 = registry->CreateEntity();
        ladder1.AddComponent<SpriteComponent>("ladder1",32,32,1);
        ladder1.AddComponent<Transformcomponent>(glm::vec2(0 * scaleT, (188 + i * 32) * scaleT), glm::vec2(scaleT, scaleT), 0.0);
    }

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
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);

                oYel = sdlEvent.key.keysym.sym == SDLK_d && !oYel;

                break;
            case SDL_KEYUP:
                eventBus->EmitEvent<KeyUpEvent>(sdlEvent.key.keysym.sym);
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

    registry->GetSystem<KeyBoardMovementSystem>().SubscribeToEvent(eventBus);

    registry->Update();

    registry->GetSystem<Movementsystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<WeightSystem>().Update(registry, map);



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