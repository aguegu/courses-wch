BIN     = ./bin
BIN_PREFIX = riscv-wch-elf

CFLAGS = -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g

SANDBOX_ROOT = ../../api/sandboxes/ch32v003

SANDBOX_DIST = $(SANDBOX_ROOT)/dist

OUTPUT_DIR = ./dist

TARGET     = $(addprefix $(OUTPUT_DIR)/, $(notdir $(CURDIR)))

Project_ROOT    = .
Project_INC_DIR = $(Project_ROOT)/inc
Project_SRC_DIR = $(Project_ROOT)
Project_SOURCES = $(notdir $(wildcard $(Project_SRC_DIR)/*.c))
Project_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(Project_SOURCES:.c=.o))

SYS_ROOT    = $(SANDBOX_ROOT)/Sys
SYS_INC_DIR = $(SYS_ROOT)/inc
SYS_SRC_DIR = $(SYS_ROOT)/src
SYS_SOURCES = $(notdir $(wildcard $(SYS_SRC_DIR)/*.c))
SYS_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(SYS_SOURCES:.c=.o))

Startup_ROOT		= $(SANDBOX_ROOT)/Startup
Startup_SOURCES = startup_ch32v00x.S
Startup_OBJECTS = $(addprefix $(SANDBOX_DIST)/asm/, $(Startup_SOURCES:.S=.o))

CORE_ROOT			= $(SANDBOX_ROOT)/Core
CORE_SOURCES 	= core_riscv.c
CORE_OBJECTS  = $(addprefix $(SANDBOX_DIST)/, $(CORE_SOURCES:.c=.o))

Peripheral_ROOT    = $(SANDBOX_ROOT)/Peripheral
Peripheral_INC_DIR = $(Peripheral_ROOT)/inc
Peripheral_SRC_DIR = $(Peripheral_ROOT)/src
Peripheral_SOURCES  = $(notdir $(wildcard $(Peripheral_SRC_DIR)/*.c))
Peripheral_OBJECTS = $(addprefix $(SANDBOX_DIST)/, $(Peripheral_SOURCES:.c=.o))

INCLUDES = -I $(CORE_ROOT) -I $(Peripheral_INC_DIR) -I $(Project_INC_DIR) -I $(SYS_INC_DIR)

VPATH = $(Startup_ROOT):$(CORE_ROOT):$(Peripheral_SRC_DIR):$(Project_SRC_DIR):$(SYS_SRC_DIR)

LINK_FILE = $(SANDBOX_ROOT)/Link.ld

LIBS =

all: $(OUTPUT_DIR) $(TARGET).hex $(TARGET).siz

version:
	$(BIN)/$(BIN_PREFIX)-gcc --version

$(OUTPUT_DIR):
	mkdir -p $(SANDBOX_DIST)/asm
	mkdir -p $(OUTPUT_DIR)/asm

clean:
	rm -rf $(SANDBOX_DIST)
	rm -rf $(OUTPUT_DIR)

$(SANDBOX_DIST)/asm/%.o: %.S
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) -MMD -MP -x assembler-with-cpp -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"

$(SANDBOX_DIST)/%.o: %.c
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) $(INCLUDES) $(DEFINES) -std=gnu99 -MMD -MP -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"

$(OUTPUT_DIR)/%.o: %.c
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) $(INCLUDES) $(DEFINES) -std=gnu99 -MMD -MP -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"


$(TARGET).elf: $(Project_OBJECTS) $(Startup_OBJECTS) $(CORE_OBJECTS) $(DEBUG_OBJECTS) $(Peripheral_OBJECTS) $(SYS_OBJECTS)
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) \
		-T "$(LINK_FILE)" -nostartfiles -Xlinker --gc-sections \
		-Wl,-Map,"$(@:%.elf=%.map)" --specs=nano.specs --specs=nosys.specs -o $@ $(OBJS) $^ $(LIBS)

$(TARGET).hex: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-objcopy -O ihex $< $@

$(TARGET).siz: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-size --format=berkeley $<

# flash: $(TARGET).hex
# 	wlink -v flash --address 0x08000000 $<

.PHONY: version clean flash
