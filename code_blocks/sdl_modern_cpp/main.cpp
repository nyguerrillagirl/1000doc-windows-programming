#include <SDL.h>
#include <memory>
#include <stdexcept>
#include <iostream>

// Custom deleters for smart pointers
struct SDL_Deleter {
    void operator()(SDL_Window* w) const noexcept { SDL_DestroyWindow(w); }
    void operator()(SDL_Renderer* r) const noexcept { SDL_DestroyRenderer(r); }
};

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    // Ensure SDL_Quit() always runs
    atexit(SDL_Quit);

    // Create window (smart pointer)
    std::unique_ptr<SDL_Window, SDL_Deleter> window(
        SDL_CreateWindow(
            "Modern C++ SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_SHOWN
        )
    );

    if (!window) {
        throw std::runtime_error("Failed to create window");
    }

    // Create renderer (smart pointer)
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
    );

    if (!renderer) {
        throw std::runtime_error("Failed to create renderer");
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Light blue color (R,G,B,A)
        SDL_SetRenderDrawColor(renderer.get(), 173, 216, 230, 255);

        SDL_RenderClear(renderer.get());
        SDL_RenderPresent(renderer.get());
    }

    return 0;
}
