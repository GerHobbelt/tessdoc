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
		float pct = monitor[page].progress;
        printf( "\r%3.1f%%", pct);
        fflush(stdout);
        if (pct == 100.0f || monitor[page].abort_the_action)
            break;
    }
}

static void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor) {
	api->Recognize();
}

int main(void) {
	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	ETEXT_DESC* monitor = new ETEXT_DESC();
    if (api->Init("/tesseract/tessdata_best", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }
	api->RegisterMonitor(monitor);
	api->SetPageSegMode(tesseract::PSM_AUTO);
	Pix* image = pixRead("phototest.tif");
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
	delete monitor;

    return 0;
}

