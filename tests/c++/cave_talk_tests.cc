#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ids.pb.h"
#include "ooga_booga.pb.h"

#include "cave_talk.h"
#include "cave_talk_link.h"
#include "cave_talk_types.h"
#include "ring_buffer.h"

static const std::size_t kMaxMessageLength = 255U;
static RingBuffer<uint8_t, kMaxMessageLength> ring_buffer;

class MockListenerCallbacks : public cave_talk::ListenerCallbacks{
    public:
        MOCK_METHOD(void, HearOogaBooga, (const cave_talk::Say), (override));
        MOCK_METHOD(void, HearMovement, ((const CaveTalk_MetersPerSecond_t), (const CaveTalk_RadiansPerSecond_t)), (override));
        MOCK_METHOD(void, HearCameraMovement, ((const CaveTalk_Radian_t), (const CaveTalk_Radian_t)), (override));
        MOCK_METHOD(void, HearLights, (const bool), (override));
        MOCK_METHOD(void, HearMode, (const bool), (override));
};

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

static const CaveTalk_LinkHandle_t kLinkHandle = {
    .send      = Send,
    .receive   = Receive,
    .available = Available,
};

TEST(CaveTalkCppTests, SpeakListenOogaBooga){

    std::vector<uint8_t>* data_receive = new std::vector<uint8_t>{0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    MockListenerCallbacks mock_calls;
    cave_talk::Talker* roverMouth;

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakOogaBooga(cave_talk::SAY_OOGA));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearOogaBooga(cave_talk::SAY_OOGA));
    ASSERT_EQ(cave_talk::ID_OOGA, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakOogaBooga(cave_talk::SAY_BOOGA));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearOogaBooga(cave_talk::SAY_OOGA));
    ASSERT_EQ(cave_talk::ID_OOGA, id);


}

TEST(CaveTalkCppTests, SpeakListenMovement){

    std::vector<uint8_t>* data_receive = new std::vector<uint8_t>{0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    MockListenerCallbacks mock_calls;
    cave_talk::Talker* roverMouth;

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakMovement(2.5, 1.7));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearMovement(2.5, 1.7));
    ASSERT_EQ(cave_talk::ID_MOVEMENT, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakMovement(0, 6));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearMovement(0, 6));
    ASSERT_EQ(cave_talk::ID_MOVEMENT, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakMovement(6, 0));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearMovement(6, 0));
    ASSERT_EQ(cave_talk::ID_MOVEMENT, id);
    
}

TEST(CaveTalkCppTests, SpeakListenCameraMovement){

    std::vector<uint8_t>* data_receive = new std::vector<uint8_t>{0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    MockListenerCallbacks mock_calls;
    cave_talk::Talker* roverMouth;

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakCameraMovement(2.5, 1.7));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearCameraMovement(2.5, 1.7));
    ASSERT_EQ(cave_talk::ID_CAMERA_MOVEMENT, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakCameraMovement(0, 6));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearCameraMovement(0, 6));
    ASSERT_EQ(cave_talk::ID_CAMERA_MOVEMENT, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakCameraMovement(6, 0));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearCameraMovement(6, 0));
    ASSERT_EQ(cave_talk::ID_CAMERA_MOVEMENT, id);
    
}

TEST(CaveTalkCppTests, SpeakListenLights){

    std::vector<uint8_t>* data_receive = new std::vector<uint8_t>{0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    MockListenerCallbacks mock_calls;
    cave_talk::Talker* roverMouth;

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakLights(true));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearLights(true));
    ASSERT_EQ(cave_talk::ID_LIGHTS, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakLights(false));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearLights(false));
    ASSERT_EQ(cave_talk::ID_LIGHTS, id);
    
}

TEST(CaveTalkCppTests, SpeakListenMode){

    std::vector<uint8_t>* data_receive = new std::vector<uint8_t>{0U};
    CaveTalk_Id_t id = 0U;
    CaveTalk_Length_t length = 0U;

    MockListenerCallbacks mock_calls;
    cave_talk::Talker* roverMouth;

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakMode(true));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearMode(true));
    ASSERT_EQ(cave_talk::ID_MODE, id);

    ring_buffer.Clear();

    ASSERT_EQ(CAVE_TALK_ERROR_NONE, roverMouth->SpeakMode(false));
    ASSERT_EQ(CAVE_TALK_ERROR_NONE, CaveTalk_Listen(&kLinkHandle, &id, static_cast<void *>(data_receive->data()), sizeof(data_receive), &length));
    EXPECT_CALL(mock_calls, HearMode(false));
    ASSERT_EQ(cave_talk::ID_MODE, id);
    
}