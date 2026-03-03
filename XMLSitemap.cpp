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

// XMLSitemap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XMLSitemap.h"
#include "XMLSitemapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @class CXMLSitemapApp
 * @brief Main application class for XML Sitemap application
 * 
 * This class represents the main application object and handles
 * application-level initialization and message routing.
 */

BEGIN_MESSAGE_MAP(CXMLSitemapApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/**
 * @brief Constructor for the CXMLSitemapApp class
 * 
 * Initializes the application object and enables HTML help support.
 */
CXMLSitemapApp::CXMLSitemapApp()
{
	EnableHtmlHelp();

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/**
 * @brief The one and only CXMLSitemapApp object
 * 
 * Global application instance that represents the running application.
 */
CXMLSitemapApp theApp;

/**
 * @brief Initializes the application instance
 * 
 * This method is called when the application starts. It performs the following tasks:
 * - Initializes common controls for Windows XP and later
 * - Initializes Windows Sockets
 * - Enables control container support
 * - Sets the registry key for application settings
 * - Configures application paths (executable and help file)
 * - Creates and displays the main dialog
 * 
 * @return FALSE to exit the application after the dialog closes, 
 *         as this is a dialog-based application
 */
BOOL CXMLSitemapApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// Call base class initialization
	CWinAppEx::InitInstance();

	// Initialize Windows Sockets support
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Enable ActiveX control container support
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	// Set the registry key for storing application settings
	SetRegistryKey(_T("Mihai Moga"));

	// Configure the executable name path
	// Delete the default executable name and allocate new buffer
	delete theApp.m_pszExeName;
	theApp.m_pszExeName = new TCHAR[_MAX_PATH];

	// Optional: Configure profile (INI file) path
	// delete theApp.m_pszProfileName;
	// theApp.m_pszProfileName = new TCHAR[_MAX_PATH];

	// Path components for file path manipulation
	TCHAR lpszDrive[_MAX_DRIVE];        // Drive letter (e.g., "C:")
	TCHAR lpszDirectory[_MAX_DIR];      // Directory path (e.g., "\Program Files\")
	TCHAR lpszFilename[_MAX_FNAME];     // File name without extension
	TCHAR lpszExtension[_MAX_EXT];      // File extension (e.g., ".exe")

	// Get pointers to application path strings
	LPTSTR lpszHelpPath = (LPTSTR) theApp.m_pszHelpFilePath;
	LPTSTR lpszExePath = (LPTSTR) theApp.m_pszExeName;
	// LPTSTR lpszIniPath = (LPTSTR) theApp.m_pszProfileName;

	// Split the help file path into components
	VERIFY(0 == _tsplitpath_s(lpszHelpPath, lpszDrive, _MAX_DRIVE, lpszDirectory, _MAX_DIR, lpszFilename, _MAX_FNAME, lpszExtension, _MAX_EXT));
	// Construct the executable path using the same drive, directory, and filename
	VERIFY(0 == _tmakepath_s(lpszExePath, _MAX_PATH, lpszDrive, lpszDirectory, lpszFilename, _T(".exe")));
	// Optional: Construct the INI file path
	// VERIFY(0 == _tmakepath_s(lpszIniPath, _MAX_PATH, lpszDrive, lpszDirectory, lpszFilename, _T(".ini")));

	// Create the main application dialog
	CXMLSitemapDlg dlgXMLSitemap;
	// Set as the main window of the application
	m_pMainWnd = &dlgXMLSitemap;
	// Display the dialog modally and get the result
	INT_PTR nResponse = dlgXMLSitemap.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
