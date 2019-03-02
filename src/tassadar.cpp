#include "tassadar.hpp"

#include <cstdlib>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


inline Pix *StringToPix(const std::string &image_str) {
  return pixReadMem(reinterpret_cast<const l_uint8*>(image_str.c_str()), image_str.size());
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
