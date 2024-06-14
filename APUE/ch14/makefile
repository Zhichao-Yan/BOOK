ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)
SRC = 

all: $(homework) $(src) 

$(homework): % : $(HOMEWORK_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $(BUILD)/$@

$(src): % : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $(BUILD)/$@

clean:
	rm -rf $(BUILD)

.PHONY: all clean 