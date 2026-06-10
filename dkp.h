#ifndef DKP_H
#define DKP_H

#define SW1    0x0E
#define SW2    0x0D
#define SW3    0x0B
#define SW4    0x07


char read(int choice);
void indicator_task(unsigned char key);
void init_dkp(void);

#endif