#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

int main(int argc, char* args[]) {
    int screenWidth = 800;
    int screenHeight = 600;
    // 1. Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Initialize SDL_image for PNG/JPG support
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    // 3. Initialize SDL_ttf for fonts
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    // 4. Initialize SDL_mixer for audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Create Window and Renderer
    SDL_Window* window = SDL_CreateWindow("SDL Extensions Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- SDL_IMAGE DEMO: Load a Background ---
    // Ensure you have a 'background.png' in your project directory
    SDL_Texture* background = IMG_LoadTexture(renderer, "background.png");

    // --- SDL_TTF DEMO: Render Text ---
    // Ensure you have a TrueType font file in your directory
    TTF_Font* font = TTF_OpenFont("sample.ttf", 48);
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Let's center text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Press SPACE to play sound!", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // 3. Define the destination rectangle and center it mathematically
    SDL_Rect destRect;
    destRect.w = textSurface->w; // Use surface dimensions
    destRect.h = textSurface->h;
    destRect.x = (screenWidth - destRect.w) / 2; // Center horizontally
    destRect.y = (screenHeight - destRect.h) / 2; // Center vertically

    // --- SDL_MIXER DEMO: Load Sound ---
    // Ensure you have a 'sound.wav' in your project directory
    Mix_Chunk* soundEffect = Mix_LoadWAV("sound.wav");

    // Game Loop
    bool quit = false;
    SDL_Event e;
    bool played_sound = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE && soundEffect != nullptr && !played_sound) {
                    played_sound = true;
                    Mix_PlayChannel(-1, soundEffect, 0); // Play sound on first free channel
                }
            }
        }

        // Render
        SDL_RenderClear(renderer);

        // Render Background (if loaded successfully)
        if (background) {
            SDL_RenderCopy(renderer, background, NULL, NULL);
        }
        // Render Text
        SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

        SDL_RenderPresent(renderer);
    }

    // Clean Up Resources
    Mix_FreeChunk(soundEffect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyTexture(background);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit Libraries
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
