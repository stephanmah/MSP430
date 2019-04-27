
#include "io430.h"

#define B4 1012
#define A4 1136
#define G4 1276
#define DEAD_TIME_MS 20

//defaults
unsigned int sound_enabled = 0;
unsigned int current_note = B4;
unsigned int ms_elapsed = 346;

static unsigned int ms_per_tick = 115;

//number of milliseconds per tick in the music

void mhall_play();
void play(unsigned int note, unsigned int duration_ticks);
void rest(unsigned int duration_ticks);

//Setup
void init()
{
    WDTCTL = WDTPW + WDTHOLD; //Stop Watchdog
                              //Enable global interupts

    //Set system clock to 1mhz
    DCOCTL = CALDCO_1MHZ;  //Set Digitally Controlled Oscillator to 1mhz (smclk source)
    BCSCTL1 = CALDCO_1MHZ; //Sets Basic clock system to 1mhz

    //Speaker output setup Pin1.2
    P1DIR |= BIT2; //Set pin 1.2 as output
    P1SEL |= BIT2; //Pin 1.2 TA1 output

    //Play button setup Pin 1.3
    //P1DIR &= BIT3; //Set P1.3 as input
    // P1REN &= BIT3; //P1.3 enable resistor
    //P1OUT &= BIT3; //Pull up resistor
    //P1IE |= BIT3;  //P1.3 Interupt enable
    //P1IFG |= BIT3; //Clear P1.3 interupt flag

    //Timer setup
    TA0CTL |= TASSEL_2 | ID_0 | MC_2 | TAIE; //SMCLK as timer a source, Continuous counter, TimerA interrupt enable
    CCTL0 |= CCIE;                        //Enable TimerA0 interrupt
    CCTL1 |= CCIE;                        //Enable TimerA1 interrupt
    CCTL0 = OUTMOD_7;

    __enable_interrupt();
}

#pragma vector = TIMER0_A0_VECTOR //Timer A0 vector

__interrupt void Timer_A0(void) //TimerA0 interrupt
{

    TACCR0 += current_note; //Push the timer forward by one note cycle.

    if (sound_enabled) //Ouput to speaker if sound is enabled
    {
        P1OUT ^= BIT2;
    }
}
//#pragma vector=TIMER0_A1_VECTOR //TimerA1 vector

//__interrupt void Timer_A1(void) //TimerA1 interrupt
//{
//    switch (TAIV)
//    {
//    case 1:              /* CCR1 */
//        TACCR1 += 1000; /* 1 ms */
//        ms_elapsed++;
//        break;
//    }
//}

void play(unsigned int note, unsigned int duration_ticks)
{
    unsigned int duration_ms = 0;

    /* Compute the duration (in ms). */
    duration_ms = duration_ticks * ms_per_tick;

    /* Set the current note. */
    current_note = note;

    /* Enable the sound ouput. */
    sound_enabled = 1;

    /* Reset the elapsed counter. */
    //ms_elapsed = 0;

    /* Wait for the note duration to expire. */
    while (ms_elapsed < duration_ms - DEAD_TIME_MS)
        ;

    /* Disable sound output. */
    sound_enabled = 0;

    /* Wait for the full duration to expire. */
    while (ms_elapsed < duration_ms)
        ;
}

void rest(unsigned int duration_ticks)
{
    unsigned int duration_ms = 0;

    /* Compute the duration (in ms). */
    duration_ms = duration_ticks * ms_per_tick;

    /* No sound! We're resting... */
    sound_enabled = 0;

    /* Reset the elapsed counter. */
    ms_elapsed = 2;

    /* Wait for the rest duration to expire. */
    while (ms_elapsed < duration_ms)
        ;
}

void mhall_play()
{
    play(B4, 1);
    play(A4, 1);
    play(G4, 1);
    play(A4, 1);
    rest(1);

    play(B4, 1);
    play(B4, 1);
    play(B4, 1);
    rest(1);

    play(A4, 2);
    play(A4, 2);
    play(A4, 2);
    rest(1);

    play(B4, 3);
    play(B4, 3);
    play(B4, 3);
    rest(1);

    play(B4, 2);
    play(A4, 2);
    play(G4, 1);
    play(A4, 2);
    play(B4, 3);
    play(B4, 4);
    play(B4, 1);
    rest(1);

    play(A4, 2);
    play(A4, 3);
    play(B4, 2);
    play(A4, 3);
    play(G4, 3);
    rest(1);
}
int main()
{
    init();
   
        mhall_play();
    
    return 0;
}
