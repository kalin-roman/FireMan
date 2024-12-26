#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


Assetstore::Assetstore(){
    Logger::Log("Assetstore constructor called");
}

Assetstore::~Assetstore(){
    ClearAssets();
    Logger::Log("Assetstore destructor called");
    
}

void Assetstore::ClearAssets(){
    for(auto texture: textures){
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();

    for(auto font:fonts){
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

void Assetstore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath){
    SDL_Surface* surface= IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
    Logger::Log("New texture has been added to AssetStore with ID = "+ assetId);
}

SDL_Texture* Assetstore::GetTexture(const std::string& assetId){
    return textures[assetId];
}

std::map<std::string, SDL_Texture*> Assetstore::GetTextureList(){
    return textures;
}

void Assetstore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize){
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));

};

TTF_Font* Assetstore::GetFont(const std::string& assetId){
    return fonts[assetId];
};

