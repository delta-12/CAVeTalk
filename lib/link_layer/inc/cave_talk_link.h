#ifndef CAVE_TALK_LINK_H
#define CAVE_TALK_LINK_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t CaveTalk_Version_t;
typedef uint8_t CaveTalk_Id_t;
typedef uint8_t CaveTalk_Length_t;
typedef uint32_t CaveTalk_Crc_t;

typedef enum
{
    CAVE_TALK_LINK_ERROR_NONE,
    CAVE_TALK_LINK_ERROR_NULL,
    CAVE_TALK_LINK_ERROR_SIZE,
    CAVE_TALK_LINK_ERROR_SOCKET_CLOSED,
    CAVE_TALK_LINK_ERROR_INCOMPLETE,
    CAVE_TALK_LINK_ERROR_CRC
} CaveTalk_LinkError_t;

typedef struct
{
    CaveTalk_LinkError_t (*send)(const void *const data, const size_t size);
    CaveTalk_LinkError_t (*receive)(void *const data, const size_t size, size_t *const bytes_received);
    CaveTalk_LinkError_t (*available)(size_t *const bytes);
} CaveTalk_LinkHandle_t;

#ifdef __cplusplus
extern "C"
{
#endif

CaveTalk_LinkError_t CaveTalk_Speak(CaveTalk_LinkHandle_t *const handle,
                                    const CaveTalk_Id_t id,
                                    const void *const data,
                                    const CaveTalk_Length_t length);
CaveTalk_LinkError_t CaveTalk_Listen(CaveTalk_LinkHandle_t *const handle,
                                        CaveTalk_Id_t *const id,
                                        void *const data,
                                        const size_t size,
                                        CaveTalk_Length_t *const length);

#ifdef __cplusplus
}
#endif

#endif /* CAVE_TALK_LINK_H */