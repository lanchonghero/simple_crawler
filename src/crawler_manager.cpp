#include <string.h>
#include <sys/times.h>
#include <sys/select.h>
#include "crawler_manager.h"
#include "configuration.h"
#include "downloader.h"
#include "string_utils.h"
#include "str2id.h"
#include "log.h"

static xmlNodePtr getsubnode(const xmlNodePtr parent, const std::string& name)
{
  xmlNodePtr node = parent->children;
  while (node) {
    if (strcmp((const char *)(node->name), name.c_str()) == 0) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

static std::string getnodetext(xmlNodePtr node)
{
  std::string text = "";
  xmlNodePtr text_node = node->children;
  if (text_node) {
    text = std::string((const char *)XML_GET_CONTENT(text_node));
  }
  return text;
}

static std::string getsubnodetext(const xmlNodePtr parent, const std::string& name)
{
  std::string text = "";
  xmlNodePtr node = getsubnode(parent, name);
  if (node) {
    text = getnodetext(node);
  }
  return text;
}


CrawlerManager::~CrawlerManager()
{
  std::map<std::string, Fetcher*>::iterator it_f;
  std::map<std::string, MysqlWrapper*>::iterator it_d;
  std::map<std::string, Extracter*>::iterator it_e;

  for (it_f = m_fetchers.begin(); it_f != m_fetchers.end(); ++it_f)
    if (it_f->second)
      delete it_f->second;
  for (it_d = m_databases.begin(); it_d != m_databases.end(); ++it_d)
    if (it_d->second)
      delete it_d->second;
  for (it_e = m_extracters.begin(); it_e != m_extracters.end(); ++it_e)
    if (it_e->second)
      delete it_e->second;
}

int CrawlerManager::InsertFetcher(Fetcher* f)
{
  if (!f) return -1;
  m_fetchers[f->GetName()] = f;
  m_fetchers_name.push_back(f->GetName());
  return 0;
}

int CrawlerManager::CreateSingleUrlFetcher(const std::string& url)
{
  Fetcher* f = new Fetcher();

  std::string _url = url;
  f->SetSeed(trimc(_url, '"'));
  f->SetUserAgent(DEFAULT_USERAGENT);
  f->SetDelay("1s");
  InsertFetcher(f);
  return 0;
}

int CrawlerManager::InitFetchers(const std::string& configuration)
{
  m_fetcher_config = configuration;
  std::vector<std::string> sections = Configuration::GetSections(configuration);
  if (sections.empty())
    return -1;

  std::string seed;
  std::string seedpath;
  std::string delay;
  std::string useragent;
  std::string scheduling;
  for (unsigned int i=0; i<sections.size(); i++) {
    if (m_fetchers.find(sections[i]) != m_fetchers.end())
      continue;
    seed = Configuration::GetString(sections[i], "seed", "", configuration);
    seedpath = Configuration::GetString(sections[i], "seedpath", "", configuration);
    delay = Configuration::GetString(sections[i], "delay", "1", configuration);
    useragent = Configuration::GetString(sections[i], "useragent", DEFAULT_USERAGENT, configuration);
    scheduling = Configuration::GetString(sections[i], "scheduling", "", configuration);

    Fetcher* fetcher = new Fetcher();
    fetcher->SetName(sections[i]);
    fetcher->SetSeed(seed);
    fetcher->SetSeedPath(seedpath);
    fetcher->SetDelay(delay);
    fetcher->SetUserAgent(useragent);
    fetcher->SetScheduling(scheduling);

    InsertFetcher(fetcher);
  }

  return 0;
}

int CrawlerManager::InitDatabases(const std::string& configuration)
{
  m_database_config = configuration;
  std::vector<std::string> sections = Configuration::GetSections(configuration);
  if (sections.empty())
    return -1;

  for (unsigned int i=0; i<sections.size(); i++) {
    if (m_databases.find(sections[i]) != m_databases.end())
      continue;
    MysqlParams param;
    param.m_sHost = Configuration::GetString(sections[i], "host", "127.0.0.1", configuration);
    param.m_sUser = Configuration::GetString(sections[i], "user", "", configuration);
    param.m_sPass = Configuration::GetString(sections[i], "password", "", configuration);
    param.m_sDB = Configuration::GetString(sections[i], "database", "", configuration);
    param.m_sDftTable = Configuration::GetString(sections[i], "table", "", configuration);
    param.m_iPort = Configuration::GetInt(sections[i], "port", 3306, configuration);
    
    std::string s = Configuration::GetString(sections[i], "sql_query_pre", "", configuration);
    splitString(s, ';', param.m_vQueryPre);

    MysqlWrapper* db = new MysqlWrapper(param, true);
    m_databases[sections[i]] = db;
  }

  return 0;
}

int CrawlerManager::InitExtracters(const std::string& configuration)
{
  m_extracter_config = configuration;
  std::vector<std::string> sections = Configuration::GetSections(configuration);
  if (sections.empty())
    return -1;

  for (unsigned int i=0; i<sections.size(); i++) {
    if (m_extracters.find(sections[i]) != m_extracters.end())
      continue;

    Extracter* extracter = new Extracter;
    extracter->SetName(sections[i]);
    extracter->SetDomain(Configuration::GetString(sections[i], "domain", "", configuration));
    extracter->SetUrlRegex(Configuration::GetString(sections[i], "urlre", "", configuration));
    extracter->SetXtr(Configuration::GetString(sections[i], "xtr", "", configuration));

    m_extracters[sections[i]] = extracter;
    m_extracters_name.push_back(sections[i]);
  }
  return 0;
}

int CrawlerManager::InsertContent(MysqlWrapper* db, xmlNodePtr content, const uint64_t& id)
{
  std::string keys = "id";
  std::string values = StringPrintf("'%llu'", (long long unsigned)id);
  for (xmlNodePtr field = content->children; field != NULL; field = field->next) {
    std::string key = std::string((const char*)field->name);
    std::string val = getnodetext(field);
    if (!key.empty() && !val.empty()) {
      keys = keys + "," + key;
      std::string eval;
      db->SqlRealEscapeString(val, eval);
      values = values + "," + StringPrintf("'%s'", eval.c_str());
    }
  }
  std::string sql =
    StringPrintf("insert into %s(%s) values(%s)",
      db->GetParams().m_sDftTable.c_str(), keys.c_str(), values.c_str());

  if (!db->SqlQuery(sql.c_str()))
    return -1;
  return 0;
}

int CrawlerManager::UpdateContent(MysqlWrapper* db, xmlNodePtr content, const uint64_t& id)
{
  std::string set_sql = "";
  for (xmlNodePtr field = content->children; field != NULL; field = field->next) {
    std::string key = std::string((const char*)field->name);
    std::string val = getnodetext(field);
    if (!key.empty() && !val.empty()) {
      std::string eval;
      db->SqlRealEscapeString(val, eval);
      set_sql = set_sql + StringPrintf("%s='%s',", key.c_str(), eval.c_str());
    }
  }
  trimc(set_sql, ',');
  std::string sql =
    StringPrintf("update %s set %s where id='%llu'",
      db->GetParams().m_sDftTable.c_str(), set_sql.c_str(), (long long unsigned)id);

  if (!db->SqlQuery(sql.c_str()))
    return -1;
  return 0;
}

int CrawlerManager::SaveItem(MysqlWrapper* db, xmlNodePtr item)
{
  int ret = -1;
  std::string key_tag = getsubnodetext(item, "key");
  if (key_tag.empty()) return ret;
  xmlNodePtr content = getsubnode(item, "content");
  if (content) {
    std::string key = getsubnodetext(content, key_tag);
    if (!key.empty()) {
      uint64_t id = ComputeID(key);
      ret = InsertContent(db, content, id);
      if (ret == 0) {
        _INFO("insert-item-ok k %s t %s", key.c_str(), db->GetParams().m_sDftTable.c_str());
      } else if (db->SqlErrno() == 1062) {
        ret = UpdateContent(db, content, id);
        if (ret == 0)
          _INFO("update-item-ok k %s t %s", key.c_str(), db->GetParams().m_sDftTable.c_str());
        else
          _INFO("update-item-fail k %s e %s t %s",
            key.c_str(), db->SqlError().c_str(), db->GetParams().m_sDftTable.c_str());
      } else {
        _INFO("insert-item-fail k %s e %s t %s",
          key.c_str(), db->SqlError().c_str(), db->GetParams().m_sDftTable.c_str());
      }
    }
  }

  return ret;
}

int CrawlerManager::StoreResult(Fetcher* fetcher, const UrlEntry* entry, const std::string& xmlret)
{
  xmlDocPtr doc = xmlReadMemory(xmlret.c_str(), xmlret.length(), 0, 0, 0);
  if (!doc) {
    return -1;
  }
  xmlNodePtr root = xmlDocGetRootElement(doc);
  for (xmlNodePtr item = root->children; item != NULL; item = item->next) {
    if (m_database_config.empty()) {
      // no need to store result
    } else {
      std::string db_name = getsubnodetext(item, "type");
      if (m_databases.find(db_name) != m_databases.end()) {
        MysqlWrapper* db = m_databases[db_name];
        if (db) {
          SaveItem(db, item);
        }
      }
    }
    if (strcmp((const char *)item->name, "template") == 0) {
      _INFO("template u %s t %s", UrlDecode(entry->url).c_str(), getnodetext(item).c_str());
    }
  }

  // for single url fetch and extract test:
  // no need to extract next hop
  // print extract xml here
  if (m_fetcher_config.empty()) {
    fprintf(stdout, "%s\n", xmlret.c_str());
    return 0;
  }

  for (xmlNodePtr item = root->children; item != NULL; item = item->next) {
    std::string link = getsubnodetext(item, "link");
    std::string refer = getsubnodetext(item, "refer");
    // std::string data = getsubnodetext(item, "data");
    if (!link.empty()) {
      UrlEntry* ue = new UrlEntry;
      ue->url = link;
      ue->refer = refer.empty() ? "" : refer;
      fetcher->PutUrlEntry(ue);
    }
  }

  xmlFreeDoc(doc);
  return 0;
}

int CrawlerManager::ExtractAndStore(Fetcher* fetcher, const UrlEntry* entry, WebPage* wp)
{
  if (m_extracter_config.empty()) // no need to extract
    return 0;

  Extracter* e = NULL;
  for (unsigned int i=0; i<m_extracters_name.size(); i++) {
    if (m_extracters[m_extracters_name[i]]->Match(wp->url)) {
      e = m_extracters[m_extracters_name[i]];
      break;
    }
  }
  std::string xmlret;
  if (e && e->DoExtract(*wp, xmlret)) {
    _INFO("extract-ok url %s", wp->url.c_str());
    StoreResult(fetcher, entry, xmlret);
    return 0;
  }
  _ERROR("extract-fail url %s", wp->url.c_str());
  return -1;
}

int CrawlerManager::FetchWebPage(Fetcher* fetcher, const UrlEntry* entry, WebPage* wp)
{
  wp->url = UrlDecode(entry->url);
  wp->refer = entry->refer;
  std::string ua = fetcher->GetUserAgent();
  std::string error;
  int ret = Downloader::DownloadPage(UrlEncode(entry->url), ua, wp->ip, wp->httpheader, wp->html, error);
  if (ret == 0) {
    _INFO("fetch-ok c %d url %s", wp->GetCodeFromHttpHeader(), wp->url.c_str());
  } else {
    _ERROR("fetch-fail [%d:%s] url %s", ret, error.c_str(), wp->url.c_str());
    ret = -1;
  }
  return ret;
}

int CrawlerManager::FetchAndStore(Fetcher* fetcher, const UrlEntry* entry)
{
  WebPage wp;
  int ret = FetchWebPage(fetcher, entry, &wp);
  if (ret == 0) {
    ret = ExtractAndStore(fetcher, entry, &wp);
    if (fetcher->GetDelay() > 0) {
      struct timeval tval;
      tval.tv_sec = 0;
      tval.tv_usec = fetcher->GetDelay();
      select(0, NULL, NULL, NULL, &tval);
    }
  }
  return ret;
}

int CrawlerManager::ExecFetcher(Fetcher* fetcher)
{
  if (!fetcher) return -1;
  if (fetcher->InitUrllist() != 0) return -2;

  UrlEntry* entry = NULL;
  while ((entry = fetcher->GetUrlEntry()) != NULL) {
    FetchAndStore(fetcher, entry);
    delete entry;
  }
  return 0;
}

void CrawlerManager::Run()
{
  for (unsigned int i=0; i<m_fetchers_name.size(); i++) {
    Fetcher* f = m_fetchers[m_fetchers_name[i]];
    int ret = ExecFetcher(f);
    if (!ret) {
      _INFO("normal-exit f %s", m_fetchers_name[i].c_str());
    } else {
      _ERROR("abnormal-exit r %d f %s", ret, m_fetchers_name[i].c_str());
    }
  }
}
