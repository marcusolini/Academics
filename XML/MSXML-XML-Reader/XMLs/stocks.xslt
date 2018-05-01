<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:template match="/">
		<html>
		<head>
	        <title>Stocks</title>
		<link rel="stylesheet" href="stocks-xslt.css"/>
            <style>
            body {background-color:#DACFE5; font-family:Arial, Helvetica, sans-serif}
            h1.unknown {color:#8A8A8A}
            h1.nasdaq {color:#000077}
            h1.nyse{color:#0D3427}
            img {border:0; margin-left:10px}
            </style>
		</head>
		<body>
		  <xsl:for-each select="/Stocks/stock">
           <xsl:sort select="@exchange" order="descending"/>
            <xsl:choose>
               <xsl:when test="@exchange = 'nasdaq'">
                  <h1 class="nasdaq">
                     <xsl:text>Name: </xsl:text>
                     <xsl:value-of select="name"/>
                     <xsl:text>  Symbol: </xsl:text>
                     <xsl:value-of select="symbol"/>
                     <xsl:text>  Price: </xsl:text>
                     <xsl:value-of select="price"/>
                     <xsl:text>  Exchange: </xsl:text>
                     <xsl:value-of select="@exchange"/>
                  </h1>
               </xsl:when>
               <xsl:when test="@exchange = 'nyse'">
                  <h1 class="nyse">
                     <xsl:text>Name: </xsl:text>
                     <xsl:value-of select="name"/>
                     <xsl:text>  Symbol: </xsl:text>
                     <xsl:value-of select="symbol"/>
                     <xsl:text>  Price: </xsl:text>
                     <xsl:value-of select="price"/>
                     <xsl:text>  Exchange: </xsl:text>
                     <xsl:value-of select="@exchange"/>
                  </h1>
               </xsl:when>
               <xsl:otherwise>
                  <h1 class="unknown">
                     <xsl:text>Name: </xsl:text>
                     <xsl:value-of select="name"/>
                     <xsl:text>  Symbol: </xsl:text>
                     <xsl:value-of select="symbol"/>
                     <xsl:text>  Price: </xsl:text>
                     <xsl:value-of select="price"/>
                     <xsl:text>  Exchange: </xsl:text>
                     <xsl:value-of select="@exchange"/>
                  </h1>
               </xsl:otherwise>
            </xsl:choose>
	</xsl:for-each>
		</body>
		</html>
	</xsl:template>

</xsl:stylesheet>
