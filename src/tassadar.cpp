#include "tassadar.hpp"

#include <cstdlib>
#include <map>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "gen-cpp/tassadar_types.h"

inline Pix *StringToPix(const std::string &image_str) {
  return pixReadMem(reinterpret_cast<const l_uint8*>(image_str.c_str()), image_str.size());
}

std::map<PageSegMode::type, tesseract::PageSegMode> psm_map = {
  {PageSegMode::PSM_SINGLE_LINE, tesseract::PSM_SINGLE_LINE},
  {PageSegMode::PSM_SINGLE_BLOCK, tesseract::PSM_SINGLE_BLOCK}
};

tesseract::TessBaseAPI* TassadarServerHandler::get_tess_api(
    const std::string &lang) {
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  if (api->Init(tessdata_, lang.c_str())) {
    syslog(LOG_ERR, "Could not initialize tesseract");
    exit(1);
  }
  return api;
}

void TassadarServerHandler::ocr_process(std::string &_return,
                                        const std::string &image,
                                        const PageSegMode::type &psm) {
  Pix *image_pix = StringToPix(image);
  if (image_pix == NULL) {
    syslog(LOG_ERR, "PixReadMem Error.");
    return;
  }

  api_->SetPageSegMode(psm_map[psm]);
  api_->SetImage(image_pix);

  char *out_text;
  out_text = api_->GetUTF8Text();
  _return.assign(out_text);

  delete [] out_text;
  pixDestroy(&image_pix);
}

void TassadarServerHandler::get_ocr(std::string& _return, const std::string& image) {
  ocr_process(_return, image, PageSegMode::PSM_SINGLE_BLOCK);
}

void TassadarServerHandler::line_ocr(std::string& _return, const std::string& image) {
  ocr_process(_return, image, PageSegMode::PSM_SINGLE_LINE);
}
