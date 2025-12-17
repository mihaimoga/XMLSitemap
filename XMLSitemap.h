/* Copyright (C) 2012-2026 Stefan-Mihai MOGA
This file is part of XML Sitemap application developed by Stefan-Mihai MOGA.

XML Sitemap is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

XML Sitemap is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
XML Sitemap. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// XMLSitemap.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CXMLSitemapApp:
// See XMLSitemap.cpp for the implementation of this class
//

class CXMLSitemapApp : public CWinAppEx
{
public:
	CXMLSitemapApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CXMLSitemapApp theApp;