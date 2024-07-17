# API examples

This documentation provides simple examples on how to use the tesseract-ocr API (v3.02.02-5.4.1) in C++.
It is expected that tesseract-ocr is correctly installed including all dependencies.
It is expected the user is familiar with C++, compiling and linking program on their platform, [though basic compilation examples are included for beginners with Linux](#compiling-c-api-programs-on-linux).

More details about tesseract-ocr API can be found at [baseapi.h](https://github.com/tesseract-ocr/tesseract/blob/main/include/tesseract/baseapi.h).



## Basic example

Code (`examples/Basic_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }

    // Open input image with leptonica library
    Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
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
```

The program must be linked to the tesseract-ocr and leptonica libraries.

If you want to restrict recognition to a sub-rectangle of the image - call _SetRectangle(left, top, width, height)_ after SetImage. Each SetRectangle clears the recognition results so multiple rectangles can be recognized with the same image. E.g.

```c++
  api->SetRectangle(30, 86, 590, 100);
```

See also the C++ example source file: `examples/SetRectangle_example.cc`


## GetComponentImages example

Code (`examples/GetComponentImages_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/pix_internal.h>

int main()
{
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->InitSimple(NULL, "eng")) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      return 1;
  }
  Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
  api->SetImage(image);
  Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
  printf("Found %d textline image components.\n", boxes->n);
  for (int i = 0; i < boxes->n; i++) {
    BOX* box = boxaGetBox(boxes, i, L_CLONE);
    api->SetRectangle(box->x, box->y, box->w, box->h);
    char* ocrResult = api->GetUTF8Text();
    int conf = api->MeanTextConf();
    fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
                    i, box->x, box->y, box->w, box->h, conf, ocrResult);
    boxDestroy(&box);
	delete[] ocrResult;
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  pixDestroy(&image);

  return 0;
}
```

## Result iterator example

It is possible to get the confidence value and BoundingBox per word from a ResultIterator.

Code (`examples/ResultIterator_example.cc`):

```c++
int main()
{
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->InitSimple(NULL, "eng")) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      return 1;
  }
  Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
  api->SetImage(image);
  api->Recognize(0);
  tesseract::ResultIterator* ri = api->GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
  if (ri != 0) {
    do {
      const char* word = ri->GetUTF8Text(level);
      float conf = ri->Confidence(level);
      int x1, y1, x2, y2;
      ri->BoundingBox(level, &x1, &y1, &x2, &y2);
      printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
               word, conf, x1, y1, x2, y2);
      delete[] word;
    } while (ri->Next(level));
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  pixDestroy(&image);
  return 0;
}
```

It is also possible to use other iterator levels (block, line, word, etc.), see [PageiteratorLevel](https://github.com/tesseract-ocr/tesseract/blob/a7a729f6c315e751764b72ea945da961638effc5/include/tesseract/publictypes.h#L216-L222).



## Orientation and script detection (OSD) example

Code (`examples/OSD_alt_example.cc`):

```c++
  const char* inputfile = "/usr/src/tesseract/testing/devatest-rotated-270.png";
  tesseract::Orientation orientation;
  tesseract::WritingDirection direction;
  tesseract::TextlineOrder order;
  float deskew_angle;

  PIX *image = pixRead(inputfile);
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  api->Init("/usr/src/tesseract/", "eng");
  api->SetPageSegMode(tesseract::PSM_AUTO_OSD);
  api->SetImage(image);
  api->Recognize(0);

  tesseract::PageIterator* it =  api->AnalyseLayout();
  it->Orientation(&orientation, &direction, &order, &deskew_angle);
  printf("Orientation: %d;\nWritingDirection: %d\nTextlineOrder: %d\n" \
         "Deskew angle: %.4f\n",
         orientation, direction, order, deskew_angle);
  delete api;
```

Explanation for result codes are in [publictypes.h](https://github.com/tesseract-ocr/tesseract/blob/a7a729f6c315e751764b72ea945da961638effc5/include/tesseract/publictypes.h#L116-L121)

An easier and potentially faster approach using the tesseract v5 API is shown below.

Code (`examples/OSD_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{
    const char* inputfile = "/usr/src/tesseract/testing/devatest-rotated-270.png";
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
```

## Example of iterator over the classifier choices for a single symbol

Code (`examples/ChoicesIterator_example.cc`):

```c++
int main()
{
  Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->InitSimple(NULL, "eng")) {
      fprintf(stderr, "Could not initialize tesseract.\n");
      return 1;
  }
  api->SetImage(image);
  api->SetVariable("save_blob_choices", "T");
  api->SetRectangle(37, 228, 548, 31);
  api->Recognize(NULL);

  tesseract::ResultIterator* ri = api->GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
  if(ri != 0) {
      do {
          const char* symbol = ri->GetUTF8Text(level);
          float conf = ri->Confidence(level);
          if(symbol != 0) {
              printf("symbol %s, conf: %f", symbol, conf);
              bool indent = false;
              tesseract::ChoiceIterator ci(*ri);
              do {
                  if (indent) printf("\t\t ");
                  printf("\t- ");
                  const char* choice = ci.GetUTF8Text();
                  printf("%s conf: %f\n", choice, ci.Confidence());
                  indent = true;
              } while(ci.Next());
          }
          printf("---------------------------------------------\n");
          delete[] symbol;
      } while((ri->Next(level)));
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  pixDestroy(&image);
  return 0;
}
```

## Example to get confidence for alternative symbol choices per character for LSTM

Code (`examples/LSTM_Choices_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
int main()
{
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
// Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }
// Open input image with leptonica library
  Pix *image = pixRead("/usr/src/tesseract/testing/trainingital.tif");
  api->SetImage(image);
// Set lstm_choice_mode to alternative symbol choices per character, bbox is at word level.
  api->SetVariable("lstm_choice_mode", "2");
  api->Recognize(0);
  tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
  tesseract::ResultIterator* res_it = api->GetIterator();
  // Get confidence level for alternative symbol choices. Code is based on
  // https://github.com/tesseract-ocr/tesseract/blob/a7a729f6c315e751764b72ea945da961638effc5/src/api/hocrrenderer.cpp#L325-L344
  std::vector<std::vector<std::pair<const char*, float>>>* choiceMap = nullptr;
  if (res_it != 0) {
    do {
      const char* word;
      float conf;
      int x1, y1, x2, y2, tcnt = 1, gcnt = 1, wcnt = 0;
     res_it->BoundingBox(level, &x1, &y1, &x2, &y2);
     choiceMap = res_it->GetBestLSTMSymbolChoices();
      for (auto timestep : *choiceMap) {
        if (timestep.size() > 0) {
          for (auto & j : timestep) {
            conf = int(j.second * 100);
            word =  j.first;
            printf("%d  symbol: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
                        wcnt, word, conf, x1, y1, x2, y2);
           gcnt++;
          }
          tcnt++;
        }
      wcnt++;
      printf("\n");
      }
    } while (res_it->Next(level));
  }
  // Destroy used object and release memory
  api->End();
  delete api;
  pixDestroy(&image);
  return 0;
}
```

## Example to get HOCR output with alternative symbol choices per character (LSTM)

This is similar to running tesseract from commandline with `-c lstm_choice_mode=2 hocr`.

Code (`examples/HOCR_altsym_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

	// Open input image with leptonica library
    Pix *image = pixRead("/usr/src/tesseract/testing/trainingital.tif");
    api->SetImage(image);
    api->SetVariable("lstm_choice_mode", "2");

	// Get HOCR result
    outText = api->GetHOCRText(0);
    printf("HOCR alternative symbol choices  per character :\n%s", outText);

	// Destroy used object and release memory
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
```

Notice the different confidence values for:

```
<span class='ocrx_cinfo' id='lstm_choices_1_4_4'>
<span class='ocr_glyph' id='choice_1_4_7' title='x_confs 83'>m</span>
<span class='ocr_glyph' id='choice_1_4_8' title='x_confs 16'>n</span>
</span>
<span class='ocrx_cinfo' id='lstm_choices_1_4_5'>
<span class='ocr_glyph' id='choice_1_4_9' title='x_confs 54'>n</span>
<span class='ocr_glyph' id='choice_1_4_10' title='x_confs 17'>i</span>
<span class='ocr_glyph' id='choice_1_4_11' title='x_confs 15'>m</span>
<span class='ocr_glyph' id='choice_1_4_12' title='x_confs 6'>a</span>
<span class='ocr_glyph' id='choice_1_4_13' title='x_confs 4'>u</span>
<span class='ocr_glyph' id='choice_1_4_14' title='x_confs 1'>r</span>
<span class='ocr_glyph' id='choice_1_4_15' title='x_confs 1'>e</span>
</span>
```


# Compiling C++ API programs on Linux

Including and linking to Tesseract's API is done in a standard Linux way. To compile a basic program against the API, you can use a command like this:

```
g++ -o myprogram myprogram.cpp -llept -ltesseract
```

If Tesseract is installed in an unusual place, you can specify the include and lib directories explicitly with g++'s -I and -L flags, like this:

```
g++ -o myprogram myprogram.cpp -I/home/nick/local/include/tesseract -L/home/nick/local/lib -llept -ltesseract
```

# C-API in python

Tesseract-ocr from version 3.02.02 provide C-API. This enable to [use tesseract-ocr shared library in python](https://github.com/tesseract-ocr/tesseract/blob/3.02.02/contrib/tesseract-c_api-demo.py) (and other languages that can use C libraries):

```python
import os
import ctypes

lang = "eng"
filename = "/usr/src/tesseract/testing/phototest.tif"
libname = "/usr/local/lib64/libtesseract.so.3"
TESSDATA_PREFIX = os.environ.get('TESSDATA_PREFIX')
if not TESSDATA_PREFIX:
    TESSDATA_PREFIX = "../"

tesseract = ctypes.cdll.LoadLibrary(libname)
tesseract.TessVersion.restype = ctypes.c_char_p
tesseract_version = tesseract.TessVersion()
api = tesseract.TessBaseAPICreate()
rc = tesseract.TessBaseAPIInit3(api, TESSDATA_PREFIX, lang)
if (rc):
    tesseract.TessBaseAPIDelete(api)
    print("Could not initialize tesseract.\n")
    exit(3)

text_out = tesseract.TessBaseAPIProcessPages(api, filename, None, 0)
result_text = ctypes.string_at(text_out)

print 'Tesseract-ocr version', tesseract_version
print result_text
```

Example of passing python file object to C-API can be found at [pastebin](http://pastebin.com/yDTkNfNm).

Example of extracting orientation from Tesseract 4.0:

```python
# /usr/bin/env python3
# coding: utf-8

PATH_TO_LIBTESS = '/path/to/development/libtesseract.so'

import cffi  # requires "pip install cffi"

ffi = cffi.FFI()
ffi.cdef("""
struct Pix;
typedef struct Pix PIX;
PIX * pixRead ( const char *filename );
const char * getLeptonicaVersion (  );

typedef struct TessBaseAPI TessBaseAPI;
typedef int BOOL;

const char* TessVersion();

TessBaseAPI* TessBaseAPICreate();
int TessBaseAPIInit3(TessBaseAPI* handle, const char* datapath, const char* language);

void TessBaseAPISetImage2(TessBaseAPI* handle, struct Pix* pix);

BOOL   TessBaseAPIDetectOrientationScript(TessBaseAPI* handle, char** best_script_name,
                                                            int* best_orientation_deg, float* script_confidence,
                                                            float* orientation_confidence);
""")

libtess = ffi.dlopen(PATH_TO_LIBTESS)
from ctypes.util import find_library
liblept = ffi.dlopen(find_library('lept'))

ffi.string(libtess.TessVersion())

ffi.string(liblept.getLeptonicaVersion())

api = libtess.TessBaseAPICreate()

libtess.TessBaseAPIInit3(api, ffi.NULL, ffi.NULL)

pix = liblept.pixRead('mono.png'.encode())

libtess.TessBaseAPISetImage2(api, pix)

script_name = ffi.new('char **')
orient_deg = ffi.new('int *')
script_conf = ffi.new('float *')
orient_conf = ffi.new('float *')
libtess.TessBaseAPIDetectOrientationScript(api, script_name, orient_deg, script_conf, orient_conf)

print(ffi.string(script_name).decode('utf-8'))
print(orient_deg[0])
print(script_conf[0])
print(orient_conf[0])

```

# Example using the C-API in a C program

The C-API can of course also be used by regular C programs, as in this very basic example.

Code (`examples/C_API_example.c`):

```c
#include <stdio.h>
#include <allheaders.h>
#include <capi.h>

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
```

On Linux you can [compile it as you would build a program using the C++ API](#compiling-c-api-programs-on-linux).

# Example creating searchable pdf from image in C++

Code (`examples/searchable_PDF_example.cc`):

```c++
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>

int main()
{
    const char* input_image = "/usr/src/tesseract/testing/phototest.tif";
    const char* output_base = "my_first_tesseract_pdf";
    const char* datapath = "/Projects/OCR/tesseract/tessdata";
    int timeout_ms = 5000;
    bool textonly = false;
    int jpg_quality = 92;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(datapath, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return EXIT_FAILURE;
    }

    tesseract::TessPDFRenderer *renderer = new tesseract::TessPDFRenderer(
              output_base, api->GetDatapath(), textonly, jpg_quality);

    bool succeed = api->ProcessPages(input_image, timeout_ms, renderer);
    if (!succeed) {
      fprintf(stderr, "Error during processing.\n");
      return EXIT_FAILURE;
    }
    api->End();
    delete api;
    return EXIT_SUCCESS;
}
```

# Example of monitoring OCR progress in C++

Code (`examples/monitoring_example.cc`):

```c++
#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>
#include <leptonica/allheaders.h>
#include <thread>

static void monitorProgress(ETEXT_DESC *monitor, int page);
static void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor);

static void monitorProgress(ETEXT_DESC *monitor, int page) {
    while (1) {
        printf( "\r%3d%%", monitor[page].progress);
        fflush (stdout);
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
    if (api->Init("/tesseract/tessdata_best", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 1;
    }
    api->SetPageSegMode(tesseract::PSM_AUTO);
    Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
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
```

More complex examples (e.g. cancelling OCR process) can be found in source code of [TesseractGui](https://github.com/sashoalm/TesseractGui/blob/master/thread.cpp), [gimagereader](https://fossies.org/linux/gimagereader/qt/src/Recognizer.cc) or android [textfairy](https://github.com/renard314/textfairy/search?p=2&q=monitor&type=&utf8=%E2%9C%93) [app](https://www.youtube.com/watch?v=vUmZnwyLH6I).
