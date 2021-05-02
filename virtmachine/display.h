#ifndef VIRTMACHINE_DISPLAY_H
#define VIRTMACHINE_DISPLAY_H

#include <virtrisc/types.h>

typedef struct VirtMachine_FrameBuffer {
    size_t width, height;
    Byte *buffer;
} VirtMachine_FrameBuffer;

void VirtMachine_FrameBuffer_Init(VirtMachine_FrameBuffer *fb, size_t width, size_t height);
void VirtMachine_FrameBuffer_Free(VirtMachine_FrameBuffer *fb);

typedef struct VirtMachine_Display {
    size_t width, height, backBufferIndex;
    VirtMachine_FrameBuffer framebuffers[2];
} VirtMachine_Display;

void VirtMachine_Display_Init();

#endif //VIRTMACHINE_DISPLAY_H
