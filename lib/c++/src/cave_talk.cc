#include "cave_talk.h"

#include <cstddef>
#include <functional>

#include "ids.pb.h"
#include "ooga_booga.pb.h"

#include "cave_talk_link.h"
#include "cave_talk_types.h"

namespace cave_talk
{

Listener::Listener(std::function<CaveTalk_Error_t(void *const data, const size_t size, size_t *const bytes_received)> &receive,
                   std::function<CaveTalk_Error_t(size_t *const bytes)> &available,
                   std::shared_ptr<ListenerCallbacks> listener_callbacks) : listener_callbacks_(listener_callbacks)
{
    link_handle_.send      = nullptr;
    link_handle_.receive   = *receive.target<CaveTalk_Error_t (*)(void *const data, const size_t size, size_t *const bytes_received)>();
    link_handle_.available = *available.target<CaveTalk_Error_t (*)(size_t *const bytes)>();
}

CaveTalk_Error_t Listener::Listen(void)
{
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;
    CaveTalk_Id_t    id    = 0U;

    /* TODO SD-157 Call CaveTalk_Listen */

    switch (static_cast<Id>(id))
    {
    case ID_NONE:
        /* TODO SD-157 */
        break;
    case ID_OOGA:
        /* TODO SD-157 */
        break;
    case ID_MOVEMENT:
        /* TODO SD-157 */
        break;
    case ID_CAMERA_MOVEMENT:
        /* TODO SD-157 */
        break;
    case ID_LIGHTS:
        /* TODO SD-157 */
        break;
    case ID_MODE:
        /* TODO SD-157 */
        break;
    default:
        error = CAVE_TALK_ERROR_ID;
        break;
    }

    return error;
}

Talker::Talker(std::function<CaveTalk_Error_t(const void *const data, const size_t size)> &send)
{
    link_handle_.send      = *send.target<CaveTalk_Error_t (*)(const void *const data, const size_t size)>();
    link_handle_.receive   = nullptr;
    link_handle_.available = nullptr;
}

CaveTalk_Error_t Talker::SpeakOogaBooga(const Say ooga_booga)
{
    OogaBooga ooga_booga_message;
    ooga_booga_message.set_ooga_booga(ooga_booga);

    std::size_t length = ooga_booga_message.ByteSizeLong();
    message_buffer_.reserve(length);
    ooga_booga_message.SerializeToArray(message_buffer_.data(), message_buffer_.capacity());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_OOGA), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakMovement(const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate)
{
    /* TODO SD-157 */
    CAVE_TALK_UNUSED(speed);
    CAVE_TALK_UNUSED(turn_rate);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Talker::SpeakCameraMovement(const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)
{
    /* TODO SD-157 */
    CAVE_TALK_UNUSED(pan);
    CAVE_TALK_UNUSED(tilt);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Talker::SpeakLights(const bool headlights)
{
    /* TODO SD-157 */
    CAVE_TALK_UNUSED(headlights);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Talker::SpeakMode(const bool manual)
{
    /* TODO SD-157 */
    CAVE_TALK_UNUSED(manual);

    return CAVE_TALK_ERROR_NONE;
}

} // namespace cave_talk