CC = gcc
CPP = g++
AS = nasm

OBJ_DIR = ./obj
BIN_DIR = ../bin
LIB_DIR = ../modules

INCLUDE = -I ./ -I ./amx/
CFLAGS = -Wwrite-strings -DNDEBUG -DAMX_ALTCORE -Wl,-E -g
CPPFLAGS = -Wwrite-strings -DNDEBUG -Wl,-E -g
LINK =
LIBS = -ldl -lpthread

ifdef JIT
	CFLAGS += -DAMX_JIT
	CPPFLAGS += -DAMX_JIT
endif

AMX_SRC = amx/getch.c amx/amx.c amx/amxexec_gcc.c amx/amxcons.c amx/amxcore.c amx/amxfloat.c \
		amx/amxfixed.c amx/amxstring.c amx/amxargs.c amx/amxdgram.c amx/amxtime.c

AMX_OBJ := $(AMX_SRC:%.c=$(OBJ_DIR)/%.o)
AMX_OBJ := $(AMX_OBJ:%.cpp=$(OBJ_DIR)/%.obj)
AMX_OBJ := $(AMX_OBJ:%.asm=$(OBJ_DIR)/%.oasm)

ARGUS_SRC = ini.c cthreadmgr.cpp cconfig.cpp argus.cpp cplugin.cpp
ARGUS_OBJ := $(ARGUS_SRC:%.c=$(OBJ_DIR)/%.o)
ARGUS_OBJ := $(ARGUS_OBJ:%.cpp=$(OBJ_DIR)/%.obj)

AMXRUN_SRC = testrun.cpp cplugin.cpp
AMXRUN_OBJ := $(AMXRUN_SRC:%.c=$(OBJ_DIR)/%.o)
AMXRUN_OBJ := $(AMXRUN_OBJ:%.cpp=$(OBJ_DIR)/%.obj)

MODULES_SRC = modules/amxpower.cpp
MODULES_LIB := $(notdir $(MODULES_SRC))
MODULES_LIB := $(MODULES_LIB:%.cpp=$(LIB_DIR)/%.so)

all: argusrun amxrun modules

argusrun: $(BIN_DIR)/argusrun

amxrun: $(BIN_DIR)/amxrun

amx: $(AMX_OBJ)

modules: $(MODULES_LIB)

remake: clean all

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(CFLAGS) $(LINK) $(LIBS) -c $< -o $@

$(OBJ_DIR)/%.obj: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(INCLUDE) $(CPPFLAGS) $(LINK) $(LIBS) -c $< -o $@

$(OBJ_DIR)/%.oasm: %.asm
	@mkdir -p $(dir $@)
	$(AS) -O2 -f elf32 $(INCLUDE) $< -o $@

$(BIN_DIR)/argusrun: $(ARGUS_OBJ) $(AMX_OBJ)
	@mkdir -p $(dir $@)
	$(CPP) $(INCLUDE) $(CFLAGS) $(LINK) $(LIBS) $(ARGUS_OBJ) $(AMX_OBJ) -o $@

$(BIN_DIR)/amxrun: $(AMXRUN_OBJ) $(AMX_OBJ)
	@mkdir -p $(dir $@)
	$(CPP) $(INCLUDE) $(CFLAGS) $(LINK) $(LIBS) $(AMXRUN_OBJ) $(AMX_OBJ) -o $@

$(LIB_DIR)/%.so: modules/$(notdir %.cpp)
	@mkdir -p $(dir $@)
	$(CPP) -O2 $(INCLUDE) $< -o $@ -shared
