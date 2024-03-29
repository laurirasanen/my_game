/*================================================================
	* entities/weapon.h
	*
	* Copyright (c) 2022 Lauri Räsänen
	* ================================

	...
=================================================================*/

#ifndef MG_WEAPON_H
#define MG_WEAPON_H

#include <gs/gs.h>

#include "../graphics/model_manager.h"

typedef enum mg_ammo_type
{
	MG_AMMO_BULLET,
	MG_AMMO_ROCKET,
	MG_AMMO_COUNT,
} mg_ammo_type;

typedef enum mg_weapon_type
{
	MG_WEAPON_MACHINE_GUN,
	MG_WEAPON_ROCKET_LAUNCHER,
	MG_WEAPON_COUNT,
} mg_weapon_type;

typedef enum mg_weapon_shoot_result
{
	MG_WEAPON_RESULT_SHOT,
	MG_WEAPON_RESULT_COOLDOWN,
	MG_WEAPON_RESULT_NO_AMMO,
	MG_WEAPON_RESULT_COUNT,
} mg_weapon_shoot_result;

typedef struct mg_weapon_t
{
	mg_weapon_type type;
	mg_model_t *model;
	gs_vqs transform;
	gs_vec3 view_scale;
	uint32_t renderable_id;
	bool uses_ammo;
	mg_ammo_type ammo_type;
	uint16_t ammo_current;
	uint16_t ammo_max;
	double shoot_interval;
	double last_shot;
} mg_weapon_t;

mg_weapon_t *mg_weapon_create(mg_weapon_type type);
void mg_weapon_free(mg_weapon_t *weapon);
mg_weapon_shoot_result mg_weapon_shoot(mg_weapon_t *weapon, gs_vqs origin);

#endif // MG_WEAPON_H