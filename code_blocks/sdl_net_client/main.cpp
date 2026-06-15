#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(0) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", 1234) < 0) {
        printf("ResolveHost failed: %s\n", SDLNet_GetError());
        return 1;
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        printf("TCP_Open failed: %s\n", SDLNet_GetError());
        return 1;
    }

    const char* msg = "Hello from client!";
    SDLNet_TCP_Send(socket, msg, strlen(msg));

    SDLNet_TCP_Close(socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
