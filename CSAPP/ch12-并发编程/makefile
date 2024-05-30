CC := gcc
CFLAGS := -Wall 
BUILD := bin
INCLUDE := ../code
PRACTICE_DIR := ./practice
HOMEWORK_DIR := ./homework
SRC_DIR := ./src
practice := 
homework := 
src := hello sharing \
echoservert echoserverp select echoservers \
badcnt goodcnt \
psum-mutex psum-array psum-local \
race norace


all: $(practice) $(homework) $(src) echoservert-pre

$(practice): % : $(PRACTICE_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $^ -o $(BUILD)/$@

$(homework): % : $(HOMEWORK_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $^ -o $(BUILD)/$@

$(src): % : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $^ -o $(BUILD)/$@

echoservert-pre: $(SRC_DIR)/echoservert-pre.c $(SRC_DIR)/echo-cnt.c $(SRC_DIR)/sbuf.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $^ -o $(BUILD)/$@

clean:
	rm -rf $(BUILD)

.PHONY: all clean 