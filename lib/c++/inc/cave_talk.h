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

class ListenerCallbacks
{
    public:
        virtual ~ListenerCallbacks() = 0;

    private:
        virtual void HearOogaBooga(const Say ooga_booga)                                                                    = 0;
        virtual void HearSpeakMovement(const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate) = 0;
        virtual void SpeakCameraMovement(const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)                         = 0;
        virtual void SpeakLights(const bool headlights)                                                                     = 0;
        virtual void SpeakMode(const bool manual)                                                                           = 0;
};

class Listener
{
    public:
        /* TODO SD-157 ListenerCallbacks dependency injection */
        Listener(std::function<CaveTalk_Error_t(void *const data, const size_t size, size_t *const bytes_received)> &receive,
                 std::function<CaveTalk_Error_t(size_t *const bytes)> &available);
        Listener(Listener& listener)                  = delete;
        Listener(Listener&& listener)                 = delete;
        Listener& operator=(const Listener& listener) = delete;
        Listener& operator=(Listener&& listener)      = delete;

    /* TODO SD-157 listen method*/

    private:
        CaveTalk_LinkHandle_t link_handle_;
};

class Talker
{
    public:
        explicit Talker(std::function<CaveTalk_Error_t(const void *const data, const size_t size)> &send);
        Talker(Talker& talker)                  = delete;
        Talker(Talker&& talker)                 = delete;
        Talker& operator=(const Talker& talker) = delete;
        Talker& operator=(Talker&& talker)      = delete;
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