/*
 * swapbytes - reorder bytes
 * usage: swapbytes swapspec
 * swapspec copies bytes from stdin to stdout, but also re-arranges them.
 * swapspec is like:
 *      rgba2rgb        (for every 4 bytes read, output the first 3)
 *      ab2ba           (swap pairs of bytes)
 *      abcd2dcba       (swap 4 bytes at a time)
 *      RGBAxxxx2RGBA   (read 8 bytes, write the first 4)
 *      rrggbb2rgb      (drop every other byte)
 *      aabb2abab       (swap middle 2 bytes)
 * 
 * this code sucks.
 * public domain
 * 2016 jrra
 */

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <string.h>     // strchr
#include <stdint.h>     // uint8_t

struct order {
    size_t inLength;
    size_t outLength;
    int *map;
};

static const char *convertErrorMessages[] = {
    "",
    "missing a 2",
    "too many 2s",
    "",
    "a character used in the outspec does not appear in the inspec",
    "your cpu is overheating",
};

int convert_swapspec( struct order *myOrder, char *swapspec );
void order_destroy( struct order *myOrder );

int main( int argc, char *argv[] ) {
    
    // ensure that swapspec was given as an argument
    if( argc < 2 ) {
        fprintf( stderr, "%s: missing swapspec\n", argv[0] );
        exit(1);
    }
    
    // convert swapspec into an array that maps output bytes to input bytes
    struct order myOrder;
    int err = convert_swapspec( &myOrder, argv[1] );
    if( err ) {
        fprintf( stderr, "%s: invalid swapspec '%s': %s\n", argv[0], argv[1],
            convertErrorMessages[err] );
        exit(2);
    };
    
    // ensure that inLength > 0
    if( myOrder.inLength <= 0 ) {
        fprintf( stderr, "%s: inLength <= 0\n", argv[0] );
        exit(3);
    }
    
    // ensure that outLength > 0
    if( myOrder.outLength <= 0 ) {
        fprintf( stderr, "%s: outLength <= 0\n", argv[0] );
        exit(3);
    }
    
    // naively copy from stdin to stdout.
    // fix this to make it fast pls
    uint8_t inbuf[myOrder.inLength];
    uint8_t outbuf[myOrder.outLength];
    while(1) {
        size_t bytesRead = fread( inbuf, myOrder.inLength, 1, stdin );
        if( bytesRead != 1 )
            break;
        for( int i=0; i<myOrder.outLength; i++ )
            outbuf[i] = inbuf[myOrder.map[i]];
        fwrite( outbuf, myOrder.outLength, 1, stdout );
    }
    
    order_destroy( &myOrder );
    return 0;
}

int convert_swapspec( struct order *myOrder, char *swapspec ) {
    
    // abcd2dcba
    
    // find the character '2' in the swapspec
    char *two = strchr( swapspec, '2' );
    if( two == NULL )
        return 1;
    
    // ensure that there is only one '2' in the swapspec
    char *nextTwo = strchr( two+1, '2' );
    if( nextTwo != NULL )
        return 2;
    
    char *end = strchr( swapspec, '\0' );
    
    // calculate inLength
    myOrder->inLength = (size_t)(two-swapspec);
    
    // calculate outLength
    myOrder->outLength = (size_t)(end-two-1);
    
    char *in = swapspec;
    char *out = two+1;
    int usedIn[256] = {0};
    int usedOut[256] = {0};
    
    // calloc for map
    myOrder->map = calloc( myOrder->outLength, sizeof(int) );
    
    // count the number of times each character is used in the in part
    for( int idx=0; idx < myOrder->inLength; idx++ )
        usedIn[(int) in[idx]] ++;
    
    // map each byte in the out part to a byte in the in part
    for( int idx=0; idx < myOrder->outLength; idx++ ) {
        // first, check that this character in the outspec is
        // also used in the inspec
        if( usedIn[(int) out[idx]] == 0 )
            return 4;
        int rep = usedOut[(int) out[idx]]++;
        int nth = (rep) % usedIn[(int) out[idx]];
        
        // find the nth usage of this out character in the in
        char *new = in-1;
        for( int i=0; i<nth+1; i++ ) {
            new = strchr( new+1, out[idx] );
            if( new == NULL )
                return 5;       // should never happen
        }
        
        myOrder->map[idx] = new-in;
    }
    
    return 0;
}

void order_destroy( struct order *myOrder ) {
    free( myOrder->map );
}
