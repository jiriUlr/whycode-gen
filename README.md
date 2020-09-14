# whycode-gen

whycode-gen is a tool to generate the [WhyCode/WhyCon markers](https://github.com/jiriUlr/whycon-ros).
To compile it, just type `make`. Then, it can generate the markers inside the current working directory. Simply run the binary followed by a number of ID bits or any other options as
```
@$ ./whycode_gen -h

Usage: whycon-id-gen [options] bits
    -h,    Display this help
    -v,    Verbose while generating canvas (default: false)
    -l,    Generating the original WhyCon marker
    -d,    Set minimal Hamming distance (default: 1)

```

## Dependencies
- `graphicsmagick-libmagick-dev-compat` - used to draw and generate .png files

