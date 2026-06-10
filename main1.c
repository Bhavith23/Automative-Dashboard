#include <xc.h>
#include "adc.h"
#include "dkp.h"
#include "can.h"


void main(void)
{
    unsigned int rpm;
    unsigned char tx_rpm[4];
   unsigned char key = 0x0F;
   unsigned char prev = 0x0F;


    init_adc();
    init_dkp();
    init_can();

    while (1)
    {
        //ECU1 : TRANSMITTER

        rpm   = (read_adc(CHANNEL4) / 10.23) * 60;
        
          tx_rpm[0] = (rpm / 1000) + '0';
          tx_rpm[1] = ((rpm / 100) % 10) + '0';
          tx_rpm[2] = ((rpm / 10) % 10) + '0';
          tx_rpm[3] = (rpm % 10) + '0';
        
        
     

        can_transmit(RPM_MSG_ID,tx_rpm,4);
        
         key = read(0);

        if (key != 0x0F)
        {
            prev = key;
        }
        
        can_transmit(INDICATOR_MSG_ID,&prev,1);
          
}
}

