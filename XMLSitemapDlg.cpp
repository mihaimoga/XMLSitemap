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

// XMLSitemapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMLSitemap.h"
#include "XMLSitemapDlg.h"
#include "XMLSitemapExt.h"

#include "VersionInfo.h"
#include "HLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
///////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

protected:
	CStatic m_ctrlVersion;
	CEdit m_ctrlWarning;
	CVersionInfo m_pVersionInfo;
	CHLinkCtrl m_ctrlWebsite;
	CHLinkCtrl m_ctrlEmail;

	DECLARE_MESSAGE_MAP()
};

/**
 * @brief Constructor for the CAboutDlg class
 * 
 * Initializes the About dialog with the specified dialog resource.
 */
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

/**
 * @brief Data exchange mechanism for dialog controls
 * 
 * Maps dialog controls to member variables using DDX (Dialog Data Exchange).
 * Establishes connections between UI controls and their corresponding member variables.
 * 
 * @param pDX Pointer to CDataExchange object that manages data transfer
 */
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION, m_ctrlVersion);
	DDX_Control(pDX, IDC_WARNING, m_ctrlWarning);
	DDX_Control(pDX, IDC_WEBSITE, m_ctrlWebsite);
	DDX_Control(pDX, IDC_EMAIL, m_ctrlEmail);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**
 * @brief Retrieves the full path of the current module (executable)
 * 
 * This function safely retrieves the module file name by dynamically
 * allocating buffer space until the full path fits. It handles cases
 * where the initial buffer size is insufficient by doubling the buffer
 * size and retrying.
 * 
 * @param pdwLastError Optional pointer to receive the last error code
 *                     (ERROR_SUCCESS on success, or error code on failure)
 * @return CString containing the full module path, or empty string on failure
 */
CString GetModuleFileName(_Inout_opt_ DWORD* pdwLastError = nullptr)
{
	CString strModuleFileName;
	DWORD dwSize{ _MAX_PATH };  // Start with standard MAX_PATH size (260 characters)

	while (true)
	{
		// Allocate buffer for the module file name
		TCHAR* pszModuleFileName{ strModuleFileName.GetBuffer(dwSize) };

		// Call Windows API to get the module file name
		const DWORD dwResult{ ::GetModuleFileName(nullptr, pszModuleFileName, dwSize) };

		if (dwResult == 0)
		{
			// Failed to get module file name - set error code
			if (pdwLastError != nullptr)
				*pdwLastError = GetLastError();
			strModuleFileName.ReleaseBuffer(0);
			return CString{};
		}
		else if (dwResult < dwSize)
		{
			// Success - buffer was large enough to hold the entire path
			if (pdwLastError != nullptr)
				*pdwLastError = ERROR_SUCCESS;
			strModuleFileName.ReleaseBuffer(dwResult);
			return strModuleFileName;
		}
		else if (dwResult == dwSize)
		{
			// Buffer too small - the path was truncated
			// Double the buffer size and retry
			strModuleFileName.ReleaseBuffer(0);
			dwSize *= 2;
		}
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Get the full path of the current executable
	CString strFullPath{ GetModuleFileName() };
	if (strFullPath.IsEmpty())
#pragma warning(suppress: 26487)
		return FALSE;

	// Load and display version information from the executable
	if (m_pVersionInfo.Load(strFullPath.GetString()))
	{
		// Extract product name and version from version resource
		CString strName = m_pVersionInfo.GetProductName().c_str();
		CString strVersion = m_pVersionInfo.GetProductVersionAsString().c_str();
		// Format version string: remove spaces and replace commas with dots
		strVersion.Replace(_T(" "), _T(""));
		strVersion.Replace(_T(","), _T("."));
		// Truncate to major.minor version (e.g., "1.0" instead of "1.0.0.0")
		const int nFirst = strVersion.Find(_T('.'));
		const int nSecond = strVersion.Find(_T('.'), nFirst + 1);
		strVersion.Truncate(nSecond);
#if _WIN32 || _WIN64
#if _WIN64
		// Display 64-bit platform indicator
		m_ctrlVersion.SetWindowText(strName + _T(" version ") + strVersion + _T(" (64-bit)"));
#else
		// Display 32-bit platform indicator
		m_ctrlVersion.SetWindowText(strName + _T(" version ") + strVersion + _T(" (32-bit)"));
#endif
#endif
	}

	m_ctrlWarning.SetWindowText(_T("This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>."));

	m_ctrlWebsite.SetHyperLink(_T("https://www.moga.doctor/"));
	m_ctrlEmail.SetHyperLink(_T("mailto:stefan-mihai@moga.doctor"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// CXMLSitemapDlg dialog
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Constructor for the main XML Sitemap dialog
 * 
 * Initializes the dialog, loads the application icon, and sets default values.
 * 
 * @param pParent Pointer to the parent window (default is NULL)
 */
CXMLSitemapDlg::CXMLSitemapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMLSitemapDlg::IDD, pParent)
{
	m_hIcon = theApp.LoadIcon(IDR_MAINFRAME);
	m_bGenerate = FALSE;
}

void CXMLSitemapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOMAIN, m_ctrlDomainName);
	DDX_Control(pDX, IDC_FREQUENCY, m_ctrlFrequency);
	DDX_Control(pDX, IDC_PRIORITY, m_ctrlPriority);
	DDX_Control(pDX, IDC_MODIFIED, m_ctrlModified);
	DDX_Control(pDX, IDC_STATUS, m_ctrlStatus);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_OUTPUT, m_ctrlFilename);
	DDX_Control(pDX, IDC_GENERATE, m_ctrlGenerate);
	DDX_Control(pDX, IDC_SELECT, m_ctrlSelect);
	DDX_Control(pDX, IDCANCEL, m_ctrlCancel);
}

BEGIN_MESSAGE_MAP(CXMLSitemapDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_DOMAIN, &CXMLSitemapDlg::OnEnChangeDomain)
	ON_CBN_SELCHANGE(IDC_FREQUENCY, &CXMLSitemapDlg::OnCbnSelchangeFrequency)
	ON_CBN_SELCHANGE(IDC_PRIORITY, &CXMLSitemapDlg::OnCbnSelchangePriority)
	ON_BN_CLICKED(IDC_SELECT, &CXMLSitemapDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_GENERATE, &CXMLSitemapDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDCANCEL, &CXMLSitemapDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CXMLSitemapDlg message handlers
///////////////////////////////////////////////////////////////////////////////

BOOL CXMLSitemapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlDomainName.SetAllowEmpty(false);
	m_ctrlDomainName.SetRegEx(ABSOLUTE_URL_REGEX);

	m_ctrlFilename.SetWindowText(_T("Please specify XML output filename!"));
	m_strDomainName = theApp.GetProfileString(SETTINGS_SECTION, LAST_DOMAIN_NAME, _T("https://www.moga.doctor/"));
	m_strOutputFile = theApp.GetProfileString(SETTINGS_SECTION, OUTPUT_FILENAME, NULL);
	if (!m_strDomainName.IsEmpty()) m_ctrlDomainName.SetWindowText(m_strDomainName);
	if (!m_strOutputFile.IsEmpty()) m_ctrlFilename.SetWindowText(m_strOutputFile);
	m_ctrlGenerate.EnableWindow(!m_strDomainName.IsEmpty() && !m_strOutputFile.IsEmpty());

	// Populate frequency combo box with standard sitemap change frequencies
	m_ctrlFrequency.AddString(_T("always"));
	m_ctrlFrequency.AddString(_T("hourly"));
	m_ctrlFrequency.AddString(_T("daily"));
	m_ctrlFrequency.AddString(_T("weekly"));
	m_ctrlFrequency.AddString(_T("monthly"));
	m_ctrlFrequency.AddString(_T("yearly"));
	m_ctrlFrequency.AddString(_T("never"));
	// Load and set previously selected frequency (default: weekly)
	const UINT nFrequency = theApp.GetProfileInt(SETTINGS_SECTION, LAST_FREQUENCY, 3);
	m_ctrlFrequency.SetCurSel(nFrequency);

	// Populate priority combo box with values from 0.0 to 1.0
	m_ctrlPriority.AddString(_T("0.0"));
	m_ctrlPriority.AddString(_T("0.1"));
	m_ctrlPriority.AddString(_T("0.2"));
	m_ctrlPriority.AddString(_T("0.3"));
	m_ctrlPriority.AddString(_T("0.4"));
	m_ctrlPriority.AddString(_T("0.5"));
	m_ctrlPriority.AddString(_T("0.6"));
	m_ctrlPriority.AddString(_T("0.7"));
	m_ctrlPriority.AddString(_T("0.8"));
	m_ctrlPriority.AddString(_T("0.9"));
	m_ctrlPriority.AddString(_T("1.0"));
	// Load and set previously selected priority (default: 0.5)
	const UINT nPriority = theApp.GetProfileInt(SETTINGS_SECTION, LAST_PRIORITY, 5);
	m_ctrlPriority.SetCurSel(nPriority);

	// Set an inspirational initial status message
	m_ctrlStatus.SetWindowText(_T("We are always getting ready to live but never living... (Ralph Waldo Emerson)"));

	CRect rectXMLSitemap;
	GetClientRect(&rectXMLSitemap);
	TRACE(_T("[CXMLSitemapDlg] Width = %d, Height = %d\n"),
		rectXMLSitemap.Width(), rectXMLSitemap.Height());

#ifdef _DEBUG
	ASSERT(IsValidURL(_T("/index.html"), TRUE));
	ASSERT(IsValidURL(_T("../index.html"), TRUE));
	ASSERT(IsValidURL(_T("/../index.html"), TRUE));
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMLSitemapDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CXMLSitemapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// Check if this is the About menu item
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		// Display the About dialog modally
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		// Not the About command - pass to base class for default handling
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXMLSitemapDlg::OnPaint()
{
	if (IsIconic())
	{
		// Window is minimized - draw the application icon
		CPaintDC dc(this); // device context for painting

		// Erase the icon background
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Get icon dimensions from system metrics
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		// Get client rectangle to calculate center position
		CRect rect;
		GetClientRect(&rect);

		// Calculate centered position for the icon
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon centered in the minimized window
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// Window is not minimized - use default painting
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXMLSitemapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXMLSitemapDlg::OnEnChangeDomain()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	m_ctrlDomainName.GetWindowText(m_strDomainName);
	m_ctrlGenerate.EnableWindow(!m_strDomainName.IsEmpty() && !m_strOutputFile.IsEmpty());
	theApp.WriteProfileString(SETTINGS_SECTION, LAST_DOMAIN_NAME, m_strDomainName);
}

void CXMLSitemapDlg::OnCbnSelchangeFrequency()
{
	const UINT nFrequency = m_ctrlFrequency.GetCurSel();
	theApp.WriteProfileInt(SETTINGS_SECTION, LAST_FREQUENCY, nFrequency);
}

void CXMLSitemapDlg::OnCbnSelchangePriority()
{
	const UINT nPriority = m_ctrlPriority.GetCurSel();
	theApp.WriteProfileInt(SETTINGS_SECTION, LAST_PRIORITY, nPriority);
}

void CXMLSitemapDlg::OnBnClickedSelect()
{
	DWORD dwFlags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_LONGNAMES;
	LPCTSTR lpszFilter = _T("XML Documents (.xml)|*.xml\0");
	CFileDialog pFileDialog(FALSE, _T(".xml"), _T("sitemap"), dwFlags, lpszFilter, this);
	if (pFileDialog.DoModal() == IDOK)
	{
		m_strOutputFile = pFileDialog.GetPathName();
		m_ctrlFilename.SetWindowText(m_strOutputFile);
		m_ctrlGenerate.EnableWindow(!m_strDomainName.IsEmpty() && !m_strOutputFile.IsEmpty());
		theApp.WriteProfileString(SETTINGS_SECTION, OUTPUT_FILENAME, m_strOutputFile);
	}
}

BOOL WaitWithMessageLoop(HANDLE hEvent, DWORD dwTimeout)
{
	DWORD dwRet;
	MSG msg;
	hEvent = hEvent ? hEvent : CreateEvent(NULL, FALSE, FALSE, NULL); 

	while (true)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwTimeout, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
			return TRUE;
		if (dwRet != WAIT_OBJECT_0 + 1)
			break;
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (hEvent && (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0))
				return TRUE;
		}
	}
	return FALSE;
}

void CXMLSitemapDlg::OnBnClickedGenerate()
{
	CString strFrequency;
	CString strPriority;
	CString strDateTime;
	COleDateTime pDateTime;

	m_ctrlFrequency.GetWindowText(strFrequency);
	m_ctrlPriority.GetWindowText(strPriority);
	VERIFY(m_ctrlModified.GetTime(pDateTime));
	strDateTime = pDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	m_ctrlProgress.ModifyStyle(0, PBS_MARQUEE);
	m_ctrlProgress.SetMarquee(TRUE, 30);
	m_ctrlProgress.EnableWindow(TRUE);
	m_ctrlProgress.UpdateWindow();
	m_ctrlFrequency.EnableWindow(FALSE);
	m_ctrlFrequency.UpdateWindow();
	m_ctrlPriority.EnableWindow(FALSE);
	m_ctrlPriority.UpdateWindow();
	m_ctrlModified.EnableWindow(FALSE);
	m_ctrlModified.UpdateWindow();
	m_ctrlGenerate.EnableWindow(FALSE);
	m_ctrlGenerate.UpdateWindow();
	m_ctrlSelect.EnableWindow(FALSE);
	m_ctrlSelect.UpdateWindow();
	m_ctrlCancel.SetWindowText(_T("Cancel"));
	m_ctrlCancel.UpdateWindow();

	m_bGenerate = TRUE;

	// try and import all files
	DWORD threadID = 0;
	HANDLE hThread = ::CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)XMLSitemap_ThreadProc,
		this,
		0 ,
		&threadID);

	VERIFY(WaitWithMessageLoop(hThread, INFINITE));

	m_bGenerate = FALSE;

	m_ctrlCancel.SetWindowText(_T("Exit"));
	m_ctrlCancel.UpdateWindow();
	m_ctrlSelect.EnableWindow(TRUE);
	m_ctrlSelect.UpdateWindow();
	m_ctrlGenerate.EnableWindow(TRUE);
	m_ctrlGenerate.UpdateWindow();
	m_ctrlModified.EnableWindow(TRUE);
	m_ctrlModified.UpdateWindow();
	m_ctrlPriority.EnableWindow(TRUE);
	m_ctrlPriority.UpdateWindow();
	m_ctrlFrequency.EnableWindow(TRUE);
	m_ctrlFrequency.UpdateWindow();
	m_ctrlProgress.SetMarquee(FALSE, 30);
	m_ctrlProgress.EnableWindow(FALSE);
	m_ctrlProgress.UpdateWindow();
	m_ctrlStatus.SetWindowText(_T("XML Sitemap has finished!"));
	m_ctrlStatus.UpdateWindow();

	VERIFY(ExportXMLSitemap(this->m_hWnd, m_strOutputFile, strFrequency, strPriority, strDateTime));

	TCHAR lpszTempPath[_MAX_PATH] = { 0 };
	DWORD nLength = GetTempPath(_MAX_PATH, lpszTempPath);
	if (nLength > 0)
	{
		TCHAR lpszFilePath[_MAX_PATH] = { 0 };
		nLength = GetTempFileName(lpszTempPath, L"TMP", 0, lpszFilePath);
		if (nLength > 0)
		{
			CString strFileName = lpszFilePath;
			strFileName.Replace(_T(".tmp"), _T(".html"));
			VERIFY(ExportBrokenLink(this->m_hWnd, strFileName, m_strDomainName));
		}
	}
}

void CXMLSitemapDlg::OnBnClickedCancel()
{
	if (m_bGenerate)
	{
		// Generation is in progress - set flag to FALSE to signal worker thread to stop
		// The worker thread checks this flag periodically and will terminate when FALSE
		m_bGenerate = FALSE;
	}
	else
	{
		// No generation in progress - close the dialog and exit application
		CDialog::OnCancel();
	}
}
