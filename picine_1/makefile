CC       := gcc
CFLAGS   := -Wall -Wextra
GLORPBIN := ../glorpbin
#INCLUDE  := 
BIN_DIR  := ../picine_1_bin

# ==============================
# ADD YOUR EXERCISES HERE
# ==============================
EXERCISES := c00/ex00/ft_putchar.c #\
#             c00/ex01/ft_print_alphabet.c \
#             c00/ex02/ft_print_reverse_alphabet.c
# ==============================

# c00/ex00/ft_putchar.c -> c00_ex00
flatten = $(subst /,_,$(patsubst %/$(notdir $(1)),%,$(1)))

TARGETS := $(foreach src, $(EXERCISES), $(BIN_DIR)/$(call flatten,$(src)))

.PHONY: all clean test

test: all
	@passed=0; failed=0; \
	for t in $(TARGETS); do \
		echo "--- Running $$t ---"; \
		./$$t; \
		if [ $$? -eq 0 ]; then passed=$$((passed+1)); \
		else failed=$$((failed+1)); fi; \
	done; \
	echo "==========================="; \
	echo "Passed: $$passed  Failed: $$failed"

all: $(TARGETS)

define TEST_RULE
$(BIN_DIR)/$(call flatten,$(1)): $(1) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$< -L$(GLORPBIN) -l$(call flatten,$(1)) -o $$@
endef

$(foreach src, $(EXERCISES), $(eval $(call TEST_RULE,$(src))))

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)