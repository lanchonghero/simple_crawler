#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string>
#include <vector>

#define COMMENT_CHAR "#"

class Configuration
{
public:
  static std::string GetString(
    const std::string& section,
    const std::string& item,
    const std::string& defaultValue,
    const std::string& cfgFile);

  static int GetInt(
    const std::string& section,
    const std::string& item,
    const int& defaultValue,
    const std::string& cfgFile);
  
  static std::vector<std::string> GetSections(const std::string& cfgFile);

private:
  static std::string ReadString(
    const std::string& section,
    const std::string& item,
    const std::string& cfgFile);

  static bool AnalyseSection(const std::string& line, std::string& section);
  static bool AnalyseItem(const std::string& line, std::string& item, std::string& value);
  static bool IsSpace(const char& c);
  static void Trim(std::string& s);
  static std::string ToLower(const std::string& s);
};

#endif //_CONFIGURATION_H

