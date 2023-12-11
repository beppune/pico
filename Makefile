SRC_DIR := src
INC_DIR := include
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))

CPPFLAGS := -iquote $(INC_DIR) -MMD -MP
CFLAGS := -g -Wall -Wpedantic 

all: pico

pico: $(OBJ) | $(BIN_DIR)
	$(CC) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean all

.PHONY: clean 

clean:
	rm -fr pico $(BIN_DIR)

-include $(OBJ:.o=.d)

