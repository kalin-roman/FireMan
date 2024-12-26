#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Assetstore{
    private:
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;

    public:
    Assetstore();
    ~Assetstore();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetId);
    std::map<std::string, SDL_Texture*> GetTextureList();
    
    void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
    TTF_Font* GetFont(const std::string& assetId);

};

#endif