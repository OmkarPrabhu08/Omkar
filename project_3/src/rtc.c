/*
 *
 *  Created on: 16-Apr-2017
 *      Author: Omkar
 */




#include "MKL25Z4.h"
#include <stdint.h>
#include "logger.h"

void rtc_init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;      //Enable Port C
    MCG_C1 |= MCG_C1_IRCLKEN_MASK;      //Enable internal reference clock
	MCG_C2 &= ~(MCG_C2_IRCS_MASK);      //Internal Reference Clock Slow
	PORTC_PCR1 |= (PORT_PCR_MUX(0x1));              //PTC1 as RTC_CLKIN
    SIM_SOPT1 |= 0x00080000;                    //RTC_CLKIN selected for OSC32KCLK
    SIM_SOPT2 |= 0x00000080;                    //MCGIRCLK selected for clock out
    PORTC_PCR3 |= (PORT_PCR_MUX(0x5));              //PTC3 as CLKOUT

    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;        //Enable the clock to RTC

    RTC_CR  |= RTC_CR_SWR_MASK;

    RTC_CR  &= ~RTC_CR_SWR_MASK;            //Disable and reset RTC

    RTC_TCR = RTC_TCR_CIR(1) | RTC_TCR_TCR(0xCF);   //Setup compensation parameters
    RTC_CR |= RTC_CR_OSCE_MASK;                     //Enable 32.768Hz oscillator
    RTC_TSR = TIME;                           //Reset the seconds register
    RTC_IER = 0x00;
    RTC_IER |=0x10;


    NVIC_EnableIRQ(RTC_Seconds_IRQn);
    __enable_irq();
}

uint32_t get_time()
{
	return RTC_TSR;
}



