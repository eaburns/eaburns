#ifndef _8259_PIC_H_
#define _8259_PIC_H_

#define PIC_0_CMD_PORT 0x20
#define PIC_0_PORT 0x21
#define PIC_0_MAX_IRQ 7

#define PIC_1_CMD_PORT 0xA0
#define PIC_1_PORT 0xA1
#define PIC_1_MAX_IRQ 15

/* First interrupt number that is an irq. */
#define IRQ_BEGIN 0x28

/* Initialzies the programmable interrupt controller. */
int init_pic();

#endif /* _8259_PIC_H_ */
