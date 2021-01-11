#include <stdio.h>
#include <omp.h>
#include <gd.h>
int main(int argc, char *argv[])
{
  int nt = 4;
  int tid, tmp, red, green, blue, color, x, h, y, w;
  tmp = red = green = blue = color = x = h = y = w = 0;
  char *iname = "in.png";
  char *oname = "out.png";
  gdImagePtr img;
  FILE *fp = {0};
  /*
    if(argc!=3)
        error(1,0,"format : object_file input.png output.png");
    else
    {*/
  //iname = "in.png";
  //oname = "out.png";
  //}
  if ((fp = fopen(iname, "r")) == NULL)
    printf("file error");
  else
  {
    img = gdImageCreateFromPng(fp);
    w = gdImageSX(img);
    h = gdImageSY(img);
  }
  double t = omp_get_wtime();
  omp_set_num_threads(nt);
#pragma omp parallel for private(y, color, red, blue, green) schedule(dynamic, 50) /*schedule(dynamic,50)  schedule(guided)*/
  for (x = 0; x < w; x++)
    for (y = 0; y < h; y++)
    {
      tid = omp_get_thread_num();

      color = gdImageGetPixel(img, x, y);
      red = gdImageRed(img, color);
      green = gdImageGreen(img, color);
      blue = gdImageBlue(img, color);
      tmp = (red + green + blue) / 3;
      red = green = blue = tmp;
      if (tid == 0)
      {
        color = gdImageColorAllocate(img, red, 0, 0);
        gdImageSetPixel(img, x, y, color);
      }
      if (tid == 1)
      {
        color = gdImageColorAllocate(img, 0, green, 0);
        gdImageSetPixel(img, x, y, color);
      }
      if (tid == 2)
      {
        color = gdImageColorAllocate(img, 0, 0, blue);
        gdImageSetPixel(img, x, y, color);
      }
      if (tid == 3)
      {
        color = gdImageColorAllocate(img, 0, 0, 0);
        gdImageSetPixel(img, x, y, color);
      }
    }
  t = omp_get_wtime() - t;
  printf("\ntime taken : %lf threads : %d", t, nt);
  if ((fp = fopen(oname, "w")) == NULL)
    printf("file error out");
  else
  {
    gdImagePng(img, fp);
    fclose(fp);
  }
  gdImageDestroy(img);
  return 0;
}
