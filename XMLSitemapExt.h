/* Copyright (C) 2012-2025 Stefan-Mihai MOGA
This file is part of XML Sitemap application developed by Stefan-Mihai MOGA.

XML Sitemap is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

XML Sitemap is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
XML Sitemap. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// XMLSitemapExt.h : header file
//

#pragma once

#include <boost/regex.hpp>

// Maximum string and URL buffer lengths
#define MAX_STR_LENGTH 0x1000
#define MAX_URL_LENGTH 0x1000

// Regular expressions for absolute and relative URL validation
#define ABSOLUTE_URL_REGEX _T("^(http[s]?)(:\\/\\/)(([\\w]+:)?//)?(([\\d\\w]|%[a-fA-f\\d]{2,2})+(:([\\d\\w]|%[a-fA-f\\d]{2,2})+)?@)?([\\d\\w][-\\d\\w]{0,253}[\\d\\w]\\.)+[\\w]{2,4}(:[\\d]+)?(/([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)*(\\?(&?([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})=?)*)?(#([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)?$")
#define RELATIVE_URL_REGEX _T("^([\\/]?[\\.\\.\\/]+)?(([\\d\\w]|%[a-fA-f\\d]{2,2})+(:([\\d\\w]|%[a-fA-f\\d]{2,2})+)?@)?([\\d\\w][-\\d\\w]{0,253}[\\d\\w]\\.)+[\\w]{2,4}(:[\\d]+)?(/([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)*(\\?(&?([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})=?)*)?(#([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)?$")

/**
 * @brief Determines if a given URL points to an HTML page.
 * @param lpszURL The URL string to check.
 * @return TRUE if the URL is likely an HTML page, FALSE otherwise.
 */
BOOL IsHtmlPage(LPCTSTR lpszURL);

/**
 * @brief Validates if a string is a well-formed URL.
 * @param lpszURL The URL string to validate.
 * @param bMatchPartialURL If TRUE, allows relative URLs; otherwise, only absolute URLs are valid.
 * @return TRUE if the URL is valid, FALSE otherwise.
 */
BOOL IsValidURL(LPCTSTR lpszURL, BOOL bMatchPartialURL);

/**
 * @brief Checks if two URLs belong to the same domain.
 * @param lpszAbsoluteURL The absolute URL to check.
 * @param lpszBaseURL The base URL to compare against.
 * @return TRUE if both URLs share the same domain, FALSE otherwise.
 */
BOOL IsLocalURL(LPCTSTR lpszAbsoluteURL, LPCTSTR lpszBaseURL);

/**
 * @brief Converts a relative URL to an absolute URL using a base URL.
 * @param lpszRelativeURL The relative URL.
 * @param lpszBaseURL The base URL.
 * @return The absolute URL as a CString, or NULL if conversion fails.
 */
CString ConvertURL(LPCTSTR lpszRelativeURL, LPCTSTR lpszBaseURL);

/**
 * @brief Exports the collected URLs as an XML sitemap file and opens it.
 * @param hWnd Handle to the parent window.
 * @param strFileName Output file name.
 * @param strFrequency Change frequency value for sitemap.
 * @param strPriority Priority value for sitemap.
 * @param strDateTime Last modification date for sitemap.
 * @return TRUE if export succeeds, FALSE otherwise.
 */
BOOL ExportXMLSitemap(HWND hWnd, CString strFileName, CString strFrequency, CString strPriority, CString strDateTime);

/**
 * @brief Exports a report of broken links as an HTML file and opens it.
 * @param hWnd Handle to the parent window.
 * @param strFileName Output file name.
 * @param strDomainName The domain name for the report title.
 * @return TRUE if export succeeds, FALSE otherwise.
 */
BOOL ExportBrokenLink(HWND hWnd, CString strFileName, CString strDomainName);

/**
 * @brief Thread procedure for generating the XML sitemap.
 *        Downloads the root page, processes it, and recursively crawls local links.
 * @param lpParam Pointer to the CXMLSitemapDlg dialog.
 * @return Always returns 0.
 */
DWORD WINAPI XMLSitemap_ThreadProc(LPVOID lpParam);
