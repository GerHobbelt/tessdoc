#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_alt_osd_main
#endif

int main()
{
    const char* inputfile = "devatest-rotated-270.png";

  tesseract::Orientation orientation;
  tesseract::WritingDirection direction;
  tesseract::TextlineOrder order;
  float deskew_angle;

  PIX *image = pixRead(inputfile);
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init("/usr/src/tesseract/", "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		return 1;
	}
  api->SetPageSegMode(tesseract::PSM_AUTO_OSD);
  api->SetImage(image);
  api->Recognize(0);

  tesseract::PageIterator* it =  api->AnalyseLayout();
  it->Orientation(&orientation, &direction, &order, &deskew_angle);
  printf("Orientation: %d;\nWritingDirection: %d\nTextlineOrder: %d\n" \
         "Deskew angle: %.4f\n",
         orientation, direction, order, deskew_angle);
    
    // Destroy used object and release memory
    api->End();
    delete api;
    pixDestroy(&image);
    
    return 0;
}
