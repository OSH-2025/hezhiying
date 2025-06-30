# 工具链
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU 配置
CPU = cortex-m3
CFLAGS = -mcpu=$(CPU) -mthumb -Wall -Werror -O0 -g
CFLAGS += -DSTM32F103xG -Iinclude

DEVICE = STM32F103xG

# 链接脚本
LDSCRIPT = generated.$(DEVICE).ld
LDFLAGS = -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections

# 源文件
SRC_DIR = lightrt/

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:.c=.o)

OPENCM3_DIR = ./libopencm3

CFLAGS += -I$(OPENCM3_DIR)/include -DSTM32F1
LDFLAGS += -L$(OPENCM3_DIR)/lib -lopencm3_stm32f1

# 输出文件
TARGET = firmware
ELF = $(TARGET).elf
BIN = $(TARGET).bin

include $(OPENCM3_DIR)/mk/genlink-config.mk
# include $(OPENCM3_DIR)/mk/gcc-config.mk

all: $(LDSCRIPT) svcc $(ELF) $(BIN)

ldscript: $(LDSCRIPT)

$(ELF): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

svcc: svccompile/svcc.cpp $(SRC_DIR)/svc.def
	g++ svccompile/svcc.cpp -o svcc
	./svcc

SVCGEN = lightrt/svc.c lightrt/svc.h lightrt/user/syscall.c lightrt/user/syscall.h

clean:
	rm -f $(OBJS) $(ELF) $(BIN) $(LDSCRIPT) svcc $(SVCGEN)

include $(OPENCM3_DIR)/mk/genlink-rules.mk
# include $(OPENCM3_DIR)/mk/gcc-rules.mk

gdb:
	gdb-multiarch -ex "target remote:19000" $(ELF)

start:
	./renode/renode simulate/simulate.resc
