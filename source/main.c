/*================================================================
    * main.c
    *
    * Copyright (c) 2021 Lauri Räsänen
    * ================================

    The main entry point of my_game.
=================================================================*/

#define GS_IMPL
#include <gs/gs.h>
#define GS_IMMEDIATE_DRAW_IMPL
#include <gs/util/gs_idraw.h>

#include "bsp/bsp_loader.c"
#include "bsp/bsp_map.c"
#include "entities/player.c"
#include "graphics/rendercontext.c"
#include "util/config.c"

bsp_map_t *bsp_map = NULL;
mg_player_t *player = NULL;

void z_up()
{
    // Orient coordinate system
    gs_absolute_up = gs_v3(0, 0, 1.0f);
    gs_absolute_forward = gs_v3(0, 1.0f, 0);
    gs_absolute_right = gs_v3(1.0f, 0, 0);
}

void y_up()
{
    // Orient coordinate system
    gs_absolute_up = gs_v3(0, 1.0f, 0);
    gs_absolute_forward = gs_v3(0, 0, -1.0f);
    gs_absolute_right = gs_v3(1.0f, 0, 0);
}

void app_init()
{
    z_up();

    render_ctx_init();
    render_ctx_use_immediate_mode = true;

    // Lock mouse at start by default
    //gs_platform_lock_mouse(gs_platform_main_window(), true);
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(gs_platform_raw_window_handle(gs_platform_main_window()), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    bsp_map = gs_malloc_init(bsp_map_t);
    load_bsp("maps/q3dm1.bsp", bsp_map);

    if (bsp_map->valid)
    {
        bsp_map_init(bsp_map);
    }

    player = mg_player_new();
    app_spawn();
}

void app_spawn()
{
    if (bsp_map->valid)
    {
        player->camera.pitch = 0;
        bsp_map_find_spawn_point(bsp_map, &player->transform.position, &player->yaw);
        player->yaw -= 90;
    }
}

void app_update()
{
    if (gs_platform_key_pressed(GS_KEYCODE_ESC))
        gs_engine_quit();

    if (gs_platform_key_pressed(GS_KEYCODE_R))
        app_spawn();

    if (gs_platform_key_pressed(GS_KEYCODE_F1))
    {
        uint32_t main_window = gs_platform_main_window();

        // TODO: monitor size should probably be in the api
        GLFWvidmode *vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        bool32_t want_fullscreen = !gs_platform_window_fullscreen(main_window);
        gs_platform_set_window_fullscreen(main_window, want_fullscreen);

        if (!want_fullscreen)
        {
            gs_platform_set_window_size(main_window, 800, 600);

            // Going back to windowed mode,
            // restore window to center of screen.
            gs_vec2 window_size = gs_platform_window_sizev(main_window);
            gs_vec2 monitor_size = gs_v2(vid_mode->width, vid_mode->height);

            // Set position
            gs_vec2 top_left = gs_vec2_scale(gs_vec2_sub(monitor_size, window_size), 0.5f);
            gs_platform_set_window_positionv(main_window, top_left);
        }
        else
        {
            // Set to fullscreen res
            gs_platform_set_window_size(main_window, vid_mode->width, vid_mode->height);
        }
    }

    // If click, then lock again (in case lost)
    if (gs_platform_mouse_pressed(GS_MOUSE_LBUTTON) && !gs_platform_mouse_locked())
    {
        gs_platform_lock_mouse(gs_platform_main_window(), true);
        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(gs_platform_raw_window_handle(gs_platform_main_window()), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }

    // Update player
    mg_player_update(player);

    // Update and render map
    if (bsp_map->valid)
    {
        bsp_map_update(bsp_map, player->camera.cam.transform.position);
        //bsp_map_render_immediate(bsp_map, &render_ctx_gsi, &player->camera.cam);
        bsp_map_render(bsp_map, &player->camera.cam);
    }

    // draw fps
    y_up();
    char temp[64];
    sprintf(temp, "fps: %d", (int)gs_round(1.0f / gs_platform_delta_time()));
    gsi_camera2D(&render_ctx_gsi);
    gsi_text(&render_ctx_gsi, 5, 15, temp, NULL, false, 255, 255, 255, 255);

    // draw map stats
    sprintf(temp, "map: %s", bsp_map->name);
    gsi_text(&render_ctx_gsi, 5, 30, temp, NULL, false, 255, 255, 255, 255);
    sprintf(temp, "tris: %zu/%zu", bsp_map->stats.visible_indices / 3, bsp_map->stats.total_indices / 3);
    gsi_text(&render_ctx_gsi, 10, 45, temp, NULL, false, 255, 255, 255, 255);
    sprintf(temp, "faces: %zu/%zu", bsp_map->stats.visible_faces, bsp_map->stats.total_faces);
    gsi_text(&render_ctx_gsi, 10, 60, temp, NULL, false, 255, 255, 255, 255);
    sprintf(temp, "patches: %zu/%zu", bsp_map->stats.visible_patches, bsp_map->stats.total_patches);
    gsi_text(&render_ctx_gsi, 10, 75, temp, NULL, false, 255, 255, 255, 255);
    sprintf(temp, "leaf: %zu", bsp_map->stats.current_leaf);
    gsi_text(&render_ctx_gsi, 10, 90, temp, NULL, false, 255, 255, 255, 255);
    z_up();

    render_ctx_update();
}

void app_shutdown()
{
    mg_player_free(player);
    bsp_map_free(bsp_map);
    render_ctx_free();
    mg_config_free();
}

gs_app_desc_t gs_main(int32_t argc, char **argv)
{
    // Load config first so we can use resolution, etc.
    mg_config_init();

    return (gs_app_desc_t){
        .init = app_init,
        .update = app_update,
        .shutdown = app_shutdown,
        .window_flags = mg_config->video.fullscreen ? GS_WINDOW_FLAGS_FULLSCREEN : 0,
        .window_width = mg_config->video.width,
        .window_height = mg_config->video.height,
        .enable_vsync = mg_config->video.vsync,
        .frame_rate = mg_config->video.max_fps,
    };
}