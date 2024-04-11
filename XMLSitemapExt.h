/* Copyright (C) 2012-2024 Stefan-Mihai MOGA
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

#define MAX_STR_LENGTH 0x1000
#define MAX_URL_LENGTH 0x1000
#define ABSOLUTE_URL_REGEX _T("^(http[s]?)(:\\/\\/)(([\\w]+:)?//)?(([\\d\\w]|%[a-fA-f\\d]{2,2})+(:([\\d\\w]|%[a-fA-f\\d]{2,2})+)?@)?([\\d\\w][-\\d\\w]{0,253}[\\d\\w]\\.)+[\\w]{2,4}(:[\\d]+)?(/([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)*(\\?(&?([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})=?)*)?(#([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)?$")
#define RELATIVE_URL_REGEX _T("^([\\/]?[\\.\\.\\/]+)?(([\\d\\w]|%[a-fA-f\\d]{2,2})+(:([\\d\\w]|%[a-fA-f\\d]{2,2})+)?@)?([\\d\\w][-\\d\\w]{0,253}[\\d\\w]\\.)+[\\w]{2,4}(:[\\d]+)?(/([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)*(\\?(&?([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})=?)*)?(#([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)?$")

BOOL IsHtmlPage(LPCTSTR lpszURL);
BOOL IsValidURL(LPCTSTR lpszURL, BOOL bMatchPartialURL);
BOOL IsLocalURL(LPCTSTR lpszAbsoluteURL, LPCTSTR lpszBaseURL);
CString ConvertURL(LPCTSTR lpszRelativeURL, LPCTSTR lpszBaseURL);
BOOL ExportXMLSitemap(HWND hWnd, CString strFileName, CString strFrequency, CString strPriority, CString strDateTime);
BOOL ExportBrokenLink(HWND hWnd, CString strFileName, CString strDomainName);
DWORD WINAPI XMLSitemap_ThreadProc(LPVOID lpParam);
