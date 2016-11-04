#ifndef _WEBPAGE_H
#define _WEBPAGE_H
#include <string>

class WebPage
{
public:
  WebPage() {}
  virtual ~WebPage() {}

  std::string GetEncoding();
  std::string GetEncodingFromHttpHeader();
  std::string GetHtmlEncoding();
  std::string GetXmlEncoding();
  std::string UpdateEncoding(const std::string& s);
  std::string GetMimetypeFromHttpHeader();
  std::string GetLocationFromHttpHeader();
  int GetCodeFromHttpHeader();

public:
  std::string url;
  std::string refer;
  std::string httpheader;
  std::string html;
  std::string encoding;
  std::string ip;
};

#endif //_WEBPAGE_H
