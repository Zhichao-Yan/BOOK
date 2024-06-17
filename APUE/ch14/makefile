ROOT=..
BUILD=bin
SRC_DIR=src
HOMEWORK_DIR=homework
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)
SRC = 14-1
HOMEWORK = 

all: $(SRC) $(HOMEWORK)

$(HOMEWORK): % : $(HOMEWORK_DIR)/%.c $(LIBAPUE)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -o $(BUILD)/$@ $(LDFLAGS) $(LDLIBS)

$(SRC): % : $(SRC_DIR)/%.c $(LIBAPUE)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -o $(BUILD)/$@ $(LDFLAGS) $(LDLIBS)

include $(ROOT)/Make.libapue.inc

clean:
	rm -rf $(BUILD)

.PHONY: all clean 