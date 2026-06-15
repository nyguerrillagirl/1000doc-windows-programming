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
    if (SDLNet_ResolveHost(&ip, NULL, 1234) < 0) {
        printf("ResolveHost failed: %s\n", SDLNet_GetError());
        return 1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        printf("TCP_Open failed: %s\n", SDLNet_GetError());
        return 1;
    }

    printf("Server listening on port 1234...\n");

    TCPsocket client = SDLNet_TCP_Accept(server);
    if (!client) {
        printf("No client connected.\n");
        return 1;
    }

    printf("Client connected!\n");

    char buffer[256];
    int len = SDLNet_TCP_Recv(client, buffer, sizeof(buffer)-1);
    if (len > 0) {
        buffer[len] = '\0';
        printf("Received: %s\n", buffer);
    } else {
        printf("Recv failed.\n");
    }

    SDLNet_TCP_Close(client);
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
