# Building a VM

## The Computer 

- 20 bytes of memory
- 3 registers: 2 general purpose and one program counter
- 5 instructions: `load_word`, `store_word`, `add`, `subtract` and `halt`

### Memory

- first 14 bytes are reserved for instructions and arguments
- bytes 14, 16 and 18 are start of input/output values (two bytes per value, stored in little endian format)
- OUT - stored in bytes 14 and 15
- IN1 - stored in bytes 16 and 17
- IN2 - stored in bytes 16 and 17

## Instructions

- `load_word`, encoded as 0x01
- `store_word`, encoded as 0x02
- `add`, encoded as 0x03
- `subtract`, encoded as 0x04
- `halt`, encoded as 0x05

## Problem

Write a function to process the input until it reaches a `halt` operation. The result would be a main memory with modified state.

```
program = [
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
```

