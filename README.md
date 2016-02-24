
# giffler

A stupid command-line GIF tool. Made primarily to create non-looping GIFs with more than 256 colors.

I made this one strictly for the fun. If you need something more sophisticated, you should probably try [gifsicle](https://www.lcdf.org/gifsicle/).

## Dependencies

*   [ImageMagick](http://www.imagemagick.org/) - a great library for image manipulation. Chances are you already have it installed.

## Installation

Just like any other program that uses Autotools:

``` bash
./bootstrap
./configure
make install
```

## TL;DR;

**How can we create a GIF with more than 256 colors?**

The GIF format specifies a custom palette of up to 256 colors (or 255 if you have a transparent layer) *per frame*.
We exploit that one step further:

*   Quantize the image to N*255, where N is the number of frames.

*   The first frame is just a normal, non-transparent GIF with 256 colors.
    We do this so the image looks at it's best before the other frames start appearing.

*   Each other frame contains up to 256 colors that are added ontop of the first one.
    Generally, the animation speed is 0.02s as this is current browser's best.

*   The resulting GIF has N+1 frames, which might not be optimal, but it's absolutely best-looking.

## Some credits

* [http://notes.tweakblogs.net/blog/8712/high-color-gif-images.html](http://notes.tweakblogs.net/blog/8712/high-color-gif-images.html) - where my idea came from.

## License

```
"THE BEER-WARE LICENSE" (Revision 42):
<Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
```
