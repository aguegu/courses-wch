BIN     = ./bin
BIN_PREFIX = riscv-wch-elf

CFLAGS = -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g

SCAFFOLD_ROOT = ..

SCAFFOLD_DIST = $(SCAFFOLD_ROOT)/dist

OUTPUT_DIR = ./dist

TARGET     = $(addprefix $(OUTPUT_DIR)/, $(notdir $(CURDIR)))

Project_ROOT    = .
Project_INC_DIR = $(Project_ROOT)/inc
Project_SRC_DIR = $(Project_ROOT)
Project_SOURCES = $(notdir $(wildcard $(Project_SRC_DIR)/*.c))
Project_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(Project_SOURCES:.c=.o))

Sys_ROOT    = $(SCAFFOLD_ROOT)/Sys
Sys_INC_DIR = $(Sys_ROOT)/inc
Sys_SRC_DIR = $(Sys_ROOT)/src
Sys_SOURCES = $(notdir $(wildcard $(Sys_SRC_DIR)/*.c))
Sys_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(Sys_SOURCES:.c=.o))

Startup_ROOT		= $(SCAFFOLD_ROOT)/Startup
Startup_SOURCES = startup_ch32v00x.S
Startup_OBJECTS = $(addprefix $(SCAFFOLD_DIST)/asm/, $(Startup_SOURCES:.S=.o))

CORE_ROOT			= $(SCAFFOLD_ROOT)/Core
CORE_SOURCES 	= core_riscv.c
CORE_OBJECTS  = $(addprefix $(SCAFFOLD_DIST)/, $(CORE_SOURCES:.c=.o))

Peripheral_ROOT    = $(SCAFFOLD_ROOT)/Peripheral
Peripheral_INC_DIR = $(Peripheral_ROOT)/inc
Peripheral_SRC_DIR = $(Peripheral_ROOT)/src
Peripheral_SOURCES  = $(notdir $(wildcard $(Peripheral_SRC_DIR)/*.c))
Peripheral_OBJECTS = $(addprefix $(SCAFFOLD_DIST)/, $(Peripheral_SOURCES:.c=.o))

Debug_ROOT		= $(SCAFFOLD_ROOT)/Debug
Debug_SOURCES = debug.c
Debug_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(Debug_SOURCES:.c=.o))

INCLUDES = -I $(CORE_ROOT) -I $(Peripheral_INC_DIR) -I $(Project_INC_DIR) -I $(Sys_INC_DIR) -I $(Debug_ROOT)

VPATH = $(Startup_ROOT):$(CORE_ROOT):$(Peripheral_SRC_DIR):$(Project_SRC_DIR):$(Sys_SRC_DIR):$(Debug_ROOT)

LINK_FILE = $(SCAFFOLD_ROOT)/Ld/Link.ld

all: $(OUTPUT_DIR) $(TARGET).hex $(TARGET).siz

version:
	$(BIN)/$(BIN_PREFIX)-gcc --version

$(OUTPUT_DIR):
	mkdir -p $(SCAFFOLD_DIST)/asm
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(SCAFFOLD_DIST)
	rm -rf $(OUTPUT_DIR)

$(SCAFFOLD_DIST)/asm/%.o: %.S
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) -MMD -MP -x assembler-with-cpp -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"

$(SCAFFOLD_DIST)/%.o: %.c
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) $(INCLUDES) $(DEFINES) -std=gnu99 -MMD -MP -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"

$(OUTPUT_DIR)/%.o: %.c
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) $(INCLUDES) $(DEFINES) -std=gnu99 -MMD -MP -MF "$(@:%.o=%.d)" -MT $@ -c -o "$@" "$<"


$(TARGET).elf: $(Project_OBJECTS) $(Startup_OBJECTS) $(CORE_OBJECTS) $(Debug_OBJECTS) $(Peripheral_OBJECTS) $(Sys_OBJECTS)
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) \
		-T "$(LINK_FILE)" -nostartfiles -Xlinker --gc-sections \
		-Wl,-Map,"$(@:%.elf=%.map)" --specs=nano.specs --specs=nosys.specs -o $@ $(OBJS) $^

$(TARGET).hex: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-objcopy -O ihex $< $@

$(TARGET).siz: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-size --format=berkeley $<

# flash: $(TARGET).hex
# 	wlink -v flash --address 0x08000000 $<

.PHONY: version clean flash
