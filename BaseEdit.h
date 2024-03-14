#if !defined(AFX_FILTEREDIT_H__222BE616_C621_4D59_A5F8_B69FCA4D4D50__INCLUDED_)
#define AFX_FILTEREDIT_H__222BE616_C621_4D59_A5F8_B69FCA4D4D50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterEdit.h : header file
//

#include <boost/regex.hpp>

#ifdef _UNICODE
typedef boost::wregex CRegEx;
#else
typedef boost::regex CRegEx;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit window
class CBaseEdit : public CEdit
{
// Operations
public:
	CBaseEdit (const bool bProcessChars = true);
	CBaseEdit (const TCHAR *pszRegEx, const bool bProcessChars = true);
	CBaseEdit (CRegEx *pRegEx, const bool bProcessChars = true);
	virtual ~CBaseEdit ();

	bool SetRegEx (const TCHAR *pszRegEx);
	void SetRegEx (CRegEx *pRegEx);
	bool IsInputValid (const bool bSetInputValid = true);
	const CString &GetProposedText () const;
	virtual void Increment (const int /*iInc*/) {};

	void SetProcessChars (const bool bProcessChars);
	bool GetProcessChars () const;
	void SetAllowEmpty (const bool bAllowEmpty);
	bool GetAllowEmpty () const;

	void EscapeString (CString &str) const;

	// Display related functions
	void SetWavyLineOnError (const bool bWavyLineOnError);
	bool GetWavyLineOnError () const;
	void SetWavyLineColour (const COLORREF crWavyLine);
	void SetBackgroundColourFocus (const COLORREF crBkFocus);
	void SetForegroundColourFocus (const COLORREF crFgFocus);
	void SetBackgroundColourOK (const COLORREF crBkOK);
	void SetForegroundColourOK (const COLORREF crFgOK);
	void SetBackgroundColourError (const COLORREF crBkError);
	void SetForegroundColourError (const COLORREF crFgError);

	// Tooltip functions
	void CreateToolTip (CWnd *pParent, const TCHAR *pszText,
		const int iIconType = 1, const TCHAR *pszTitle = _T("Data Format"));
	void SetToolTipText (const TCHAR *pszText);
	void SetToolTipTitle (const int iIconType, const TCHAR *pszTitle);
	void SetDisplayTTonNull (const bool bDisplayTTonNull);
	bool GetDisplayTTonNull () const;
	bool GetToolTipCreated () const;

protected:
	bool IsTextValid (const CString &strText);
	virtual bool SyntaxCheck (const CString &strText);
	virtual bool SemanticCheck (const CString &strText,
		CString *pstrErrorMsg = 0);
	bool ValidateChar (UINT nChar, const bool bDisplayToolTip = true);
	bool Match (const CString &strText, const bool bDisplayToolTip = true);

	bool CanDelete ();
	bool CanPaste ();
	void ResetValid ();
	void SetInputValid (const bool bInputValid);

	void DisplayToolTip (const bool bDisplay, const TCHAR *pszText = 0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	bool m_bProcessChars;
	bool m_bAllowEmpty;
	bool m_bRegExOwner;
	CRegEx *m_pRegEx;

	bool m_bInputValid;
	bool m_bControlDown;
	CString m_strProposedText;
	CString m_strPrevToolTipText;

	// Display related members
	bool m_bWavyLineOnError;
	COLORREF m_crWavyLineColour;
	COLORREF m_crFgFocus;
	COLORREF m_crBkFocus;
	COLORREF m_crFgOK;
	COLORREF m_crBkOK;
	COLORREF m_crFgError;
	COLORREF m_crBkError;
	CBrush m_brFocus;
	CBrush m_brOK;
	CBrush m_brError;
	bool m_bDisplayTTonNull;
	CToolTipCtrl m_ToolTip;

	void SetDefaults ();
	void ConstructRegEx ();

	CBaseEdit (CBaseEdit const &); // No copy construction.
	CBaseEdit &operator = (CBaseEdit const &); // No assignment.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTEREDIT_H__222BE616_C621_4D59_A5F8_B69FCA4D4D50__INCLUDED_)
