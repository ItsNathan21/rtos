#include "../include/flash.h"

#define KEY1 0x45670123
#define KEY2 0xCDEF89AB
#define BSY (1 << 16)

void flash_unlock(void) {
    flash_regset_t *flash = FLASH_BASE;
    while (flash->CR & BSY);
    flash->KEYR = KEY1;
    flash->KEYR = KEY2;
    while (flash->CR & BSY);
}

#define STRT (1 << 16)
#define SER (1 << 1)
#define MER (1 << 2)

void flash_sector_erase(uint8_t secnum) {
    flash_regset_t *flash = FLASH_BASE;
    while (flash->SR & BSY);
    flash->CR &= ~(0b111 << 3);
    flash->CR |= (secnum << 3);
    flash->CR |= SER;
    flash->CR |= STRT;
    while (flash->SR & BSY);
    flash->CR &= ~SER;
}

#define PG (1 << 0)

void flash_write_address(volatile uint8_t *address, uint8_t val) {
    flash_regset_t *flash = FLASH_BASE;
    while (flash->SR & BSY);
    flash->CR |= PG;
    *address = val;
    while (flash->SR & BSY);
    flash->CR &= ~PG;
}