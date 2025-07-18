# Toolchain (using gcc for everything)
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -O0 -g3 \
         -ffunction-sections -fdata-sections \
         -Wall -Wextra -Iinclude -nostdlib
ASFLAGS = -mcpu=cortex-m4 -mthumb -g3
LDFLAGS = -T linker.ld -mcpu=cortex-m4 -mthumb \
          -nostartfiles -Wl,--gc-sections \
          -Wl,-Map=$(BUILD_DIR)/kernel.map \
          -specs=nano.specs

# Directories
ASM_DIR = asm
KERNEL_DIR = kernel
USER_CODE_DIR = user/code
USER_ASM_DIR = user/asm
OBJ_DIR = obj
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
ASM_SRCS       = $(wildcard $(ASM_DIR)/*.S)
KERNEL_SRCS    = $(wildcard $(KERNEL_DIR)/*.c)
USER_CODE_SRCS = $(wildcard $(USER_CODE_DIR)/*.c)
USER_ASM_SRCS  = $(wildcard $(USER_ASM_DIR)/*.S)

# Object files
ASM_OBJS       = $(patsubst $(ASM_DIR)/%.S,$(OBJ_DIR)/asm_%.o,$(ASM_SRCS))
KERNEL_OBJS    = $(patsubst $(KERNEL_DIR)/%.c,$(OBJ_DIR)/kernel_%.o,$(KERNEL_SRCS))
USER_CODE_OBJS = $(patsubst $(USER_CODE_DIR)/%.c,$(OBJ_DIR)/usercode_%.o,$(USER_CODE_SRCS))
USER_ASM_OBJS  = $(patsubst $(USER_ASM_DIR)/%.S,$(OBJ_DIR)/userasm_%.o,$(USER_ASM_SRCS))

ALL_OBJS = $(ASM_OBJS) $(KERNEL_OBJS) $(USER_CODE_OBJS) $(USER_ASM_OBJS)

# Output files
ELF = $(BUILD_DIR)/kernel.elf
BIN = $(BUILD_DIR)/kernel.bin
HEX = $(BUILD_DIR)/kernel.hex

default: clean all flash

# Default target
all: $(ELF) $(BIN) $(HEX) size

# Create directories if they don't exist
$(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

# Compile assembly
$(OBJ_DIR)/asm_%.o: $(ASM_DIR)/%.S | $(OBJ_DIR)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJ_DIR)/userasm_%.o: $(USER_ASM_DIR)/%.S | $(OBJ_DIR)
	$(AS) $(ASFLAGS) -c $< -o $@

# Compile C files
$(OBJ_DIR)/kernel_%.o: $(KERNEL_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/usercode_%.o: $(USER_CODE_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(ELF): $(ALL_OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

# Convert ELF to BIN
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Convert ELF to HEX
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Print size info
size: $(ELF)
	$(SIZE) $<

# Flash (assumes OpenOCD already running on localhost:3333)
flash: $(ELF)
	@echo "Flashing via OpenOCD + GDB..."
	gdb-multiarch -x init.gdb

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: default all flash clean size