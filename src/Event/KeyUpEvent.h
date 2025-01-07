#ifndef KEYUPEVENT_H
#define KEYUPEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

class KeyUpEvent: public Event {
    public:

    SDL_Keycode key;
    
    KeyUpEvent(SDL_Keycode key): key(key){};
};

#endif