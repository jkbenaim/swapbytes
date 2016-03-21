swapbytes - reorder bytes
usage: swapbytes swapspec
swapspec copies bytes from stdin to stdout, but also re-arranges them.
swapspec is like:
     rgba2rgb        (for every 4 bytes read, output the first 3)
     ab2ba           (swap pairs of bytes)
     abcd2dcba       (swap 4 bytes at a time)
     RGBAxxxx2RGBA   (read 8 bytes, write the first 4)
     rrggbb2rgb      (drop every other byte)
     aabb2abab       (swap middle 2 bytes)

this code sucks.
public domain
2016 jrra


TO DO:
- filenames as arguments
- performance
- bit mode