EXEC = as_exec

CC ?= gcc
CFLAGS = -Wall -std=gnu99 -g

.PHONY: all
all: $(EXEC)

OBJS = \
	vm.o \
	as.o \
	opcode.o \
	driver.o \
	elf.o

deps := $(OBJS:%.o=.%.o.d)


$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c opcode.h
	$(CC) $(CFLAGS) -c -o $@ -MMD -MF .$@.d $<

TEST_SRCS = $(wildcard tests/*.s)
TEST_DONE = $(TEST_SRCS:.s=.done)

PASS_COLOR = \x1b[32;01m
NO_COLOR = \x1b[0m

tests/%.done: tests/%.s
	@./$(EXEC) $< && /bin/echo -e "$(PASS_COLOR)$< pass$(NO_COLOR)\n"
check: $(EXEC) $(TEST_DONE)
	@$(RM) $(TEST_DONE)

test: $(EXEC)
	@python tests/runner.py

clean:
	$(RM) $(EXEC) $(OBJS) $(deps) opcode.h

opcode.h: scripts/gen_opcode.py opcode.def
	@python scripts/gen_opcode.py $@

-include $(deps)
