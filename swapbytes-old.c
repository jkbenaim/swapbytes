/*
 * swapbytes
 * 
 * Usage:
 *      swapbytes [OPTIONS] INSPEC OUTSPEC [INFILE] [OUTFILE]
 * 
 * swapbytes is useful for changing the order of bytes in the input file.
 * The conversion schema looks like one of these:
 *      abcd dcba
 *      ab ba
 *      abcde abcd
 *      rgba a
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>     // for getopt
#include <getopt.h>     // also for getopt?
#include <stdlib.h>

enum {
    SWAPBYTES_MODE_UNDEF=0,
    SWAPBYTES_MODE_BYTES,
    SWAPBYTES_MODE_BITS,
};

int check_valid_spec( char *inspec, char *outspec );

int main( int argc, char *argv[] )
{
  int mode = SWAPBYTES_MODE_UNDEF;
  int opt;
  while( (opt=getopt(argc,argv, "bBh")) != -1 ) {
      switch( opt ) {
          case 'b':
              mode = SWAPBYTES_MODE_BITS;
              fprintf( stderr, "%s: bits mode not implemented\n", argv[0] );
              exit(EXIT_FAILURE);
              break;
          case 'B':
              mode = SWAPBYTES_MODE_BYTES;
              break;
          case 'h':
          default:
              fprintf( stderr, "Usage: %s [OPTIONS] INSPEC OUTSPEC [INFILE] [OUTFILE]\n", argv[0] );
              exit(EXIT_FAILURE);
      }
  }
  
//   fprintf( stderr, "mode=%d; optind=%d\n", mode, optind );
  if( mode == SWAPBYTES_MODE_UNDEF )
      mode = SWAPBYTES_MODE_BYTES;
  
  
  if( optind >= argc ) {
    fprintf( stderr, "%s: expected INSPEC\n", argv[0] );
    fprintf( stderr, "Try '%s -h' for more information.\n", argv[0] );
    return 1;
  }
  char *inspec = argv[optind++];
  if( optind >= argc ) {
    fprintf( stderr, "%s: expected OUTSPEC\n", argv[0] );
    fprintf( stderr, "Try '%s -h' for more information.\n", argv[0] );
    return 1;
  }
  char *outspec = argv[optind++];
//   fprintf( stderr, "inspec: %s; outspec: %s\n", inspec, outspec );
  int spec_err = check_valid_spec( inspec, outspec );
  if( spec_err ) {
      fprintf( stderr, "%s: invalid spec pair: %s %s (%d)\n", argv[0], inspec, outspec, spec_err );
      fprintf( stderr, "Try '%s -h' for more information.\n", argv[0] );
      exit(EXIT_FAILURE);
  }
  
  char *infile="-", *outfile="-";
  FILE *fileIn, *fileOut;
  
  if( optind < argc )
      infile = argv[optind++];
  if( strncmp(infile, "-", 2) == 0 )
      fileIn = stdin;
  else
      fileIn = fopen( infile, "r" );
  if( fileIn == NULL ) {
      fprintf( stderr, "%s: unable to open file %s\n", argv[0], infile );
      exit(EXIT_FAILURE);
  }
  if( optind < argc )
      outfile = argv[optind++];
  if( strncmp(outfile, "-", 2) == 0 )
      fileOut = stdout;
  else
      fileOut = fopen( outfile, "w" );
  if( fileOut == NULL ) {
      fprintf( stderr, "%s: unable to open file %s\n", argv[0], outfile );
      exit(EXIT_FAILURE);
  }
  
//   fprintf( stderr, "infile: %s; outfile: %s\n", infile, outfile);
  
  if( fileIn != stdin )
    fclose(fileIn);
  if( fileOut != stdout )
    fclose(fileOut);
  
  // actually do the conversion
  size_t in_buf_size = strlen(inspec);
  size_t out_buf_size = strlen(outspec);
  
  char *in_buf = calloc( 1, in_buf_size );
  char *out_buf = calloc( 1, out_buf_size );
  
  // count how many times each letter appears in the inspec
  int inspec_letter_count[256] = {0};
//   int inspec_letter_progress[256] = {0};
  char c;
  int i=0;
  while(1) {
      c = inspec[i++];
      if( c == '\0' ) break;
      else inspec_letter_count[(unsigned int)c]++;
  }
  
  // fill in letter bufs
  char *in_letter_bufs[256] = { NULL };
  for(i=0; i<256; i++)
      if( inspec_letter_count[i] != 0 )
          in_letter_bufs[i] = calloc( 1, inspec_letter_count[i]+1 );
  
  for(i=0; i<256; i++)
      if( inspec_letter_count[i] != 0 )
      fprintf(stderr, "inspec_letter_count['%c'] = %d\n", (char)i, inspec_letter_count[i]);
  
  int map[out_buf_size] = {0};
  
  i=0;
  while(1) {
      size_t bytesRead = fread( in_buf, in_buf_size, 1, inFile );
      if( bytesRead != in_buf_size ) break;
      while(1) {
        c = in_buf[i++];
      }
  }
  
  for(i=0; i<256; i++)
      if( in_letter_bufs[i] != NULL )
          free( in_letter_bufs[i] );
  
  free( out_buf );
  free( in_buf );
  exit(EXIT_SUCCESS);
  
  // old stuff below
//   FILE *fileIn, *fileOut;
//   char *ptrTo2 = strchr( argv[1], '2' );
//   int numBytesToRead = ptrTo2-argv[1];
//   int numBytesToWrite = strlen(argv[1])-numBytesToRead-1;
//   fprintf(stderr, "Reading %d byte%s at a time.\n", numBytesToRead, numBytesToRead==1?"":"s");
//   fprintf(stderr, "Writing %d byte%s at a time.\n", numBytesToWrite, numBytesToWrite==1?"":"s");
//   
//   char in[256];
//   char out[256];
//   int conv[256];
//   memset(in, 0, 256);
//   memset(out, 0, 256);
//   memset(conv, 0, 256);
//   
//   strncpy( in, argv[1], numBytesToRead );
//   strcpy( out, ptrTo2+1 );
//   fprintf(stderr, "%s\n", in);
//   fprintf(stderr, "%s\n", out);
//   int i;
//   for(i=0; i<numBytesToWrite; ++i)
//   {
//     char c = out[i];
//     int a = strchr(in, c)-in;
//     conv[i] = a;
//     fprintf(stderr, "%d ", a);
//   }
//   fprintf(stderr, "\n");
//   
//   int c;
//   int numBytesRead = 0;
//   while( (c = fgetc(fileIn)) != EOF)
//   {
//     in[numBytesRead++] = (char)c;
//     if(numBytesRead == numBytesToRead)
//     {
//       numBytesRead = 0;
//       int i;
//       for( i=0; i<numBytesToWrite; ++i )
//       {
//         fputc(in[conv[i]], fileOut);
//       }
//     }
//   }
  
  return 0;
}


int check_valid_spec( char *inspec, char *outspec )
{
    int i, j;
    char c, d;
 
    // check that every character in the inspec is a lower-case letter.
    // assumes ascii   
    i=0;
    while(1) {
        c = inspec[i++];
        if( c == '\0' )
            break;
        if( c < 'a' || c > 'z' )
            return 1;
    }
    
    // check that every character in the outspec is a lower-case letter.
    // assumes ascii
    i=0;
    while(1) {
        c = outspec[i++];
        if( c == '\0' )
            break;
        if( c < 'a' || c > 'z' )
            return 2;
    }
    
    // check that every character in the outspec is also in the inspec.
    i=0;
    while(1) {
        c = outspec[i++];
        if( c == '\0' )
            break;
        j=0;
        while(1) {
            d = inspec[j++];
            if( d == c )
                break;
            if( d == '\0' )
            {
                fprintf( stderr, "%c %c\n", d, c );
                return 3;
            }
        }
    }
    
    return 0;
}
