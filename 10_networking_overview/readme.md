# Network Packet Parse

Data format is pcap.

compilation instruction to use guint16

```bash
cc `pkg-config --cflags glib-2.0` parse.c -o parse
```

Ref: https://docs.gtk.org/glib/compiling.html

## References

- https://wiki.wireshark.org/Development/LibpcapFileFormat

## Notes

- it might be useful to check the position in the file after reads and terminate execution then
- the value of snaplen might refer to the number of bytes in capture and not packets


## Questions

- What's the significance of the 1514 value in global header?
- Why does fgets() cause problems while getc() seems to work well?
- What's a good way to handle the EOF while reading the file?
- what are the high-order bits (octet) in the IP protocol header used for? (the lower order ones are 0x06)
