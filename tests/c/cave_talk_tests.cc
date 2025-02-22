#include <cstddef>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ooga_booga.pb.h"

#include "cave_talk.h"
#include "cave_talk_link.h"
#include "cave_talk_types.h"
#include "ring_buffer.h"


static const std::size_t kMaxMessageLength = 255U;
static RingBuffer<uint8_t, kMaxMessageLength> ring_buffer;

CaveTalk_Error_t Send(const void *const data, const size_t size)
{    
    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if (size > ring_buffer.Capacity() - ring_buffer.Size())
    {
        error = CAVE_TALK_ERROR_INCOMPLETE;
    }
    else
    {
        ring_buffer.Write(static_cast<const uint8_t *const>(data), size);
    }

    return error;
}

CaveTalk_Error_t Receive(void *const data, const size_t size, size_t *const bytes_received)
{
    *bytes_received = ring_buffer.Read(static_cast<uint8_t *const>(data), size);

    return CAVE_TALK_ERROR_NONE;
}


CaveTalk_Error_t Available(size_t *const bytes_available)
{
    *bytes_available = ring_buffer.Capacity() - ring_buffer.Size();

    return CAVE_TALK_ERROR_NONE;
}

const CaveTalk_LinkHandle_t kCaveTalk_CTests_LinkHandle = {
    .send = Send,
    .receive = Receive,
    .available = Available,
};

uint8_t buffer[255U] = {0U};

class ListenCallbacksInterface
{
    public:
        virtual ~ListenCallbacksInterface()                                                                                   = 0;
        virtual void HearOogaBooga(const cave_talk_Say ooga_booga)                                                               = 0;
        virtual void HearMovement(const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate) = 0;
        virtual void HearCameraMovement(const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)                     = 0;
        virtual void HearLights(const bool headlights)                                                                 = 0;
        virtual void HearMode(const bool manual)  = 0;
};



// const CaveTalk_ListenCallbacks_t kCaveTalk_ListenCallbacksStructInterface = {
//     .hear_ooga_booga = ListenCallbacksInterface::HearOogaBooga,
//     .hear_movement = ListenCallbacksInterface::HearMovement,
//     .hear_camera_movement = ListenCallbacksInterface::HearCameraMovement,
//     .hear_lights = ListenCallbacksInterface::HearLights,
//     .hear_mode = ListenCallbacksInterface::HearMode,
// };

ListenCallbacksInterface::~ListenCallbacksInterface() = default;

class MockListenerCallbacks : public ListenCallbacksInterface
{
    public:
        MOCK_METHOD(void, HearOogaBooga, (const cave_talk_Say), (override));
        MOCK_METHOD(void, HearMovement, ((const CaveTalk_MetersPerSecond_t), (const CaveTalk_RadiansPerSecond_t)), (override));
        MOCK_METHOD(void, HearCameraMovement, ((const CaveTalk_Radian_t), (const CaveTalk_Radian_t)), (override));
        MOCK_METHOD(void, HearLights, (const bool), (override));
        MOCK_METHOD(void, HearMode, (const bool), (override));
};

const CaveTalk_Handle_t kCaveTalk_Handle = {
    .link_handle = kCaveTalk_CTests_LinkHandle,
    .buffer = buffer,
    .buffer_size = 255U,
    .listen_callbacks = NULL,
};

TEST(CaveTalkCppTests, SpeakListenOogaBooga){

    uint8_t data_receive[255U] = {0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    // std::shared_ptr<MockListenerCallbacks> mock_listen_callbacks = std::make_shared<MockListenerCallbacks>();

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_SpeakOogaBooga(&kCaveTalk_Handle, cave_talk_Say_SAY_OOGA));
    //EXPECT_CALL(mock object.get(), ListenCallback HearOB ).Times(1);

    // ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth.SpeakOogaBooga(cave_talk::SAY_OOGA));
    // EXPECT_CALL(*mock_listen_callbacks.get(), HearOogaBooga(cave_talk::SAY_OOGA)).Times(1);
    // ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverEars.Listen());

}