#include <stdio.h>
#include <stdlib.h>

int PPMRead(char *inPPMVersion, char *inputFilename);
int PPMWrite(char *outPPMVersion, char *outputFilename);
int PPMDataWrite(int ppmVersionNum, FILE *outputFile);


// create a stuct that represents a single pixel, same as what we did in class
typedef struct PPMRGBpixel{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} PPMRGBpixel;

typedef struct PPMimage{
  int width;
  int height;
  int maxColorValue;
  PPMRGBpixel *data;
} PPMimage;

PPMimage PPMRead(char *inPPMVersion, char *inputFilename){
  PPMimage *buffer;
  FILE* fh = fopen(inputFilename, "r");
  if (fh == NULL){
    fprintf(stderr, "Error: open the file unsuccessfully. \n");
    return (1);
  }
  int c = fgetc(fh);            // get the first character of the file
  if (c != "P"){
    fprintf(stderr, "Error: incorrect input file formart, the file should be a PPM file. \n");
    return (1);
  }
  c = fgetc(fh);              // get the second character of the file, which should be a number with char type
  int ppmVersionNum = atoi(c);
  if (ppmVersionNum != 3 | ppmVersionNum != 6) {
    fprintf(stderr, "Error: invalid magic number, the ppm version should be either P3 or P6. \n");
    return (1)
  }

  c = fgetc(fh);
  c = fgetc(fh);
  //skip comments
  while (c == "#"){
    while (c != "\n"){
      c = fgetc(fh);
    }
  }

  int wh = fscanf(fh, "%d %d", &buffer->width, &buffer->height);
  if (wh != 2){
    fprintf(stderr, "Error: the size of image has to include width and height, invalid size. \n");
    return (1);
  }
  int mcv = fscanf(fh, "%d", &buffer->maxColorValue);
  if (mcv != 1){
    fprintf(stderr, "Error: the max color value has to be one single value. \n");
    return (1);
  }
  if (buffer->maxColorValue != 255){
    fprintf(stderr, "Error: the image has to be 8-bit per channel. \n");
    return (1);
  }

  buffer->data = (PPMRGBpixel*)malloc(buffer->width*buffer->height*sizeof(PPMRGBpixel));
  if (buffer == NULL){
    fprintf(stderr, "Error: allocate the memory unsuccessfully. \n");
    return (1);
  }
  if (ppmVersionNum == 3){
    for (int i=0; i<buffer->height; i++){
      for (int j=0; j<buffer->width; j++){
        PPMRGBpixel *pixel;
        fscanf(fh, "%d %d %d", pixel->r, pixel->g, pixel->b);
        buffer->data[i*buffer->width*3+j*3] = pixel->r;
        buffer->data[i*buffer->width*3+j*3+1] = pixel->g;
        buffer->data[i*buffer->width*3+j*3+2] = pixel->b;
      }
    }
  }
  else if (ppmVersionNum == 6){
    // read the pixel data, the type size_t might be used
    size_t s = fread(buffer->data, sizeof(PPMRGBpixel), buffer->width*buffer->height, fh);
    if (s < buffer->width*buffer->height){
      fprintf(stderr, "Error: ");
      return (1);
    }
  }
  else {
    fprintf(stderr, "Error: the ppm version cannot be read. \n");
    return (1);
  }
  fclose(fh);
  return buffer;
}

int PPMWrite(char *outPPMVersion, char *outputFilename){
  int width = buffer->width;
  int height = buffer->height;
  int maxColorValue = buffer->maxColorValue;
  int ppmVersionNum = atoi(outPPMVersion[1]);
  fh = fopen(outputFilename, "w");
  if (fh == NULL){
    fprintf(stedrr, "Error: open the file unscuccessfully. \n");
    return (1);
  }
  char *comment = "# output.ppm";
  fprintf(fh, "P%d\n %s\n %d %d\n %d\n", ppmVersionNum, comment, width, height, 255);
  PPMDataWrite(ppmVersionNum, fh);
  fclose(fh);
}

int PPMDataWrite(int ppmVersionNum, FILE *outputFile){
  // write image data bytes to the file if the ppm version is P6
  if (ppmVersionNum == 6){
    fwrite(buffer->data, sizeof(PPMRGBpixel), buffer->width*buffer->height, outputFile);
    printf("The file saved successfully! \n");
    return (0);
  }
  else if (ppmVersionNum == 3){
    fprintf(outputFile, buffer->width*buffer->height, sizeof(PPMRGBpixel),buffer->data);
    printf("The file saved successfully! \n");
    return (0);
  }
  else {
    fprintf(stedrr, "Error: incorrect ppm version. \n");
    return (1);
  }
}

int main(int argc, char *argv[]){
  if (argc != 4){
    fprintf(stderr, "Error: the magic number, input file name and output file name are required. \n");
  }
  char *ppmVersion = argv[1];
  char *inputFilename = argv[2];
  char *outputFilename = argv[3];

  PPMRead(ppmVersion, inputFilename);

}
