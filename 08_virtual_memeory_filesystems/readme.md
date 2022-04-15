# Virtual Memory and File Systems

## Pre-class questions

- What is the page size for your operating system's virtual memory system?
- Approximate the size (in bytes) of the page table for your operating system right now, based on the size of programs that are running
- Write a program which consumes more and more physical memory at a predictable rate. Use `top` or a similar program to observe its execution. What happens as your memory utilization approaches total available memory? What happens when it reaches it?
- Suppose the designers of your operating system propose quadrupling the page size. What would be the trade-offs?

## Implementation exercise

### Write a small `ls` clone

Requirements: 

Phase1

- describe the contents of current working directory (where tool is run from)
- list the names of files/dirs contained and whether each is a dir or file

Phase2

- accept a CLI argument to specify directory to be listed
- list filesize of elements
- list directories first
- list modification/creation date
- list owner/group
- list permissions
