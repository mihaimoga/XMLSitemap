/* This file is part of XML Sitemap application developed by Mihai MOGA.

XML Sitemap is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

XML Sitemap is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
XML Sitemap. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// XMLSitemapDlg.h : header file
//

#pragma once

#include "BaseEdit.h"

// CXMLSitemapDlg dialog
class CXMLSitemapDlg : public CDialog
{
// Construction
public:
	CXMLSitemapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XMLSITEMAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CBaseEdit m_ctrlDomainName;
	CComboBox m_ctrlFrequency;
	CComboBox m_ctrlPriority;
	CDateTimeCtrl m_ctrlModified;
	CStatic m_ctrlStatus;
	CProgressCtrl m_ctrlProgress;
	CEdit m_ctrlFilename;
	CButton m_ctrlGenerate;
	CButton m_ctrlSelect;
	CButton m_ctrlCancel;

// Implementation
protected:
	HICON m_hIcon;

public:
	CString m_strDomainName;
	CString m_strOutputFile;
	CString m_strGenerate;
	BOOL m_bGenerate;

	// Generated message map functions
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
public:
	afx_msg void OnEnChangeDomain();
	afx_msg void OnCbnSelchangeFrequency();
	afx_msg void OnCbnSelchangePriority();
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedGenerate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCancel();
};
