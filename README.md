#swapbytes - reorder bytes

Swapbytes copies bytes from stdin to stdout, but also re-arranges them.

Usage: swapbytes swapspec

Some swapspec examples:

     rgba2rgb        (for every 4 bytes read, output the first 3)
     ab2ba           (swap pairs of bytes)
     abcd2dcba       (swap 4 bytes at a time)
     RGBAxxxx2RGBA   (read 8 bytes, write the first 4)
     rrggbb2rgb      (drop every other byte)
     aabb2abab       (swap middle 2 bytes)


TO DO:

- performance
- filenames as arguments
- bit mode
