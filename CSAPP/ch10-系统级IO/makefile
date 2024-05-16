CC := gcc
CFLAGS := -Wall 
BUILD := bin
INCLUDE := ../code
PRACTICE_DIR := ./practice
HOMEWORK_DIR := ./homework
SRC_DIR := ./src
practice := 10-1
homework :=

all: $(practice) $(homework)

$(practice): % : $(PRACTICE_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $< -o $(BUILD)/$@

$(homework): % : $(HOMEWORK_DIR)/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE)/csapp.c $< -o $(BUILD)/$@

clean:
	rm -rf $(BUILD)

.PHONY: all clean 