CREATE TABLE `book` (
  `id` bigint(20) unsigned NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `title`  varchar(1024) DEFAULT NULL,
  `author`  varchar(256) DEFAULT NULL,
  `category`  varchar(256) DEFAULT NULL,
  `description` text DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
