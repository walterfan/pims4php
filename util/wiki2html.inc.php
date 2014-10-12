<?php
 /**
  * Project: HesaSys easy Site Administration System
  *
  * PHP Version 4 and 5
  *
  * Copyright (C) 2005-2006 HesaSys Team
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 2
  * of the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
  *
  * @package    HesaSys
  * @subpackage Core
  * @author     HesaSys Team
  * @copyright  2005-2006 HesaSys Team
  * @license    http://www.fsf.org/licensing/licenses/gpl.txt GNU GPL Version 2
  * @version    SVN: $Id$
  * @link       http://hesasys.org/
  */

  /**
   * Parses html2wiki or wiki2html
   */
  class WikiParser {

    /**
     * Parses wiki to html
     *
     * <b>Formatting:</b>
     * - **foo** -> <strong>foo</strong>
     * - //foo// -> <em>foo</em>
     * - __foo__ -> <u>foo</u>
     * - ''foo'' -> <<code>>foo<</code>>
     * - foo\n   -> foo<<br />>
     * - \nfoo\n -> <p>foo</p>
     *
     * <b>Links:</b>
     * - [[http://foo.tld]]     -> <a href="http://foo.tld">http://foo.tld</a>
     * - [[http://foo.tld|foo]] -> <a href="http://foo.tld">foo</a>
     * - [[foo]]       -> <a href="{cms:link foo}">foo</a>
     * - [[foo|bar]]   -> <a href="{cms:link foo}">bar</a>
     *
     * <b>Images:</b>
     * - {{http://foo.ext}}       -> <img src="http://foo.ext" />
     * - {{http://foo.ext|12}}    -> <img src="http://foo.ext" width="12" />
     * - {{http://foo.ext|12x30}} -> <img src="http://foo.ext" width="12" height="30" />
     * - {{foo.ext}}       -> <img src="{cms:absoluteuri}foo.ext" />
     * - {{foo.ext|12}}    -> <img src="{cms:absoluteuri}foo.ext" width="12" />
     * - {{foo.ext|12x30}} -> <img src="{cms:absoluteuri}foo.ext" width="12" height="30" />
     *
     * Generally: Add an additional "|str" before every image tag's closing brackets to define
     * the html 'alt'-parameter.
     * For instance: {{http://foo.ext|foo}} -> <img src="http://foo.ext" alt="foo" />
     * PLEASE NOTE: For some limitations the alt text can NOT begin with a decimal!
     *
     * <b>HTML:</b>
     * - <html>foo</html>
     *
     * <b>Reverse (do not parse):</b>
     * - <noparse>foo</noparse>
     *
     * @param string $str The String to parse
     *
     * @uses WikiParser::hte()
     * @uses WikiParser::reverse()
     *
     * @return string
     */
    function wiki2html($str) {
      $str = str_replace("\r\n", "\n", $str);

      // Basic formatting
      $str = preg_replace('/\*\*(.*)\*\*/iU', '<strong>\\1</strong>', $str);
      $str = preg_replace('#(?<!:)//(.+)//#iU', '<em>\\1</em>', $str);
      $str = preg_replace('/__(.*)__/iU', '<u>\\1</u>', $str);
      $str = preg_replace("/''(.*)''/iU", '<code>\\1</code>', $str);

      // Newline and paragraphs
      $str = preg_replace('/^(.+)(\n{2}|$)/iU', "<p>\\1</p>\n", $str);
      $str = preg_replace('/^\n(.+)(\n{2}|$)/imU', "<p>\\1</p>\n", $str);
      //$str = preg_replace('/(?<!\\\\|\n<\/p>|<\/p>)\n/iU', "<br />\n", $str);


      // Links
      $str = preg_replace('/\[\[(.+:\/\/.+)\|(.+)\]\]/imU', '<a href="\\1">\\2</a>', $str);
      $str = preg_replace('/\[\[(.+:\/\/.+)\]\]/imU', '<a href="\\1">\\1</a>', $str);

      $str = preg_replace('/\[\[(?!.\:\/\/)(.+)\|(.+)\]\]/iU', '<a href="'.HS_CFG_MARKER_LEFT.'cms:link \\1'.HS_CFG_MARKER_RIGHT.'">\\2</a>', $str);
      $str = preg_replace('/\[\[(?!.\:\/\/)(.+)\]\]/iU', '<a href="'.HS_CFG_MARKER_LEFT.'cms:link \\1'.HS_CFG_MARKER_RIGHT.'">\\1</a>', $str);

      // Images
      $str = preg_replace('/\{\{(.+:\/\/.+)\|(\d+)(?:\|(.+))?\}\}/iU', '<img src="\\1" width="\\2" alt="\\3" />', $str);
      $str = preg_replace('/\{\{(.+:\/\/.+)\|(\d+)x(\d+)(?:\|(.+))?\}\}/iU', '<img src="\\1" width="\\2" height="\\3" alt="\\4" />', $str);
      $str = preg_replace('/\{\{(.+:\/\/.+)(?:\|(\D.+))?\}\}/iU', '<img src="\\1" alt="\\2" />', $str);

      $str = preg_replace('/\{\{(?!.\:\/\/)(.+)\|(\d+)(?:\|(.+))?\}\}/iU', '<img src="'.HS_CFG_ABSOLUTEURI.'\\1" width="\\2" alt="\\3" />', $str);
      $str = preg_replace('/\{\{(?!.\:\/\/)(.+)\|(\d+)x(\d+)(?:\|(.+))?\}\}/iU', '<img src="'.HS_CFG_ABSOLUTEURI.'\\1" width="\\2" height="\\3" alt="\\4" />', $str);
      $str = preg_replace('/\{\{(?!.\:\/\/)(.+)(?:\|(\D.+))?\}\}/iU', '<img src="'.HS_CFG_ABSOLUTEURI.'\\1" alt="\\2" />', $str);

      // HTML
      $str = preg_replace_callback('/<html>(.+)<\/html>/imsU', array('WikiParser', 'hte'), $str);

      // noparse
      $str = preg_replace_callback('/<noparse>(.+)<\/noparse>/ims', array('WikiParser', 'reverse'), $str);

      // Cleanup <noparse>-Tags
      $str = str_replace('<noparse>', '', $str);
      $str = str_replace('</noparse>', '', $str);

      return $str;
    }

    /**
     * Opposite of the wiki2html method.
     *
     * @see WikiParser::wiki2html()
     * @param string $str String to parse (partly hte'd! see be_core_page)
     * @return string
     */
    function html2wiki($str) {
      $str = str_replace("\r\n", "\n", $str);

      // Basic formatting
      $str = preg_replace('/<strong>(.*)<\/strong>/iU', '**\\1**', $str);
      $str = preg_replace('/<em>(.*)<\/em>/iU', '//\\1//', $str);
      $str = preg_replace('/<u>(.*)<\/u>/iU', '__\\1__', $str);
      $str = preg_replace("/<code>(.*)<\/code>/iU", "''\\1''", $str);

      // Newline and paragraphs
      //  $str = preg_replace('/<br \/>/iU', '\\\\ ', $str);
      $str = preg_replace('/<p>(.+)<\/p>\n?/isU', "\n\\1\n", $str);

      // Links
      $str = preg_replace('/<a href="(.+\:\/\/.+)">(\\1)<\/a>/iU', '[[\\1]]', $str);
      $str = preg_replace('/<a href="(.+\:\/\/.+)">(.+)<\/a>/iU', '[[\\1|\\2]]', $str);

      $str = preg_replace('/<a href="'.preg_quote(HS_CFG_MARKER_LEFT, '/').'cms:link (.+)'.preg_quote(HS_CFG_MARKER_RIGHT, '/').'">\\1<\/a>/iU', '[[\\1]]', $str);
      $str = preg_replace('/<a href="'.preg_quote(HS_CFG_MARKER_LEFT, '/').'cms:link (.+)'.preg_quote(HS_CFG_MARKER_RIGHT, '/').'">(.+)<\/a>/iU', '[[\\1|\\2]]', $str);

      // Images
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" width="(\d+)" alt="(.+)" />#iU', '{{\\1|\\2||\\3}}',$str);
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" width="(\d+)" />#iU', '{{\\1|\\2}}',$str);
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" width="(\d+)" height="(\d+)" alt="(.+)" />#iU', '{{\\1|\\2x\\3|\\4}}',$str);
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" width="(\d+)" height="(\d+)" />#iU', '{{\\1|\\2x\\3}}',$str);
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" alt="(.+)" />#iU', '{{\\1|\\2}}',$str);
      $str = preg_replace('#<img src="'.preg_quote(HS_CFG_ABSOLUTEURI, '#').'(.+)" />#iU', '{{\\1}}',$str);

      $str = preg_replace('/<img src="(.+)" width="(\d+)" alt="(.+)" \/>/iU', '{{\\1|\\2|\\3}}', $str);
      $str = preg_replace('/<img src="(.+)" width="(\d+)" \/>/iU', '{{\\1|\\2}}', $str);
      $str = preg_replace('/<img src="(.+)" width="(\d+)" height="(\d+)" alt="(.+)" \/>/iU', '{{\\1|\\2x\\3|\\4}}', $str);
      $str = preg_replace('/<img src="(.+)" width="(\d+)" height="(\d+)" \/>/iU', '{{\\1|\\2x\\3}}', $str);
      $str = preg_replace('/<img src="(.+)" alt="(.+)" \/>/iU', '{{\\1|\\2}}',$str);
      $str = preg_replace('/<img src="(.+)" \/>/iU', '{{\\1}}',$str);


      return $str;
    }

    /**
     * Alias for WikiParser::html2wiki with reverse = true
     *
     * @uses WikiParses::html2wiki()
     * @param string $str String to parse
     * @return string
     */
    function reverse($str) {
      return WikiParser::html2wiki($str[1]);
    }

    /**
     * Returns a html-escaped (htmlspecialchars) string
     *
     * @param string $str The string
     *
     * @return string
     */
    function hte($str) {
      return hte($str[1]);
    }
  }


?>

