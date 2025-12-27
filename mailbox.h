#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>

#define MAILBOX_BASE       0xFE00B880UL
#define MAILBOX_READ       (MAILBOX_BASE + 0x00)
#define MAILBOX_STATUS     (MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE      (MAILBOX_BASE + 0x20)
#define MAILBOX_FULL       0x80000000
#define MAILBOX_EMPTY      0x40000000
#define MAILBOX_CHAN_PROP 8

int mailbox_call(uint32_t *msg);
#endif // MAILBOX_H