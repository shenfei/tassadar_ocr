#ifndef TASSADAR_H_
#define TASSADAR_H_

#include <string>
#include <vector>
#include <syslog.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <tesseract/baseapi.h>

#include "gen-cpp/TassadarServer.h"

struct Pix;
namespace tesseract {
class TessBaseAPI;
}

class TassadarServerHandler : virtual public TassadarServerIf {
 public:
  TassadarServerHandler() {
    openlog(NULL, LOG_PERROR, LOG_USER);
    tessdata_ = getenv("TESSDATA_PREFIX");
    if (tessdata_ == NULL) {
      syslog(LOG_ERR, "Could not find TESSDATA_PREFIX.");
      exit(1);
    }
    syslog(LOG_INFO, "Set TESSDATA to: %s\n", tessdata_);

    std::string lang("chi_sim+eng");
    api_ = get_tess_api(lang);
    syslog(LOG_INFO, "Tassadar init ok\n");
  }

  ~TassadarServerHandler() {
    api_->End();
    delete api_;
    closelog();
  }

  void version(std::string& _return) {
    _return = version_;
  }

  void get_ocr(std::string& _return, const std::string& image);
  void line_ocr(std::string& _return, const std::string& image);
  void cut_image(std::vector<std::string> & _return, const std::string& image, const int8_t cut_type);

 protected:
  tesseract::TessBaseAPI *get_tess_api(const std::string &lang);

 private:
  char *tessdata_;
  tesseract::TessBaseAPI *api_;
  const std::string version_ = "0.3.0";
};

#endif
