#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
///////////////////////////////////////////////////////////
// some stuff needed for Discord RPC
////////////////////////////////////////////////////////////
// feel free to hide this if your ide supports it.
////////////////////////////////////////////////////////////

#ifdef DISCORD
#include <discord_rpc.h>

static const char* APPLICATION_ID = "905202859686129784"; // todo check if this gets freed
static int64_t StartTime;
static int SendPresence = 1;

static void updateDiscordPresence(char* message)
{
    time_t seconds;
    seconds = time(NULL);
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Bittens adventure";
    discordPresence.details = message;
    discordPresence.startTimestamp = seconds;
    discordPresence.endTimestamp = seconds+818;
    discordPresence.largeImageKey = "window";
    discordPresence.largeImageText = "bittens-adventure";
    Discord_UpdatePresence(&discordPresence);
}


static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
    int response = -1;
    char yn[4];
    printf("\nDiscord: join request from %s#%s - %s\n",
           request->username,
           request->discriminator,
           request->userId);
        response = DISCORD_REPLY_NO;
    if (response != -1) {
        Discord_Respond(request->userId, response);
    }
}

static void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

#endif

// bitten headers
#include "sdl_bittendef.h"
#include "r_render.h"
#include "i_event.h"
#include "e_entity.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO > 0))
	{
		printf("ERROR: SDL has failed to init %s\n", SDL_GetError());
	}
	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		printf("ERROR: SDL_image has failed to init png %s\n", SDL_GetError());
	}
#ifdef DISCORD
	discordInit();
	updateDiscordPresence("title screen");
#endif
	RenderWindow(GAME_NAME, SCREENWIDTH, SCREENHEIGHT);
	
	// load sprite
	e_entity player;
	player.sprite = loadTexture("../assets/bitten.png");
	player.src.x = 0;
	player.src.y = 0;
	player.src.w = 32;
	player.src.h = 32;

	player.dst.x = 0;
	player.dst.y = 0;
	player.dst.w = 32;
	player.dst.h = 32;
	
	// keep on running game while it is open
	
	bool gameRunning = true;

	while (gameRunning)
	{
		i_poll(&gameRunning);
		r_clear();
		player.src.x=64;
		r_renderer(&player);
		r_display();
	}
	CloseWindow();
	SDL_Quit();
	return 0;
}
