#include <stdio.h>
#include <leptonica/allheaders.h>
#include <tesseract/capi.h>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main     tessdoc_example_C_api_main
#endif

static int die(const char *errstr) {
	fputs(errstr, stderr);
	return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
	TessBaseAPI *handle;
	PIX *img;
	char *text;

	if((img = pixRead("img.png")) == NULL)
		return die("Error reading image\n");

	handle = TessBaseAPICreate();
	if(TessBaseAPIInit3(handle, NULL, "eng") != 0)
		return die("Error initializing tesseract\n");

	TessBaseAPISetImage2(handle, img);
	if(TessBaseAPIRecognize(handle, NULL) != 0)
		return die("Error in Tesseract recognition\n");

	if((text = TessBaseAPIGetUTF8Text(handle)) == NULL)
		return die("Error getting text\n");

	fputs(text, stdout);

	TessDeleteText(text);
	TessBaseAPIEnd(handle);
	TessBaseAPIDelete(handle);
	pixDestroy(&img);

	return EXIT_SUCCESS;
}

