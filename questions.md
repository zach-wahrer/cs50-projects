# Questions

## What's `stdint.h`?

A header that declares sets of int types having specified widths, as well as corresponding sets of macros.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To limit your int, make sure it is the correct ype, and make sure it is the correct length.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 4, 4, 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM

## What's the difference between `bfSize` and `biSize`?

`bfSize` is the size of the bitmap file, in bytes (used by the BITMAPFILEHEADER). `biSize` is the number of bytes required by the structure (in the BITMAPINFOHEADER).

## What does it mean if `biHeight` is negative?

It indicates the file is a top down Device Independent Bitmap and its origin is in the upper left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in `copy.c`?

If it cannot open or create a file.

## Why is the third argument to `fread` always `1` in our code?

Because we have set up the size to read the length for either BITMAPFILEHEADER or BITMAPINFOHEADER, and we want to read 1 of those lengths.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

Sets the file posistion indicator for the stream.

## What is `SEEK_CUR`?

It is the argument specifing the `fseek` is to go to go `offset` amount of bytes relitive to the current position indicator.
