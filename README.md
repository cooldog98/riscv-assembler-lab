# RISC-V Assembler Lab

This repository contains a simple two-pass assembler for a subset of the RISC-V RV32I instruction set, along with unit tests and example programs inspired by the CS61C RISC-V reference card.

## Project Layout

- `src/registers.c,h` – parse integer register names like `x5` → `5`.
- `src/parser.c,h` – split a source line into optional label, mnemonic, and operands.
- `src/symbols.c,h` – fixed-size array-based symbol table for labels.
- `src/instr_table.c,h` – instruction metadata table (format, opcode, funct3, funct7).
- `src/encode.c,h` – helpers to pack fields into R/I/S/B/U/J type encodings.
- `src/assembler.c,h` – two-pass assembler:
  - **Pass 1**: record labels and their byte addresses.
  - **Pass 2**: encode each instruction using the symbol table and instruction table.
- `src/main.c` – command‑line front-end that assembles a `.s` file and prints machine code.
- `tests/` – standalone unit tests for each module (`test_registers`, `test_parser`, `test_symbols`, `test_assembler`).
- `examples/` – larger RISC-V programs (`prog20.s`, `prog50.s`, `prog120.s`) for end-to-end testing.

## Building

From the repository root:

```bash
make assembler       # build the main assembler binary
make test_registers  # build register parser test
make test_parser     # build parser test
make test_symbols    # build symbol table test
make test_assembler  # build assembler pipeline test
```

All targets use `gcc` with `-Wall -Wextra` and include headers from `src/`.

## Running Unit Tests

After building:

```bash
./test_registers
./test_parser
./test_symbols
./test_assembler
```

Each test program exits silently on success and aborts with an assertion failure on error.

## Running the Assembler

To assemble a RISC-V source file (`.s`) and dump machine code (one 32‑bit word per line in hex):

```bash
./assembler examples/prog20.s
./assembler examples/prog50.s
./assembler examples/prog120.s
```

Example output line:

```text
0x00a00093
```

This value should match the instruction word produced by the Venus RISC-V simulator for the corresponding instruction and address.

## End-to-End Comparison with Venus

For each program in `examples/`:

1. Run the assembler and capture its output:

   ```bash
   ./assembler examples/prog20.s  > out_prog20.txt
   ./assembler examples/prog50.s  > out_prog50.txt
   ./assembler examples/prog120.s > out_prog120.txt
   ```

2. Load the same `.s` file into the Venus simulator, run it, and obtain the machine code dump.
3. Compare Venus’s instruction words with the corresponding lines in `out_prog*.txt`. They should match bit‑for‑bit for all supported instructions.

You can record your findings in a short test report (for example, `examples/test_report.txt`) showing Venus output alongside assembler output.

