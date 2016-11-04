#ifndef _DOWNLOADER_H
#define _DOWNLOADER_H
#include <string>

class Downloader
{
public:
  Downloader() {}
  virtual ~Downloader() {}

  static int DownloadPage(
    const std::string& url,
    const std::string& useragent,
    std::string& ip,
    std::string& header,
    std::string& data,
    std::string& error);
};

#endif //_DOWNLOADER_H
