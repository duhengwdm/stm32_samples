
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define BRICK_BUTTON

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x04)
#define RCC_APB2RSTR    (RCCBASE+0x0C)
#define RCC_APB1RSTR    (RCCBASE+0x10)
#define RCC_AHBENR      (RCCBASE+0x14)
#define RCC_APB2ENR     (RCCBASE+0x18)
#define RCC_APB1ENR     (RCCBASE+0x1C)
#define RCC_CFGR2       (RCCBASE+0x2C)

#define GPIOABASE 0x48000000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_IDR       (GPIOABASE+0x10)
#define GPIOA_ODR       (GPIOABASE+0x14)
#define GPIOA_BSRR      (GPIOABASE+0x18)
#define GPIOA_LCKR      (GPIOABASE+0x1C)
#define GPIOA_AFRL      (GPIOABASE+0x20)
#define GPIOA_AFRH      (GPIOABASE+0x24)
#define GPIOA_BRR       (GPIOABASE+0x28)

#define GPIOBBASE 0x48000400
#define GPIOB_MODER     (GPIOBBASE+0x00)
#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_OSPEEDR   (GPIOBBASE+0x08)
#define GPIOB_PUPDR     (GPIOBBASE+0x0C)
#define GPIOB_IDR       (GPIOBBASE+0x10)
#define GPIOB_ODR       (GPIOBBASE+0x14)
#define GPIOB_BSRR      (GPIOBBASE+0x18)
#define GPIOB_LCKR      (GPIOBBASE+0x1C)
#define GPIOB_AFRL      (GPIOBBASE+0x20)
#define GPIOB_AFRH      (GPIOBBASE+0x24)
#define GPIOB_BRR       (GPIOBBASE+0x28)

#define GPIOCBASE 0x48000800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)
#define GPIOC_ODR       (GPIOCBASE+0x14)
#define GPIOC_BSRR      (GPIOCBASE+0x18)
#define GPIOC_LCKR      (GPIOCBASE+0x1C)
#define GPIOC_AFRL      (GPIOCBASE+0x20)
#define GPIOC_AFRH      (GPIOCBASE+0x24)
#define GPIOC_BRR       (GPIOCBASE+0x28)

#define USART1BASE 0x40013800
#define USART1_CR1      (USART1BASE+0x00)
#define USART1_BRR      (USART1BASE+0x0C)
#define USART1_ISR      (USART1BASE+0x1C)
#define USART1_RDR      (USART1BASE+0x24)
#define USART1_TDR      (USART1BASE+0x28)

#define USART2BASE 0x40004400
#define USART2_CR1      (USART2BASE+0x00)
#define USART2_BRR      (USART2BASE+0x0C)
#define USART2_ISR      (USART2BASE+0x1C)
#define USART2_RDR      (USART2BASE+0x24)
#define USART2_TDR      (USART2BASE+0x28)

#define FLASHBASE 0x40022000
#define FLASH_ACR (FLASHBASE+0x00)

//PA9 is USART1_TX alternate function 1
//PA10 is USART1_RX alternate function 1

//PA2 is USART2_TX alternate function 1
//PA3 is USART2_RX alternate function 1

//------------------------------------------------------------------------
#ifdef BRICK_BUTTON
//------------------------------------------------------------------------
static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<19; //enable port C
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOC_MODER);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_MODER,ra);

    ra=GET32(GPIOC_PUPDR);
    ra&=~(3<<26); //PC13
    PUT32(GPIOC_PUPDR,ra);
}
//------------------------------------------------------------------------
static unsigned int read_button ( void )
{
    return(GET32(GPIOC_IDR)&(1<<13));
}
//------------------------------------------------------------------------
static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOA_MODER,ra);
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<5); //PA5
    PUT32(GPIOA_OTYPER,ra);
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<10; //PA5
    PUT32(GPIOA_OSPEEDR,ra);
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<10); //PA5
    PUT32(GPIOA_PUPDR,ra);
}
//------------------------------------------------------------------------
static void led_on ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<0));
}
//------------------------------------------------------------------------
static void led_off ( void )
{
    PUT32(GPIOA_BSRR,((1<<5)<<16));
}
//------------------------------------------------------------------------
#endif //BRICK_BUTTON
//------------------------------------------------------------------------
static int clock_init ( void )
{
    unsigned int ra;

#ifdef BRICK_BUTTON
    //this is a safety measure, if the code below that messes with
    //the clock source, bricks the microcontroller then press and
    //hold the user button, and press reset, on that reset this code
    //will see the button pressed and go into this infinite loop
    //instead of into the code below, allowing us to reprogram the
    //microcontrollers flash with hopefully a not buggy program.
    button_init();
    if(read_button()==0)
    {
        //make the led glow
        led_init();
        while(1)
        {
            led_on();
            led_off();
        }
    }
    //as delivered this code does not brick the microcontroller
    //the above was for me to develop this code and for you to change
    //this code as desired.
#endif //BRICK_BUTTON

    //switch to external clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==1) break;





    //make sure hse is up, external 8mhz clock
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    //use the hse clock for now
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==1) break;


    //in order to change any pll settings must disable pll first
    ra=GET32(RCC_CR);
    ra&=~(1<<24);
    PUT32(RCC_CR,ra);
    while(1) if((GET32(RCC_CR)&(1<<25))==0) break;
    //no hse pre-division
    PUT32(RCC_CFGR2,0);
    //set other pll settings 6*8=48mhz, max speed for this part
    ra=GET32(RCC_CFGR);
    ra&=~(0x3F<<16);
    ra|=0x4<<18; //multiply by 6
    ra|=1<<16; //hse source
    PUT32(RCC_CFGR,ra);
    //turn pll back on with new settings and wait for it to be ready
    ra=GET32(RCC_CR);
    ra|=(1<<24);
    PUT32(RCC_CR,ra);
    while(1) if((GET32(RCC_CR)&(1<<25))!=0) break;
    //We cannot overclock the flash, so slow down the flash first
    //then speed up the clock
    //PUT32(FLASH_ACR,0x01); //>24Mhz no prefetch
    PUT32(FLASH_ACR,0x11); // >24MHZ with prefetch.
    //switch system to use pll clock
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=2;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==2) break;
    //now running at 48MHz.

    return(0);
}
//------------------------------------------------------------------------
int uart2_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17; //enable USART2
    PUT32(RCC_APB1ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra|=2<<4; //PA2
    ra|=2<<6; //PA3
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRL);
    ra&=~(0xF<<8); //PA2
    ra&=~(0xF<<12); //PA3
    ra|=0x1<<8; //PA2
    ra|=0x1<<12; //PA3
    PUT32(GPIOA_AFRL,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<17; //reset USART2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<17);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(USART2_BRR,417);
    PUT32(USART2_CR1,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart2_send ( unsigned int x )
{
    while(1) if(GET32(USART2_ISR)&(1<<7)) break;
    PUT32(USART2_TDR,x);
}
//------------------------------------------------------------------------
unsigned int uart2_recv ( void )
{
    while(1) if((GET32(USART2_ISR))&(1<<5)) break;
    return(GET32(USART2_RDR));
}
////------------------------------------------------------------------------
//unsigned int uart2_check ( void )
//{
    //if((GET32(USART2_ISR))&(1<<5)) return(1);
    //return(0);
//}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart2_send(rc);
        if(rb==0) break;
    }
    uart2_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart2_send(0x0D);
    uart2_send(0x0A);
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static void scl_high ( void )
{
    PUT32(GPIOB_BSRR,((1<<8)<<0));
}
//------------------------------------------------------------------------
static void scl_low ( void )
{
    PUT32(GPIOB_BSRR,((1<<8)<<16));
}
//------------------------------------------------------------------------
static void sda_high ( void )
{
    PUT32(GPIOB_BSRR,((1<<9)<<0));
}
//------------------------------------------------------------------------
static void sda_low ( void )
{
    PUT32(GPIOB_BSRR,((1<<9)<<16));
}
//------------------------------------------------------------------------
static void sda_output ( void )
{
    unsigned int ra;

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<18); //PB9
    ra|=1<<18; //PB9
    PUT32(GPIOB_MODER,ra);
}
//------------------------------------------------------------------------
static void sda_input ( void )
{
    unsigned int ra;

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<18); //PB9
    PUT32(GPIOB_MODER,ra);
}
//------------------------------------------------------------------------
static unsigned int sda_read ( void )
{
    return(GET32(GPIOB_IDR)&(1<<9));
}
//------------------------------------------------------------------------
static void i2c_init ( void )
{
    unsigned int ra;

    //make them outputs.

    ra=GET32(RCC_AHBENR);
    ra|=1<<18; //enable port B
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=~(3<<16); //PB8
    ra&=~(3<<18); //PB9
    ra|=1<<16; //PB8
    ra|=1<<18; //PB9
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=~(1<<8); //PB8
    ra&=~(1<<9); //PB9
    PUT32(GPIOB_OTYPER,ra);

    ra=GET32(GPIOB_OSPEEDR);
    ra|=3<<16; //PB8
    ra|=3<<18; //PB9
    PUT32(GPIOB_OSPEEDR,ra);

    ra=GET32(GPIOB_PUPDR);
    ra&=~(3<<16); //PB8
    ra&=~(3<<18); //PB9
    ra|=1<<18; //PB9
    PUT32(GPIOB_PUPDR,ra);

    scl_high();
    sda_low();
    sda_high();
}
//------------------------------------------------------------------------
#define I2C_DELAY_COUNT 100
static void i2c_delay ( void )
{
    unsigned int ra;

    for(ra=0;ra<I2C_DELAY_COUNT;ra++) dummy(ra);
}
//------------------------------------------------------------------------
static void i2c_start ( void )
{
    sda_low();
    i2c_delay();
    scl_low();
    i2c_delay();
}
//------------------------------------------------------------------------
static void i2c_stop ( void )
{

    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
}
//------------------------------------------------------------------------
static unsigned int i2c_write ( unsigned int data )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0x80;ra;ra>>=1)
    {
        if(ra&data) sda_high();
        else        sda_low();
        i2c_delay();
        scl_high();
        i2c_delay();
        scl_low();
        i2c_delay();
    }
    sda_input();
    i2c_delay();
    scl_high();
    i2c_delay();
    rb=sda_read();
    scl_low();
    i2c_delay();
    sda_output();
    i2c_delay();
    return(rb);
}
//------------------------------------------------------------------------
static unsigned int i2c_read ( void )
{
    unsigned int ra;
    unsigned int rb;

    rb=0;
    sda_input();
    for(ra=0x80;ra;ra>>=1)
    {
        i2c_delay();
        scl_high();
        i2c_delay();
        if(sda_read()) rb|=ra;
        scl_low();
        i2c_delay();
    }
    sda_output();
    i2c_delay();
    sda_low();
    i2c_delay();
    scl_high();
    i2c_delay();
    scl_low();
    i2c_delay();
    return(rb);
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int rx;
    unsigned int last;
    unsigned int now;

    unsigned int d[4];

    clock_init();
    uart2_init();
    for(rx=0;rx<20;rx++)
    {
        uart2_send(0x30+(rx&7));
    }
    uart2_send(0x0D);
    uart2_send(0x0A);


    i2c_init();

    //for(rx=0;rx<256;rx+=2)
    //{
        //i2c_start();
        //if(i2c_write(rx|0)) ; else hexstring(rx);
        //i2c_stop();
    //}

    //i2c_start();
    //i2c_write(0xD0);
    //i2c_write(0x00);
    //i2c_stop();
    //i2c_start();
    //i2c_write(0xD0);
    //i2c_write(0x00);
    //i2c_write(0x20);
    //i2c_write(0x38);
    //i2c_write(0x15);
    //i2c_stop();

    last=0;

    for(rx=0;;rx++)
    {
        i2c_start();
        i2c_write(0xD0);
        i2c_write(0x00);
        //i2c_stop();
        //i2c_start();
        sda_high();
        i2c_delay();
        scl_high();
        i2c_delay();
        sda_low();
        i2c_delay();
        scl_low();

        i2c_write(0xD1);
        d[0]=i2c_read();
        d[1]=i2c_read();
        d[2]=i2c_read();
        i2c_stop();


        now=d[2]; now<<=8;
        now|=d[1]; now<<=8;
        now|=d[0];
        if(now!=last)
        {
            last=now;
            hexstring(last);
        }
    }
    hexstring(0xAABBCCDD);

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
