# Basic Raytracer in C

This is an implementation of a basic raytracer with simple shading as described in the first two chapters of **_[Computer Graphics from Scratch](https://gabrielgambetta.com/computer-graphics-from-scratch/)_**. 

These files generate a scene consisting of ominous floating spheres affected by simple lighting.

## File Format
__Reference: [NETBPM file formats](https://en.wikipedia.org/wiki/Netpbm#File_formats)__

This program generates a PGM file, which, while trivial to read/write, are an egregious offence to image compression standards everywhere. PGM stands for "Portable Gray Map". It comes in two formats: ASCII and binary (denoted by magic numbers **P2** and **P5**, respectively).

The header is structured (in ASCII text, regardless of format) as follows:
- The "magic number" format descriptor, followed by a newline character;
- The width and height parameters (separated by a space), followed by a newline character; and
- The maximum possible colour value, followed by a newline character.

### Header Example
The following is a valid ASCII-formatted PGM file. This image describes a tiny black square with 4 white pixels in the middle.
```bash
# Header Section
P2     # ASCII format
10 10 # 10w x 10h
255   # MAX_VAL
# Data Section
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 255 255 0 0 0 0
0 0 0 0 255 255 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
```

The binary format is far less wasteful than its ASCII-format counterpart. The binary format is harder to parse in a text editor, however. 

Is that really a downside? Who looks at images in text editors?

## Execution
This program was compiled and executed on 5.4.0-66-generic Linux Kernel running Mint, with gcc version 9.3.0.

First, clone the repository:
```bash
git clone https://github.com/c-grimshaw/basic-raycaster
```

Next, compile and modify the executable permissions of the compiled binary.
```bash
cd basic-raycaster
make
chmod +x raycaster
```
After that, you should be good to execute the program and enjoy the unparalleled nirvana of shaded, floating spheres.
```bash
./raycaster
xdg-open spheres.pgm
```
