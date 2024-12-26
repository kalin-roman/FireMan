#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "../ECS/Registry.h"
#include "../AssetStore/AssetStore.h"
// #include "../EventBus/EventBus.h"



const int FPS =  60;
const int MILLISECS_PER_FRAME = 1000/FPS;

class Game{
    private:
        bool isRunning;
        // bool oYel;
        int milliSecondsPreviousFrame = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unique_ptr <Registry> registry;
        std::unique_ptr <Assetstore> assetstore;
        // std::unique_ptr <EventBus> eventBus;
        // SDL_Rect camera;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void LevelLoad(int LevelNum);
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        static int displayWidth;
        static int displayHeight;
        static int mapWidth;
        static int mapHeight;


};

#endif