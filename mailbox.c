#include "mailbox.h"

static inline void mmio_write(uintptr_t reg, uint32_t val) {
    *((volatile uint32_t*)reg) = val;
}

static inline uint32_t mmio_read(uintptr_t reg) {
    return *((volatile uint32_t*)reg);
}

int mailbox_call(uint32_t *msg) {
    uint32_t r;
    do { r = mmio_read(MAILBOX_STATUS); } while (r & MAILBOX_FULL);

    uint32_t msg_addr = (uint32_t)((uintptr_t)msg & 0xFFFFFFFF);
    msg_addr &= ~0xF;
    msg_addr |= MAILBOX_CHAN_PROP;

    mmio_write(MAILBOX_WRITE, msg_addr);

    do { r = mmio_read(MAILBOX_STATUS); } while (r & MAILBOX_EMPTY);
    r = mmio_read(MAILBOX_READ);
    return r == (msg_addr & ~0xF);
}
