<?xml version="1.0" encoding="utf8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" encoding="utf8"/>
	<xsl:param name="template"/>
	<xsl:param name="url"/>
  <xsl:param name="location"/>

	<xsl:template match="/">
		<display>
			<template><xsl:value-of select="$template"/></template>
			<url><xsl:value-of select="$url"/></url>
      <item>
        <type>baidu_search_result</type>
        <key>url</key>
        <content>
          <url><xsl:value-of select="$url"/></url>
          <location><xsl:value-of select="$location"/></location>
        </content>
      </item>
		</display>
	</xsl:template>

	<xsl:template match="text()|@*"/>

</xsl:stylesheet>
