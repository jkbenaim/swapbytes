/*
 * swapbytes - reorder bytes
 * usage: swapbytes SWAPSPEC
 *      where SWAPSPEC is like:
 *      rgba2rgb
 *      ab2ba
 *      abcd2dcba
 *      RGBAxxxx2RGBA
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

int convert_swapspec( struct order *myOrder, char *swapSpec );
void order_destroy( struct order *myOrder );

int main( int argc, char *argv[] ) {
    
    // ensure that SWAPSPEC was given as an argument
    if( argc < 2 ) {
        fprintf( stderr, "%s: missing swapspec\n", argv[0] );
        exit(1);
    }
    
    // convert SWAPSPEC into an array that maps output bytes to input bytes
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
    
    // HACK WARNING TODO debug prints
    printf( "inLength : %zu\n", myOrder.inLength );
    printf( "outLength: %zu\n", myOrder.outLength );
    printf( "map: " );
    for( int idx=0; idx < myOrder.outLength; idx++ )
        printf( "%d ", myOrder.map[idx] );
    printf( "\n" );
    
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

int convert_swapspec( struct order *myOrder, char *swapSpec ) {
    
    // abcd2dcba
    
    // find the character '2' in the swapSpec
    char *two = strchr( swapSpec, '2' );
    if( two == NULL )
        return 1;
    
    // ensure that there is only one '2' in the swapSpec
    char *nextTwo = strchr( two+1, '2' );
    if( nextTwo != NULL )
        return 2;
    
    char *end = strchr( swapSpec, '\0' );
    
    // calculate inLength
    myOrder->inLength = (size_t)(two-swapSpec);
    
    // calculate outLength
    myOrder->outLength = (size_t)(end-two-1);
    
    // separate the string into two
//     two[0] = '\0';
    
    char *in = swapSpec;
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
        // first, check that this character in the outspec is also used in the inspec
        if( usedIn[(int) out[idx]] == 0 )
            return 4;
        int rep = usedOut[(int) out[idx]]++;
        int nth = (rep) % usedIn[(int) out[idx]];
//         printf( "rep: %d, u: %d, nth: %d\n", rep, usedIn[(int) out[idx]], nth );
        
        // find the nth usage of this out character in the in
        char *new = in-1;
        for( int i=0; i<nth+1; i++ ) {
            new = strchr( new+1, out[idx] );
            if( new == NULL )
                return 5;       // should never happen
        }
        
        myOrder->map[idx] = new-in;
        
//         for( int idx=0; idx < myOrder->outLength; idx++ )
//             printf( "%d ", myOrder->map[idx] );
//         printf( "\n" );
    }
    
    
    // HACK TODO WARNING debug prints
//     for( int idx=0; idx < myOrder->outLength; idx++ )
//         printf( "%d ", myOrder->map[idx] );
//     printf( "\n" );
//     for( int idx=0; idx <= 128; idx++ )
//         printf( "%u ", usedIn[idx] );
//     printf("\n");
    return 0;
}

void order_destroy( struct order *myOrder ) {
    free( myOrder->map );
}
