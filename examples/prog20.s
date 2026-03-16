.text
start:  addi x1, x0, 10       # x1 = 10
        addi x2, x0, 1        # x2 = 1
        add  x3, x1, x2       # x3 = 11
        sub  x4, x3, x2       # x4 = 10
        and  x5, x3, x4       # some bitwise ops
        or   x6, x3, x4
        xor  x7, x3, x6

loop:   addi x1, x1, -1       # decrement counter
        add  x8, x8, x2       # x8 accumulates 1 each loop
        bne  x1, x0, loop     # loop until x1 == 0

        lui  x9, 0x10010      # upper address
        addi x9, x9, 0        # base in x9

        sw   x8, 0(x9)        # store sum to memory[base]
        lw   x10, 0(x9)       # load it back

        addi x11, x0, -1      # negative value test
        slt  x12, x11, x0     # x12 = 1
        sltu x13, x11, x0     # x13 = 0 (unsigned)

end:    add  x14, x10, x12    # final result

