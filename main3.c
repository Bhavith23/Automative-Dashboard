/*******************************************
* Name        :  Bhavith M Gowda
* Title       :  Automative Dashboard
* Date        :  08-01-2026
* Description :  An automotive dashboard using the PIC18F4580 is an embedded system that displays real-time vehicle parameters like speed, 
                 RPM, gear position, and indicators using LCD/CLCD, LEDs, or SSDs.
* Sample Output: If I change the potentiometer the rpm value will vary from 0 to 6000 and speed value 0 to 100
*********************************************/

#include <xc.h>
#include "can.h"
#include "clcd.h"
#include "dkp.h"

void main(void)
{
    uint16_t msg_id;
    uint8_t rx_data[8];
    uint8_t len;

    unsigned char rpm[5]   = "0000";
    unsigned char speed[4] = "000";
    unsigned char gear     = '0';
    unsigned char ind[3]   = "--";

    unsigned char indicator_state = SW3;   // OFF by default

    //LED pins setup 
    TRISB0 = 0;
    TRISB5 = 0;
    RB0 = 0;
    RB5 = 0;

    init_clcd();
    init_can();

    /* LINE 1 labels */
    clcd_print("RPM", LINE1(0));
    clcd_print("IND", LINE1(5));
    clcd_print("SPD", LINE1(10));
    clcd_print("G",   LINE1(14));

    while (1)
    {
        //CAN RECEIVE
        can_receive(&msg_id, rx_data, &len);

        if (len != 0)
        {
            switch (msg_id)
            {
                case RPM_MSG_ID:
                    rpm[0] = rx_data[0];
                    rpm[1] = rx_data[1];
                    rpm[2] = rx_data[2];
                    rpm[3] = rx_data[3];
                    rpm[4] = '\0';
                    break;

                case SPEED_MSG_ID:
                    speed[0] = rx_data[0];
                    speed[1] = rx_data[1];
                    speed[2] = rx_data[2];
                    speed[3] = '\0';
                    break;

                case GEAR_MSG_ID:
                    gear = rx_data[0];
                    break;

                case INDICATOR_MSG_ID:
                    indicator_state = rx_data[0];   // STORE ONLY
                    break;
            }
        }

        //INDICATOR DRIVER
        indicator_task(indicator_state);

        //CLCD INDICATOR SYMBOL
        if (indicator_state == SW1)
        {
            ind[0] = '<'; 
            ind[1] = '-';
        }
        else if (indicator_state == SW2)
        {
            ind[0] = '-';
            ind[1] = '>';
        }
        else if (indicator_state == SW4)
        {
            ind[0] = '<'; 
            ind[1] = '>';
        }
        else
        {
            ind[0] = '-'; 
            ind[1] = '-';
        }
        ind[2] = '\0';

        // CLCD UPDATE
        clcd_print(rpm,   LINE2(0));
        clcd_print(ind,   LINE2(5));
        clcd_print(speed, LINE2(10));
        clcd_putch(gear,  LINE2(14));
    }
}
