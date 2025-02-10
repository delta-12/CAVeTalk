# CAVeTalk

CAVEMAN controller message protocol

## Protocol Structure

| Version | ID | Length | Payload |
| ----------- | ----------- | ----------- | ----------- |
| 1 Byte | 1 Byte | 1 Byte | Up to 255 Bytes |

1. Version refers to the communication version that packet adheres to, starting from V1 represented as 0x01
2. ID refers to the specific message being sent by the packet. Here is a list of the currently supported messages.

| ID | Name | Descriptor |
| ----------- | ----------- | ----------- |
| 0x01 |  Ooga | CAVeTalk Ping Message, should receive "Booga" back from the controller |
| 0x02 | Movement | Describes the Speed [m/s] & Turn Rate of the Rover [rad/s]  |
| 0x03 | Camera Movement | Describes the Camera Pan [radians] and Tilt Servo Angles [radians] |
| 0x04 | Lights |  Toggles the Onboard Headlights |
| 0x05 | Mode | Switches between Manual Driving Mode and Autonomous Driving Mode |

3. Length refers to the length of the packet in bytes
4. Payload refers to the main piece of information sent in the packet