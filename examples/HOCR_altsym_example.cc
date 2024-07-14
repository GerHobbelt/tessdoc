#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_hocr_altsym_main
#endif

int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }

	// Open input image with leptonica library
    Pix *image = pixRead("trainingital.tif");
    api->SetImage(image);
    api->SetVariable("lstm_choice_mode", "2");

	// Get HOCR result
    outText = api->GetHOCRText(0);
    printf("HOCR alternative symbol choices  per character :\n%s", outText);

	// Destroy used object and release memory
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
