/*================================================================
	* bsp/bsp_map.h
	*
	* Copyright (c) 2021 Lauri Räsänen
	* ================================

	...
=================================================================*/

#ifndef BSP_MAP_H
#define BSP_MAP_H

#include <gs/gs.h>
#include <gs/util/gs_idraw.h>

#include "../graphics/types.h"
#include "../util/math.h"
#include "../util/string.h"
#include "bsp_entity.h"
#include "bsp_patch.h"
#include "bsp_types.h"

void bsp_map_init(bsp_map_t *map);
void _bsp_load_entities(bsp_map_t *map);
void _bsp_load_textures(bsp_map_t *map);
void _bsp_load_lightmaps(bsp_map_t *map);
void _bsp_load_lightvols(bsp_map_t *map);
void _bsp_create_patch(bsp_map_t *map, bsp_face_lump_t face);
void _bsp_map_create_buffers(bsp_map_t *map);
void bsp_map_update(bsp_map_t *map, gs_camera_t *cam, const gs_vec2 fb);
void bsp_map_render_immediate(bsp_map_t *map, gs_immediate_draw_t *gsi, gs_camera_t *cam);
void bsp_map_render(bsp_map_t *map, gs_camera_t *cam, gs_handle(gs_graphics_renderpass_t) rp, gs_command_buffer_t *cb, const gs_vec2 fb);
void bsp_map_find_spawn_point(bsp_map_t *map, gs_vec3 *position, float32_t *yaw);
void bsp_map_free(bsp_map_t *map);
int32_t _bsp_find_camera_leaf(bsp_map_t *map, gs_vec3 view_position);
void _bsp_calculate_visible_faces(bsp_map_t *map, int32_t leaf, gs_camera_t *cam, const gs_vec2 fb);
bool32_t _bsp_cluster_visible(bsp_map_t *map, int32_t view_cluster, int32_t test_cluster);
bsp_lightvol_lump_t bsp_get_lightvol(bsp_map_t *map, gs_vec3 position, gs_vec3 *center);
mg_renderer_light_t bsp_sample_lightvol(bsp_map_t *map, gs_vec3 position);

#endif // BSP_MAP_H