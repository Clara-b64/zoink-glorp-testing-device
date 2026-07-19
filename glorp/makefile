
CC := gcc
CFLAGS := -Ishared -fsanitize=address -g
LINKER_FLAGS := 
AR := ar

ARTIFACT_DIR = ../artifacts
TEST_OBJ = ../artifacts_tests
GLORPBIN = ../glorpbin

SHARED_FILES = $(wildcard shared/*.c)
SHARED_ARTIFACTS = $(patsubst shared/%.c, $(ARTIFACT_DIR)/%.o, $(SHARED_FILES))

# Header exercises (e.g. ft_boolean.h) need a piscine's header on the include
# path, so their test files are compiled per-piscine by the ROOT makefile and are
# NOT archived here (they will not compile standalone without that header).
HEADER_TESTS = tests/c08_ex00.c tests/c08_ex01.c tests/c08_ex02.c tests/c08_ex03.c
TEST_FILES = $(filter-out $(HEADER_TESTS), $(wildcard tests/*.c))
TEST_OBJECTS = $(patsubst tests/%.c, $(GLORPBIN)/lib%.a, $(TEST_FILES))

#$(info TEST_OBJECTS = $(TEST_OBJECTS))

.PHONY: all
all: $(TEST_OBJECTS)

.PHONY: clean
clean:
	rm -rf $(SHARED_ARTIFACTS) $(TEST_OBJECTS)

$(ARTIFACT_DIR) $(GLORPBIN) $(TEST_OBJ):
	mkdir -p $@

$(ARTIFACT_DIR)/%.o: shared/%.c | $(ARTIFACT_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(TEST_OBJ)/%.o: tests/%.c | $(TEST_OBJ)
	$(CC) -c $(CFLAGS) $< -o $@

$(GLORPBIN)/lib%.a: $(TEST_OBJ)/%.o $(SHARED_ARTIFACTS) | $(GLORPBIN)
	$(AR) rcs $@ $^
