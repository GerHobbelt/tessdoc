#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_searchable_pdf_main
#endif


int main()
{
    const char* input_image = "phototest.tif";
    const char* output_base = "my_first_tesseract_pdf";
    const char* datapath = "/Projects/OCR/tesseract/tessdata";
    int timeout_ms = 5000;
    bool textonly = false;
    int jpg_quality = 92;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->InitSimple(datapath, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return EXIT_FAILURE;
    }

		api->SetVariable("jpg_quality", jpg_quality);
		api->SetVariable("activity_timeout_millisec", timeout_ms);
		tesseract::TessPDFRenderer *renderer = new tesseract::TessPDFRenderer(
              output_base, api->GetDatapath(), textonly);

    bool succeed = api->ProcessPages(input_image, renderer);
    if (!succeed) {
      fprintf(stderr, "Error during processing.\n");
      return EXIT_FAILURE;
    }
    api->End();
    delete api;
    return EXIT_SUCCESS;
}

