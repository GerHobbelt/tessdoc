#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_set_rectangle_main
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
    // Restrict recognition to a sub-rectangle of the image
    // SetRectangle(left, top, width, height)
    api->SetRectangle(30, 86, 590, 100);
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
