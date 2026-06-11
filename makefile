CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -fsanitize=address -g
GLORPBIN := glorpbin

# Piscines to build & test. Each is a directory next to this makefile that holds
# the same exercises; module dirs may be lowercase (c00) or uppercase (C00).
PISCINES := picine_1 picine_2

# Exercises (lib = <module>_<exercise>) run WITHOUT leak detection because a
# reference solution knowingly leaks there; every other exercise gets
# LeakSanitizer. c07_ex01 / c07_ex02: picine_2 mallocs before its min >= max
# guard and drops the pointer, leaking on the min == max path. Remove an entry to
# force that exercise leak-clean (it will then fail picine_2 until the leak is
# fixed).
LEAK_EXEMPT := c07_ex01 c07_ex02

# Header exercises (lib = <module>_<exercise>) ship a header, not a function, so
# there is no archive to link. The glorp test #includes the piscine's header and
# is compiled together with glorp/shared and -I<piscine exercise dir> instead of
# linking lib<lib>.a. The EXERCISES entry points at the header (its presence is
# the existence check). List such exercises here.
HEADER_EXERCISES := c08_ex01 c08_ex02 c08_ex03

# ==============================
# ADD YOUR EXERCISES HERE
# module is lowercase here; the glorp archive linked is lib<module>_<exercise>.a
# (the loop finds the real source dir whether it is cNN or CNN).
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
             c02/ex09/ft_strcapitalize.c \
             c03/ex00/ft_strcmp.c \
             c03/ex01/ft_strncmp.c \
             c03/ex02/ft_strcat.c \
             c03/ex03/ft_strncat.c \
             c03/ex04/ft_strstr.c \
             c03/ex05/ft_strlcat.c \
             c04/ex00/ft_strlen.c \
             c04/ex01/ft_putstr.c \
             c04/ex02/ft_putnbr.c \
             c04/ex03/ft_atoi.c \
             c04/ex04/ft_putnbr_base.c \
             c04/ex05/ft_atoi_base.c \
             c05/ex00/ft_iterative_factorial.c \
             c05/ex01/ft_recursive_factorial.c \
             c05/ex02/ft_iterative_power.c \
             c05/ex03/ft_recursive_power.c \
             c05/ex04/ft_fibonacci.c \
             c05/ex05/ft_sqrt.c \
             c05/ex06/ft_is_prime.c \
             c05/ex07/ft_find_next_prime.c \
             c05/ex08/ft_ten_queens_puzzle.c \
             c07/ex00/ft_strdup.c \
             c07/ex01/ft_range.c \
             c07/ex02/ft_ultimate_range.c \
             c07/ex03/ft_strjoin.c \
             c07/ex04/ft_convert_base.c \
             c07/ex05/ft_split.c \
             c08/ex01/ft_boolean.h \
             c08/ex02/ft_abs.h \
             c08/ex03/ft_point.h \

# ==============================

.PHONY: all test test-1 test-2 libs glorp clean

# Convenience only: run the two independent phases in order. You can just as
# well run `make libs` and `make test` separately.
all:
	$(MAKE) libs
	$(MAKE) test

# PHASE 1 -- build glorp (the test framework) into $(GLORPBIN)/*.a.
# This is fully separate from the piscine code: the executables below only LINK
# against these archives. Re-run this whenever you change a glorp test file.
libs glorp:
	$(MAKE) -C glorp all

# PHASE 2 -- build & run the piscine executables against the existing archives.
# Does NOT rebuild glorp; if an archive is missing you'll get a link error.
test:   PISCINE_LIST := $(PISCINES)
test-1: PISCINE_LIST := picine_1
test-2: PISCINE_LIST := picine_2

test test-1 test-2:
	@total_pass=0; total_fail=0; total_skip=0; \
	for pis in $(PISCINE_LIST); do \
		echo "================ $$pis ================"; \
		mkdir -p $${pis}_bin; \
		pass=0; fail=0; skip=0; \
		for ex in $(EXERCISES); do \
			mod=$${ex%%/*}; rest=$${ex#*/}; exo=$${rest%%/*}; file=$${rest#*/}; \
			lib=$${mod}_$${exo}; \
			moddir=$$mod; \
			[ -d "$$pis/$$moddir/$$exo" ] || moddir=$$(echo $$mod | tr '[:lower:]' '[:upper:]'); \
			src="$$pis/$$moddir/$$exo/$$file"; \
			bin="$${pis}_bin/$$lib"; \
			if [ ! -f "$$src" ]; then \
				echo "  [SKIP]  $$lib ($$src not found)"; skip=$$((skip+1)); continue; fi; \
				case " $(HEADER_EXERCISES) " in \
				*" $$lib "*) built=no; err=$$($(CC) $(CFLAGS) "glorp/tests/$$lib.c" glorp/shared/*.c -Iglorp/shared -I"$$pis/$$moddir/$$exo" -o "$$bin" 2>&1) && built=yes ;; \
				*) built=no; err=$$($(CC) $(CFLAGS) "$$pis/$$moddir/$$exo/"*.c -L$(GLORPBIN) -l$$lib -o "$$bin" 2>&1) && built=yes ;; \
				esac; \
				if [ "$$built" = yes ]; then \
				case " $(LEAK_EXEMPT) " in *" $$lib "*) leaks=0 ;; *) leaks=1 ;; esac; \
				if out=$$(ASAN_OPTIONS=detect_leaks=$$leaks ./"$$bin" 2>&1); then \
					echo "  [OK]    $$lib"; pass=$$((pass+1)); \
				else \
					echo "  [FAIL]  $$lib"; echo "$$out" | sed 's/^/          /'; fail=$$((fail+1)); fi; \
			else \
				echo "  [BUILD] $$lib"; echo "$$err" | sed 's/^/          /'; fail=$$((fail+1)); fi; \
		done; \
		echo "---- $$pis: $$pass passed, $$fail failed, $$skip skipped ----"; \
		echo; \
		total_pass=$$((total_pass+pass)); total_fail=$$((total_fail+fail)); total_skip=$$((total_skip+skip)); \
	done; \
	echo "================================================"; \
	echo "TOTAL: $$total_pass passed, $$total_fail failed, $$total_skip skipped"; \
	[ $$total_fail -eq 0 ]

clean:
	rm -rf picine_1_bin picine_2_bin
