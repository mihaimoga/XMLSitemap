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

#pragma once

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#ifndef WINVER					// Specifies that the minimum required platform is Windows XP.
#define WINVER 0x0501			// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT			// Specifies that the minimum required platform is Windows XP.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS			// Specifies that the minimum required platform is Windows XP.
#define _WIN32_WINDOWS 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_IE				// Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700		// Change this to the appropriate value to target other versions of IE.
#endif
