#include "cave_talk.h"

#include <stdbool.h>

#include "cave_talk_link.h"
#include "cave_talk_types.h"

CaveTalk_Error_t CaveTalk_Hear(const CaveTalk_LinkHandle_t *const handle)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakOogaBooga(const CaveTalk_LinkHandle_t *const handle, const cave_talk_Say ooga_booga)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakMovement(const CaveTalk_LinkHandle_t *const handle, const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakCameraMovement(const CaveTalk_LinkHandle_t *const handle, const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakLights(const CaveTalk_LinkHandle_t *const handle, const bool headlights)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}

CaveTalk_Error_t CaveTalk_SpeakMode(const CaveTalk_LinkHandle_t *const handle, const bool manual)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (NULL == handle)
    {
        error = CAVE_TALK_ERROR_NULL;
    }

    /* TODO SD-158 */

    return error;
}