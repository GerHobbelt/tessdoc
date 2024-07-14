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
    const char* retry_config = nullptr;
    bool textonly = false;
    int jpg_quality = 92;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(datapath, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return EXIT_FAILURE;
    }

    tesseract::TessPDFRenderer *renderer = new tesseract::TessPDFRenderer(
              output_base, api->GetDatapath(), textonly, jpg_quality);

    bool succeed = api->ProcessPages(input_image, retry_config, timeout_ms, renderer);
    if (!succeed) {
      fprintf(stderr, "Error during processing.\n");
      return EXIT_FAILURE;
    }
    api->End();
    delete api;
    return EXIT_SUCCESS;
}

