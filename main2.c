#include <xc.h>
#include "adc.h"
#include "dkp.h"
#include "can.h"


void main(void)
{
    unsigned int speed;
   unsigned int i = 0;
    unsigned char gear1[2];
     unsigned char gear[7] = {N,ONE,TWO,THREE,FOUR,FIVE,R};
    unsigned char tx_speed[3];
    unsigned char key = 0x0F;
     static unsigned char prev = 0x0F; 

    init_adc();
    init_dkp();
    init_can();

    while (1)
    {
       
        speed = (read_adc(CHANNEL4) * 100) / 1023;

        tx_speed[0] = (speed / 100) + '0';
        tx_speed[1] = ((speed / 10) % 10) + '0';
        tx_speed[2] = (speed % 10) + '0';

        can_transmit(SPEED_MSG_ID, tx_speed, 3);

        /* Read keypad */
        key = read(0);
        
        if (key != 0x0F)
        {
        prev = key;
        }

        /* Gear logic */
            if ( key == SW1)
            {
                if(i < 6)
                {
                    i++;
                }
            }
            else if ( key == SW2)
            {
                if(i > 0)
                {
                    i--;
                }
            }

        /* ASCII conversion */
        gear1[0] = gear[i] + '0';
        gear1[1] = '\0';

        can_transmit(GEAR_MSG_ID, gear1, 2);
    }
}
