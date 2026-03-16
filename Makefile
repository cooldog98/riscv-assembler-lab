CC = gcc
CFLAGS = -Wall -Wextra -Isrc

SRC = src/main.c src/assembler.c src/parser.c \
      src/registers.c src/instr_table.c \
      src/symbols.c src/encode.c

.PHONY: assembler test_registers test_parser test_symbols test_assembler clean

assembler: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o assembler

test_registers: tests/test_registers.c src/registers.c
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser: tests/test_parser.c src/parser.c
	$(CC) $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser

test_symbols: tests/test_symbols.c src/symbols.c
	$(CC) $(CFLAGS) tests/test_symbols.c src/symbols.c -o test_symbols

test_assembler: tests/test_assembler.c \
	src/assembler.c src/parser.c src/registers.c src/symbols.c \
	src/instr_table.c src/encode.c
	$(CC) $(CFLAGS) $^ -o test_assembler

clean:
	rm -f assembler test_registers test_parser test_symbols test_assembler

