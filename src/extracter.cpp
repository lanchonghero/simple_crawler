#include <string.h>
#include <fstream>
#include "extracter.h"
#include "string_utils.h"
#include "log.h"

Extracter::Extracter():
  urlre(NULL), xslt_xtr(NULL)
{
}

Extracter::~Extracter()
{
  if (!urlre)
    pcre_free(urlre);
  if (!xslt_xtr)
    xsltFreeStylesheet(xslt_xtr);
}

void Extracter::SetName(const std::string& s)
{
  name = s;
}

void Extracter::SetDomain(const std::string& s)
{
  domain = s;
}

void Extracter::SetEncoding(const std::string& s)
{
  encoding = s;
}

void Extracter::SetXtr(const std::string& s)
{
	std::ifstream f(s.c_str());
	if (!f)
		_ERROR_EXIT(-1, "[cannot open extracter file %s]", s.c_str());
  f.close();

  xslt_xtr = xsltParseStylesheetFile(BAD_CAST(s.c_str()));
  if (!xslt_xtr)
    _ERROR_EXIT(-1, "[load xslt file failed %s]", s.c_str());
}

void Extracter::SetUrlRegex(const std::string& s)
{
  const char *error;
  int erroroffset;
  urlre = pcre_compile(s.c_str(), 0, &error, &erroroffset, NULL);
}

bool Extracter::Match(const std::string& url) const
{
  return MatchDomain(url) & MatchUrlRegex(url);
}

bool Extracter::MatchDomain(const std::string& url) const
{
  if (domain == "*") return true;
  std::string d = GetDomainFromUrl(url);
  return (d.find(domain) != std::string::npos);
}

bool Extracter::MatchUrlRegex(const std::string& url) const
{
  if (urlre && !url.empty()) {
    int hit[48];
    int n_hit = sizeof(hit) / sizeof(hit[0]);
    if (pcre_exec(urlre, NULL, url.c_str(), url.length(), 0, 0, hit, n_hit) <= 0) {
      return false;
    }
  }
  return true;
}

bool Extracter::DoExtract(WebPage& wp, std::string& xml)
{
  if (Match(wp.url)) {
    std::string encoding = wp.GetEncoding();
    std::string mimetype = wp.GetMimetypeFromHttpHeader();
    std::string location = wp.GetLocationFromHttpHeader();
    std::string tempname = name;

    xmlSetGenericErrorFunc(NULL, &XmlError);
    htmlDocPtr doc = NULL;
    if (mimetype.find("text/xml") != std::string::npos ||
      wp.html.find("<?xml version=\"1.0\" encoding=") != std::string::npos) {
      doc = wp.html.empty() ? NULL : xmlReadDoc(BAD_CAST(wp.html.c_str()), NULL, encoding.c_str(), XML_PARSE_RECOVER);
    } else if (mimetype.find("application/json") != std::string::npos) {
    } else {
      doc = wp.html.empty() ? NULL : htmlParseDoc(BAD_CAST(wp.html.c_str()), encoding.c_str());
    }

    if (doc != NULL) {
      std::vector<std::pair<std::string, std::string> > params_vec;
      params_vec.push_back(std::make_pair("template", quotstr(tempname, '"')));
      params_vec.push_back(std::make_pair("url", quotstr(wp.url, '"')));
      params_vec.push_back(std::make_pair("refer", quotstr(wp.refer, '"')));
      params_vec.push_back(std::make_pair("encoding", quotstr(encoding, '"')));
      params_vec.push_back(std::make_pair("mimetype", quotstr(mimetype, '"')));
      params_vec.push_back(std::make_pair("location", quotstr(location, '"')));

      int n = 0;
      const char ** params_array = new const char*[params_vec.size() * 2 + 1];
      for (unsigned int i=0; i<params_vec.size(); i++) {
        params_array[n++] = params_vec[i].first.c_str();
        params_array[n++] = params_vec[i].second.c_str();
      }
      params_array[n] = NULL;
      xmlDocPtr res = xsltApplyStylesheet(xslt_xtr, doc, params_array);
      if (res != NULL) {
        char* data;
        int data_len;
        xsltSaveResultToString((xmlChar **)&data, &data_len, res, xslt_xtr);
        xml = std::string(data, data_len);
        free(data);
        xmlFreeDoc(res);
      }
      delete [] params_array;
      xmlFreeDoc(doc);
      return true;
    }
  }
  return false;
}

std::string Extracter::GetDomainFromUrl(const std::string& url) const
{
  std::string s = url;
  trim(s);
  int pos = 0;
  if ((pos = s.find("http://")) == 0)
    pos = 7;
  else if ((pos = s.find("https://")) == 0)
    pos = 8;
  s.erase(0, pos);
  if ((pos = s.find('/')) != -1)
    s.erase(pos);
  return s;
}

void XMLCDECL Extracter::XmlError(void *ctx, const char *msg, ...)
{
  va_list v;
  if (ctx) {
    va_start(v, msg);
    vfprintf((FILE *)ctx, msg, v);
    va_end(v);
  }
}
