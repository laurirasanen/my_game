#ifndef MG_GAME_MANAGER_H
#define MG_GAME_MANAGER_H

#include <gs/gs.h>

#include "../bsp/bsp_loader.h"
#include "../bsp/bsp_map.h"
#include "../entities/player.h"

typedef struct mg_game_manager_t
{
	bsp_map_t *map;
	mg_player_t *player;
} mg_game_manager_t;

typedef struct mg_player_input_t
{
	gs_vec2 delta_aim;
	gs_vec2 move;
	bool jump;
	bool crouch;
	bool shoot;
	int32_t wish_slot;
} mg_player_input_t;

void mg_game_manager_init();
void mg_game_manager_free();
void mg_game_manager_update();

void mg_game_manager_load_map(char *filename);
void mg_game_manager_spawn_player();

mg_player_input_t mg_game_manager_get_input();
void mg_game_manager_input_alive();
void mg_game_manager_input_console();
void mg_game_manager_input_menu();
void mg_game_manager_input_general();

extern mg_game_manager_t *g_game_manager;

#endif // MG_GAME_MANAGER_H