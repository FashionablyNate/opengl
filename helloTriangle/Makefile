CC := g++
DBGFLAGS := -g
CCOBJFLAGS := -c

# stores compiled code
OBJ_PATH := obj
# stores source code
SRC_PATH := src
# stores code compiled with -g flag
DBG_PATH := obj_dbg

# name of regular executable
TARGET := main
# name of debug executable
TARGET_DEBUG := dbg

# loops through everything in src folder with .c suffix
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
# loops through obj folder and matches basenames from src folder to fetch .o files
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
# does the same as above, but loops through obj_debug folder
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(OBJ_PATH) \
			  $(DBG_PATH) 

# default makes regular executables
default: makedir all
# debug makes debug executables
debug: makedir dbg	

# non-phony targets
# called with standard make, creates executable from object files
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
# creates object files
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

# called with make debug, creates object files with -g flag
$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $<
# creates executable from debug object files
$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
# creates directories
.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: dbg
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -rf $(CLEAN_LIST)
