<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform version="1.0"
               xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               xmlns:convert="http://synce.org/convert"
               xmlns:tz="http://synce.org/tz"
               exclude-result-prefixes="convert tz"
               xmlns:AS="http://synce.org/formats/airsync_wm5/airsync"
               xmlns:T="http://synce.org/formats/airsync_wm5/tasks">

        <!-- AirSync tasks do not use a timezone. Instead we need to maintain two time fields for each
             timed entity: StartDate and UTCStartDate, also DueDate and UTCDueDate. How brain-damaged
             is this... -->

    <xsl:template match="/vcal">
        <AS:ApplicationData>
            <xsl:apply-templates/>
        </AS:ApplicationData>
    </xsl:template>

    <xsl:template match="Todo">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="Status">
        <xsl:if test="not(../PercentComplete) and Content = 'COMPLETED'">
            <T:Complete>1</T:Complete>
        </xsl:if>
    </xsl:template>

    <xsl:template match="PercentComplete">
        <!--
             When in conflict, PercentComplete wins over Status.
             That's an arbitrary decision, it can change if a good reason is found
        -->
        <xsl:if test="Content = '100'">
            <T:Complete>1</T:Complete>
        </xsl:if>
    </xsl:template>

    <xsl:template match="DateDue">
        <xsl:value-of select="convert:task_due_date_to_airsync()"/>
    </xsl:template>

    <xsl:template match="Priority">
        <T:Importance>
            <!--
                Here. let us not destroy the 'unspecified' priority when going
                _to_ airsync. We can't really help reassigning this as 'low' 
                in the other direction.
            -->
            <xsl:choose>
                <xsl:when test="string(Content) = '7'">
                    <xsl:text>0</xsl:text>
                </xsl:when>
                <xsl:when test="string(Content) = '5'">
                    <xsl:text>1</xsl:text>
                </xsl:when>
                <xsl:when test="string(Content) = '3'">
                    <xsl:text>2</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>0</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        </T:Importance>
    </xsl:template>

    <xsl:template match="RecurrenceRule">
        <T:Recurrence>
            <xsl:value-of select="convert:event_recurrence_to_airsync()"/>
        </T:Recurrence>
    </xsl:template>

    <xsl:template match="Class">
        <T:Sensitivity>
            <xsl:choose>
                <xsl:when test="Content = 'PRIVATE'">
                    <xsl:text>2</xsl:text>
                </xsl:when>
                <xsl:when test="Content = 'CONFIDENTIAL'">
                    <xsl:text>3</xsl:text>
                </xsl:when>
                <xsl:otherwise> <!-- "PUBLIC" is our default value -->
                    <xsl:text>0</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        </T:Sensitivity>
    </xsl:template>

    <xsl:template match="DateStarted">
        <xsl:value-of select="convert:task_start_date_to_airsync()"/>
    </xsl:template>

    <xsl:template match="Categories">
        <T:Categories>
            <xsl:apply-templates/>
        </T:Categories>
    </xsl:template>

    <xsl:template match="Category">
        <T:Category><xsl:value-of select="."/></T:Category>
    </xsl:template>

    <xsl:template match="Summary">
        <T:Subject><xsl:value-of select="Content"/></T:Subject>
    </xsl:template>

    <xsl:template match="Description">
        <T:Rtf><xsl:value-of select="convert:all_description_to_airsync()"/></T:Rtf>
    </xsl:template>

    <xsl:template match="*">
        <!-- <xsl:message>Ignored tag <xsl:value-of select="name(.)"/></xsl:message> -->
    </xsl:template>

</xsl:transform>
