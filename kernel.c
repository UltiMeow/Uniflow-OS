#include <stdint.h>
#include "mailbox.h"

static volatile uint32_t *framebuffer;
static uint32_t fb_width, fb_height, fb_pitch;

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    framebuffer[y * (fb_pitch/4) + x] = color;
}

void draw_text(const char *str, uint32_t x, uint32_t y, uint32_t color) {
    int i, j, xi;
    for (i = 0; str[i]; i++) {
        char c = str[i];
        // Simple 8x8 font: draw a filled rectangle for each character (placeholder)
        for (j = 0; j < 8; j++) {
            for (xi = 0; xi < 8; xi++) {
                put_pixel(x + i*8 + xi, y + j, color);
            }
        }
    }
}

void init_framebuffer(void) {
    static uint32_t fb_msg[26] __attribute__((aligned(16)));
    for (int i = 0; i < 26; i++) fb_msg[i] = 0;

    fb_msg[0] = 26 * 4;   // total size in bytes
    fb_msg[1] = 0;        // request

    // Set physical size: tag, value_buf_size, req/resp size, width, height
    fb_msg[2] = 0x00048003; fb_msg[3] = 8; fb_msg[4] = 0; fb_msg[5] = 1024; fb_msg[6] = 768;

    // Set virtual size
    fb_msg[7] = 0x00048004; fb_msg[8] = 8; fb_msg[9] = 0; fb_msg[10] = 1024; fb_msg[11] = 768;

    // Set depth (bits per pixel)
    fb_msg[12] = 0x00048005; fb_msg[13] = 4; fb_msg[14] = 0; fb_msg[15] = 32;

    // Allocate framebuffer: alignment (16)
    fb_msg[16] = 0x00040001; fb_msg[17] = 8; fb_msg[18] = 0; fb_msg[19] = 16; fb_msg[20] = 0;

    // Get pitch
    fb_msg[21] = 0x00040008; fb_msg[22] = 4; fb_msg[23] = 0; fb_msg[24] = 0;

    fb_msg[25] = 0; // end tag

    if (!mailbox_call(fb_msg)) return;

    uint32_t fb_addr32 = fb_msg[19];
    uint32_t pitch = fb_msg[24];

    uintptr_t fb_addr = (uintptr_t)fb_addr32;
    // Convert GPU bus address to ARM physical where needed (clear GPU high bits)
    if (fb_addr & 0xC0000000u) fb_addr &= 0x3FFFFFFFu;

    framebuffer = (volatile uint32_t*)(uintptr_t)fb_addr;
    fb_pitch = pitch;
    fb_width = fb_msg[5];
    fb_height = fb_msg[6];
}

void kernel_main(void) {
    init_framebuffer();

    // Fill background black
    for (uint32_t y=0; y<fb_height; y++)
        for (uint32_t x=0; x<fb_width; x++)
            framebuffer[y*(fb_pitch/4)+x] = 0x000000;

    // Draw text placeholder: each char = 8x8 white block
    draw_text("hello kernel", 100, 100, 0xFFFFFF);

    while (1) {
        __asm__("wfe");
    }
}
