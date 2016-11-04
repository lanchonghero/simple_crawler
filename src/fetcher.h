#ifndef _FETCHER_H
#define _FETCHER_H
#include <string>
#include <list>

struct UrlEntry {
  std::string url;
  std::string refer;
};

enum {
  SCHEDULING_FIFO = 1,
  SCHEDULING_FILO,

  SCHEDULING_UNSET
};

class Fetcher
{
public:
  Fetcher();
  virtual ~Fetcher();

  std::string GetName() const {return name;}
  std::string GetSeed() const {return seed;}
  std::string GetSeedPath() const {return seedpath;}
  std::string GetUserAgent() const {return useragent;}
  int GetDelay() const {return delay;}
  int GetScheduling() const {return scheduling;}

  void SetName(const std::string& s);
  void SetSeed(const std::string& s);
  void SetSeedPath(const std::string& s);
  void SetDelay(const std::string& s);
  void SetUserAgent(const std::string& s);
  void SetScheduling(const std::string& s);

  int InitUrllist();
  UrlEntry* GetUrlEntry();
  void PutUrlEntry(UrlEntry* entry);

private:
  std::string name;
  std::string seed;
  std::string seedpath;
  std::string useragent;
  int scheduling;
  int delay;

  std::list<UrlEntry*> urllist;
};

#endif //_FETCHER_H
