//
// Created by darkhead on 21/5/20.
//

#include <SDL_image.h>
#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(int totalWidth, int totalHeight) :
                        total_width(totalWidth), total_height(totalHeight)
{
    atlas_surface = SDL_CreateRGBSurface(0, totalWidth, totalHeight,32, 0, 0, 0, 0 );

    mask = new bool[total_width * total_height];

    for (int i = 0; i < (total_width * total_height) ; i++) {
        mask[ i ] = false;
    }
}

SDL_Surface *TextureAtlas::getAtlasSurface() const {
    return atlas_surface;
}

bool TextureAtlas::addTexture(SDL_surface *texture_surface)
{

    int texw = texture_surface->w;
    int texh = texture_surface->h;

    for (int y = 0 ; y < total_height ; y++) {
        for (int x = 0 ; x < total_width ; x++) {

            if ( this->checkForAllocate(x, y, texw, texh ) ) {
                Logging::getInstance()->Log("Add texture to atlas SUCCESS(" + texture->getFilename() + ", x: "+ std::to_string(x)+", y: " +std::to_string(y) + ", width: "+ std::to_string(texw)+", height:" + std::to_string(texh) + ")!");

                TextureAtlasImageInfo t_info;
                t_info.name = texture->getFilename();
                t_info.x = (float) x;
                t_info.y = (float) y;
                t_info.width = (float) texw;
                t_info.height = (float) texh;

                textures_info.push_back( t_info );
                textures.push_back( texture );

                allocateMask(x, y, texw, texh);

                SDL_Rect r;
                r.x = x;
                r.y = y;
                r.w = texw;
                r.h = texh;

                SDL_BlitSurface( texture_surface, NULL, atlas_surface, &r);

                return true;
            }
        }
    }

    Logging::getInstance()->Log("addTexture failed!", "ERROR");

    return false;
}

bool TextureAtlas::checkForAllocate(int xpos, int ypos, int width, int height)
{
    int baseOffset = ypos * total_width + xpos;
    int max_global_x = (total_width * ypos) + total_width;

    if (baseOffset + width > max_global_x) {
        //std::string msg = "checkForAllocate hit final line!! w: " + std::to_string(width) + ", h: " + std::to_string(height) + ", baseOffset: " + std::to_string(baseOffset) + ", max_global_x:" + std::to_string(max_global_x);
        //Logging::getInstance()->Log(msg);
        return false;
    }

    for (int y = 0 ; y < height ; y++) {
        for (int x = 0 ; x < width ; x++) {
            int localIndex = y * width + x;
            int globalIndex = baseOffset + localIndex;

            if (mask[globalIndex]) {
                return false;
            }
        }

        baseOffset += total_width - width;
    }

    return true;
}

void TextureAtlas::allocateMask(int xpos, int ypos, int width, int height)
{
    int baseOffset = ypos * total_width + xpos;

    for (int y = 0 ; y < height ; y++) {
        for (int x = 0 ; x < width ; x++) {
            int localIndex = y * width + x;
            int globalIndex = baseOffset + localIndex;

            mask[ globalIndex ] = true;
        }
        baseOffset += total_width - width;
    }
}

TextureAtlasImageInfo TextureAtlas::getAtlasTextureInfoForName(std::string name)
{
    for (int i = 0; i < this->textures_info.size(); i++ ) {
        if (!textures_info[i].name.compare( name )) {
            return textures_info[i];
        }
    }
}

void TextureAtlas::saveJPG(std::string name)
{
    IMG_SavePNG(getAtlasSurface(), name.c_str());
}

