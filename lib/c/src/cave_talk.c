#include "cave_talk.h"

#include <stdbool.h>

#include "camera_movement.pb.h"
#include "ids.pb.h"
#include "lights.pb.h"
#include "mode.pb.h"
#include "movement.pb.h"
#include "ooga_booga.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

#include "cave_talk_link.h"
#include "cave_talk_types.h"

CaveTalk_Error_t CaveTalk_Hear(const CaveTalk_Handle_t *const handle)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakOogaBooga(const CaveTalk_Handle_t *const handle, const cave_talk_Say ooga_booga)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NULL;

    if ((NULL == handle) || (NULL == handle->buffer) || (NULL == handle->link_handle.send))
    {
    }
    else
    {
        pb_ostream_t        ostream            = pb_ostream_from_buffer(handle->buffer, handle->buffer_size);
        cave_talk_OogaBooga ooga_booga_message = cave_talk_OogaBooga_init_zero;

        ooga_booga_message.ooga_booga = ooga_booga;

        if (!pb_encode(&ostream, cave_talk_OogaBooga_fields, &ooga_booga_message))
        {
            error = CAVE_TALK_ERROR_SIZE;
        }
        else
        {
            error = CaveTalk_Speak(&handle->link_handle, (CaveTalk_Id_t)cave_talk_Id_ID_OOGA, handle->buffer, ostream.bytes_written);
        }
    }

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakMovement(const CaveTalk_Handle_t *const handle, const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NULL;

    if ((NULL == handle) || (NULL == handle->buffer) || (NULL == handle->link_handle.send))
    {
    }
    else
    {
        pb_ostream_t       ostream          = pb_ostream_from_buffer(handle->buffer, handle->buffer_size);
        cave_talk_Movement movement_message = cave_talk_Movement_init_zero;

        movement_message.speed_meters_per_second      = speed;
        movement_message.turn_rate_radians_per_second = turn_rate;

        if (!pb_encode(&ostream, cave_talk_Movement_fields, &movement_message))
        {
            error = CAVE_TALK_ERROR_SIZE;
        }
        else
        {
            error = CaveTalk_Speak(&handle->link_handle, (CaveTalk_Id_t)cave_talk_Id_ID_MOVEMENT, handle->buffer, ostream.bytes_written);
        }
    }

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakCameraMovement(const CaveTalk_Handle_t *const handle, const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NULL;

    if ((NULL == handle) || (NULL == handle->buffer) || (NULL == handle->link_handle.send))
    {
    }
    else
    {
        pb_ostream_t             ostream                 = pb_ostream_from_buffer(handle->buffer, handle->buffer_size);
        cave_talk_CameraMovement camera_movement_message = cave_talk_CameraMovement_init_zero;

        camera_movement_message.pan_angle_radians  = pan;
        camera_movement_message.tilt_angle_radians = tilt;

        if (!pb_encode(&ostream, cave_talk_CameraMovement_fields, &camera_movement_message))
        {
            error = CAVE_TALK_ERROR_SIZE;
        }
        else
        {
            error = CaveTalk_Speak(&handle->link_handle, (CaveTalk_Id_t)cave_talk_Id_ID_CAMERA_MOVEMENT, handle->buffer, ostream.bytes_written);
        }
    }

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakLights(const CaveTalk_Handle_t *const handle, const bool headlights)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NULL;

    if ((NULL == handle) || (NULL == handle->buffer) || (NULL == handle->link_handle.send))
    {
    }
    else
    {
        pb_ostream_t     ostream        = pb_ostream_from_buffer(handle->buffer, handle->buffer_size);
        cave_talk_Lights lights_message = cave_talk_Lights_init_zero;

        lights_message.headlights = headlights;

        if (!pb_encode(&ostream, cave_talk_Lights_fields, &lights_message))
        {
            error = CAVE_TALK_ERROR_SIZE;
        }
        else
        {
            error = CaveTalk_Speak(&handle->link_handle, (CaveTalk_Id_t)cave_talk_Id_ID_LIGHTS, handle->buffer, ostream.bytes_written);
        }
    }

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakMode(const CaveTalk_Handle_t *const handle, const bool manual)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NULL;

    if ((NULL == handle) || (NULL == handle->buffer) || (NULL == handle->link_handle.send))
    {
    }
    else
    {
        pb_ostream_t   ostream      = pb_ostream_from_buffer(handle->buffer, handle->buffer_size);
        cave_talk_Mode mode_message = cave_talk_Mode_init_zero;

        mode_message.manual = manual;

        if (!pb_encode(&ostream, cave_talk_Mode_fields, &mode_message))
        {
            error = CAVE_TALK_ERROR_SIZE;
        }
        else
        {
            error = CaveTalk_Speak(&handle->link_handle, (CaveTalk_Id_t)cave_talk_Id_ID_MODE, handle->buffer, ostream.bytes_written);
        }
    }

    return error;
}