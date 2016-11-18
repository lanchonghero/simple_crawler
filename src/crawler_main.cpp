#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "crawler_manager.h"

void usage(const char* bin_name)
{
  printf("Usage:\n");
  printf("  %s [options]\n\n", bin_name);
  printf("Options:\n");
  printf("  -h:             Show help messages.\n");
  printf("  -f file:        fetcher configuration file.\n");
  printf("  -d file:        database configuration file.\n");
  printf("  -x file:        extracter configuration file.\n");
  printf("  -u file:        single url to cralw/extract/store. No need -f.\n");
}

int main(int argc, char** argv)
{
  const char *fetcher_config = NULL;
  const char *database_config = NULL;
  const char *extracter_config = NULL;
  const char *url = NULL;
  char opt_char;
  while ((opt_char = getopt(argc, argv, "f:d:x:u:h")) != -1) {
    switch (opt_char) {
    case 'f':
      fetcher_config = optarg;
      break;
    case 'd':
      database_config = optarg;
      break;
    case 'x':
      extracter_config = optarg;
      break;
    case 'u':
      url = optarg;
      break;
    case 'h':
      usage(argv[0]);
      return EXIT_SUCCESS;
    default:
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  CrawlerManager* cm = new CrawlerManager();
  if (url) {
    if (fetcher_config) {
      usage(argv[0]);
      goto error_return;
    } else {
      cm->CreateSingleUrlFetcher(url);
    }
  }

  if (fetcher_config && cm->InitFetchers(fetcher_config))
    goto error_return;
  if (database_config && cm->InitDatabases(database_config))
    goto error_return;
  if (extracter_config && cm->InitExtracters(extracter_config))
    goto error_return;

  cm->Run();
  delete(cm);
  return EXIT_SUCCESS;

error_return:
  delete(cm);
  return EXIT_FAILURE;
}
