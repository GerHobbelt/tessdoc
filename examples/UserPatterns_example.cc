#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_user_patterns_main
#endif

int main()
{
    Pix *image;
    char *outText;
	std::vector<std::string> configs{"path/to/my.patterns.config"};
	std::vector<std::string> vars;   // empty
	std::vector<std::string> values; // empty
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->InitFull(NULL, "eng", tesseract::OEM_LSTM_ONLY, configs, vars, values)) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      return 1;
    }
    image = pixRead("Arial.png");
    api->SetImage(image);
    outText = api->GetUTF8Text();
    printf(outText);
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);
    return 0;
}
