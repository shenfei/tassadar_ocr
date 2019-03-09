#include "tassadar.hpp"

#include <cstdlib>
#include <map>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

std::map<int8_t, tesseract::PageIteratorLevel> CUT_LEVEL_MAP = {
  {0, tesseract::RIL_BLOCK},
  {1, tesseract::RIL_PARA},
  {2, tesseract::RIL_TEXTLINE},
  {3, tesseract::RIL_WORD},
  {4, tesseract::RIL_SYMBOL}
};


inline Pix *StringToPix(const std::string &image_str) {
  return pixReadMem(reinterpret_cast<const l_uint8*>(image_str.c_str()), image_str.size());
}

inline std::string PixToString(Pix *pix) {
  l_uint8 *data = NULL;
  size_t size = 0;
  if (pixWriteMem(&data, &size, pix, IFF_PNG) != 0) {
    syslog(LOG_ERR, "error on PixToString");
    return "";
  }
  std::string res;
  res.resize(size);
  for (size_t i = 0; i < size; ++i) {
    res[i] = data[i];
  }
  free(data);
  return res;
}

tesseract::TessBaseAPI* TassadarServerHandler::get_tess_api(
    const std::string &lang) {
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  if (api->Init(tessdata_, lang.c_str())) {
    syslog(LOG_ERR, "Could not initialize tesseract");
    exit(1);
  }
  return api;
}

void TassadarServerHandler::get_ocr(std::string &_return, const std::string &image) {
  Pix *image_pix = StringToPix(image);
  if (image_pix == NULL) {
    syslog(LOG_ERR, "PixReadMem Error.");
    return;
  }

  api_->SetImage(image_pix);

  char *out_text;
  out_text = api_->GetUTF8Text();
  _return.assign(out_text);

  delete [] out_text;
  pixDestroy(&image_pix);
}

void TassadarServerHandler::line_ocr(std::string& _return, const std::string& image) {
  Pix *image_pix = StringToPix(image);
  if (image_pix == NULL) {
    syslog(LOG_ERR, "PixReadMem Error.");
    return;
  }

  std::vector<std::string> result_lines;
  api_->SetImage(image_pix);
  api_->Recognize(0);
  tesseract::ResultIterator* ri = api_->GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_TEXTLINE;
  if (ri != 0) {
    do {
      const char* line_text = ri->GetUTF8Text(level);
      result_lines.push_back(line_text);
      delete[] line_text;
    } while (ri->Next(level));
  }

  std::string result;
  for (std::string& line: result_lines) {
    result += line;
  }
  _return = result;

  pixDestroy(&image_pix);
}

void TassadarServerHandler::cut_image(std::vector<std::string> & _return,
                                      const std::string& image, const int8_t cut_type) {
  Pix *image_pix = StringToPix(image);
  if (image_pix == NULL) {
    syslog(LOG_ERR, "PixReadMem Error.");
    return;
  }
  api_->SetImage(image_pix);
  Pixa* pixa = pixaCreate(1);
  tesseract::PageIteratorLevel level = CUT_LEVEL_MAP[cut_type];
  Boxa* boxes = api_->GetComponentImages(level, true, true, 1, &pixa, NULL, NULL);
  for (auto i = 0; i < pixa->n; ++i) {
    Pix* component_pix = pixaGetPix(pixa, i, L_CLONE);
    _return.push_back(PixToString(component_pix));
    pixDestroy(&component_pix);
  }
  boxaDestroy(&boxes);
  pixaDestroy(&pixa);
  pixDestroy(&image_pix);
}
