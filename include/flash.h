#include <inttypes.h>

typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
} flash_regset_t;

#define FLASH_BASE (flash_regset_t *) 0x40023C00

#define SEC0_SIZE 16 * 1024
#define SEC1_SIZE 16 * 1024 
#define SEC2_SIZE 16 * 1024
#define SEC3_SIZE 16 * 1024
#define SEC4_SIZE 64 * 1024
#define SEC5_SIZE 128 * 1024
#define SEC6_SIZE 128 * 1024
#define SEC7_SIZE 128 * 1024

typedef struct {
    volatile uint8_t SEC0[SEC0_SIZE];
    volatile uint8_t SEC1[SEC1_SIZE];
    volatile uint8_t SEC2[SEC2_SIZE];
    volatile uint8_t SEC3[SEC3_SIZE];
    volatile uint8_t SEC4[SEC4_SIZE];
    volatile uint8_t SEC5[SEC5_SIZE];
    volatile uint8_t SEC6[SEC6_SIZE];
    volatile uint8_t SEC7[SEC7_SIZE];

} flash_sectors_t;

#define FLASH_MEM_BASE (flash_sectors_t *) 0x08000000

void flash_unlock(void);
void flash_sector_erase(uint8_t secnum);
void flash_write_address(volatile uint8_t *address, uint8_t val);
void flash_write_data(volatile uint8_t *address, uint8_t *data, uint32_t size);
