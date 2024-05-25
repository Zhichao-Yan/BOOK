CC := gcc
CFLAGS := -Wall 
BUILD := bin
INCLUDE := ../code
PRACTICE_DIR := ./practice
HOMEWORK_DIR := ./homework
SRC_DIR := ./src
practice := 8-2 8-3 8-4 8-6 8-7 8-8
homework := 
src := fork waitpid1 waitpid2 shellex kill sigint sigintsafe signal1 signal2

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