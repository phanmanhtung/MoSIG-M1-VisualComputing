#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

// My code
// https://stackoverflow.com/questions/1787996/c-library-function-to-perform-sort
int comp (const void * elem1, const void * elem2)
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

gray* median_filter(gray* new_image, gray* current_image, int cols, int rows){
  int i,j,median;

  for(i=1; i < rows-1; i++){
    for(j=1; j < cols-1 ; j++){

      int x[9] = { current_image[(i+1) * cols + (j+1)], current_image[(i) * cols + (j+1)], current_image[(i-1) * cols + (j+1)],
      current_image[(i+1) * cols + (j)], current_image[(i) * cols + (j)], current_image[(i-1) * cols + (j)], current_image[(i+1) * cols + (j-1)],
      current_image[(i) * cols + (j+1)], current_image[(i-1) * cols + (j-1)] };

      int len_x = sizeof(x) / sizeof(*x);

      qsort(x, sizeof(x)/sizeof(*x), sizeof(*x), comp);
      // Find median
      if ( len_x % 2 == 0){
        median = ( x[len_x/2-1] + x[len_x/2] ) / 2.0;
      }
      else{
        median = x[len_x/2];
      }

      new_image[i * cols + j] = median;
    }
  }
  return new_image;
}

gray* binomial_filter_3x3(gray* new_image, gray* current_image, int cols, int rows){
  int i,j;
  int denom = 16;

  // Binomial filter 3x3
  int myMatrix[3][3] = {
      { 1, 2, 1 },
      { 2, 4, 2 },
      { 1, 2, 1 }
  };

  for(i=1; i < rows-1; i++)
    for(j=1; j < cols-1 ; j++)
      new_image[i * cols + j] = (current_image[(i+1) * cols + (j+1)]*myMatrix[0][0] + current_image[(i) * cols + (j+1)]*myMatrix[0][1]
      + current_image[(i-1) * cols + (j+1)]*myMatrix[0][2] + current_image[(i+1) * cols + (j)]*myMatrix[1][0]
      + current_image[(i) * cols + (j)]*myMatrix[1][1] + current_image[(i-1) * cols + (j)]*myMatrix[1][2]
      + current_image[(i+1) * cols + (j-1)]*myMatrix[2][0] + current_image[(i) * cols + (j+1)]*myMatrix[2][1]
      + current_image[(i-1) * cols + (j-1)]*myMatrix[2][2])/denom;
  return new_image;
}

gray* binomial_filter_5x5(gray* new_image, gray* current_image, int cols, int rows){
  int i,j;
  int denom = 256;

  // Binomial filter 5x5
  int myMatrix[5][5] = {
      { 1, 4, 6, 4, 1 },
      { 4, 16, 24, 16, 4 },
      { 6, 24, 36, 24, 6 },
      { 4, 16, 24, 16, 4 },
      { 1, 4, 6, 4, 1 }
  };

  for(i=1; i < rows-1; i++)
    for(j=1; j < cols-1 ; j++)
      new_image[i * cols + j] = (current_image[(i+1) * cols + (j+1)]*myMatrix[0][0] + current_image[(i) * cols + (j+1)]*myMatrix[0][1]
      + current_image[(i-1) * cols + (j+1)]*myMatrix[0][2] + current_image[(i-2) * cols + (j+1)]*myMatrix[0][3] + current_image[(i-3) * cols + (j+1)]*myMatrix[0][4]

      + current_image[(i+1) * cols + (j)]*myMatrix[1][0] + current_image[(i) * cols + (j)]*myMatrix[1][1]
      + current_image[(i-1) * cols + (j)]*myMatrix[1][2] + current_image[(i-2) * cols + (j)]*myMatrix[1][3] + current_image[(i-3) * cols + (j)]*myMatrix[1][4]

      + current_image[(i+1) * cols + (j-1)]*myMatrix[2][0] + current_image[(i) * cols + (j-1)]*myMatrix[2][1] + current_image[(i-1) * cols + (j-1)]*myMatrix[2][2]
      + current_image[(i-2) * cols + (j-1)]*myMatrix[2][3] + current_image[(i-3) * cols + (j-1)]*myMatrix[2][4]

      + current_image[(i+1) * cols + (j-2)]*myMatrix[3][0] + current_image[(i) * cols + (j-2)]*myMatrix[3][1] + current_image[(i-1) * cols + (j-2)]*myMatrix[3][2]
      + current_image[(i-2) * cols + (j-2)]*myMatrix[3][3] + current_image[(i-3) * cols + (j-2)]*myMatrix[3][4]

      + current_image[(i+1) * cols + (j-3)]*myMatrix[4][0] + current_image[(i) * cols + (j-3)]*myMatrix[4][1] + current_image[(i-1) * cols + (j-3)]*myMatrix[4][2]
      + current_image[(i-2) * cols + (j-3)]*myMatrix[4][3] + current_image[(i-3) * cols + (j-3)]*myMatrix[4][4]
    )/denom;
  return new_image;
}



int main(int argc, char* argv[])
    {
    FILE* ifp;

    gray* graymap;
    gray* my_img;
    gray* cur_img;

    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j, n;

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
    my_img = (gray *) malloc(cols * rows * sizeof(gray));
    cur_img = (gray *) malloc(cols * rows * sizeof(gray));

    /* Reading */
    for(i=0; i < rows; i++) {
      for(j=0; j < cols ; j++)
        if(pgmraw)
          graymap[i * cols + j] = pm_getrawbyte(ifp) ;
        else
          graymap[i * cols + j] = pm_getint(ifp);
    }

    /* My code */

    // Ex1: binomial filter 3x3, 5x3, median filter with n times
    // Number of times execute the filter
    n = 1;

    // Assign current image to the inital image
    cur_img = graymap;

    for(i=0;i<n;i++) {
      // Change my image by filtering the current image
      my_img = median_filter(my_img, cur_img, cols, rows);

      // Assign back the current image for the next iteration
      cur_img = my_img;
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
          printf("%d ", my_img[i * cols + j]);
        else
          printf("%c", my_img[i * cols + j]);
        /*putc(graymap[i * cols + j],stdout);*/

    /* Closing */
    fclose(ifp);
    return 0;
}
