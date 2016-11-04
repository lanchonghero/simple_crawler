CREATE TABLE `baidu_search_result` (
  `id` bigint(20) unsigned NOT NULL,
  `query` varchar(1024) DEFAULT NULL,
  `title` varchar(1024) DEFAULT NULL,
  `description` text,
  `doctime` varchar(128) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `location` varchar(1024) DEFAULT NULL,
  `snapshot` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
