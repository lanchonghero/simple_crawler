#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "crawler_manager.h"

#define DEFAULT_CMD_DELAY          "1s"
#define DEFAULT_CMD_SCHEDULING     "FILO"

void usage(const char* bin_name)
{
  printf("Usage:\n");
  printf("  %s [options]\n\n", bin_name);
  printf("Options:\n");
  printf("  -h:             Show help messages.\n");
  printf("  -f file:        fetcher configuration file.\n");
  printf("  -d file:        database configuration file.\n");
  printf("  -x file:        extracter configuration file.\n\n");
  printf("You can use next options to create a single fetcher for specific url seed only when option -f is not set.\n");
  printf("  -U url:                 single fetcher url seed. No need -f.\n");
  printf("  -D time:                single fetcher delay time between to url entry. Support s/ms/us Default[%s].\n", DEFAULT_CMD_DELAY);
  printf("  -S schedule stragety:   single fetcher schedule stragety to cralw. Options:[FIFO|FILO] Default[%s].\n", DEFAULT_CMD_SCHEDULING);
}

int main(int argc, char** argv)
{
  const char *fetcher_config = NULL;
  const char *database_config = NULL;
  const char *extracter_config = NULL;
  const char *seed  = NULL;
  const char *delay = DEFAULT_CMD_DELAY;
  const char *scheduling = DEFAULT_CMD_SCHEDULING;
  char opt_char;
  while ((opt_char = getopt(argc, argv, "f:d:x:U:D:S:h")) != -1) {
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
    case 'U':
      seed = optarg;
      break;
    case 'D':
      delay = optarg;
      break;
    case 'S':
      scheduling = optarg;
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
  if (seed) {
    if (fetcher_config) {
      usage(argv[0]);
      goto error_return;
    } else {
      cm->CreateSingleUrlFetcher(seed, delay, scheduling);
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
