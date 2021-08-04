/*================================================================
    * util/transform.h
    *
    * Copyright (c) 2021 Lauri Räsänen
    * ================================

    ...
=================================================================*/

#ifndef MG_TRANSFORM_H
#define MG_TRANSFORM_H

#include <gs/gs.h>

static inline gs_vec3 mg_get_forward(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_absolute_forward));
}

static inline gs_vec3 mg_get_backward(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_vec3_scale(gs_absolute_forward, -1.0f)));
}

static inline gs_vec3 mg_get_up(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_absolute_up));
}

static inline gs_vec3 mg_get_down(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_vec3_scale(gs_absolute_up, -1.0f)));
}

static inline gs_vec3 mg_get_right(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_absolute_right));
}

static inline gs_vec3 mg_get_left(gs_quat rotation)
{
    return (gs_quat_rotate(rotation, gs_vec3_scale(gs_absolute_right, -1.0f)));
}

#endif // MG_TRANSFORM_H