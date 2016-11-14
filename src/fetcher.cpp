#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "fetcher.h"
#include "log.h"

Fetcher::Fetcher():
  delay(1 * 1000 * 1000)
{
}

Fetcher::~Fetcher()
{
}

void Fetcher::SetName(const std::string& s)
{
  name = s;
}

void Fetcher::SetSeed(const std::string& s)
{
  seed = s;
}

void Fetcher::SetSeedPath(const std::string& s)
{
  seedpath = s;
}

void Fetcher::SetUserAgent(const std::string& s)
{
  useragent = s;
}

void Fetcher::SetScheduling(const std::string& s)
{
  if (strcasecmp(s.c_str(), "FIFO") == 0) {
    scheduling = SCHEDULING_FIFO;
  } else if (strcasecmp(s.c_str(), "FILO") == 0) {
    scheduling = SCHEDULING_FILO;
  } else {
    scheduling = SCHEDULING_UNSET;
  }
}

void Fetcher::SetDelay(const std::string& s)
{
  int pos = -1;
  if ((pos = s.find("us")) != -1) {
    delay = atoi(s.substr(0, pos).c_str());
  } else if ((pos = s.find("ms")) != -1) {
    delay = atoi(s.substr(0, pos).c_str()) * 1000;
  } else if ((pos = s.find("s")) != -1) {
    delay = atoi(s.substr(0, pos).c_str()) * 1000 * 1000;
  } else {
    delay = 1 * 1000 * 1000;
  }
}

int Fetcher::InitUrllist()
{
  if (!seed.empty()) {
    UrlEntry* entry = new UrlEntry();
    entry->url = seed;
    entry->refer = "";
    urllist.push_back(entry);
  }

  if (!seedpath.empty()) {
    std::ifstream f(seedpath.c_str());
    if (!f) {
      _ERROR_EXIT(-1, "[cannot open seed file %s]", seedpath.c_str());
    }
    std::string line;
    while (getline(f, line)) {
      UrlEntry* entry = new UrlEntry();
      entry->url = line;
      entry->refer = "";
      PutUrlEntry(entry);
      // urllist.push_back(entry);
    }
    f.close();
  }

  return 0;
}

UrlEntry* Fetcher::GetUrlEntry()
{
  if (urllist.empty())
    return NULL;

  UrlEntry* entry = NULL;
  switch (scheduling) {
  case SCHEDULING_FIFO:
    entry = urllist.front();
    urllist.pop_front();
    break;
  case SCHEDULING_FILO:
  default:
    entry = urllist.back();
    urllist.pop_back();
    break;
  }
  return entry;
}

void Fetcher::PutUrlEntry(UrlEntry* entry)
{
  if (entry) {
    urllist.push_back(entry);
  }
  return;
}
