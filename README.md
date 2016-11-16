Introduction
------------

As the project name, [Simple Crawler][] is a simple crawler system. According to
user-customized xsl rules, it can do page analysis, extract page and store the
result.

[Simple Crawler]:https://github.com/lanchonghero/simple_crawler

Dependence
----------
* libmysqlclient
* libxml2
* libxslt
* libcurl
* libpcre

On ubuntu:

    sudo apt-get install libmysqlclient-dev libxml2-dev

    sudo apt-get install libxslt1-dev libcurl4-openssl-dev libpcre3-dev

Configuration
-------------
The configuration is necessary. Some examples lists in conf path.
* fetcher-example.ini
* db-example.ini
* xtrt-example.ini

------------------------------------
`fetcher-example.ini` is the crawler configuration. It can specify multiple
crawler to crawl different pages by define different section such as:

    [book_fetcher_section1]
    "delay"="1s"
    "seed"="http://a.qidian.com/?page=1"

    # [book_fetcher_section2]
    # "delay"="500ms"
    # "seedpath"="data/base/seed.txt"
    # "scheduling"="FIFO"

- delay

    sepcify time interval to crawl specific page.
    support s/ms/us (second/millisecond/microsecond) default 1s

- seed

    the seed of the page to be crawled

- seedpath

    the seed file of the page to be crawled

- scheduling

    schedule strategy of crawling.

    FIFO -- first in first out
    FILO -- first in last out. By default.

------------------------------------
`db-example.ini` describe database configuration. It can also config multiple
database by using different section name.

    [book]
    "host"="127.0.0.1"
    "port"="3306"
    "user"="mysql"
    "password"=""
    "database"="test"
    "table"="book"
    "sql_query_pre"="set names utf8;delete from book;"

- host

    host of mysql server

- port

    port number of mysql server

- user

    username

- password

    password

- database

    database to use

- table

    table to use

- sql_query_pre

    sql to be executed before any action of this database. use semicolon to
    separate multiple sql.

------------------------------------
`xtrt-example.ini` defines different strategy to extract web pages. You can
customize your domain and url regex to restrict pages to use this extraction
rule. A extraction rule can be set in your xsl file so that you can extract
content from web page by your customization and store it in database.

    [book_extracter]
    "domain"="qidian.com"
    "urlre"="^http://a\.qidian\.com/\?page=\d+$"
    "xtr"="data/base/qidian-result.xsl"

- domain

    defines domain to match. The extraction rule cannot be used if the domain
    not matched.

- urlre

    url regex. The extraction rule cannot be used if the url not match the
    regex.

- xtr

    xsl file of the extraction rule. An example xsl file to extract result
    list in data/base/. You can see it and learn how to use it to extract.

Building
--------
[CMake][] is a C++ Makefiles/Solution generator. It is usually available on most
Linux system as package. On Ubuntu:

    sudo apt-get install cmake

From the command-line on Unix in the source directory:

    mkdir release
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

[CMake]: http://www.cmake.org

How to use
----------
You need to create a database and a table to store result in your mysql server.
Create database test for example: `CREATE DATABASE test;`. And then create a
table in test. Sql can be found in data/base/create.sql. Enter in your Simple
Crawler project directory next.

    ./release/bin/simple_crawler -f conf/fetcher-example.ini -d conf/db-example.ini -x conf/xtrt-example.ini
