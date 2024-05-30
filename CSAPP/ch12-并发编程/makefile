CC := gcc
CFLAGS := -Wall 
BUILD := bin
INCLUDE := ../code
PRACTICE_DIR := ./practice
HOMEWORK_DIR := ./homework
SRC_DIR := ./src
practice := 
homework := 
src := hello echoservert sharing echoserverp select echoservers badcnt goodcnt

all: $(practice) $(homework) $(src)

$(practice): % : $(PRACTICE_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $< -o $(BUILD)/$@

$(homework): % : $(HOMEWORK_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $< -o $(BUILD)/$@

$(src): % : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $< -o $(BUILD)/$@

clean:
	rm -rf $(BUILD)

.PHONY: all clean 