//
// Created by darkhead on 21/5/20.
//

#ifndef BRAKEDA3D_TEXTUREATLAS_H
#define BRAKEDA3D_TEXTUREATLAS_H

#include <vector>

struct TextureAtlasImageInfo {
    std::string name;
    float x;
    float y;
    float width;
    float height;
};

class TextureAtlas {

public:
    TextureAtlas(int totalWidth, int totalHeight);

    bool addTexture(SDL_Surface *texture);
    bool checkForAllocate(int xpos, int ypos, int width, int height);
    void allocateMask(int xpos, int ypos, int width, int height);
    void saveJPG(std::string name);
    SDL_Surface *getAtlasSurface() const;

    // atlas surface
    SDL_Surface *atlas_surface;

    // collection
    std::vector<SDL_Surface*> textures;

    // mapping atlas info
    std::vector<TextureAtlasImageInfo> textures_info;

    // get mapping atlas info for a texture name
    TextureAtlasImageInfo getAtlasTextureInfoForName(std::string name);

    int total_width;
    int total_height;
    bool *mask;
};


#endif //BRAKEDA3D_TEXTUREATLAS_H


