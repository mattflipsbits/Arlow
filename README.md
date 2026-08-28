# Arlow
A simple HAL for the Arduino UNO R3 that allows for lower-level access to peripherals.

Arlow is currently in active development and incomplete. All filenames containing
`_TODO` are placeholders and should not be considered currently usable.

## About
Arlow is a series of peripheral drivers for Arduino AVR development boards. It is being
developed initially for the UNO R3 due to its popularity, but support for other
popular AVR boards such as the Mega 2560 or Nano is planned.

Its purpose is to provide a relatively low-friction step outside of the Arduino ecosystem
towards "pure" microcontroller programming. The header files are of the all-in-one variety,
containing both function prototypes as well as their definitions. This allows the user to
easily compile and flash their board without having to worry about a complicated build
or linking system. They are written in such a manner that they may be easily split into
`.h` and `.c` files should the user wish, although the requisite linking system will need
to be provided.

## Dependencies
Arlow is intended to be used as a stand-alone tool and currently uses no external dependencies.
While it does not use the AVR development toolchain, it is being developed with it in mind,
and the user should be able to add the standard AVR headers without conflict.

The `gcc-avr` cross-compiler and `avrdude` uploader are required should the user wish to use
the provided makefile.

## Build
A simple makefile has been provided for users in a Linux/WSL environment. It assumes uploading
to the standard `/dev/ttyACM0` port, but this may be easily changed with the PORT variable if
necessary.

`APP`: The name of the `.c` file without the extension.

`PORT`: Serial port of the connected Arduino.

The makefile options are:
- `all`: The default option. Compiles the program, flashes it to the board, and removes the
         produced `.hex` and `.elf` files.
- `build`: Compiles the program and leaves the `.hex` and `.elf` files in the current directory
           to be flashed to the board later.
- `flash`: Flashes the `.hex` and `.elf` files to the board.
- `clean`: Removes all `.hex` and `.elf` files from the current directory.

## Documentation
Documentation for each header is provided in the repository `/docs` folder in markdown format.
