#include <xc.h>
#include "dkp.h"

char read(int choice)
{
    if (choice == 1)  //LEVEL TRIGGERING
    {
        return (PORTC & 0x0F);
    }
    else if (choice == 0)  //EDGE TRIGGERING
    {
        static int count = 0;
        char cur = (PORTC & 0x0F);

        if ((cur != 0x0F) && (count == 0))
        {
            count = 1;
            return cur;
        }
        else if (cur == 0x0F)
        {
            count = 0;
        }

        return 0x0F;
    }

    return 0x0F;
}

void init_dkp(void)
{
    TRISC = TRISC | 0x0F;    /* RC0..RC3 as inputs, others unchanged */
}

void indicator_task(unsigned char key)
{
    static unsigned char prev = 0x0F;    
    static unsigned int delay = 0;         

    /* Store last valid key */
    if (key != 0x0F)
    {
        prev = key;
    }

    /* LED control timing */
    if (++delay >= 100)
    {
        delay = 0;

        if (prev == SW1)
        {
            RB0 = 1;
            RB5 = 0;
        }
        else if (prev == SW2)
        {
            RB0 = 0;
            RB5 = 1;
        }
        else if (prev == SW3)
        {
            RB0 = 0;
            RB5 = 0;
        }
        else if (prev == SW4)
        {
            if (RB0 == RB5)
            {
                RB0 = !RB0;
                RB5 = !RB5;
            }
        }
    }
}