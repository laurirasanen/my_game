/*================================================================
    * bsp/bsp_map.c
    *
    * Copyright (c) 2021 Lauri Räsänen
    * Copyright (c) 2018 Krzysztof Kondrak
    *
    * See README.md for license.
    * ================================

    BSP rendering.
=================================================================*/

#include <gs/util/gs_idraw.h>

#include "bsp_patch.c"
#include "bsp_types.h"

void _bsp_load_entities(bsp_map_t *map);
void _bsp_load_textures(bsp_map_t *map);
void _bsp_load_lightmaps(bsp_map_t *map);
void _bsp_load_lightvols(bsp_map_t *map);
void _bsp_create_patch(bsp_map_t *map, bsp_face_lump_t face);

void bsp_map_init(bsp_map_t *map)
{
    if (map->faces.count == 0)
    {
        return;
    }

    // Init dynamic arrays
    gs_dyn_array_reserve(map->render_faces, map->faces.count);
    gs_dyn_array_reserve(map->visible_faces, 1);
    gs_dyn_array_reserve(map->patches, 1);

    // Load stuff
    _bsp_load_entities(map);
    _bsp_load_textures(map);
    _bsp_load_lightmaps(map);
    _bsp_load_lightvols(map);

    // Data agregators for vertex and index buffer creation
    gs_dyn_array(bsp_face_lump_t) face_data = gs_dyn_array_new(bsp_face_lump_t);
    gs_dyn_array(bsp_patch_t) patch_data = gs_dyn_array_new(bsp_patch_t);

    uint32_t face_array_idx = 0;
    uint32_t patch_array_idx = 0;

    // Create renderable faces and patches
    for (size_t i = 0; i < map->faces.count; i++)
    {
        bsp_face_renderable_t face = {
            .type = map->faces.data[i].type,
        };

        if (face.type == BSP_FACE_TYPE_PATCH)
        {
            _bsp_create_patch(map, map->faces.data[i]);
            // index to map->patches
            face.index = patch_array_idx;
            patch_array_idx++;
        }
        else
        {
            gs_dyn_array_push(face_data, map->faces.data[i]);
            // index to map->faces
            face.index = i;
            face_array_idx++;
        }

        gs_dyn_array_push(map->render_faces, face);
    }

    // Create buffers
    // TODO

    gs_dyn_array_free(face_data);
    face_data = NULL;
    gs_dyn_array_free(patch_data);
    patch_data = NULL;

    // Static stats
    map->stats.total_vertices = map->vertices.count;
    map->stats.total_faces = map->faces.count;
    map->stats.total_patches = patch_array_idx;
}

void _bsp_load_entities(bsp_map_t *map)
{
}

void _bsp_load_textures(bsp_map_t *map)
{
    int32_t num_textures = map->header.dir_entries[BSP_LUMP_TYPE_TEXTURES].length / sizeof(bsp_texture_lump_t);

    map->stats.total_textures = num_textures;

    map->texture_handles.count = num_textures;
    map->texture_handles.data = gs_malloc(sizeof(bsp_texture_handle_t) * num_textures);
    for (size_t i = 0; i < num_textures; i++)
    {
        map->texture_handles.data[i].data = NULL;
    }

    char extensions[2][5] = {
        ".jpg",
        ".tga",
    };

    for (size_t i = 0; i < map->faces.count; i++)
    {
        int32_t texture_index = map->faces.data[i].texture;

        if (texture_index < 0)
        {
            continue;
        }

        // Don't attempt to load the same texture multiple times if already loaded
        if (map->texture_handles.data[texture_index].data != NULL)
        {
            continue;
        }

        // Don't attempt to load the same texture multiple times if failed
        if (map->texture_handles.data[texture_index].load_attempts > 0)
        {
            map->faces.data[i].texture = -1;
            continue;
        }

        map->texture_handles.data[texture_index].name = map->textures.data[texture_index].name;

        bool32_t success = false;
        size_t malloc_sz = strlen(map->textures.data[texture_index].name) + 5;
        char *filename = gs_malloc(malloc_sz);
        memset(filename, 0, malloc_sz);
        strcat(filename, map->textures.data[texture_index].name);
        strcat(filename, extensions[0]);

        for (size_t j = 0; j < 2; j++)
        {
            if (j > 0)
            {
                strcpy(filename + strlen(filename) - 4, extensions[j]);
            }

            map->texture_handles.data[texture_index].load_attempts++;

            if (gs_util_file_exists(filename))
            {
                success = gs_util_load_texture_data_from_file(
                    filename,
                    &map->texture_handles.data[texture_index].width,
                    &map->texture_handles.data[texture_index].height,
                    &map->texture_handles.data[texture_index].num_comps,
                    &map->texture_handles.data[texture_index].data,
                    true);
            }
            else if (j == 1)
            {
                gs_println("Warning: could not load texture: %s, file not found", map->textures.data[texture_index].name);
            }

            if (success)
            {
                map->stats.loaded_textures++;
                break;
            }
        }

        if (!success)
        {
            map->texture_handles.data[texture_index].data == NULL;
            map->faces.data[i].texture = -1;
        }

        gs_free(filename);
    }
}

void _bsp_load_lightmaps(bsp_map_t *map)
{
}

void _bsp_load_lightvols(bsp_map_t *map)
{
}

void _bsp_create_patch(bsp_map_t *map, bsp_face_lump_t face)
{
    bsp_patch_t patch = {
        .width = face.size[0],
        .height = face.size[1],
        .lightmap_idx = face.lm_index,
        .texture_idx = face.texture,
    };

    uint32_t num_patches_x = (patch.width - 1) >> 1;
    uint32_t num_patches_y = (patch.height - 1) >> 1;

    gs_dyn_array_reserve(patch.quadratic_patches, num_patches_x * num_patches_y);
    // not using push, increment size ourselves
    gs_dyn_array_head(patch.quadratic_patches)->size = num_patches_x * num_patches_y;

    for (size_t x = 0; x < num_patches_x; x++)
    {
        for (size_t y = 0; y < num_patches_y; y++)
        {
            uint32_t patch_idx = y * num_patches_x + x;

            bsp_quadratic_patch_t quadratic = {
                .tesselation = 10,
            };

            // Get the 9 vertices used as control points for this quadratic patch.
            for (size_t row = 0; row < 3; row++)
            {
                for (size_t col = 0; col < 3; col++)
                {
                    uint32_t control_point_idx = row * 3 + col;
                    // I understood this index when I wrote it but didn't add comments...
                    // Let's just hope I never have to debug this.
                    //                    offset               ???                             ???
                    uint32_t vertex_idx = face.first_vertex + (2 * y * patch.width + 2 * x) + (row * patch.width + col);
                    quadratic.control_points[control_point_idx] = map->vertices.data[vertex_idx];
                }
            }

            gs_dyn_array_set_data_i(&patch.quadratic_patches, &quadratic, sizeof(bsp_quadratic_patch_t), patch_idx);
            bsp_quadratic_patch_tesselate(&patch.quadratic_patches[patch_idx]);
        }
    }

    uint32_t temp = gs_dyn_array_size(patch.quadratic_patches);
    gs_dyn_array_push(map->patches, patch);
}

void bsp_map_update(bsp_map_t *map)
{
}

void bsp_map_render(bsp_map_t *map, gs_immediate_draw_t *gsi, gs_camera_t *cam)
{
    gsi_camera(gsi, cam);
    gsi_depth_enabled(gsi, true);
    //gsi_face_cull_enabled(gsi, true);

    for (size_t i = 0; i < gs_dyn_array_size(map->render_faces); i++)
    {
        int32_t index = map->render_faces[i].index;

        if (map->render_faces[i].type == BSP_FACE_TYPE_PATCH)
        {
            bsp_patch_t patch = map->patches[index];
            for (size_t j = 0; j < gs_dyn_array_size(patch.quadratic_patches); j++)
            {
                bsp_quadratic_patch_t quadratic = patch.quadratic_patches[j];

                for (size_t k = 0; k < bsp_quadratic_patch_index_count(&quadratic) - 2; k += 3)
                {
                    uint16_t index1 = quadratic.indices[k + 0];
                    uint16_t index2 = quadratic.indices[k + 1];
                    uint16_t index3 = quadratic.indices[k + 2];

                    gsi_trianglevx(
                        gsi,
                        quadratic.vertices[index1].position,
                        quadratic.vertices[index2].position,
                        quadratic.vertices[index3].position,
                        quadratic.vertices[index1].tex_coord,
                        quadratic.vertices[index1].tex_coord,
                        quadratic.vertices[index1].tex_coord,
                        quadratic.vertices[index1].color,
                        GS_GRAPHICS_PRIMITIVE_TRIANGLES);
                }
            }
        }
        else
        {
            bsp_face_lump_t face = map->faces.data[index];
            int32_t first_index = face.first_index;
            int32_t first_vertex = face.first_vertex;

            for (size_t j = 0; j < face.num_indices - 2; j += 3)
            {
                int32_t offset1 = map->indices.data[first_index + j + 0].offset;
                int32_t offset2 = map->indices.data[first_index + j + 1].offset;
                int32_t offset3 = map->indices.data[first_index + j + 2].offset;

                gsi_trianglevx(
                    gsi,
                    map->vertices.data[first_vertex + offset1].position,
                    map->vertices.data[first_vertex + offset2].position,
                    map->vertices.data[first_vertex + offset3].position,
                    map->vertices.data[first_vertex + offset1].tex_coord,
                    map->vertices.data[first_vertex + offset1].tex_coord,
                    map->vertices.data[first_vertex + offset1].tex_coord,
                    map->vertices.data[first_vertex + offset1].color,
                    GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            }
        }
    }
}

void bsp_map_free(bsp_map_t *map)
{
    if (map == NULL)
    {
        return;
    }

    for (size_t i = 0; i < gs_dyn_array_size(map->patches); i++)
    {
        bsp_patch_free(&map->patches[i]);
    }
    gs_dyn_array_free(map->patches);
    gs_dyn_array_free(map->visible_faces);
    gs_dyn_array_free(map->render_faces);

    map->patches = NULL;
    map->visible_faces = NULL;
    map->render_faces = NULL;

    for (size_t i = 0; i < map->texture_handles.count; i++)
    {
        gs_free(map->texture_handles.data[i].data);
        map->texture_handles.data[i].data = NULL;
    }
    gs_free(map->texture_handles.data);
    map->texture_handles.data = NULL;

    gs_free(map->entities.ents);
    gs_free(map->textures.data);
    gs_free(map->planes.data);
    gs_free(map->nodes.data);
    gs_free(map->leaves.data);
    gs_free(map->leaf_faces.data);
    gs_free(map->leaf_brushes.data);
    gs_free(map->models.data);
    gs_free(map->brushes.data);
    gs_free(map->brush_sides.data);
    gs_free(map->vertices.data);
    gs_free(map->indices.data);
    gs_free(map->effects.data);
    gs_free(map->faces.data);
    gs_free(map->lightmaps.data);
    gs_free(map->lightvols.data);
    gs_free(map->visdata.vecs);

    map->entities.ents = NULL;
    map->textures.data = NULL;
    map->planes.data = NULL;
    map->nodes.data = NULL;
    map->leaves.data = NULL;
    map->leaf_faces.data = NULL;
    map->leaf_brushes.data = NULL;
    map->models.data = NULL;
    map->brushes.data = NULL;
    map->brush_sides.data = NULL;
    map->vertices.data = NULL;
    map->indices.data = NULL;
    map->effects.data = NULL;
    map->faces.data = NULL;
    map->lightmaps.data = NULL;
    map->lightvols.data = NULL;
    map->visdata.vecs = NULL;

    gs_free(map);
    map = NULL;
}