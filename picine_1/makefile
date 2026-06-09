CC       := gcc
CFLAGS   := -Wall -Wextra -Werror
GLORPBIN := ../glorpbin
#INCLUDE  := 
BIN_DIR  := ../picine_1_bin

# ==============================
# ADD YOUR EXERCISES HERE
# ==============================
EXERCISES := c00/ex00/ft_putchar.c \
             c00/ex01/ft_print_alphabet.c \
             c00/ex02/ft_print_reverse_alphabet.c \
             c00/ex03/ft_print_numbers.c \
             c00/ex04/ft_is_negative.c \
             c00/ex05/ft_print_comb.c \
             c00/ex06/ft_print_comb2.c \
             c00/ex07/ft_putnbr.c \
             c00/ex08/ft_print_combn.c \
             c01/ex00/ft_ft.c \
             c01/ex01/ft_ultimate_ft.c \
             c01/ex02/ft_swap.c \
             c01/ex03/ft_div_mod.c \
             c01/ex04/ft_ultimate_div_mod.c \
             c01/ex05/ft_putstr.c \
             c01/ex06/ft_strlen.c \
             c01/ex07/ft_rev_int_tab.c \
             c01/ex08/ft_sort_int_tab.c \
             c02/ex00/ft_strcpy.c \
             c02/ex01/ft_strncpy.c \
             c02/ex02/ft_str_is_alpha.c \
             c02/ex03/ft_str_is_numeric.c \
             c02/ex04/ft_str_is_lowercase.c \
             c02/ex05/ft_str_is_uppercase.c \
             c02/ex06/ft_str_is_printable.c \
             c02/ex07/ft_strupcase.c \
             c02/ex08/ft_strlowcase.c \
             c02/ex09/ft_strcapitalize.c
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