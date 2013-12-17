Memory Thing
============
Use this to read or write physical memory (via /dev/mem) 32-bits at a time.

Usage
-----
    memthing r 0xf000baaa
    memthing w 0xba11f0be 0xfaceb00b

Building
--------
    make memthing
