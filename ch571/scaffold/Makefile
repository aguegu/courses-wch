BIN     = ./bin
BIN_PREFIX = riscv-wch-elf

CFLAGS 	= -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -g -fdiagnostics-color=always

SANDBOX_ROOT = ../../api/sandboxes/ch571

SANDBOX_DIST = $(SANDBOX_ROOT)/dist

OUTPUT_DIR = ./dist

TARGET     = $(addprefix $(OUTPUT_DIR)/, $(notdir $(CURDIR)))

Project_ROOT    = .
Project_SRC_DIR = $(Project_ROOT)
Project_SOURCES = $(notdir $(wildcard $(Project_SRC_DIR)/*.c))
Project_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(Project_SOURCES:.c=.o))

RVMSIS_ROOT			= $(SANDBOX_ROOT)/RVMSIS
RVMSIS_SOURCES 	= core_riscv.c
RVMSIS_OBJECTS  = $(addprefix $(SANDBOX_DIST)/, $(RVMSIS_SOURCES:.c=.o))

StdPeriphDriver_ROOT    = $(SANDBOX_ROOT)/StdPeriphDriver
StdPeriphDriver_INC_DIR = $(StdPeriphDriver_ROOT)/inc
StdPeriphDriver_SRC_DIR = $(StdPeriphDriver_ROOT)
StdPeriphDriver_SOURCES  = $(notdir $(wildcard $(StdPeriphDriver_SRC_DIR)/*.c))
StdPeriphDriver_OBJECTS = $(addprefix $(SANDBOX_DIST)/, $(StdPeriphDriver_SOURCES:.c=.o))

Startup_ROOT		= $(SANDBOX_ROOT)/Startup
Startup_SOURCES = startup_CH573.S
Startup_OBJECTS = $(addprefix $(SANDBOX_DIST)/asm/, $(Startup_SOURCES:.S=.o))

LINK_FILE = $(SANDBOX_ROOT)/ch571-app.ld

INCLUDES = -I $(StdPeriphDriver_INC_DIR) -I $(RVMSIS_ROOT)

VPATH = $(Project_SRC_DIR):$(StdPeriphDriver_SRC_DIR):$(Startup_ROOT):$(RVMSIS_ROOT)

LIBS = -lISP573

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

$(TARGET).elf: $(Project_OBJECTS) $(StdPeriphDriver_OBJECTS) $(Startup_OBJECTS) $(RVMSIS_OBJECTS)
	$(BIN)/$(BIN_PREFIX)-gcc $(CFLAGS) \
		-T "$(LINK_FILE)" -nostartfiles -Xlinker --gc-sections \
		-L"$(StdPeriphDriver_ROOT)" \
		-Xlinker --print-memory-usage -Wl,-Map,"$(@:%.elf=%.map)" --specs=nano.specs --specs=nosys.specs -o $@ $^ $(LIBS)

$(TARGET).hex: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-objcopy -O ihex $< $@

$(TARGET).siz: $(TARGET).elf
	$(BIN)/$(BIN_PREFIX)-size --format=berkeley $^

.PHONY: version clean flash lint
