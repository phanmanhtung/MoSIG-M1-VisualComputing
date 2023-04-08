#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include <string.h>

int main(int argc, char* argv[])
    {
    FILE* ifp;
    gray* graymap;
    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j;

    /* Arguments */
    if ( argc != 2 ){
      printf("\nUsage: %s file \n\n", argv[0]);
      exit(0);
    }

    /* Opening */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }

    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error / magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF /read error / magic number" );
    if(ich2 != '2' && ich2 != '5')
      pm_erreur(" wrong file type ");
    else
      if(ich2 == '2')
	     pgmraw = 0;
      else pgmraw = 1;

    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    /* Memory allocation  */
    graymap = (gray *) malloc(cols * rows * sizeof(gray));

    /* Reading */
    for(i=0; i < rows; i++) {
      for(j=0; j < cols ; j++)
        if(pgmraw)
          graymap[i * cols + j] = pm_getrawbyte(ifp) ;
        else
          graymap[i * cols + j] = pm_getint(ifp);
    }

    /* My code */
    // Ex2 : Histogram Equalization

    // read the image into an 1-D array
    int length = cols * rows;
    int x[length];
    for(i=0; i < rows; i++) {
      for(j=0; j < cols ; j++)
        x[i * cols + j] = graymap[i * cols + j];
    }

    // Step 1: build the histogram
    int hist_len = 256; // possible intensity values from 0 to 255 -> 256
    int hist[hist_len];

    memset(hist, 0, sizeof(hist));
    //
    // loop through all value in x and count the frenquency then add to hist
    for (int i = 0 ; i < length ; i++)
      hist[x[i]] += 1;

    // Step 2: Build the cumsum histogram
    int cumsum[hist_len];
    int sum = 0;

    for (int i = 0; i < length ; i++){
      sum +=hist[i];
      cumsum[i]=sum;
    }

    int min = cumsum[0];
    int max = cumsum[sizeof(cumsum)/sizeof(cumsum[0]) - 1];

    // Step 3: Perform Histogram Equalization
    for (int i = 0 ; i < hist_len ; i++){
      cumsum[i] = ((cumsum[i] - min) * 255) / (max-min); // max intensity = 255
    }

    // Step 4: Create the new 1-D result array as the new image
    int new_image_arr[length];
    for (int i = 0 ; i < length ; i++){
      new_image_arr[i] = cumsum[graymap[i]];
    }

    // Cast the array back to the gray image
    for(i=0; i < rows; i++) {
      for(j=0; j < cols ; j++)
        graymap[i * cols + j] = new_image_arr[i * cols + j];
    }

    /* Writing */
    if(pgmraw)
      printf("P2\n");
    else
      printf("P5\n");

    printf("%d %d \n", cols, rows);
    printf("%d\n",maxval);

    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
        if(pgmraw)
          printf("%d ", graymap[i * cols + j]);
        else
          printf("%c", graymap[i * cols + j]);
        /*putc(graymap[i * cols + j],stdout);*/

    /* Closing */
    fclose(ifp);
    return 0;
}
