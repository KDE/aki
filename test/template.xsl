<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xhtml="http://www.w3.org/1999/xhtml">
    <xsl:output method="html" doctype-public="-//W3C//DTD XHTML 1.1//EN"
                doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
                omit-xml-declaration="no" indent="yes" />
    <xsl:template match="/">
        <xsl:element name="html">
            <xsl:element name="head">
                <xsl:element name="title" />
            </xsl:element>
        </xsl:element>
        <xsl:element name="body">
            <xsl:apply-templates />
        </xsl:element>
    </xsl:template>
</xsl:stylesheet>