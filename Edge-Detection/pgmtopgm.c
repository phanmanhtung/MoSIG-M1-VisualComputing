#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include <math.h>

// My code
gray* gaussian_kernel(gray* new_image, gray* current_image, int cols, int rows){
  int i,j;
  int denom = 273;

  int myMatrix[5][5] = {
      { 1, 4, 7, 4, 1 },
      { 4, 16, 26, 16, 4 },
      { 7, 26, 41, 26, 7 },
      { 4, 16, 26, 16, 4 },
      { 1, 4, 7, 4, 1 }
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

gray* scharr(gray* new_image, gray* current_image, int cols, int rows){
  int i,j;
  int denom = 1;
  int size = cols*rows;
  int Ix[size], Iy[size];

  // Scharr x
  int Ix_matrix[3][3] = {
      { 3, 0, -3 },
      { 10, 0, -10 },
      { 3, 0, -3 }
  };

  for(i=1; i < rows-1; i++){
    for(j=1; j < cols-1 ; j++)
      Ix[i * cols + j] = (current_image[(i+1) * cols + (j+1)]*Ix_matrix[0][0] + current_image[(i) * cols + (j+1)]*Ix_matrix[0][1]
      + current_image[(i-1) * cols + (j+1)]*Ix_matrix[0][2] + current_image[(i+1) * cols + (j)]*Ix_matrix[1][0]
      + current_image[(i) * cols + (j)]*Ix_matrix[1][1] + current_image[(i-1) * cols + (j)]*Ix_matrix[1][2]
      + current_image[(i+1) * cols + (j-1)]*Ix_matrix[2][0] + current_image[(i) * cols + (j+1)]*Ix_matrix[2][1]
      + current_image[(i-1) * cols + (j-1)]*Ix_matrix[2][2])/denom;
  };

  // Scharr y
  int Iy_matrix[3][3] = {  { 3, 10, 3 },
      { 0, 0, 0 },
      { -3, -10, -3 } };

  for(i=1; i < rows-1; i++){
    for(j=1; j < cols-1 ; j++)
      Iy[i * cols + j] = (current_image[(i+1) * cols + (j+1)]*Iy_matrix[0][0] + current_image[(i) * cols + (j+1)]*Iy_matrix[0][1]
      + current_image[(i-1) * cols + (j+1)]*Iy_matrix[0][2] + current_image[(i+1) * cols + (j)]*Iy_matrix[1][0]
      + current_image[(i) * cols + (j)]*Iy_matrix[1][1] + current_image[(i-1) * cols + (j)]*Iy_matrix[1][2]
      + current_image[(i+1) * cols + (j-1)]*Iy_matrix[2][0] + current_image[(i) * cols + (j+1)]*Iy_matrix[2][1]
      + current_image[(i-1) * cols + (j-1)]*Iy_matrix[2][2])/denom;
  }

  int max = __INT_MAX__ *-1;
  int min = __INT_MAX__;
  int gradient;

  for(i=0; i<size;i++){
    gradient = sqrt(Ix[i]*Ix[i] + Iy[i]*Iy[i]);
    if(gradient > max)
      max = gradient;
    if(gradient < min)
      min = gradient;
    new_image[i]=gradient;}

  // for(i=0; i < size; i++){
  //     new_image[i] = (new_image[i]-min) * 255 / max;
  //}

  return new_image;
};

// Double Thresholding
// Classify into 3 categories: weak, strong, and nahh
gray* double_thresholding(gray* current_image, int cols, int rows){
  int i;
  int size = cols*rows;

  // Find max value of the image
  int max = -1 * __INT_MAX__;
  for(i=0; i<size;i++){
    if(max < current_image[i])
      max = current_image[i];
  }
  int high_threshold = max * 0.9; // top 5 %
  int low_threshold = max * 0.7; // top 25 %

  int edges = 255; //white
  int undecided = 128; //invisible gray -> black
  int not_edges = 0;// black

  for(i=0; i<size;i++){
    if(current_image[i] >= high_threshold){
      current_image[i] = edges;}

    if(current_image[i] < high_threshold && current_image[i] >= low_threshold){
      current_image[i] = undecided;}

    if(current_image[i] < low_threshold){
      current_image[i] = not_edges;}
  };
  return current_image;
};

gray* hysteresis(gray* current_image, int cols, int rows){

  int edges = 255; //white
  int undecided = 128;
  int not_edges = 0;//black: not relate to the edge

  int i,j;

  for(i=1; i < rows-1; i++){
    for(j=1; j < cols-1 ; j++){
      if (current_image[i * cols + j] == undecided){
        if (current_image[(i+1) * cols + (j+1)] == edges || current_image[(i) * cols + (j+1)] == edges || current_image[(i-1) * cols + (j+1)] == edges
          || current_image[(i+1) * cols + (j)] == edges || current_image[(i-1) * cols + (j)] == edges
          || current_image[(i+1) * cols + (j-1)] == edges || current_image[(i) * cols + (j-1)] == edges || current_image[(i-1) * cols + (j-1)] == edges)
            current_image[i * cols + j] = edges;
        else
          current_image[i * cols + j] = not_edges;
      }
    }
  }
  return current_image;
}

int main(int argc, char* argv[])
    {
    FILE* ifp;

    gray* graymap;
    gray* my_img;
    gray* cur_img;

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

    cur_img = graymap;

    my_img = gaussian_kernel(my_img, cur_img, cols, rows);
    cur_img = my_img;

    my_img = scharr(my_img, cur_img, cols, rows);
    //cur_img = my_img;

    //my_img = double_thresholding(my_img, cols, rows);
    //my_img = hysteresis(my_img, cols, rows);

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
