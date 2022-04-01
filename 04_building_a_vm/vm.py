#!/usr/bin/env python3

def load_word(vm, mem, reg, addr):
    # load value in addr to reg
    vm[reg] = mem[addr] + 256 * mem[addr+1]

def store_word(vm, mem, reg, addr):
    mem[addr] = vm[reg] % 256
    mem[addr + 1] = vm[reg] // 256

def add(vm, reg1, reg2):
    vm[0x01] = vm[reg1] + vm[reg2]

def sub(vm, reg1, reg2):
    vm[0x01] = vm[reg1] - vm[reg2]

def print_mem(mem):
    print("-- memory dump --")
    for b in mem:
        print("0x%x" % b)
    print("-- ----------- --")

def main():
    vm = [
        0, # programme counter - refers to address of current instruction
        0, # general purpose register at 0x01
        0  # general register at 0x02
    ]

    memory = [
        0x01, 0x01, 0x10, 
        0x01, 0x02, 0x12,
        0x03, 0x01, 0x02,
        0x02, 0x01, 0x0e,
        0xff,
        0x00,
        0x00, 0x00,
        0xa1, 0x14,
        0x0c, 0x00        
    ]

    ins = memory[vm[0x00]]
    while ins != 0xFF:
        ins = memory[vm[0x00]]     # extract the current instruction from memory
        op1 = memory[vm[0x00] + 1] # extract 1st operand for current instruction
        op2 = memory[vm[0x00] + 2] # extract 2nd operand for current instruction
        
        # check current instruction
        if ins == 0x01:   # 0x01 = load_word
            load_word(vm, memory, op1, op2)
        elif ins == 0x02: # 0x02 = store_word
            store_word(vm, memory, op1, op2)
        elif ins == 0x03: # 0x03 = add
            add(vm, op1, op2)
        elif ins == 0x04: # 0x04 = subtract
            sub(vm, op1, op2)
            
        # update PC to next instruction
        vm[0x00] += 3

    print_mem(memory)

if __name__ == "__main__":
    main()
