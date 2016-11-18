#ifndef _CRAWLER_MANAGER_H
#define _CRAWLER_MANAGER_H

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "fetcher.h"
#include "mysql_wrapper.h"
#include "extracter.h"
#include "webpage.h"

#define DEFAULT_USERAGENT "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1"

class CrawlerManager
{
public:
  CrawlerManager() {}
  virtual ~CrawlerManager();

  int InsertFetcher(Fetcher* f);
  int CreateSingleUrlFetcher(const std::string& url);

  int InitFetchers(const std::string& configuration);
  int InitDatabases(const std::string& configuration);
  int InitExtracters(const std::string& configuration);

  int InsertContent(MysqlWrapper* db, xmlNodePtr content, const uint64_t& id);
  int UpdateContent(MysqlWrapper* db, xmlNodePtr content, const uint64_t& id);
  int SaveItem(MysqlWrapper* db, xmlNodePtr item);

  int StoreResult(Fetcher* fetcher, const UrlEntry* entry, const std::string& xmlret);
  int ExtractAndStore(Fetcher* fetcher, const UrlEntry* entry, WebPage* wp);
  int FetchWebPage(Fetcher* fetcher, const UrlEntry* entry, WebPage* wp);
  int FetchAndStore(Fetcher* fetcher, const UrlEntry* entry);
  int ExecFetcher(Fetcher* fetcher);

  void Run();

private:
  std::map<std::string, Fetcher*> m_fetchers;
  std::map<std::string, MysqlWrapper*> m_databases;
  std::map<std::string, Extracter*> m_extracters;

  std::vector<std::string> m_fetchers_name;
  std::vector<std::string> m_extracters_name;

  std::string m_fetcher_config;
  std::string m_database_config;
  std::string m_extracter_config;
};

#endif //_CRAWLER_MANAGER_H
