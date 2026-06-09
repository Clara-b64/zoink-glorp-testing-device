
CC := gcc
CFLAGS := -Ishared
LINKER_FLAGS := 
AR := ar

ARTIFACT_DIR = ../artifacts
TEST_OBJ = ../artifacts_tests
GLORPBIN = ../glorpbin

SHARED_FILES = $(wildcard shared/*.c)
SHARED_ARTIFACTS = $(patsubst shared/%.c, $(ARTIFACT_DIR)/%.o, $(SHARED_FILES))

TEST_FILES = $(wildcard tests/*.c)
TEST_OBJECTS = $(patsubst tests/%.c, $(GLORPBIN)/lib%.a, $(TEST_FILES))

$(info TEST_OBJECTS = $(TEST_OBJECTS))

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
