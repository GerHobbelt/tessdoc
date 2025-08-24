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

struct PixDeallocator
{
	template<typename Type>
	inline void operator()(Type ptr) const {
		pixDestroy(&ptr);
	}
};

int main(void) {
	std::unique_ptr<tesseract::TessBaseAPI> api = std::make_unique<tesseract::TessBaseAPI>();
	std::unique_ptr<ETEXT_DESC> monitor = std::make_unique<ETEXT_DESC>();
    if (api->Init("/tesseract/tessdata_best", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }
	api->RegisterMonitor(monitor.get());
	api->SetPageSegMode(tesseract::PSM_AUTO);
	std::unique_ptr<Pix, PixDeallocator> image { pixRead("phototest.tif") };
    if (!image) {
       fprintf(stderr, "Leptonica can't process input file!\n");
       return 2;
    }
    api->SetImage(image.get());
    int page = 0;
    std::thread t1(ocrProcess, api.get(), monitor.get());
    std::thread t2(monitorProgress, monitor.get(), page);
    t1.join();
    t2.join();

	//pixDestroy(&image); -->
	image.reset();

    char *outText = api->GetUTF8Text();
    printf("\n%s\n", outText);
    delete [] outText;

	api->End();

#if 0
	// nuke `api` before `monitor`: not mandatory -- the std::unique_ptr will clean up both instances properly at the end of local scope.
	api.reset();
	monitor.reset();
#endif

    return 0;
}

