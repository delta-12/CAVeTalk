#ifndef CAVE_TALK_H
#define CAVE_TALK_H

#include <cstddef>
#include <functional>
#include <vector>

#include "ooga_booga.pb.h"

#include "cave_talk_link.h"
#include "cave_talk_types.h"

namespace cave_talk
{

class Talker
{
    public:
        Talker(std::function<CaveTalk_Error_t(const void *const data, const size_t size)> &send,
               std::function<CaveTalk_Error_t(void *const data, const size_t size, size_t *const bytes_received)> &receive,
               std::function<CaveTalk_Error_t(size_t *const bytes)> &available);
        CaveTalk_Error_t SpeakOogaBooga(const Say ooga_booga);
        CaveTalk_Error_t SpeakMovement(const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate);
        CaveTalk_Error_t SpeakCameraMovement(const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt);
        CaveTalk_Error_t SpeakLights(const bool headlights);
        CaveTalk_Error_t SpeakMode(const bool manual);

    private:
        CaveTalk_LinkHandle_t link_handle_;
        std::vector<uint8_t> message_buffer_;
};

} // namespace cave_talk

#endif // CAVE_TALK_H