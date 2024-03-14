/* This file is part of XML Sitemap application developed by Mihai MOGA.

XML Sitemap is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

XML Sitemap is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
XML Sitemap. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// XMLSitemapExt.cpp : implementation file
//

#include "stdafx.h"
#include "XMLSitemap.h"
#include "XMLSitemapDlg.h"
#include "XMLSitemapExt.h"
#include "BaseEdit.h"

#include <Urlmon.h>
#pragma comment(lib, "Urlmon")

CStringArray g_arrURL;
CStringArray g_arrBrokenLink;
CStringArray g_arrBrokenPage;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PUMP_MESSAGES()
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL IsHtmlPage(LPCTSTR lpszURL)
{
	if ((lpszURL != NULL) && (_tcslen(lpszURL) > 0))
	{
		if ((_tcsstr(lpszURL, _T(".htm")) != NULL) || // also .html
			(_tcsstr(lpszURL, _T(".php")) != NULL) ||
			(_tcsstr(lpszURL, _T(".asp")) != NULL)) // also .aspx
		{
			return TRUE;
		}

		if ((_tcsstr(lpszURL, _T(".7z")) != NULL) ||
			(_tcsstr(lpszURL, _T(".ace")) != NULL) ||
			(_tcsstr(lpszURL, _T(".ai")) != NULL) ||
			(_tcsstr(lpszURL, _T(".asf")) != NULL) ||
			(_tcsstr(lpszURL, _T(".avi")) != NULL) ||
			(_tcsstr(lpszURL, _T(".bat")) != NULL) ||
			(_tcsstr(lpszURL, _T(".bmp")) != NULL) ||
			(_tcsstr(lpszURL, _T(".bin")) != NULL) ||
			(_tcsstr(lpszURL, _T(".cab")) != NULL) ||
			(_tcsstr(lpszURL, _T(".cdr")) != NULL) ||
			(_tcsstr(lpszURL, _T(".chm")) != NULL) ||
			(_tcsstr(lpszURL, _T(".css")) != NULL) ||
			(_tcsstr(lpszURL, _T(".dat")) != NULL) ||
			(_tcsstr(lpszURL, _T(".divx")) != NULL) ||
			(_tcsstr(lpszURL, _T(".dll")) != NULL) ||
			(_tcsstr(lpszURL, _T(".doc")) != NULL) ||
			(_tcsstr(lpszURL, _T(".dwg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".eml")) != NULL) ||
			(_tcsstr(lpszURL, _T(".eps")) != NULL) ||
			(_tcsstr(lpszURL, _T(".exe")) != NULL) ||
			(_tcsstr(lpszURL, _T(".fla")) != NULL) ||
			(_tcsstr(lpszURL, _T(".ico")) != NULL) ||
			(_tcsstr(lpszURL, _T(".iso")) != NULL) ||
			(_tcsstr(lpszURL, _T(".jar")) != NULL) ||
			(_tcsstr(lpszURL, _T(".jpeg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".jpg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".gif")) != NULL) ||
			(_tcsstr(lpszURL, _T(".gz")) != NULL) ||
			(_tcsstr(lpszURL, _T(".indd")) != NULL) ||
			(_tcsstr(lpszURL, _T(".log")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mov")) != NULL) ||
			(_tcsstr(lpszURL, _T(".m4a")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mdb")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mp3")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mp4")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mpeg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".mpg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".msg")) != NULL) ||
			(_tcsstr(lpszURL, _T(".msi")) != NULL) ||
			(_tcsstr(lpszURL, _T(".pdf")) != NULL) ||
			(_tcsstr(lpszURL, _T(".png")) != NULL) ||
			(_tcsstr(lpszURL, _T(".pps")) != NULL) ||
			(_tcsstr(lpszURL, _T(".ppt")) != NULL) ||
			(_tcsstr(lpszURL, _T(".psd")) != NULL) ||
			(_tcsstr(lpszURL, _T(".pst")) != NULL) ||
			(_tcsstr(lpszURL, _T(".pub")) != NULL) ||
			(_tcsstr(lpszURL, _T(".qbb")) != NULL) ||
			(_tcsstr(lpszURL, _T(".rar")) != NULL) ||
			(_tcsstr(lpszURL, _T(".rm")) != NULL) ||
			(_tcsstr(lpszURL, _T(".rmvb")) != NULL) ||
			(_tcsstr(lpszURL, _T(".rtf")) != NULL) ||
			(_tcsstr(lpszURL, _T(".sit")) != NULL) ||
			(_tcsstr(lpszURL, _T(".swt")) != NULL) ||
			(_tcsstr(lpszURL, _T(".tgz")) != NULL) ||
			(_tcsstr(lpszURL, _T(".tif")) != NULL) ||
			(_tcsstr(lpszURL, _T(".ttf")) != NULL) ||
			(_tcsstr(lpszURL, _T(".txt")) != NULL) ||
			(_tcsstr(lpszURL, _T(".wav")) != NULL) ||
			(_tcsstr(lpszURL, _T(".wma")) != NULL) ||
			(_tcsstr(lpszURL, _T(".wmv")) != NULL) ||
			(_tcsstr(lpszURL, _T(".xls")) != NULL) ||
			(_tcsstr(lpszURL, _T(".xml")) != NULL) ||
			(_tcsstr(lpszURL, _T(".zip")) != NULL))
		{
			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

BOOL IsValidURL(LPCTSTR lpszURL, BOOL bMatchPartialURL)
{
	// Check to see if <lpszURL> is a valid URL identifier
	if ((lpszURL != NULL) && (_tcslen(lpszURL) > 0))
	{
		if (lpszURL[0] == _T('#')) return FALSE;

		CRegEx pRegEx;
		boost::match_results<const TCHAR *> lpszWhat;
		try
		{
			pRegEx.assign(bMatchPartialURL ? RELATIVE_URL_REGEX : ABSOLUTE_URL_REGEX);
			if (boost::regex_match(static_cast<const TCHAR *>(lpszURL), lpszWhat, pRegEx, boost::match_default))
				return TRUE;
		}
		catch (const std::exception &regexException)
		{
			CString strError(regexException.what());

			strError = _T("IsValidURL failed: ") + strError;
			AfxMessageBox(strError, MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	return FALSE;
}

BOOL IsLocalURL(LPCTSTR lpszAbsoluteURL, LPCTSTR lpszBaseURL)
{
	// Check to see if <lpszAbsoluteURL> has the same domain as <lpszBaseURL>
	TCHAR lpszFooBarURL[MAX_URL_LENGTH] = { 0 };
	TCHAR lpszDomainURL[MAX_URL_LENGTH] = { 0 };
	ASSERT(IsValidURL(lpszAbsoluteURL, FALSE));
	ASSERT(IsValidURL(lpszBaseURL, FALSE));
	if ((lpszAbsoluteURL != NULL) && (_tcslen(lpszAbsoluteURL) > 0))
	{
		if ((lpszBaseURL != NULL) && (_tcslen(lpszBaseURL) > 0))
		{
			_tcscpy_s(lpszDomainURL, MAX_URL_LENGTH, lpszBaseURL);
			LPCTSTR lpszStart = _tcsstr(lpszDomainURL, _T("//"));
			if (lpszStart != NULL)
			{
				lpszStart++;
				lpszStart++;
				ASSERT(lpszStart != NULL);
				LPTSTR lpszEnd = (LPTSTR) _tcschr(lpszStart, _T('/'));
				if (lpszEnd != NULL)
					lpszEnd[0] = _T('\0'); // modify lpszDomainURL

				_tcscpy_s(lpszFooBarURL, MAX_URL_LENGTH, lpszAbsoluteURL);
				LPCTSTR lpszBegin = _tcsstr(lpszFooBarURL, _T("//"));
				if (lpszBegin != NULL)
				{
					lpszBegin++;
					lpszBegin++;
					ASSERT(lpszBegin != NULL);
					lpszEnd = (LPTSTR) _tcschr(lpszBegin, _T('/'));
					if (lpszEnd != NULL)
						lpszEnd[0] = _T('\0'); // modify lpszFooBarURL

					if (_tcsstr(lpszBegin, lpszStart) != NULL)
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}

CString ConvertURL(LPCTSTR lpszRelativeURL, LPCTSTR lpszBaseURL)
{
	// Convert relative URL (e.g. "../../somedir") to absolute URL
	TCHAR lpszAbsoluteURL[MAX_URL_LENGTH] = { 0 };
	TCHAR lpszDomainURL[MAX_URL_LENGTH] = { 0 };
	ASSERT(IsValidURL(lpszRelativeURL, TRUE));
	ASSERT(IsValidURL(lpszBaseURL, FALSE));
	_tcscpy_s(lpszDomainURL, MAX_URL_LENGTH, lpszBaseURL);
	if (lpszRelativeURL[0] == _T('/'))
	{
		LPCTSTR lpszStart = _tcsstr(lpszDomainURL, _T("//"));
		if (lpszStart != NULL)
		{
			lpszStart++;
			lpszStart++;
			ASSERT(lpszStart != NULL);
			LPTSTR lpszEnd = (LPTSTR) _tcschr(lpszStart, _T('/'));
			if (lpszEnd != NULL)
				lpszEnd[0] = _T('\0'); // modify lpszDomainURL
		}
	}

	if ((lpszRelativeURL != NULL) && (_tcslen(lpszRelativeURL) > 0))
	{
		if ((lpszBaseURL != NULL) && (_tcslen(lpszBaseURL) > 0))
		{
			DWORD dwLength = MAX_URL_LENGTH;
			if (CoInternetCombineUrl(lpszDomainURL, lpszRelativeURL, 0, lpszAbsoluteURL, MAX_URL_LENGTH, &dwLength, 0) == S_OK)
			{
				lpszAbsoluteURL[dwLength] = '\0';
				return lpszAbsoluteURL;
			}
			OutputDebugString(_T("ERROR: CoInternetCombineUrl has failed\n"));
			return NULL;
		}
	}
	return NULL;
}

BOOL ExportXMLSitemap(HWND hWnd, CString strFileName, CString strFrequency, CString strPriority, CString strDateTime)
{
	CString strFileLine;
	BOOL bRetVal = TRUE;
	try
	{
		CStdioFile pOutputFile(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		pOutputFile.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
		pOutputFile.WriteString(_T("<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n"));
		const INT_PTR nSize = g_arrURL.GetCount();
		for (INT_PTR nIndex = 0; nIndex < nSize; nIndex++)
		{
			pOutputFile.WriteString(_T("<url>\n"));

			const CString strURL = g_arrURL.GetAt(nIndex);
			strFileLine.Format(_T("<loc>%s</loc>\n"), static_cast<LPCWSTR>(strURL));
			pOutputFile.WriteString(strFileLine);

			strFileLine.Format(_T("<lastmod>%s</lastmod>\n"), static_cast<LPCWSTR>(strDateTime));
			pOutputFile.WriteString(strFileLine);

			strFileLine.Format(_T("<changefreq>%s</changefreq>\n"), static_cast<LPCWSTR>(strFrequency));
			pOutputFile.WriteString(strFileLine);

			strFileLine.Format(_T("<priority>%s</priority>\n"), static_cast<LPCWSTR>(strPriority));
			pOutputFile.WriteString(strFileLine);

			pOutputFile.WriteString(_T("</url>\n"));
		}
		pOutputFile.WriteString(_T("</urlset>\n"));
		pOutputFile.Close();

		::ShellExecute(hWnd, _T("open"), strFileName, NULL, NULL, SW_NORMAL);
	}
	catch (CFileException * pFileException)
	{
		TCHAR lpszError[MAX_STR_LENGTH] = { 0 };
		pFileException->GetErrorMessage(lpszError, MAX_STR_LENGTH);
		pFileException->Delete();
		OutputDebugString(lpszError);
		bRetVal = FALSE;
	}
	return bRetVal;
}

BOOL ExportBrokenLink(HWND hWnd, CString strFileName, CString strDomainName)
{
	CString strFileLine;
	BOOL bRetVal = TRUE;
	try
	{
		CStdioFile pOutputFile(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		pOutputFile.WriteString(_T("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"));
		pOutputFile.WriteString(_T("<html xmlns=\"http://www.w3.org/1999/xhtml\" >\n"));
		pOutputFile.WriteString(_T("<head>\n"));
		pOutputFile.WriteString(_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"));
		strFileLine.Format(_T("<title>Broken link report for %s</title>\n"), static_cast<LPCWSTR>(strDomainName));
		pOutputFile.WriteString(strFileLine);
		pOutputFile.WriteString(_T("</head>\n"));
		pOutputFile.WriteString(_T("<body>\n"));
		const INT_PTR nSize = g_arrBrokenLink.GetCount();
		for (INT_PTR nIndex = 0; nIndex < nSize; nIndex++)
		{
			const CString strBrokenLink = g_arrBrokenLink.GetAt(nIndex);
			const CString strBrokenPage = g_arrBrokenPage.GetAt(nIndex);
			strFileLine.Format(_T("Broken link %s found on page %s.<br />\n"), static_cast<LPCWSTR>(strBrokenLink), static_cast<LPCWSTR>(strBrokenPage));
			pOutputFile.WriteString(strFileLine);
		}
		pOutputFile.WriteString(_T("</body>\n"));
		pOutputFile.WriteString(_T("</html>\n"));
		pOutputFile.Close();

		::ShellExecute(hWnd, _T("open"), strFileName, NULL, NULL, SW_NORMAL);
	}
	catch (CFileException * pFileException)
	{
		TCHAR lpszError[MAX_STR_LENGTH] = { 0 };
		pFileException->GetErrorMessage(lpszError, MAX_STR_LENGTH);
		pFileException->Delete();
		OutputDebugString(lpszError);
		bRetVal = FALSE;
	}
	return bRetVal;
}

BOOL ProcessHTML(CXMLSitemapDlg* dlgXMLSitemap, CString strFileName, CString strBaseURL)
{
	CString strMessage;
	CString strFileLine;
	BOOL bRetVal = TRUE;
	CStringArray arrURL;
	TCHAR lpszTempPath[MAX_STR_LENGTH] = { 0 };
	TCHAR lpszTempFile[MAX_STR_LENGTH] = { 0 };

	if (dlgXMLSitemap != NULL)
	{
		if (!strFileName.IsEmpty())
		{
			try
			{
				CStdioFile pInputFile(strFileName, CFile::modeRead | CFile::typeText);
				while (pInputFile.ReadString(strFileLine))
				{
					int nIndex = strFileLine.Find(_T("href="), 0);
					while (nIndex >= 0)
					{
						const int nFirst = strFileLine.Find(_T('\"'), nIndex);
						if (nFirst >= 0)
						{
							const int nLast = strFileLine.Find(_T('\"'), nFirst + 1);
							if (nLast >= 0)
							{
								CString strURL = strFileLine.Mid(nFirst + 1, nLast - nFirst - 1);
								strMessage.Format(_T("URL found - %s\n"), static_cast<LPCWSTR>(strURL));
								OutputDebugString(strMessage);
								arrURL.Add(strURL);
							}
						}
						nIndex = (nFirst == -1) ? -1 : strFileLine.Find(_T("href="), nFirst + 1);
					}
				}
				pInputFile.Close();
			}
			catch (CFileException * pFileException)
			{
				TCHAR lpszError[MAX_STR_LENGTH] = { 0 };
				pFileException->GetErrorMessage(lpszError, MAX_STR_LENGTH);
				pFileException->Delete();
				OutputDebugString(lpszError);
				bRetVal = FALSE;
			}

			const INT_PTR nSize = arrURL.GetCount();
			for (INT_PTR nIndex = 0; ((nIndex < nSize) && (dlgXMLSitemap->m_bGenerate)); nIndex++)
			{
				CString strURL = arrURL.GetAt(nIndex);
				if (IsHtmlPage(strURL))
				{
					if (strURL[0] == _T('#') || ((strURL[0] == _T('/')) && (strURL[1] == _T('/'))))
						continue;

					if (!IsValidURL(strURL, FALSE))
					{
						strURL = ConvertURL(strURL, strBaseURL);
						if (strURL.IsEmpty()) continue;
					}

					if (IsValidURL(strURL, FALSE))
					{
						if (IsLocalURL(strURL, strBaseURL))
						{
							const int nFind = strURL.ReverseFind(_T('#'));
							if (nFind >= 0)
							{
								const int nSlash = strURL.Find(_T('/'), nFind);
								if (nSlash == -1)
								{
									strURL = strURL.Left(nFind);
								}
							}

							if (strURL.GetAt(strURL.GetLength() - 1) == _T('/'))
								strURL = strURL.Left(strURL.GetLength() - 1);

							INT_PTR nSearch = 0;
							const INT_PTR nArraySize = g_arrURL.GetCount();
							for (; nSearch < nArraySize; nSearch++)
							{
								if (strURL.CompareNoCase(g_arrURL.GetAt(nSearch)) == 0)
									break;
							}
							if (nSearch == nArraySize)
							{
								strMessage.Format(_T("URL added - %s\n"), static_cast<LPCWSTR>(strURL));
								OutputDebugString(strMessage);
								g_arrURL.Add(strURL); // add URL to XML sitemap
								dlgXMLSitemap->m_ctrlStatus.SetWindowText(strURL);
								dlgXMLSitemap->m_ctrlStatus.UpdateWindow();

								const DWORD dwTempPath = GetTempPath(MAX_STR_LENGTH, lpszTempPath);
								lpszTempPath[dwTempPath] = '\0';
								strMessage.Format(_T("lpszTempPath = %s\n"), lpszTempPath);
								OutputDebugString(strMessage);

								if (GetTempFileName(lpszTempPath, _T("map"), 0, lpszTempFile) != 0)
								{
									strMessage.Format(_T("lpszTempFile = %s\n"), lpszTempFile);
									OutputDebugString(strMessage);

									if (URLDownloadToFile(NULL, strURL, lpszTempFile, 0, NULL) == S_OK)
									{
										if (!ProcessHTML(dlgXMLSitemap, lpszTempFile, strURL))
										{
											strMessage.Format(_T("ProcessHTML(%s) has failed\n"), lpszTempFile);
											OutputDebugString(strMessage);
										}
									}
									else
									{
										strMessage.Format(_T("Broken link %s found in page %s\n"), static_cast<LPCWSTR>(strURL), static_cast<LPCWSTR>(strBaseURL));
										OutputDebugString(strMessage);
										g_arrBrokenLink.Add(strURL);
										g_arrBrokenPage.Add(strBaseURL);
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			OutputDebugString(_T("ERROR: strFileName is empty\n"));
			bRetVal = FALSE;
		}
	}
	else
	{
		OutputDebugString(_T("ERROR: dlgXMLSitemap = NULL\n"));
		bRetVal = FALSE;
	}
	VERIFY(DeleteFile(strFileName));
	return bRetVal;
}

DWORD WINAPI XMLSitemap_ThreadProc(LPVOID lpParam)
{
	CString strMessage, strURL;
	TCHAR lpszTempPath[MAX_STR_LENGTH] = { 0 };
	TCHAR lpszTempFile[MAX_STR_LENGTH] = { 0 };
	CXMLSitemapDlg* dlgXMLSitemap = (CXMLSitemapDlg*) lpParam;

	if (dlgXMLSitemap != NULL)
	{
		g_arrURL.RemoveAll();
		g_arrBrokenLink.RemoveAll();
		g_arrBrokenPage.RemoveAll();
		strURL = dlgXMLSitemap->m_strDomainName;
		if (!strURL.IsEmpty())
		{
			if (strURL.GetAt(strURL.GetLength() - 1) == _T('/'))
				strURL = strURL.Left(strURL.GetLength() - 1);

			const DWORD dwTempPath = GetTempPath(MAX_STR_LENGTH, lpszTempPath);
			lpszTempPath[dwTempPath] = '\0';
			strMessage.Format(_T("lpszTempPath = %s\n"), lpszTempPath);
			OutputDebugString(strMessage);

			if (GetTempFileName(lpszTempPath, _T("map"), 0, lpszTempFile) != 0)
			{
				strMessage.Format(_T("lpszTempFile = %s\n"), lpszTempFile);
				OutputDebugString(strMessage);

				strMessage.Format(_T("URLDownloadToFile(%s)...\n"), static_cast<LPCWSTR>(strURL));
				OutputDebugString(strMessage);
				if (URLDownloadToFile(NULL, strURL, lpszTempFile, 0, NULL) == S_OK)
				{
					strMessage.Format(_T("URL added - %s\n"), static_cast<LPCWSTR>(strURL));
					OutputDebugString(strMessage);
					g_arrURL.Add(strURL); // add URL to XML sitemap
					dlgXMLSitemap->m_ctrlStatus.SetWindowText(strURL);
					dlgXMLSitemap->m_ctrlStatus.UpdateWindow();

					if (!ProcessHTML(dlgXMLSitemap, lpszTempFile, strURL))
					{
						strMessage.Format(_T("ProcessHTML(%s) has failed\n"), lpszTempFile);
						OutputDebugString(strMessage);
					}
				}
				else
				{
					OutputDebugString(_T("URLDownloadToFile has failed\n"));
				}
			}
			else
			{
				OutputDebugString(_T("GetTempFileName has failed\n"));
			}
		}
		else
		{
			OutputDebugString(_T("ERROR: dlgXMLSitemap->m_strDomainName is empty!\n"));
		}
	}
	else
	{
		OutputDebugString(_T("ERROR: dlgXMLSitemap = NULL\n"));
	}
	return 0;
}
