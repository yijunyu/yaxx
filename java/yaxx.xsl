<?xml version="1.0"?>
<!DOCTYPE stylesheet [
    <!ENTITY xsl "http://www.w3.org/1999/XSL/Transform">
    <!ENTITY yaxx "urn:YAcc-Xml-eXtension">
    <!ENTITY separator1 ":">
    <!ENTITY separator2 "&#10;">
    <!ENTITY separator3 " ">
]>
<stylesheet version="1.0" xmlns="&xsl;" xmlns:xsl="&xsl;" xmlns:yaxx="&yaxx;">
    <output method="text" indent="no"/>
    <template match="yaxx:*">
        <choose>
            <when test="count(child::yaxx:*)=0">
                <if test="./text()!='' and ./text()!='&#10;'">
                    <value-of select="."/>
                    <text>&separator3;</text>
                </if>
            </when>
            <otherwise>
                <for-each select="child::yaxx:*"> 
                    <apply-templates select="."/>
                </for-each>
            </otherwise>
        </choose>
    </template>
</stylesheet>
