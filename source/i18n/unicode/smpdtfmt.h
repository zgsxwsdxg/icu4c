/*
* Copyright (C) 1997-2013, International Business Machines Corporation and
* others. All Rights Reserved.
*******************************************************************************
*
* File SMPDTFMT.H
*
* Modification History:
*
*   Date        Name        Description
*   02/19/97    aliu        Converted from java.
*   07/09/97    helena      Make ParsePosition into a class.
*   07/21/98    stephen     Added GMT_PLUS, GMT_MINUS
*                            Changed setTwoDigitStartDate to set2DigitYearStart
*                            Changed getTwoDigitStartDate to get2DigitYearStart
*                            Removed subParseLong
*                            Removed getZoneIndex (added in DateFormatSymbols)
*   06/14/99    stephen     Removed fgTimeZoneDataSuffix
*   10/14/99    aliu        Updated class doc to describe 2-digit year parsing
*                           {j28 4182066}.
*******************************************************************************
*/

#ifndef SMPDTFMT_H
#define SMPDTFMT_H

#include "unicode/utypes.h"

/**
 * \file
 * \brief C++ API: Format and parse dates in a language-independent manner.
 */

#if !UCONFIG_NO_FORMATTING

#include "unicode/datefmt.h"
#include "unicode/udisplaycontext.h"

U_NAMESPACE_BEGIN

class DateFormatSymbols;
class DateFormat;
class MessageFormat;
class FieldPositionHandler;
class TimeZoneFormat;

/**
 *
 * SimpleDateFormat is a concrete class for formatting and parsing dates in a
 * language-independent manner. It allows for formatting (millis -> text),
 * parsing (text -> millis), and normalization. Formats/Parses a date or time,
 * which is the standard milliseconds since 24:00 GMT, Jan 1, 1970.
 * <P>
 * Clients are encouraged to create a date-time formatter using DateFormat::getInstance(),
 * getDateInstance(), getDateInstance(), or getDateTimeInstance() rather than
 * explicitly constructing an instance of SimpleDateFormat.  This way, the client
 * is guaranteed to get an appropriate formatting pattern for whatever locale the
 * program is running in.  However, if the client needs something more unusual than
 * the default patterns in the locales, he can construct a SimpleDateFormat directly
 * and give it an appropriate pattern (or use one of the factory methods on DateFormat
 * and modify the pattern after the fact with toPattern() and applyPattern().
 *
 * <p><strong>Date and Time Patterns:</strong></p>
 *
 * <p>Date and time formats are specified by <em>date and time pattern</em> strings.
 * Within date and time pattern strings, all unquoted ASCII letters [A-Za-z] are reserved
 * as pattern letters representing calendar fields. <code>SimpleDateFormat</code> supports
 * the date and time formatting algorithm and pattern letters defined by
 * <a href="http://www.unicode.org/reports/tr35/tr35-dates.html#Date_Field_Symbol_Table">UTS#35
 * Unicode Locale Data Markup Language (LDML)</a> and further documented for ICU in the
 * <a href="https://sites.google.com/site/icuprojectuserguide/formatparse/datetime?pli=1#TOC-Date-Field-Symbol-Table">ICU
 * User Guide</a>. The following pattern letters are currently available:</p>
 *
 * <table border="1">
 *     <tr>
 *         <th>Field</th>
 *         <th style="text-align: center">Sym.</th>
 *         <th style="text-align: center">No.</th>
 *         <th>Example</th>
 *         <th>Description</th>
 *     </tr>
 *     <tr>
 *         <th rowspan="3">era</th>
 *         <td style="text-align: center" rowspan="3">G</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>AD</td>
 *         <td rowspan="3">Era - Replaced with the Era string for the current date. One to three letters for the 
 *         abbreviated form, four letters for the long form, five for the narrow form.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Anno Domini</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>A</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="6">year</th>
 *         <td style="text-align: center">y</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>1996</td>
 *         <td>Year. Normally the length specifies the padding, but for two letters it also specifies the maximum
 *         length. Example:<div align="center">
 *             <center>
 *             <table border="1" cellpadding="2" cellspacing="0">
 *                 <tr>
 *                     <th>Year</th>
 *                     <th style="text-align: right">y</th>
 *                     <th style="text-align: right">yy</th>
 *                     <th style="text-align: right">yyy</th>
 *                     <th style="text-align: right">yyyy</th>
 *                     <th style="text-align: right">yyyyy</th>
 *                 </tr>
 *                 <tr>
 *                     <td>AD 1</td>
 *                     <td style="text-align: right">1</td>
 *                     <td style="text-align: right">01</td>
 *                     <td style="text-align: right">001</td>
 *                     <td style="text-align: right">0001</td>
 *                     <td style="text-align: right">00001</td>
 *                 </tr>
 *                 <tr>
 *                     <td>AD 12</td>
 *                     <td style="text-align: right">12</td>
 *                     <td style="text-align: right">12</td>
 *                     <td style="text-align: right">012</td>
 *                     <td style="text-align: right">0012</td>
 *                     <td style="text-align: right">00012</td>
 *                 </tr>
 *                 <tr>
 *                     <td>AD 123</td>
 *                     <td style="text-align: right">123</td>
 *                     <td style="text-align: right">23</td>
 *                     <td style="text-align: right">123</td>
 *                     <td style="text-align: right">0123</td>
 *                     <td style="text-align: right">00123</td>
 *                 </tr>
 *                 <tr>
 *                     <td>AD 1234</td>
 *                     <td style="text-align: right">1234</td>
 *                     <td style="text-align: right">34</td>
 *                     <td style="text-align: right">1234</td>
 *                     <td style="text-align: right">1234</td>
 *                     <td style="text-align: right">01234</td>
 *                 </tr>
 *                 <tr>
 *                     <td>AD 12345</td>
 *                     <td style="text-align: right">12345</td>
 *                     <td style="text-align: right">45</td>
 *                     <td style="text-align: right">12345</td>
 *                     <td style="text-align: right">12345</td>
 *                     <td style="text-align: right">12345</td>
 *                 </tr>
 *             </table>
 *             </center></div>
 *         </td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">Y</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>1997</td>
 *         <td>Year (in "Week of Year" based calendars). Normally the length specifies the padding,
 *         but for two letters it also specifies the maximum length. This year designation is used in ISO
 *         year-week calendar as defined by ISO 8601, but can be used in non-Gregorian based calendar systems
 *         where week date processing is desired. May not always be the same value as calendar year.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">u</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>4601</td>
 *         <td>Extended year. This is a single number designating the year of this calendar system, encompassing
 *         all supra-year fields. For example, for the Julian calendar system, year numbers are positive, with an
 *         era of BCE or CE. An extended year value for the Julian calendar system assigns positive values to CE
 *         years and negative values to BCE years, with 1 BCE being year 0.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center" rowspan="3">U</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>&#30002;&#23376;</td>
 *         <td rowspan="3">Cyclic year name. Calendars such as the Chinese lunar calendar (and related calendars)
 *         and the Hindu calendars use 60-year cycles of year names. Use one through three letters for the abbreviated
 *         name, four for the full name, or five for the narrow name (currently the data only provides abbreviated names,
 *         which will be used for all requested name widths). If the calendar does not provide cyclic year name data,
 *         or if the year value to be formatted is out of the range of years for which cyclic name data is provided,
 *         then numeric formatting is used (behaves like 'y').</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>(currently also &#30002;&#23376;)</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>(currently also &#30002;&#23376;)</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="6">quarter</th>
 *         <td rowspan="3" style="text-align: center">Q</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>02</td>
 *         <td rowspan="3">Quarter - Use one or two for the numerical quarter, three for the abbreviation, or four 
 *         for the full name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Q2</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>2nd quarter</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="3" style="text-align: center">q</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>02</td>
 *         <td rowspan="3"><b>Stand-Alone</b> Quarter - Use one or two for the numerical quarter, three for the abbreviation, 
 *         or four for the full name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Q2</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>2nd quarter</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="8">month</th>
 *         <td rowspan="4" style="text-align: center">M</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>09</td>
 *         <td rowspan="4">Month - Use one or two for the numerical month, three for the abbreviation, four for
 *         the full name, or five for the narrow name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Sept</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>September</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>S</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="4" style="text-align: center">L</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>09</td>
 *         <td rowspan="4"><b>Stand-Alone</b> Month - Use one or two for the numerical month, three for the abbreviation, 
 *         or four for the full name, or 5 for the narrow name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Sept</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>September</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>S</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="2">week</th>
 *         <td style="text-align: center">w</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>27</td>
 *         <td>Week of Year.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">W</td>
 *         <td style="text-align: center">1</td>
 *         <td>3</td>
 *         <td>Week of Month</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="4">day</th>
 *         <td style="text-align: center">d</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>1</td>
 *         <td>Date - Day of the month</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">D</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>345</td>
 *         <td>Day of year</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">F</td>
 *         <td style="text-align: center">1</td>
 *         <td>2</td>
 *         <td>Day of Week in Month. The example is for the 2nd Wed in July</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">g</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>2451334</td>
 *         <td>Modified Julian day. This is different from the conventional Julian day number in two regards.
 *         First, it demarcates days at local zone midnight, rather than noon GMT. Second, it is a local number;
 *         that is, it depends on the local time zone. It can be thought of as a single number that encompasses 
 *         all the date-related fields.</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="14">week<br>
 *         day</th>
 *         <td rowspan="4" style="text-align: center">E</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>Tues</td>
 *         <td rowspan="4">Day of week - Use one through three letters for the short day, or four for the full name, 
 *         five for the narrow name, or six for the short name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Tuesday</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>T</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">6</td>
 *         <td>Tu</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="5" style="text-align: center">e</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>2</td>
 *         <td rowspan="5">Local day of week. Same as E except adds a numeric value that will depend on the local
 *         starting day of the week, using one or two letters. For this example, Monday is the first day of the week.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Tues</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Tuesday</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>T</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">6</td>
 *         <td>Tu</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="5" style="text-align: center">c</td>
 *         <td style="text-align: center">1</td>
 *         <td>2</td>
 *         <td rowspan="5"><b>Stand-Alone</b> local day of week - Use one letter for the local numeric value (same
 *         as 'e'), three for the short day, four for the full name, five for the narrow name, or six for
 *         the short name.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Tues</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Tuesday</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>T</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">6</td>
 *         <td>Tu</td>
 *     </tr>
 *     <tr>
 *         <th>period</th>
 *         <td style="text-align: center">a</td>
 *         <td style="text-align: center">1</td>
 *         <td>AM</td>
 *         <td>AM or PM</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="4">hour</th>
 *         <td style="text-align: center">h</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>11</td>
 *         <td>Hour [1-12]. When used in skeleton data or in a skeleton passed in an API for flexible data pattern
 *         generation, it should match the 12-hour-cycle format preferred by the locale (h or K); it should not match
 *         a 24-hour-cycle format (H or k). Use hh for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">H</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>13</td>
 *         <td>Hour [0-23]. When used in skeleton data or in a skeleton passed in an API for flexible data pattern
 *         generation, it should match the 24-hour-cycle format preferred by the locale (H or k); it should not match a
 *         12-hour-cycle format (h or K). Use HH for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">K</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>0</td>
 *         <td>Hour [0-11]. When used in a skeleton, only matches K or h, see above. Use KK for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">k</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>24</td>
 *         <td>Hour [1-24]. When used in a skeleton, only matches k or H, see above. Use kk for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <th>minute</th>
 *         <td style="text-align: center">m</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>59</td>
 *         <td>Minute. Use one or two for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="3">second</th>
 *         <td style="text-align: center">s</td>
 *         <td style="text-align: center">1..2</td>
 *         <td>12</td>
 *         <td>Second. Use one or two for zero padding.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">S</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>3456</td>
 *         <td>Fractional Second - truncates (like other time fields) to the count of letters.
 *         (example shows display using pattern SSSS for seconds value 12.34567)</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">A</td>
 *         <td style="text-align: center">1..n</td>
 *         <td>69540000</td>
 *         <td>Milliseconds in day. This field behaves <i>exactly</i> like a composite of all time-related fields,
 *         not including the zone fields. As such, it also reflects discontinuities of those fields on DST transition
 *         days. On a day of DST onset, it will jump forward. On a day of DST cessation, it will jump backward. This
 *         reflects the fact that is must be combined with the offset field to obtain a unique local time value.</td>
 *     </tr>
 *     <tr>
 *         <th rowspan="23">zone</th>
 *         <td rowspan="2" style="text-align: center">z</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>PDT</td>
 *         <td>The <i>short specific non-location format</i>.
 *         Where that is unavailable, falls back to the <i>short localized GMT format</i> ("O").</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Pacific Daylight Time</td>
 *         <td>The <i>long specific non-location format</i>.
 *         Where that is unavailable, falls back to the <i>long localized GMT format</i> ("OOOO").</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="3" style="text-align: center">Z</td>
 *         <td style="text-align: center">1..3</td>
 *         <td>-0800</td>
 *         <td>The <i>ISO8601 basic format</i> with hours, minutes and optional seconds fields.
 *         The format is equivalent to RFC 822 zone format (when optional seconds field is absent).
 *         This is equivalent to the "xxxx" specifier.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>GMT-8:00</td>
 *         <td>The <i>long localized GMT format</i>.
 *         This is equivalent to the "OOOO" specifier.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>-08:00<br>
 *         -07:52:58</td>
 *         <td>The <i>ISO8601 extended format</i> with hours, minutes and optional seconds fields.
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.
 *         This is equivalent to the "XXXXX" specifier.</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="2" style="text-align: center">O</td>
 *         <td style="text-align: center">1</td>
 *         <td>GMT-8</td>
 *         <td>The <i>short localized GMT format</i>.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>GMT-08:00</td>
 *         <td>The <i>long localized GMT format</i>.</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="2" style="text-align: center">v</td>
 *         <td style="text-align: center">1</td>
 *         <td>PT</td>
 *         <td>The <i>short generic non-location format</i>.
 *         Where that is unavailable, falls back to the <i>generic location format</i> ("VVVV"),
 *         then the <i>short localized GMT format</i> as the final fallback.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Pacific Time</td>
 *         <td>The <i>long generic non-location format</i>.
 *         Where that is unavailable, falls back to <i>generic location format</i> ("VVVV").
 *     </tr>
 *     <tr>
 *         <td rowspan="4" style="text-align: center">V</td>
 *         <td style="text-align: center">1</td>
 *         <td>uslax</td>
 *         <td>The short time zone ID.
 *         Where that is unavailable, the special short time zone ID <i>unk</i> (Unknown Zone) is used.<br>
 *         <i><b>Note</b>: This specifier was originally used for a variant of the short specific non-location format,
 *         but it was deprecated in the later version of the LDML specification. In CLDR 23/ICU 51, the definition of
 *         the specifier was changed to designate a short time zone ID.</i></td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">2</td>
 *         <td>America/Los_Angeles</td>
 *         <td>The long time zone ID.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>Los Angeles</td>
 *         <td>The exemplar city (location) for the time zone.
 *         Where that is unavailable, the localized exemplar city name for the special zone <i>Etc/Unknown</i> is used
 *         as the fallback (for example, "Unknown City"). </td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>Los Angeles Time</td>
 *         <td>The <i>generic location format</i>.
 *         Where that is unavailable, falls back to the <i>long localized GMT format</i> ("OOOO";
 *         Note: Fallback is only necessary with a GMT-style Time Zone ID, like Etc/GMT-830.)<br>
 *         This is especially useful when presenting possible timezone choices for user selection, 
 *         since the naming is more uniform than the "v" format.</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="5" style="text-align: center">X</td>
 *         <td style="text-align: center">1</td>
 *         <td>-08<br>
 *         +0530<br>
 *         Z</td>
 *         <td>The <i>ISO8601 basic format</i> with hours field and optional minutes field.
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">2</td>
 *         <td>-0800<br>
 *         Z</td>
 *         <td>The <i>ISO8601 basic format</i> with hours and minutes fields.
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>-08:00<br>
 *         Z</td>
 *         <td>The <i>ISO8601 extended format</i> with hours and minutes fields.
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>-0800<br>
 *         -075258<br>
 *         Z</td>
 *         <td>The <i>ISO8601 basic format</i> with hours, minutes and optional seconds fields.
 *         (Note: The seconds field is not supported by the ISO8601 specification.)
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>-08:00<br>
 *         -07:52:58<br>
 *         Z</td>
 *         <td>The <i>ISO8601 extended format</i> with hours, minutes and optional seconds fields.
 *         (Note: The seconds field is not supported by the ISO8601 specification.)
 *         The ISO8601 UTC indicator "Z" is used when local time offset is 0.</td>
 *     </tr>
 *     <tr>
 *         <td rowspan="5" style="text-align: center">x</td>
 *         <td style="text-align: center">1</td>
 *         <td>-08<br>
 *         +0530</td>
 *         <td>The <i>ISO8601 basic format</i> with hours field and optional minutes field.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">2</td>
 *         <td>-0800</td>
 *         <td>The <i>ISO8601 basic format</i> with hours and minutes fields.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">3</td>
 *         <td>-08:00</td>
 *         <td>The <i>ISO8601 extended format</i> with hours and minutes fields.</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">4</td>
 *         <td>-0800<br>
 *         -075258</td>
 *         <td>The <i>ISO8601 basic format</i> with hours, minutes and optional seconds fields.
 *         (Note: The seconds field is not supported by the ISO8601 specification.)</td>
 *     </tr>
 *     <tr>
 *         <td style="text-align: center">5</td>
 *         <td>-08:00<br>
 *         -07:52:58</td>
 *         <td>The <i>ISO8601 extended format</i> with hours, minutes and optional seconds fields.
 *         (Note: The seconds field is not supported by the ISO8601 specification.)</td>
 *     </tr>
 * </table>
 *
 * <P>
 * Any characters in the pattern that are not in the ranges of ['a'..'z'] and
 * ['A'..'Z'] will be treated as quoted text. For instance, characters
 * like ':', '.', ' ', '#' and '@' will appear in the resulting time text
 * even they are not embraced within single quotes.
 * <P>
 * A pattern containing any invalid pattern letter will result in a failing
 * UErrorCode result during formatting or parsing.
 * <P>
 * Examples using the US locale:
 * <pre>
 * \code
 *    Format Pattern                         Result
 *    --------------                         -------
 *    "yyyy.MM.dd G 'at' HH:mm:ss vvvv" ->>  1996.07.10 AD at 15:08:56 Pacific Time
 *    "EEE, MMM d, ''yy"                ->>  Wed, July 10, '96
 *    "h:mm a"                          ->>  12:08 PM
 *    "hh 'o''clock' a, zzzz"           ->>  12 o'clock PM, Pacific Daylight Time
 *    "K:mm a, vvv"                     ->>  0:00 PM, PT
 *    "yyyyy.MMMMM.dd GGG hh:mm aaa"    ->>  1996.July.10 AD 12:08 PM
 * \endcode
 * </pre>
 * Code Sample:
 * <pre>
 * \code
 *     UErrorCode success = U_ZERO_ERROR;
 *     SimpleTimeZone* pdt = new SimpleTimeZone(-8 * 60 * 60 * 1000, "PST");
 *     pdt->setStartRule( Calendar::APRIL, 1, Calendar::SUNDAY, 2*60*60*1000);
 *     pdt->setEndRule( Calendar::OCTOBER, -1, Calendar::SUNDAY, 2*60*60*1000);
 *
 *     // Format the current time.
 *     SimpleDateFormat* formatter
 *         = new SimpleDateFormat ("yyyy.MM.dd G 'at' hh:mm:ss a zzz", success );
 *     GregorianCalendar cal(success);
 *     UDate currentTime_1 = cal.getTime(success);
 *     FieldPosition fp(0);
 *     UnicodeString dateString;
 *     formatter->format( currentTime_1, dateString, fp );
 *     cout << "result: " << dateString << endl;
 *
 *     // Parse the previous string back into a Date.
 *     ParsePosition pp(0);
 *     UDate currentTime_2 = formatter->parse(dateString, pp );
 * \endcode
 * </pre>
 * In the above example, the time value "currentTime_2" obtained from parsing
 * will be equal to currentTime_1. However, they may not be equal if the am/pm
 * marker 'a' is left out from the format pattern while the "hour in am/pm"
 * pattern symbol is used. This information loss can happen when formatting the
 * time in PM.
 *
 * <p>
 * When parsing a date string using the abbreviated year pattern ("y" or "yy"),
 * SimpleDateFormat must interpret the abbreviated year
 * relative to some century.  It does this by adjusting dates to be
 * within 80 years before and 20 years after the time the SimpleDateFormat
 * instance is created. For example, using a pattern of "MM/dd/yy" and a
 * SimpleDateFormat instance created on Jan 1, 1997,  the string
 * "01/11/12" would be interpreted as Jan 11, 2012 while the string "05/04/64"
 * would be interpreted as May 4, 1964.
 * During parsing, only strings consisting of exactly two digits, as defined by
 * <code>Unicode::isDigit()</code>, will be parsed into the default century.
 * Any other numeric string, such as a one digit string, a three or more digit
 * string, or a two digit string that isn't all digits (for example, "-1"), is
 * interpreted literally.  So "01/02/3" or "01/02/003" are parsed (for the
 * Gregorian calendar), using the same pattern, as Jan 2, 3 AD.  Likewise (but
 * only in lenient parse mode, the default) "01/02/-3" is parsed as Jan 2, 4 BC.
 *
 * <p>
 * If the year pattern has more than two 'y' characters, the year is
 * interpreted literally, regardless of the number of digits.  So using the
 * pattern "MM/dd/yyyy", "01/11/12" parses to Jan 11, 12 A.D.
 *
 * <p>
 * When numeric fields abut one another directly, with no intervening delimiter
 * characters, they constitute a run of abutting numeric fields.  Such runs are
 * parsed specially.  For example, the format "HHmmss" parses the input text
 * "123456" to 12:34:56, parses the input text "12345" to 1:23:45, and fails to
 * parse "1234".  In other words, the leftmost field of the run is flexible,
 * while the others keep a fixed width.  If the parse fails anywhere in the run,
 * then the leftmost field is shortened by one character, and the entire run is
 * parsed again. This is repeated until either the parse succeeds or the
 * leftmost field is one character in length.  If the parse still fails at that
 * point, the parse of the run fails.
 *
 * <P>
 * For time zones that have no names, SimpleDateFormat uses strings GMT+hours:minutes or
 * GMT-hours:minutes.
 * <P>
 * The calendar defines what is the first day of the week, the first week of the
 * year, whether hours are zero based or not (0 vs 12 or 24), and the timezone.
 * There is one common number format to handle all the numbers; the digit count
 * is handled programmatically according to the pattern.
 *
 * <p><em>User subclasses are not supported.</em> While clients may write
 * subclasses, such code will not necessarily work and will not be
 * guaranteed to work stably from release to release.
 */
class U_I18N_API SimpleDateFormat: public DateFormat {
public:
    /**
     * Construct a SimpleDateFormat using the default pattern for the default
     * locale.
     * <P>
     * [Note:] Not all locales support SimpleDateFormat; for full generality,
     * use the factory methods in the DateFormat class.
     * @param status    Output param set to success/failure code.
     * @stable ICU 2.0
     */
    SimpleDateFormat(UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern and the default locale.
     * The locale is used to obtain the symbols used in formatting (e.g., the
     * names of the months), but not to provide the pattern.
     * <P>
     * [Note:] Not all locales support SimpleDateFormat; for full generality,
     * use the factory methods in the DateFormat class.
     * @param pattern    the pattern for the format.
     * @param status     Output param set to success/failure code.
     * @stable ICU 2.0
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern, numbering system override, and the default locale.
     * The locale is used to obtain the symbols used in formatting (e.g., the
     * names of the months), but not to provide the pattern.
     * <P>
     * A numbering system override is a string containing either the name of a known numbering system,
     * or a set of field and numbering system pairs that specify which fields are to be formattied with
     * the alternate numbering system.  For example, to specify that all numeric fields in the specified
     * date or time pattern are to be rendered using Thai digits, simply specify the numbering system override
     * as "thai".  To specify that just the year portion of the date be formatted using Hebrew numbering,
     * use the override string "y=hebrew".  Numbering system overrides can be combined using a semi-colon
     * character in the override string, such as "d=decimal;M=arabic;y=hebrew", etc.
     *
     * <P>
     * [Note:] Not all locales support SimpleDateFormat; for full generality,
     * use the factory methods in the DateFormat class.
     * @param pattern    the pattern for the format.
     * @param override   the override string.
     * @param status     Output param set to success/failure code.
     * @stable ICU 4.2
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     const UnicodeString& override,
                     UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern and locale.
     * The locale is used to obtain the symbols used in formatting (e.g., the
     * names of the months), but not to provide the pattern.
     * <P>
     * [Note:] Not all locales support SimpleDateFormat; for full generality,
     * use the factory methods in the DateFormat class.
     * @param pattern    the pattern for the format.
     * @param locale     the given locale.
     * @param status     Output param set to success/failure code.
     * @stable ICU 2.0
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     const Locale& locale,
                     UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern, numbering system override, and locale.
     * The locale is used to obtain the symbols used in formatting (e.g., the
     * names of the months), but not to provide the pattern.
     * <P>
     * A numbering system override is a string containing either the name of a known numbering system,
     * or a set of field and numbering system pairs that specify which fields are to be formattied with
     * the alternate numbering system.  For example, to specify that all numeric fields in the specified
     * date or time pattern are to be rendered using Thai digits, simply specify the numbering system override
     * as "thai".  To specify that just the year portion of the date be formatted using Hebrew numbering,
     * use the override string "y=hebrew".  Numbering system overrides can be combined using a semi-colon
     * character in the override string, such as "d=decimal;M=arabic;y=hebrew", etc.
     * <P>
     * [Note:] Not all locales support SimpleDateFormat; for full generality,
     * use the factory methods in the DateFormat class.
     * @param pattern    the pattern for the format.
     * @param override   the numbering system override.
     * @param locale     the given locale.
     * @param status     Output param set to success/failure code.
     * @stable ICU 4.2
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     const UnicodeString& override,
                     const Locale& locale,
                     UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern and locale-specific
     * symbol data.  The formatter takes ownership of the DateFormatSymbols object;
     * the caller is no longer responsible for deleting it.
     * @param pattern           the given pattern for the format.
     * @param formatDataToAdopt the symbols to be adopted.
     * @param status            Output param set to success/faulure code.
     * @stable ICU 2.0
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     DateFormatSymbols* formatDataToAdopt,
                     UErrorCode& status);

    /**
     * Construct a SimpleDateFormat using the given pattern and locale-specific
     * symbol data.  The DateFormatSymbols object is NOT adopted; the caller
     * remains responsible for deleting it.
     * @param pattern           the given pattern for the format.
     * @param formatData        the formatting symbols to be use.
     * @param status            Output param set to success/faulure code.
     * @stable ICU 2.0
     */
    SimpleDateFormat(const UnicodeString& pattern,
                     const DateFormatSymbols& formatData,
                     UErrorCode& status);

    /**
     * Copy constructor.
     * @stable ICU 2.0
     */
    SimpleDateFormat(const SimpleDateFormat&);

    /**
     * Assignment operator.
     * @stable ICU 2.0
     */
    SimpleDateFormat& operator=(const SimpleDateFormat&);

    /**
     * Destructor.
     * @stable ICU 2.0
     */
    virtual ~SimpleDateFormat();

    /**
     * Clone this Format object polymorphically. The caller owns the result and
     * should delete it when done.
     * @return    A copy of the object.
     * @stable ICU 2.0
     */
    virtual Format* clone(void) const;

    /**
     * Return true if the given Format objects are semantically equal. Objects
     * of different subclasses are considered unequal.
     * @param other    the object to be compared with.
     * @return         true if the given Format objects are semantically equal.
     * @stable ICU 2.0
     */
    virtual UBool operator==(const Format& other) const;


    using DateFormat::format;

    /**
     * Format a date or time, which is the standard millis since 24:00 GMT, Jan
     * 1, 1970. Overrides DateFormat pure virtual method.
     * <P>
     * Example: using the US locale: "yyyy.MM.dd e 'at' HH:mm:ss zzz" ->>
     * 1996.07.10 AD at 15:08:56 PDT
     *
     * @param cal       Calendar set to the date and time to be formatted
     *                  into a date/time string.
     * @param appendTo  Output parameter to receive result.
     *                  Result is appended to existing contents.
     * @param pos       The formatting position. On input: an alignment field,
     *                  if desired. On output: the offsets of the alignment field.
     * @return          Reference to 'appendTo' parameter.
     * @stable ICU 2.1
     */
    virtual UnicodeString& format(  Calendar& cal,
                                    UnicodeString& appendTo,
                                    FieldPosition& pos) const;

    /**
     * Format a date or time, which is the standard millis since 24:00 GMT, Jan
     * 1, 1970. Overrides DateFormat pure virtual method.
     * <P>
     * Example: using the US locale: "yyyy.MM.dd e 'at' HH:mm:ss zzz" ->>
     * 1996.07.10 AD at 15:08:56 PDT
     *
     * @param cal       Calendar set to the date and time to be formatted
     *                  into a date/time string.
     * @param appendTo  Output parameter to receive result.
     *                  Result is appended to existing contents.
     * @param posIter   On return, can be used to iterate over positions
     *                  of fields generated by this format call.  Field values
     *                  are defined in UDateFormatField.
     * @param status    Input/output param set to success/failure code.
     * @return          Reference to 'appendTo' parameter.
     * @stable ICU 4.4
     */
    virtual UnicodeString& format(  Calendar& cal,
                                    UnicodeString& appendTo,
                                    FieldPositionIterator* posIter,
                                    UErrorCode& status) const;

    using DateFormat::parse;

    /**
     * Parse a date/time string beginning at the given parse position. For
     * example, a time text "07/10/96 4:5 PM, PDT" will be parsed into a Date
     * that is equivalent to Date(837039928046).
     * <P>
     * By default, parsing is lenient: If the input is not in the form used by
     * this object's format method but can still be parsed as a date, then the
     * parse succeeds. Clients may insist on strict adherence to the format by
     * calling setLenient(false).
     * @see DateFormat::setLenient(boolean)
     *
     * @param text  The date/time string to be parsed
     * @param cal   A Calendar set on input to the date and time to be used for
     *              missing values in the date/time string being parsed, and set
     *              on output to the parsed date/time. When the calendar type is
     *              different from the internal calendar held by this SimpleDateFormat
     *              instance, the internal calendar will be cloned to a work
     *              calendar set to the same milliseconds and time zone as the
     *              cal parameter, field values will be parsed based on the work
     *              calendar, then the result (milliseconds and time zone) will
     *              be set in this calendar.
     * @param pos   On input, the position at which to start parsing; on
     *              output, the position at which parsing terminated, or the
     *              start position if the parse failed.
     * @stable ICU 2.1
     */
    virtual void parse( const UnicodeString& text,
                        Calendar& cal,
                        ParsePosition& pos) const;


    /**
     * Set the start UDate used to interpret two-digit year strings.
     * When dates are parsed having 2-digit year strings, they are placed within
     * a assumed range of 100 years starting on the two digit start date.  For
     * example, the string "24-Jan-17" may be in the year 1817, 1917, 2017, or
     * some other year.  SimpleDateFormat chooses a year so that the resultant
     * date is on or after the two digit start date and within 100 years of the
     * two digit start date.
     * <P>
     * By default, the two digit start date is set to 80 years before the current
     * time at which a SimpleDateFormat object is created.
     * @param d      start UDate used to interpret two-digit year strings.
     * @param status Filled in with U_ZERO_ERROR if the parse was successful, and with
     *               an error value if there was a parse error.
     * @stable ICU 2.0
     */
    virtual void set2DigitYearStart(UDate d, UErrorCode& status);

    /**
     * Get the start UDate used to interpret two-digit year strings.
     * When dates are parsed having 2-digit year strings, they are placed within
     * a assumed range of 100 years starting on the two digit start date.  For
     * example, the string "24-Jan-17" may be in the year 1817, 1917, 2017, or
     * some other year.  SimpleDateFormat chooses a year so that the resultant
     * date is on or after the two digit start date and within 100 years of the
     * two digit start date.
     * <P>
     * By default, the two digit start date is set to 80 years before the current
     * time at which a SimpleDateFormat object is created.
     * @param status Filled in with U_ZERO_ERROR if the parse was successful, and with
     *               an error value if there was a parse error.
     * @stable ICU 2.0
     */
    UDate get2DigitYearStart(UErrorCode& status) const;

    /**
     * Return a pattern string describing this date format.
     * @param result Output param to receive the pattern.
     * @return       A reference to 'result'.
     * @stable ICU 2.0
     */
    virtual UnicodeString& toPattern(UnicodeString& result) const;

    /**
     * Return a localized pattern string describing this date format.
     * In most cases, this will return the same thing as toPattern(),
     * but a locale can specify characters to use in pattern descriptions
     * in place of the ones described in this class's class documentation.
     * (Presumably, letters that would be more mnemonic in that locale's
     * language.)  This function would produce a pattern using those
     * letters.
     *
     * @param result    Receives the localized pattern.
     * @param status    Output param set to success/failure code on
     *                  exit. If the pattern is invalid, this will be
     *                  set to a failure result.
     * @return          A reference to 'result'.
     * @stable ICU 2.0
     */
    virtual UnicodeString& toLocalizedPattern(UnicodeString& result,
                                              UErrorCode& status) const;

    /**
     * Apply the given unlocalized pattern string to this date format.
     * (i.e., after this call, this formatter will format dates according to
     * the new pattern)
     *
     * @param pattern   The pattern to be applied.
     * @stable ICU 2.0
     */
    virtual void applyPattern(const UnicodeString& pattern);

    /**
     * Apply the given localized pattern string to this date format.
     * (see toLocalizedPattern() for more information on localized patterns.)
     *
     * @param pattern   The localized pattern to be applied.
     * @param status    Output param set to success/failure code on
     *                  exit. If the pattern is invalid, this will be
     *                  set to a failure result.
     * @stable ICU 2.0
     */
    virtual void applyLocalizedPattern(const UnicodeString& pattern,
                                       UErrorCode& status);

    /**
     * Gets the date/time formatting symbols (this is an object carrying
     * the various strings and other symbols used in formatting: e.g., month
     * names and abbreviations, time zone names, AM/PM strings, etc.)
     * @return a copy of the date-time formatting data associated
     * with this date-time formatter.
     * @stable ICU 2.0
     */
    virtual const DateFormatSymbols* getDateFormatSymbols(void) const;

    /**
     * Set the date/time formatting symbols.  The caller no longer owns the
     * DateFormatSymbols object and should not delete it after making this call.
     * @param newFormatSymbols the given date-time formatting symbols to copy.
     * @stable ICU 2.0
     */
    virtual void adoptDateFormatSymbols(DateFormatSymbols* newFormatSymbols);

    /**
     * Set the date/time formatting data.
     * @param newFormatSymbols the given date-time formatting symbols to copy.
     * @stable ICU 2.0
     */
    virtual void setDateFormatSymbols(const DateFormatSymbols& newFormatSymbols);

    /**
     * Return the class ID for this class. This is useful only for comparing to
     * a return value from getDynamicClassID(). For example:
     * <pre>
     * .   Base* polymorphic_pointer = createPolymorphicObject();
     * .   if (polymorphic_pointer->getDynamicClassID() ==
     * .       erived::getStaticClassID()) ...
     * </pre>
     * @return          The class ID for all objects of this class.
     * @stable ICU 2.0
     */
    static UClassID U_EXPORT2 getStaticClassID(void);

    /**
     * Returns a unique class ID POLYMORPHICALLY. Pure virtual override. This
     * method is to implement a simple version of RTTI, since not all C++
     * compilers support genuine RTTI. Polymorphic operator==() and clone()
     * methods call this method.
     *
     * @return          The class ID for this object. All objects of a
     *                  given class have the same class ID.  Objects of
     *                  other classes have different class IDs.
     * @stable ICU 2.0
     */
    virtual UClassID getDynamicClassID(void) const;

    /**
     * Set the calendar to be used by this date format. Initially, the default
     * calendar for the specified or default locale is used.  The caller should
     * not delete the Calendar object after it is adopted by this call.
     * Adopting a new calendar will change to the default symbols.
     *
     * @param calendarToAdopt    Calendar object to be adopted.
     * @stable ICU 2.0
     */
    virtual void adoptCalendar(Calendar* calendarToAdopt);

    /* Cannot use #ifndef U_HIDE_DRAFT_API for the following draft method since it is virtual */
    /**
     * Set a particular UDisplayContext value in the formatter, such as
     * UDISPCTX_CAPITALIZATION_FOR_STANDALONE.
     * @param value The UDisplayContext value to set.
     * @param status Input/output status. If at entry this indicates a failure
     *               status, the function will do nothing; otherwise this will be
     *               updated with any new status from the function. 
     * @draft ICU 51
     */
    virtual void setContext(UDisplayContext value, UErrorCode& status);

    /* Cannot use #ifndef U_HIDE_DRAFT_API for the following draft method since it is virtual */
    /**
     * Get the formatter's UDisplayContext value for the specified UDisplayContextType,
     * such as UDISPCTX_TYPE_CAPITALIZATION.
     * @param type The UDisplayContextType whose value to return
     * @param status Input/output status. If at entry this indicates a failure
     *               status, the function will do nothing; otherwise this will be
     *               updated with any new status from the function. 
     * @return The UDisplayContextValue for the specified type.
     * @draft ICU 51
     */
    virtual UDisplayContext getContext(UDisplayContextType type, UErrorCode& status) const;

    /* Cannot use #ifndef U_HIDE_INTERNAL_API for the following methods since they are virtual */
    /**
     * Sets the TimeZoneFormat to be used by this date/time formatter.
     * The caller should not delete the TimeZoneFormat object after
     * it is adopted by this call.
     * @param timeZoneFormatToAdopt The TimeZoneFormat object to be adopted.
     * @internal ICU 49 technology preview
     */
    virtual void adoptTimeZoneFormat(TimeZoneFormat* timeZoneFormatToAdopt);

    /**
     * Sets the TimeZoneFormat to be used by this date/time formatter.
     * @param newTimeZoneFormat The TimeZoneFormat object to copy.
     * @internal ICU 49 technology preview
     */
    virtual void setTimeZoneFormat(const TimeZoneFormat& newTimeZoneFormat);

    /**
     * Gets the time zone format object associated with this date/time formatter.
     * @return the time zone format associated with this date/time formatter.
     * @internal ICU 49 technology preview
     */
    virtual const TimeZoneFormat* getTimeZoneFormat(void) const;

#ifndef U_HIDE_INTERNAL_API
    /**
     * This is for ICU internal use only. Please do not use.
     * Check whether the 'field' is smaller than all the fields covered in
     * pattern, return TRUE if it is. The sequence of calendar field,
     * from large to small is: ERA, YEAR, MONTH, DATE, AM_PM, HOUR, MINUTE,...
     * @param field    the calendar field need to check against
     * @return         TRUE if the 'field' is smaller than all the fields
     *                 covered in pattern. FALSE otherwise.
     * @internal ICU 4.0
     */
    UBool isFieldUnitIgnored(UCalendarDateFields field) const;


    /**
     * This is for ICU internal use only. Please do not use.
     * Check whether the 'field' is smaller than all the fields covered in
     * pattern, return TRUE if it is. The sequence of calendar field,
     * from large to small is: ERA, YEAR, MONTH, DATE, AM_PM, HOUR, MINUTE,...
     * @param pattern  the pattern to check against
     * @param field    the calendar field need to check against
     * @return         TRUE if the 'field' is smaller than all the fields
     *                 covered in pattern. FALSE otherwise.
     * @internal ICU 4.0
     */
    static UBool isFieldUnitIgnored(const UnicodeString& pattern,
                                    UCalendarDateFields field);

    /**
     * This is for ICU internal use only. Please do not use.
     * Get the locale of this simple date formatter.
     * It is used in DateIntervalFormat.
     *
     * @return   locale in this simple date formatter
     * @internal ICU 4.0
     */
    const Locale& getSmpFmtLocale(void) const;
#endif  /* U_HIDE_INTERNAL_API */

private:
    friend class DateFormat;

    void initializeDefaultCentury(void);

    SimpleDateFormat(); // default constructor not implemented

    /**
     * Used by the DateFormat factory methods to construct a SimpleDateFormat.
     * @param timeStyle the time style.
     * @param dateStyle the date style.
     * @param locale    the given locale.
     * @param status    Output param set to success/failure code on
     *                  exit.
     */
    SimpleDateFormat(EStyle timeStyle, EStyle dateStyle, const Locale& locale, UErrorCode& status);

    /**
     * Construct a SimpleDateFormat for the given locale.  If no resource data
     * is available, create an object of last resort, using hard-coded strings.
     * This is an internal method, called by DateFormat.  It should never fail.
     * @param locale    the given locale.
     * @param status    Output param set to success/failure code on
     *                  exit.
     */
    SimpleDateFormat(const Locale& locale, UErrorCode& status); // Use default pattern

    /**
     * Hook called by format(... FieldPosition& ...) and format(...FieldPositionIterator&...)
     */
    UnicodeString& _format(Calendar& cal, UnicodeString& appendTo, FieldPositionHandler& handler, UErrorCode& status) const;

    /**
     * Called by format() to format a single field.
     *
     * @param appendTo  Output parameter to receive result.
     *                  Result is appended to existing contents.
     * @param ch        The format character we encountered in the pattern.
     * @param count     Number of characters in the current pattern symbol (e.g.,
     *                  "yyyy" in the pattern would result in a call to this function
     *                  with ch equal to 'y' and count equal to 4)
     * @param capitalizationContext Capitalization context for this date format.
     * @param fieldNum  Zero-based numbering of current field within the overall format.
     * @param handler   Records information about field positions.
     * @param cal       Calendar to use
     * @param status    Receives a status code, which will be U_ZERO_ERROR if the operation
     *                  succeeds.
     */
    void subFormat(UnicodeString &appendTo,
                   UChar ch,
                   int32_t count,
                   UDisplayContext capitalizationContext,
                   int32_t fieldNum,
                   FieldPositionHandler& handler,
                   Calendar& cal,
                   UErrorCode& status) const; // in case of illegal argument

    /**
     * Used by subFormat() to format a numeric value.
     * Appends to toAppendTo a string representation of "value"
     * having a number of digits between "minDigits" and
     * "maxDigits".  Uses the DateFormat's NumberFormat.
     *
     * @param currentNumberFormat 
     * @param appendTo  Output parameter to receive result.
     *                  Formatted number is appended to existing contents.
     * @param value     Value to format.
     * @param minDigits Minimum number of digits the result should have
     * @param maxDigits Maximum number of digits the result should have
     */
    void zeroPaddingNumber(NumberFormat *currentNumberFormat,
                           UnicodeString &appendTo,
                           int32_t value,
                           int32_t minDigits,
                           int32_t maxDigits) const;

    /**
     * Return true if the given format character, occuring count
     * times, represents a numeric field.
     */
    static UBool isNumeric(UChar formatChar, int32_t count);

    /**
     * Returns TRUE if the patternOffset is at the start of a numeric field.
     */
    static UBool isAtNumericField(const UnicodeString &pattern, int32_t patternOffset);

    /**
     * Returns TRUE if the patternOffset is right after a non-numeric field.
     */
    static UBool isAfterNonNumericField(const UnicodeString &pattern, int32_t patternOffset);

    /**
     * initializes fCalendar from parameters.  Returns fCalendar as a convenience.
     * @param adoptZone  Zone to be adopted, or NULL for TimeZone::createDefault().
     * @param locale Locale of the calendar
     * @param status Error code
     * @return the newly constructed fCalendar
     */
    Calendar *initializeCalendar(TimeZone* adoptZone, const Locale& locale, UErrorCode& status);

    /**
     * initializes fSymbols from parameters.
     * @param locale Locale of the symbols
     * @param calendar Alias to Calendar that will be used.
     * @param status Error code
     */
    void initializeSymbols(const Locale& locale, Calendar* calendar, UErrorCode& status);

    /**
     * Called by several of the constructors to load pattern data and formatting symbols
     * out of a resource bundle and initialize the locale based on it.
     * @param timeStyle     The time style, as passed to DateFormat::createDateInstance().
     * @param dateStyle     The date style, as passed to DateFormat::createTimeInstance().
     * @param locale        The locale to load the patterns from.
     * @param status        Filled in with an error code if loading the data from the
     *                      resources fails.
     */
    void construct(EStyle timeStyle, EStyle dateStyle, const Locale& locale, UErrorCode& status);

    /**
     * Called by construct() and the various constructors to set up the SimpleDateFormat's
     * Calendar and NumberFormat objects.
     * @param locale    The locale for which we want a Calendar and a NumberFormat.
     * @param status    Filled in with an error code if creating either subobject fails.
     */
    void initialize(const Locale& locale, UErrorCode& status);

    /**
     * Private code-size reduction function used by subParse.
     * @param text the time text being parsed.
     * @param start where to start parsing.
     * @param field the date field being parsed.
     * @param stringArray the string array to parsed.
     * @param stringArrayCount the size of the array.
     * @param monthPattern pointer to leap month pattern, or NULL if none.
     * @param cal a Calendar set to the date and time to be formatted
     *            into a date/time string.
     * @return the new start position if matching succeeded; a negative number
     * indicating matching failure, otherwise.
     */
    int32_t matchString(const UnicodeString& text, int32_t start, UCalendarDateFields field,
                        const UnicodeString* stringArray, int32_t stringArrayCount,
                        const UnicodeString* monthPattern, Calendar& cal) const;

    /**
     * Private code-size reduction function used by subParse.
     * @param text the time text being parsed.
     * @param start where to start parsing.
     * @param field the date field being parsed.
     * @param stringArray the string array to parsed.
     * @param stringArrayCount the size of the array.
     * @param cal a Calendar set to the date and time to be formatted
     *            into a date/time string.
     * @return the new start position if matching succeeded; a negative number
     * indicating matching failure, otherwise.
     */
    int32_t matchQuarterString(const UnicodeString& text, int32_t start, UCalendarDateFields field,
                               const UnicodeString* stringArray, int32_t stringArrayCount, Calendar& cal) const;
    
    /**
     * Private function used by subParse to match literal pattern text.
     *
     * @param pattern the pattern string
     * @param patternOffset the starting offset into the pattern text. On
     *        outupt will be set the offset of the first non-literal character in the pattern
     * @param text the text being parsed
     * @param textOffset the starting offset into the text. On output
     *                   will be set to the offset of the character after the match
     * @param whitespaceLenient <code>TRUE</code> if whitespace parse is lenient, <code>FALSE</code> otherwise.
     * @param partialMatchLenient <code>TRUE</code> if partial match parse is lenient, <code>FALSE</code> otherwise.
     *
     * @return <code>TRUE</code> if the literal text could be matched, <code>FALSE</code> otherwise.
     */
    static UBool matchLiterals(const UnicodeString &pattern, int32_t &patternOffset,
                               const UnicodeString &text, int32_t &textOffset, 
                               UBool whitespaceLenient, UBool partialMatchLenient);
    
    /**
     * Private member function that converts the parsed date strings into
     * timeFields. Returns -start (for ParsePosition) if failed.
     * @param text the time text to be parsed.
     * @param start where to start parsing.
     * @param ch the pattern character for the date field text to be parsed.
     * @param count the count of a pattern character.
     * @param obeyCount if true then the count is strictly obeyed.
     * @param allowNegative
     * @param ambiguousYear If true then the two-digit year == the default start year.
     * @param saveHebrewMonth Used to hang onto month until year is known.
     * @param cal a Calendar set to the date and time to be formatted
     *            into a date/time string.
     * @param patLoc
     * @param numericLeapMonthFormatter If non-null, used to parse numeric leap months.
     * @return the new start position if matching succeeded; a negative number
     * indicating matching failure, otherwise.
     */
    int32_t subParse(const UnicodeString& text, int32_t& start, UChar ch, int32_t count,
                     UBool obeyCount, UBool allowNegative, UBool ambiguousYear[], int32_t& saveHebrewMonth, Calendar& cal,
                     int32_t patLoc, MessageFormat * numericLeapMonthFormatter) const;

    void parseInt(const UnicodeString& text,
                  Formattable& number,
                  ParsePosition& pos,
                  UBool allowNegative,
                  NumberFormat *fmt) const;

    void parseInt(const UnicodeString& text,
                  Formattable& number,
                  int32_t maxDigits,
                  ParsePosition& pos,
                  UBool allowNegative,
                  NumberFormat *fmt) const;

    int32_t checkIntSuffix(const UnicodeString& text, int32_t start,
                           int32_t patLoc, UBool isNegative) const;

    /**
     * Translate a pattern, mapping each character in the from string to the
     * corresponding character in the to string. Return an error if the original
     * pattern contains an unmapped character, or if a quote is unmatched.
     * Quoted (single quotes only) material is not translated.
     * @param originalPattern   the original pattern.
     * @param translatedPattern Output param to receive the translited pattern.
     * @param from              the characters to be translited from.
     * @param to                the characters to be translited to.
     * @param status            Receives a status code, which will be U_ZERO_ERROR
     *                          if the operation succeeds.
     */
    static void translatePattern(const UnicodeString& originalPattern,
                                UnicodeString& translatedPattern,
                                const UnicodeString& from,
                                const UnicodeString& to,
                                UErrorCode& status);

    /**
     * Sets the starting date of the 100-year window that dates with 2-digit years
     * are considered to fall within.
     * @param startDate the start date
     * @param status    Receives a status code, which will be U_ZERO_ERROR
     *                  if the operation succeeds.
     */
    void         parseAmbiguousDatesAsAfter(UDate startDate, UErrorCode& status);

    /**
     * Return the length matched by the given affix, or -1 if none.
     * Runs of white space in the affix, match runs of white space in
     * the input.
     * @param affix pattern string, taken as a literal
     * @param input input text
     * @param pos offset into input at which to begin matching
     * @return length of input that matches, or -1 if match failure
     */
    int32_t compareSimpleAffix(const UnicodeString& affix,
                   const UnicodeString& input,
                   int32_t pos) const;

    /**
     * Skip over a run of zero or more Pattern_White_Space characters at
     * pos in text.
     */
    int32_t skipPatternWhiteSpace(const UnicodeString& text, int32_t pos) const;

    /**
     * Skip over a run of zero or more isUWhiteSpace() characters at pos
     * in text.
     */
    int32_t skipUWhiteSpace(const UnicodeString& text, int32_t pos) const;

    /**
     * Initialize NumberFormat instances used for numbering system overrides.
     */
    void initNumberFormatters(const Locale &locale,UErrorCode &status);

    /**
     * Get the numbering system to be used for a particular field.
     */
     NumberFormat * getNumberFormatByIndex(UDateFormatField index) const;

    /**
     * Parse the given override string and set up structures for number formats
     */
    void processOverrideString(const Locale &locale, const UnicodeString &str, int8_t type, UErrorCode &status);

    /**
     * Used to map pattern characters to Calendar field identifiers.
     */
    static const UCalendarDateFields fgPatternIndexToCalendarField[];

    /**
     * Map index into pattern character string to DateFormat field number
     */
    static const UDateFormatField fgPatternIndexToDateFormatField[];

    /**
     * Lazy TimeZoneFormat instantiation, semantically const
     */
    TimeZoneFormat *tzFormat() const;

    /**
     * Used to map Calendar field to field level.
     * The larger the level, the smaller the field unit.
     * For example, UCAL_ERA level is 0, UCAL_YEAR level is 10,
     * UCAL_MONTH level is 20.
     */
    static const int32_t fgCalendarFieldToLevel[];
    static const int32_t fgPatternCharToLevel[];

    /**
     * The formatting pattern for this formatter.
     */
    UnicodeString       fPattern;

    /**
     * The numbering system override for dates.
     */
    UnicodeString       fDateOverride;

    /**
     * The numbering system override for times.
     */
    UnicodeString       fTimeOverride;


    /**
     * The original locale used (for reloading symbols)
     */
    Locale              fLocale;

    /**
     * A pointer to an object containing the strings to use in formatting (e.g.,
     * month and day names, AM and PM strings, time zone names, etc.)
     */
    DateFormatSymbols*  fSymbols;   // Owned

    /**
     * The time zone formatter
     */
    TimeZoneFormat* fTimeZoneFormat;

    /**
     * If dates have ambiguous years, we map them into the century starting
     * at defaultCenturyStart, which may be any date.  If defaultCenturyStart is
     * set to SYSTEM_DEFAULT_CENTURY, which it is by default, then the system
     * values are used.  The instance values defaultCenturyStart and
     * defaultCenturyStartYear are only used if explicitly set by the user
     * through the API method parseAmbiguousDatesAsAfter().
     */
    UDate                fDefaultCenturyStart;

    /**
     * See documentation for defaultCenturyStart.
     */
    /*transient*/ int32_t   fDefaultCenturyStartYear;

    int32_t tztype; // here to avoid api change

    typedef struct NSOverride {
        NumberFormat *nf;
        int32_t hash;
        NSOverride *next;
    } NSOverride;

    NumberFormat    **fNumberFormatters;

    NSOverride      *fOverrideList;

    UBool fHaveDefaultCentury;

    UDisplayContext fCapitalizationContext;
};

inline UDate
SimpleDateFormat::get2DigitYearStart(UErrorCode& /*status*/) const
{
    return fDefaultCenturyStart;
}

U_NAMESPACE_END

#endif /* #if !UCONFIG_NO_FORMATTING */

#endif // _SMPDTFMT
//eof
