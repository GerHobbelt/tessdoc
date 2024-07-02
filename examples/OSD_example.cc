#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_osd_main
#endif

int main()
{
    const char* inputfile = "devatest-rotated-270.png";
    PIX *image = pixRead(inputfile);
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->InitSimple(NULL, "osd")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		return 1;
	}
	api->SetPageSegMode(tesseract::PSM_OSD_ONLY);
    api->SetImage(image);
        
    int orient_deg;
    float orient_conf;
    const char* script_name;
    float script_conf;
    api->DetectOrientationScript(&orient_deg, &orient_conf, &script_name, &script_conf);
    printf("************\n Orientation in degrees: %d\n Orientation confidence: %.2f\n"
    " Script: %s\n Script confidence: %.2f\n",
    orient_deg, orient_conf,
    script_name, script_conf);
    
    // Destroy used object and release memory
    api->End();
    delete api;
    pixDestroy(&image);
    
    return 0;
}
