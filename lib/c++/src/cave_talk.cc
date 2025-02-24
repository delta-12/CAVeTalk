#include "cave_talk.h"

#include <cstddef>
#include <functional>

#include "camera_movement.pb.h"
#include "ids.pb.h"
#include "lights.pb.h"
#include "mode.pb.h"
#include "movement.pb.h"
#include "ooga_booga.pb.h"
#include "odometry.pb.h"
#include "log.pb.h"

#include "cave_talk_link.h"
#include "cave_talk_types.h"

namespace cave_talk
{

Listener::Listener(CaveTalk_Error_t (*receive)(void *const data, const size_t size, size_t *const bytes_received),
                   CaveTalk_Error_t (*available)(size_t *const bytes_available),
                   std::shared_ptr<ListenerCallbacks> listener_callbacks) : listener_callbacks_(listener_callbacks)
{
    link_handle_.send      = nullptr;
    link_handle_.receive   = receive;
    link_handle_.available = available;
}

CaveTalk_Error_t Listener::Listen(void)
{
    CaveTalk_Id_t     id     = 0U;
    CaveTalk_Length_t length = 0U;
    CaveTalk_Error_t  error  = CaveTalk_Listen(&link_handle_, &id, buffer_.data(), buffer_.size(), &length);

    if (CAVE_TALK_ERROR_NONE == error)
    {
        switch (static_cast<Id>(id))
        {
        case ID_NONE:
            if (0U != length)
            {
                error = CAVE_TALK_ERROR_ID;
            }
            break;
        case ID_OOGA:
            error = HandleOogaBooga(length);
            break;
        case ID_MOVEMENT:
            error = HandleMovement(length);
            break;
        case ID_CAMERA_MOVEMENT:
            error = HandleCameraMovement(length);
            break;
        case ID_LIGHTS:
            error = HandleLights(length);
            break;
        case ID_MODE:
            error = HandleMode(length);
            break;
        case ID_ODOMETRY:
            error = HandleOdometry(length);
            break;
        case ID_LOG:
            error = HandleLog(length);
            break;
        default:
            error = CAVE_TALK_ERROR_ID;
            break;
        }
    }

    return error;
}

CaveTalk_Error_t Listener::HandleOogaBooga(CaveTalk_Length_t length) const
{

    OogaBooga ooga_booga_message;

    if (!ooga_booga_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const Say ooga_booga = ooga_booga_message.ooga_booga();

    listener_callbacks_->HearOogaBooga(ooga_booga);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Listener::HandleMovement(CaveTalk_Length_t length) const
{

    Movement movement_message;

    if (!movement_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const CaveTalk_MetersPerSecond_t  speed     = movement_message.speed_meters_per_second();
    const CaveTalk_RadiansPerSecond_t turn_rate = movement_message.turn_rate_radians_per_second();

    listener_callbacks_->HearMovement(speed, turn_rate);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Listener::HandleCameraMovement(CaveTalk_Length_t length) const
{

    CameraMovement camera_movement_message;

    if (!camera_movement_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const CaveTalk_Radian_t pan  = camera_movement_message.pan_angle_radians();
    const CaveTalk_Radian_t tilt = camera_movement_message.tilt_angle_radians();

    listener_callbacks_->HearCameraMovement(pan, tilt);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Listener::HandleLights(CaveTalk_Length_t length) const
{

    Lights lights_message;

    if (!lights_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const bool headlights = lights_message.headlights();

    listener_callbacks_->HearLights(headlights);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Listener::HandleMode(CaveTalk_Length_t length) const
{

    Mode mode_message;

    if (!mode_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const bool manual = mode_message.manual();

    listener_callbacks_->HearMode(manual);

    return CAVE_TALK_ERROR_NONE;
}

CaveTalk_Error_t Listener::HandleOdometry(CaveTalk_Length_t length) const
{

    Odometry odometry_message;

    if (!odometry_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const CaveTalk_MetersPerSecondSquared_t x_accel      = odometry_message.x_accel_meters_per_second_squared();
    const CaveTalk_MetersPerSecondSquared_t y_accel      = odometry_message.y_accel_meters_per_second_squared();
    const CaveTalk_MetersPerSecondSquared_t z_accel      = odometry_message.z_accel_meters_per_second_squared();
    const CaveTalk_RadiansPerSecond_t       roll         = odometry_message.roll_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       pitch        = odometry_message.pitch_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       yaw          = odometry_message.yaw_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       wheel_0_rate = odometry_message.wheel_0_rate_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       wheel_1_rate = odometry_message.wheel_1_rate_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       wheel_2_rate = odometry_message.wheel_2_rate_radians_per_second();
    const CaveTalk_RadiansPerSecond_t       wheel_3_rate = odometry_message.wheel_3_rate_radians_per_second();

    listener_callbacks_->HearOdometry(
        x_accel,
        y_accel,
        z_accel,
        roll,
        pitch,
        yaw,
        wheel_0_rate,
        wheel_1_rate,
        wheel_2_rate,
        wheel_3_rate
        );

    return CAVE_TALK_ERROR_NONE;

}

CaveTalk_Error_t Listener::HandleLog(CaveTalk_Length_t length) const
{

    Log log_message;

    if (!log_message.ParseFromArray(buffer_.data(), length))
    {
        return CAVE_TALK_ERROR_PARSE;
    }

    const char*              log_ptr  = (log_message.log_string()).c_str();
    const CaveTalk_Message_t log_text = const_cast<CaveTalk_Message_t>(log_ptr);

    listener_callbacks_->HearLog(log_text);

    return CAVE_TALK_ERROR_NONE;
}

Talker::Talker(CaveTalk_Error_t (*send)(const void *const data, const size_t size))
{
    link_handle_.send      = send;
    link_handle_.receive   = nullptr;
    link_handle_.available = nullptr;
}

CaveTalk_Error_t Talker::SpeakOogaBooga(const Say ooga_booga)
{
    OogaBooga ooga_booga_message;
    ooga_booga_message.set_ooga_booga(ooga_booga);

    std::size_t length = ooga_booga_message.ByteSizeLong();
    ooga_booga_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_OOGA), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakMovement(const CaveTalk_MetersPerSecond_t speed, const CaveTalk_RadiansPerSecond_t turn_rate)
{
    Movement movement_message;
    movement_message.set_speed_meters_per_second(speed);
    movement_message.set_turn_rate_radians_per_second(turn_rate);

    std::size_t length = movement_message.ByteSizeLong();
    movement_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_MOVEMENT), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakCameraMovement(const CaveTalk_Radian_t pan, const CaveTalk_Radian_t tilt)
{
    CameraMovement camera_movement_message;
    camera_movement_message.set_pan_angle_radians(pan);
    camera_movement_message.set_tilt_angle_radians(tilt);

    std::size_t length = camera_movement_message.ByteSizeLong();
    camera_movement_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_CAMERA_MOVEMENT), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakLights(const bool headlights)
{
    Lights lights_message;
    lights_message.set_headlights(headlights);

    std::size_t length = lights_message.ByteSizeLong();
    lights_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_LIGHTS), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakMode(const bool manual)
{
    Mode mode_message;
    mode_message.set_manual(manual);

    std::size_t length = mode_message.ByteSizeLong();
    mode_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_MODE), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakOdometry(   const CaveTalk_MetersPerSecondSquared_t x_accel,
                                          const CaveTalk_MetersPerSecondSquared_t y_accel,
                                          const CaveTalk_MetersPerSecondSquared_t z_accel,
                                          const CaveTalk_RadiansPerSecond_t roll,
                                          const CaveTalk_RadiansPerSecond_t pitch,
                                          const CaveTalk_RadiansPerSecond_t yaw,
                                          const CaveTalk_RadiansPerSecond_t wheel_0_rate,
                                          const CaveTalk_RadiansPerSecond_t wheel_1_rate,
                                          const CaveTalk_RadiansPerSecond_t wheel_2_rate,
                                          const CaveTalk_RadiansPerSecond_t wheel_3_rate)
{
    Odometry odometry_message;
    odometry_message.set_x_accel_meters_per_second_squared(x_accel);
    odometry_message.set_y_accel_meters_per_second_squared(y_accel);
    odometry_message.set_z_accel_meters_per_second_squared(z_accel);
    odometry_message.set_roll_radians_per_second(roll);
    odometry_message.set_pitch_radians_per_second(pitch);
    odometry_message.set_yaw_radians_per_second(yaw);
    odometry_message.set_wheel_0_rate_radians_per_second(wheel_0_rate);
    odometry_message.set_wheel_1_rate_radians_per_second(wheel_1_rate);
    odometry_message.set_wheel_2_rate_radians_per_second(wheel_2_rate);
    odometry_message.set_wheel_3_rate_radians_per_second(wheel_3_rate);

    size_t length = odometry_message.ByteSizeLong();
    odometry_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_ODOMETRY), message_buffer_.data(), length);
}

CaveTalk_Error_t Talker::SpeakLog(const CaveTalk_Message_t log_text)
{
    Log log_message;
    log_message.set_log_string(log_text);

    std::size_t length = log_message.ByteSizeLong();
    log_message.SerializeToArray(message_buffer_.data(), message_buffer_.max_size());

    return CaveTalk_Speak(&link_handle_, static_cast<CaveTalk_Id_t>(ID_LOG), message_buffer_.data(), length);
}



} // namespace cave_talk