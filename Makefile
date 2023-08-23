.PHONY: all run clean check debug

CC := gcc-9

# compile flags
CFLAGS += -std=c18 -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings
CFLAGS += -Wvla -Wfloat-equal

DFLAGS := -g3
VFLAGS += --leak-check=full --track-origins=yes

SRC_DIR := src
OBJ_DIR := obj
DOCS_DIR := docs
TEST_DIR := test
HDR_DIR := include
LIB_DIR := lib

LIB := $(wildcard $(LIB_DIR)/*.a)
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# executable
BIN := radix
CHECK := $(BIN)_check

TESTS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(filter-out $(OBJ_DIR)/driver.o, $(OBJS))
TEST_OBJS += $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TESTS))
TEST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += $(DFLAGS) -pg
debug: $(BIN)

check: $(CHECK)

valgrind: CFLAGS += $(DFLAGS)
valgrind: clean $(BIN)
	@valgrind $(VFLAGS) ./$(BIN) $(ARGS)

clean: 
	@rm -rf $(BIN) $(OBJ_DIR) $(CHECK)
	
indent:
	indent -linux $(SRC_DIR)/*.c
	@rm $(SRC_DIR)/*.c~
	indent -linux $(TEST_DIR)/*.c
	@rm $(TEST_DIR)/*.c~
	indent -linux $(HDR_DIR)/*.h
	@rm $(HDR_DIR)/*.h~

# creates object files from .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

# creates executable from object
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) 

$(CHECK): $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS) $(LIB)
	./$(CHECK)

print:
	$(info $$SRCS is [$(SRCS)])
	$(info $$OBJS is [$(OBJS)])
	$(info $$TSTS is [$(TSTS)])
	$(info $$TST_OBJS is [$(TST_OBJS)])
	$(info $$TST_LIBS is [$(TST_LIBS)])