#include <fstream>
#include <algorithm>
#include "configuration.h"
#include "log.h"

std::string Configuration::GetString(
	const std::string& section,
	const std::string& item,
	const std::string& defaultValue,
	const std::string& cfgFile)
{
	std::string r = ReadString(section, item, cfgFile);
	if (r.empty()) {
		r = defaultValue;
	}
	return r;
}

int Configuration::GetInt(
	const std::string& section,
	const std::string& item,
	const int& defaultValue,
	const std::string& cfgFile)
{
	int r = defaultValue;
	std::string s = ReadString(section, item, cfgFile);
	if (!s.empty()) {
		r = atoi(s.c_str());
	}
	return r;
}

std::vector<std::string> Configuration::GetSections(const std::string& cfgFile)
{
	std::ifstream f(cfgFile.c_str());
	if (!f){
		_ERROR_EXIT(-1, "[cannot open configuration file %s]", cfgFile.c_str());
	}

  std::vector<std::string> sections;
	std::string curSection;

	std::string line;
	while (getline(f, line)) {
		if (!AnalyseSection(line, curSection))
			continue;
    sections.push_back(curSection);
	}
	f.close();
	return sections;
}

std::string Configuration::ReadString(
	const std::string& section,
	const std::string& item,
	const std::string& cfgFile)
{
	std::ifstream f(cfgFile.c_str());
	if (!f){
		_ERROR_EXIT(-1, "[cannot open configuration file %s]", cfgFile.c_str());
	}

	bool findSection = false;
	bool findItem = false;

	std::string curSection;
	std::string curItem;
	std::string curValue;

	std::string line;
	while (getline(f, line)) {
		if (!AnalyseSection(line, curSection))
			continue;
		if (ToLower(section) == ToLower(curSection)) {
			findSection = true;
			break;
		}
	}

	if (!findSection) {
		f.close();
		_ERROR_EXIT(-1, "[cannot find section:%s in configuration file]", section.c_str());
	}

	while (getline(f, line)) {
		if (AnalyseSection(line, curSection))
			break;
		if (!AnalyseItem(line, curItem, curValue))
			continue;
		if (ToLower(item) == ToLower(curItem)) {
			findItem = true;
			break;
		}
	}

	if (!findItem) {
		f.close();
    curValue = "";
	}
	
	f.close();
	return curValue;
}

bool Configuration::AnalyseSection(const std::string& line, std::string& section)
{
	if (line.empty())
		return false;

	int start_pos = 0;
	int end_pos   = line.size() - 1;

	int pos;
	if ((pos = line.find(COMMENT_CHAR)) != -1) {
		// the first character is the comment character
		if (pos == 0) {
			return false;
		}
		end_pos = pos - 1;
	}
	// delete the comment content
	std::string new_line = line.substr(start_pos, end_pos - start_pos + 1);

	Trim(new_line);
	if (new_line[0] != '[' && new_line[new_line.length() - 1] != ']')
		return false;

	section = new_line.substr(1, new_line.length() - 2);

	Trim(section);
	if (section.empty()) {
		return false;
	}

	return true;
}

bool Configuration::AnalyseItem(const std::string& line, std::string& item, std::string& value)
{
	if (line.empty())
		return false;

	int start_pos = 0;
	int end_pos   = line.size() - 1;

	int pos;
	if ((pos = line.find(COMMENT_CHAR)) != -1) {
		// the first character is the comment character
		if (pos == 0) {
			return false;
		}
		end_pos = pos - 1;
	}
	// delete the comment content
	std::string new_line = line.substr(start_pos, end_pos - start_pos + 1);

	if ((pos = new_line.find('=')) == -1)
		return false;

	item  = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	Trim(item);
	if (item.empty()) {
		return false;
	}
	Trim(value);

  if (item[0] == '"' && item[item.length() - 1] == '"') {
    item.erase(0, 1);
    item.erase(item.length() - 1, 1);
  }
  if (value[0] == '"' && value[value.length() - 1] == '"') {
    value.erase(0, 1);
    value.erase(value.length() - 1, 1);
  }

	return true;
}

bool Configuration::IsSpace(const char& c)
{
	if (c == ' ' || c == '\t')
		return true;
	return false;
}

void Configuration::Trim(std::string& s)
{
  if (s.empty())
    return;
  std::string::iterator it;
    for (it=s.begin(); it!=s.end() && IsSpace(*it++););
	s.erase(s.begin(), --it);
    for (it=s.end(); it!=s.begin() && IsSpace(*--it););
	s.erase(++it, s.end());
}

std::string Configuration::ToLower(const std::string& s)
{
	std::string os(s);
	transform(os.begin(), os.end(), os.begin(), tolower);  
	return os;
}
