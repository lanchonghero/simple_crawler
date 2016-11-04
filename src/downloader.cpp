#include "downloader.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

static size_t write_data(void* ptr, size_t size, size_t nmemb, void *stream)
{
  return fwrite(ptr, size, nmemb, (FILE *)stream);
}

static void scanip(const std::string& s, std::string& ip)
{
  //"Connected to www.sina.com.cn (202.108.33.60) port 80 (#0)\n"
  int p = s.find("Connected to ");
  int q = s.find("\n");
  if (p != -1 && q != -1) {
    int m = s.find("(", p);
    int n = s.find(")", p);
    if (m != -1 && n != -1 && n - m > 1) {
      ip = s.substr(m+1, n-m-1);
    }
  }
}

int Downloader::DownloadPage(
  const std::string& url,
  const std::string& useragent,
  std::string& ip,
  std::string& header,
  std::string& data,
  std::string& error)
{
  CURL *curl;
  int ret;

  FILE *datafp;
  char *data_out;
  size_t data_out_len;

  FILE *headerfp;
  char *header_out;
  size_t header_out_len;

  FILE *errfp;
  char *err;
  size_t lerr;
  
  char errbuf[CURL_ERROR_SIZE];
  errbuf[0] = 0;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); 
  curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent.c_str());


  datafp = open_memstream(&data_out, &data_out_len);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, datafp);
  headerfp = open_memstream(&header_out, &header_out_len);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerfp);
  errfp = open_memstream(&err, &lerr);
  curl_easy_setopt(curl, CURLOPT_STDERR, errfp);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); 
  curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

  ret = curl_easy_perform(curl);
  fclose(errfp);
  fclose(datafp);
  fclose(headerfp);

  if (ret == CURLE_OK) {
    scanip(std::string(err, lerr), ip);
    data = std::string(data_out, data_out_len);
    header = std::string(header_out, header_out_len);
  } else {
    size_t len = strlen(errbuf);
    if (len) {
      error = std::string(errbuf, len);
    } else {
      error = std::string(curl_easy_strerror(static_cast<CURLcode>(ret)));
    }
  }
  free(err);
  free(data_out);
  free(header_out);

  curl_easy_cleanup(curl);
  return ret;
}
