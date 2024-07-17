#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>
#include <leptonica/allheaders.h>
#include <thread>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_monitoring_main
#endif

using namespace tesseract;

static void monitorProgress(ETEXT_DESC *monitor, int page);
static void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor);

static void monitorProgress(ETEXT_DESC *monitor, int page) {
    while (1) {
        printf( "\r%3d%%", monitor[page].progress);
        fflush(stdout);
        if (monitor[page].progress==100)
            break;
    }
}

static void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor) {
    api->Recognize(monitor);
}

int main() {
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    ETEXT_DESC *monitor = new ETEXT_DESC();
    if (api->InitSimple("/tesseract/tessdata_best", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }
    api->SetPageSegMode(tesseract::PSM_AUTO);
    Pix *image = pixRead("phototest.tif");
    if (!image) {
       fprintf(stderr, "Leptonica can't process input file!\n");
       return 2;
     }
    api->SetImage(image);
    int page = 0;
    std::thread t1(ocrProcess, api, monitor);
    std::thread t2(monitorProgress, monitor, page);
    t1.join();
    t2.join();
    pixDestroy(&image);
    char *outText = api->GetUTF8Text();
    printf("\n%s\n", outText);
    delete [] outText;
    api->End();
    delete api;
    return 0;
}

