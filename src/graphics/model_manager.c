/*================================================================
	* graphics/model_manager.c
	*
	* Copyright (c) 2021 Lauri Räsänen
	* ================================

	...
=================================================================*/

#include "model_manager.h"
#include "../game/console.h"
#include "../util/string.h"

mg_model_manager_t *g_model_manager;

void mg_model_manager_init()
{
	// Allocate
	g_model_manager		= gs_malloc_init(mg_model_manager_t);
	g_model_manager->models = gs_dyn_array_new(mg_model_t);

	// Test
	_mg_model_manager_load("players/sarge/head.md3", "basic");
	_mg_model_manager_load("players/sarge/upper.md3", "basic");
	_mg_model_manager_load("players/sarge/lower.md3", "basic");
}

void mg_model_manager_free()
{
	for (size_t i = 0; i < gs_dyn_array_size(g_model_manager->models); i++)
	{
		mg_free_md3(g_model_manager->models[i].data);
	}

	gs_dyn_array_free(g_model_manager->models);

	gs_free(g_model_manager);
	g_model_manager = NULL;
}

mg_model_t *mg_model_manager_find(const char *filename)
{
	for (size_t i = 0; i < gs_dyn_array_size(g_model_manager->models); i++)
	{
		if (strcmp(filename, g_model_manager->models[i].filename) == 0)
		{
			return &g_model_manager->models[i];
		}
	}

	mg_println("WARN: mg_model_manager_find invalid model %s", filename);
	return NULL;
}

mg_model_t *mg_model_manager_find_or_load(const char *filename, const char *shader)
{
	for (size_t i = 0; i < gs_dyn_array_size(g_model_manager->models); i++)
	{
		if (strcmp(filename, g_model_manager->models[i].filename) == 0)
		{
			return &g_model_manager->models[i];
		}
	}

	if (_mg_model_manager_load(filename, shader))
	{
		return &gs_dyn_array_back(g_model_manager->models);
	}

	mg_println("WARN: mg_model_manager_find_or_load invalid model %s", filename);

	return NULL;
}

bool _mg_model_manager_load(const char *filename, const char *shader)
{
	char *path = mg_append_string("assets/models/", filename);

	md3_t *data = gs_malloc_init(md3_t);
	if (!mg_load_md3(path, data))
	{
		mg_println("WARN: _mg_model_manager_load failed, model %s", filename);
		mg_free_md3(data);
		gs_free(path);
		return false;
	}

	mg_model_t model = {
		.filename = filename,
		.shader	  = shader,
		.data	  = data,
	};

	gs_dyn_array_push(g_model_manager->models, model);

	mg_println("Model: Loaded %s", filename);
	gs_free(path);
	return true;
}