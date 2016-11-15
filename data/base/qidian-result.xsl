<?xml version="1.0" encoding="utf8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" encoding="utf8"/>
  <xsl:param name="template"/>
  <xsl:param name="url"/>

  <xsl:template match="/">
    <display> <!-- fixed mode -->
      <template><xsl:value-of select="$template"/></template> <!-- fixed mode -->
      <url><xsl:value-of select="$url"/></url> <!-- fixed mode -->
      <xsl:apply-templates />
    </display>
  </xsl:template>

  <xsl:template match="//ul[@class='all-img-list cf']/li"> <!-- a match -->
    <item>                    <!-- tag:item is necessary. It defines a result you need to extract. -->
      <type>book</type>       <!-- tag:type is necessary. The value specifies witch database of this result need to be insert/update. Here book point to db-example.ini section: book. -->
      <key>url</key>          <!-- tag:key is necessary. The value is a tag name in content. Use this tag to generatge id in database. -->
      <content>               <!-- tag:content is necessary. List all fields of a result in this tag. -->
        <url><xsl:value-of select=".//div[@class='book-mid-info']//h4/a/@href"/></url> <!-- tag name need to be same with field name in table of the database -->
        <title><xsl:value-of select=".//div[@class='book-mid-info']//h4/a"/></title> <!-- -->
        <author><xsl:value-of select=".//div[@class='book-mid-info']//p[@class='author']//a[@class='name']"/></author> <!-- -->
        <category><xsl:value-of select=".//div[@class='book-mid-info']//p[@class='author']//a[@data-eid='qd_B60']"/></category> <!-- -->
        <description><xsl:value-of select="normalize-space(translate(.//div[@class='book-mid-info']//p[@class='intro']/text(), '\r\n', ''))"/></description> <!-- -->
      </content>

      <!-- tag:link is optional. The value is a url that will be pushed in crawl list. It will be crawled later by your scheduling strategy. -->
      <!--
      <link></link>
      -->
    </item>
  </xsl:template>
  
  <xsl:template match="//div[@id='page-container']">  <!-- a match -->
    <xsl:if test="number(./@data-pagemax) > number(./@data-page)">
      <item>
        <!-- tag:link is optional. The value is a url that will be pushed in crawl list. It will be crawled later by your scheduling strategy. -->
        <link><xsl:value-of select="concat(concat(substring-before($url, 'page='), 'page='), string(number(./@data-page) + 1))"/></link>
      </item>
    </xsl:if>
  </xsl:template>

  <xsl:template match="text()|@*"/>

</xsl:stylesheet>
