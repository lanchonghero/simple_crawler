#include "string_utils.h"
#include "webpage.h"

std::string WebPage::GetEncoding()
{
  std::string en = encoding;
  if (en.empty())
    en = GetEncodingFromHttpHeader();
  if (en.empty())
    en = GetHtmlEncoding();
  if (en.empty())
    en = GetXmlEncoding();
  if (!en.empty())
    en = UpdateEncoding(en);
  if (en.empty())
    en = "utf8";
  return en;
}

std::string WebPage::GetEncodingFromHttpHeader()
{
  if (httpheader.empty())
    return "";
  int p = httpheader.find("\nContent-Type: ");
  if (p != -1) {
    p += 15;
    int q = httpheader.find("\n", p);
    p = httpheader.find("charset=", p);
    if (p != -1 && q != -1) {
      std::string charset = httpheader.substr(p, q-p);
      charset = replace(charset, "charset=", "");
      int pos = charset.find(";");
      if (pos != -1) {
        charset = charset.substr(0, pos);
      }
      charset = trim(charset);
      charset = toLower(charset);
      return charset;
    }
  }
  return "";
}

std::string WebPage::GetHtmlEncoding()
{
  if (html.empty())
    return "";

  size_t p = 0;
  size_t q = 0;
  size_t r = 0;
  while (true) {
    p = html.find("<meta", p);
    if (p != std::string::npos) {
      p += 5;
      r = p;
      q = html.find(">", r);
      if (q != std::string::npos) {
        r = html.find("content=", r);
        if (r != std::string::npos && r < q) {
          r = html.find("charset=", r);
          if (r != std::string::npos && r < q) {
            r += 8;
            size_t e = html.find_first_of(" ;\"/>", r);
            if (e != std::string::npos && e < q) {
              return html.substr(r, e - r);
            }
          }
        }
      }
    } else {
      break;
    }
  }
  return "";
}

std::string WebPage::GetXmlEncoding()
{
  return "";
}

std::string WebPage::UpdateEncoding(const std::string& s)
{
  if (s == "gb2312" || s == "gbk") {
    return "gb18030";
  } else {
    return s;
  }
}

std::string WebPage::GetMimetypeFromHttpHeader()
{
  if (httpheader.empty())
    return "";
  int p = httpheader.find("\nContent-Type: ");
  if (p != -1) {
    p += 15;
    int q = httpheader.find("\n", p);
    if (q != -1) {
      std::string mimetype = httpheader.substr(p, q-p);
      int pos = mimetype.find(";");
      if (pos != -1) {
        mimetype = mimetype.substr(0, pos);
      }
      mimetype = toLower(trim(mimetype));
      return mimetype;
    }
  }
  return "";
}

std::string WebPage::GetLocationFromHttpHeader()
{
  if (httpheader.empty())
    return "";
  int p = httpheader.find("\nLocation: ");
  if (p != -1) {
    p += 11;
    int q = httpheader.find("\r\n", p);
    if (q != -1) {
      std::string location = httpheader.substr(p, q-p);
      location = trim(location);
      return location;
    }
  }
  return "";
}

int WebPage::GetCodeFromHttpHeader()
{
  int code = -1;
  if (!httpheader.empty()) {
    int len = static_cast<int>(httpheader.length());
    int p = httpheader.find("HTTP/");
    if (p != -1) {
      p += 5;
      while (p < len && !std::isspace(httpheader[p]))
        p++;
      while (p < len && std::isspace(httpheader[p]))
        p++;
      int q = p;
      while (q < len && std::isdigit(httpheader[q]))
        q++;

      std::string code_str = httpheader.substr(p, q-p);
      code = atoi(code_str.c_str());
    }
  }
  return code;
}
