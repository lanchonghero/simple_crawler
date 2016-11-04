<?xml version="1.0" encoding="utf8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" encoding="utf8"/>
	<xsl:param name="template"/>
	<xsl:param name="url"/>

	<xsl:template match="/">
		<display>
			<template><xsl:value-of select="$template"/></template>
			<url><xsl:value-of select="$url"/></url>
			<xsl:apply-templates />
		</display>
	</xsl:template>

	<xsl:template match="//div[@id='content_left']//div[@class='result c-container ']">
    <xsl:variable name="keyurl" select=".//h3/a/@href"/>
		<item>
			<type>baidu_search_result</type>
			<key>url</key>
			<content>
				<url><xsl:value-of select="$keyurl"/></url>
        <query>
          <xsl:variable name="querystr" select="substring-after($url, 'wd=')"/>
          <xsl:choose>
            <xsl:when test="contains($querystr, '&amp;')">
              <xsl:value-of select="substring-before($querystr, '&amp;')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$querystr"/>
            </xsl:otherwise>
          </xsl:choose>
        </query>
        <title><xsl:value-of select=".//h3/a"/></title>
        <description>
          <xsl:value-of select="translate(.//div[@class='c-abstract'], .//div[@class='c-abstract']/span[@class=' newTimeFactor_before_abs m'], '')"/>
        </description>
        <doctime>
          <xsl:if test="count(.//div[@class='c-abstract']/span[@class=' newTimeFactor_before_abs m']) > 0">
            <xsl:value-of select=".//div[@class='c-abstract']/span[@class=' newTimeFactor_before_abs m']"/>
          </xsl:if>
        </doctime>
        <snapshot>
          <xsl:value-of select="normalize-space(.//div[@class='f13']//a[@class='c-showurl'])"/>
        </snapshot>
			</content>
      <link><xsl:value-of select="$keyurl"/></link>
		</item>
	</xsl:template>
  
  <xsl:template match="//div[@id='page']">
    <xsl:if test=".//strong//span[@class='pc'] &lt; 2 and count(.//strong/following-sibling::a) &gt; 0">
			<item>
        <xsl:variable name="pn" select="substring-after($url, '&amp;pn=')"/>
        <xsl:variable name="newpn" select="concat('&amp;pn=', $pn+10)"/>
        <link><xsl:value-of select="concat(substring-before($url, '&amp;pn='), $newpn)"/></link> <!-- for next page -->
			</item>
		</xsl:if>
	</xsl:template>

	<xsl:template match="text()|@*"/>

</xsl:stylesheet>
