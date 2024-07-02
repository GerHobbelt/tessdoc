#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_get_component_images_main
#endif

int main()
{
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->InitSimple(NULL, "eng")) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      return 1;
  }
  Pix *image = pixRead("phototest.tif");
  api->SetImage(image);
  Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
  printf("Found %d textline image components.\n", boxes->n);
  for (int i = 0; i < boxes->n; i++) {
    BOX* box = boxaGetBox(boxes, i, L_CLONE);
    api->SetRectangle(box->x, box->y, box->w, box->h);
    char* ocrResult = api->GetUTF8Text();
    int conf = api->MeanTextConf();
    fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
                    i, box->x, box->y, box->w, box->h, conf, ocrResult);
    boxDestroy(&box);
	delete[] ocrResult;
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  pixDestroy(&image);

  return 0;
}
