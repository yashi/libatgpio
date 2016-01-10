# Library for Linux GPIO Sysfs Interface with Glib

This library is to demonstrate how to manupilate Linux GPIO Sysfs
interface using glib main loop and gsource.

## Build
`./autogen.sh && make`

## Note

To test with Armadillo-840's SW1, SW2, SW3, and SW4 on the LCD Ext
board, you need to apply the patch in the `patches/` to your kernel
*and* reconfigure your kernel to disable GPIO Keys the switches are
assigned to.

## License
MIT
