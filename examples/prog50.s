.text
# Simple loop with memory and branches (≈50+ instructions)

init:   addi x1, x0, 0        # i = 0
        addi x2, x0, 1        # step = 1
        addi x3, x0, 20       # limit = 20

        lui  x4, 0x10010      # base addr high
        addi x4, x4, 0        # base = 0x10010000 (in Venus)

        addi x5, x0, 0        # sum = 0
        addi x6, x0, 2        # mul = 2

store_loop:
        add  x7, x1, x2       # tmp = i + 1
        add  x8, x7, x6       # tmp2 = tmp + 2
        add  x5, x5, x8       # sum += tmp2

        sll  x9, x1, x2       # idx = i << 1
        add  x10, x4, x9      # addr = base + idx
        sw   x8, 0(x10)       # store tmp2

        addi x1, x1, 1        # i++
        blt  x1, x3, store_loop

verify_loop:
        addi x1, x0, 0        # i = 0
        addi x11, x0, 0       # sum2 = 0

verify_body:
        sll  x9, x1, x2       # idx = i << 1
        add  x10, x4, x9      # addr = base + idx
        lw   x12, 0(x10)      # load value
        add  x11, x11, x12    # sum2 += value

        addi x1, x1, 1
        blt  x1, x3, verify_body

done:
        sub  x13, x5, x11     # should be 0 if ok

        addi x14, x0, 5
        slli x15, x14, 1      # test shifts
        srli x16, x15, 1
        srai x17, x15, 1

        beq  x13, x0, success
        jal  x0, failure

success:
        addi x18, x0, 1
        jal  x0, end

failure:
        addi x18, x0, -1

end:
        add  x19, x18, x13

