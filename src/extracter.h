#ifndef _EXTRACTER_H
#define _EXTRACTER_H
#include <libxslt/xslt.h>
#include <libxslt/xsltutils.h>
#include <libxml/HTMLparser.h>
#include <libxslt/transform.h>
#include <pcre.h>
#include <string>
#include "webpage.h"

class Extracter
{
public:
  Extracter();
  virtual ~Extracter();

  void SetName(const std::string& s);
  void SetDomain(const std::string& s);
  void SetEncoding(const std::string& s);
  void SetUrlRegex(const std::string& s);
  void SetXtr(const std::string& s);
  
  bool Match(const std::string& url) const;
  bool MatchDomain(const std::string& url) const;
  bool MatchUrlRegex(const std::string& url) const;

  bool DoExtract(WebPage& wp, std::string& xml);

private:
  std::string GetDomainFromUrl(const std::string& url) const;
  static void XMLCDECL XmlError(void *ctx, const char *msg, ...);

private:
  std::string name;
  std::string domain;
  std::string encoding;
  xsltStylesheetPtr xslt_xtr;
  pcre *urlre;
};

#endif //_EXTRACTER_H
