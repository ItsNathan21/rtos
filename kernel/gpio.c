#include <gpio.h>
#include <rcc.h>

#define BITS_PER_ALT 4
#define BITS_PER_MODE 2
#define BITS_PER_SPEED 2
#define BITS_PER_PUPD 2
#define BITS_PER_TYPE 1
#define GPIOS_PER_ALT_REG 8

typedef struct{
    volatile unsigned long mode;    /**< 0 - Mode */
    volatile unsigned long o_type;  /**< 4 - Output Type */
    volatile unsigned long o_speed; /**< 8 - Output Speed */
    volatile unsigned long pu_pd;   /**< C - Pull-Up/Pull-Down */
    volatile unsigned long idr;     /**< 10 - Input Data */
    volatile unsigned long odr;     /**< 14 - Output Data*/
    volatile unsigned long bsrr;    /**< 18 - Set/Reset */
    volatile unsigned long lckr;    /**< 1C - Configuration Lock */
    volatile unsigned long afr[2];  /**< 20 - Alternate Function Control*/
} gpio_reg;

const int32_t gpio_en[] = {0x01, 0x02, 0x04, 0x08, 0x10};
gpio_reg* const gpio_regs[] = {(void*)0x40020000, (void*)0x40020400, (void*)0x40020800};


void gpio_init(gpio_port port, unsigned int num, unsigned int mode, unsigned int otype, unsigned int speed, unsigned int pupd, unsigned int alt){
    struct rcc_reg_map *rcc = (struct rcc_reg_map *)RCC_BASE;
    rcc->ahb1_enr |= gpio_en[port];  

    gpio_reg *gp = gpio_regs[port];

    gp->mode |= (mode << (num * BITS_PER_MODE));
    gp->o_type |= (otype << (num * BITS_PER_TYPE));
    gp->o_speed |= (speed << (num * BITS_PER_SPEED));
    gp->pu_pd |= (pupd << (num * BITS_PER_PUPD));

    int high = num >= GPIOS_PER_ALT_REG;

    int shift_num = num % 8;
    gp->afr[high] |= (alt << (shift_num * BITS_PER_ALT));
}

void gpio_set(gpio_port port, unsigned int num){
    gpio_regs[port]->bsrr = 1 << num;  
}


void gpio_clr(gpio_port port, unsigned int num){
    gpio_regs[port]->bsrr = 1 << (num + 16);   
}

int32_t gpio_read(gpio_port port, unsigned int num) {
    return !!(gpio_regs[port]->idr & (1 << num));
}