#!/usr/bin/env python3

def main():
    vm = {
        "pc": 0,      # programme counter - refers to address of current instruction
        "reg1": 0x00, # register 1 - holds value for manipulation by VM
        "reg2": 0x00  # register 2 - holds value for manipulation by VM
    };

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

    while memory[vm['pc']] != 0xFF:
        # check current instruction

        # update PC, depending on current instruction
        pass

if __name__ == "__main__":
    main()
