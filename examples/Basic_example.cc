#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_basic_main
#endif

int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->InitSimple(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }

    // Open input image with leptonica library
    Pix *image = pixRead("phototest.tif");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
