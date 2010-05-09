<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:import href="template.xsl" />
    <xsl:template match="/">
        <xsl:apply-imports />
    </xsl:template>

    <xsl:template match="cars">
        <xsl:for-each select="car">
            <b><xsl:value-of select="maker" /></b>
            <b style="color: {color}"><xsl:value-of select="model" /></b>
        </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>