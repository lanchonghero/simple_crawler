#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <algorithm>
#include <string>
#include <vector>

std::string& ltrimc(std::string &s, const char c);
std::string& rtrimc(std::string &s, const char c);
std::string& trimc(std::string &s, const char c);

std::string& ltrimc(std::string &s, const std::string &cs);
std::string& rtrimc(std::string &s, const std::string &cs);
std::string& trimc(std::string &s, const std::string &cs);

std::string& ltrim(std::string &s);
std::string& rtrim(std::string &s);
std::string& trim(std::string &s);

std::string& replace(std::string &s, const std::string &find_s, const std::string &replace_s);
std::string& quotstr(std::string &s, const char c);

bool strfind(const std::string &s, const std::string &f);
void splitString(const std::string &s, char ch, std::vector<std::string> &vs);
int xsplit(const std::string& in ,std::string& out1, std::string& out2, const char sep);

std::string& toLower(std::string& s);
std::string& toUpper(std::string& s);

int iconv_convert(
  const std::string& from_charset,
  const std::string& to_charset,
  const std::string& in,
  std::string& out);
int utf8_to_gbk(const std::string& in, std::string& out);
int gbk_to_utf8(const std::string& in, std::string& out);

const std::string StringPrintf(const char* format, ...);

#endif //_STRING_UTILS_H
