// FilterEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BaseEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// NOTE: Be very careful how you specify your regular expressions.
// For strictly formatted data, you will want a rigid regex
// and rely on automatic formatting in a derived class to make
// data entry easier (see the CDateEdit example to see this approach
// in action). For easier validations (say unsigned int) a much looser
// regex can be used without problems.

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit

// public functions:
CBaseEdit::CBaseEdit (const bool bProcessChars/* = true*/) :
	m_bProcessChars (bProcessChars),
	m_bRegExOwner (true),
	m_pRegEx (0)
{
	SetDefaults ();
}

CBaseEdit::CBaseEdit (const TCHAR *pszRegEx,
	const bool bProcessChars/* = true*/) :
	m_bProcessChars (bProcessChars),
	m_bRegExOwner (true),
	m_pRegEx (0)
{
	SetDefaults ();
	SetRegEx (pszRegEx);
}

CBaseEdit::CBaseEdit (CRegEx *pRegEx,
	const bool bProcessChars/* = true*/) :
	m_bProcessChars (bProcessChars),
	m_bRegExOwner (pRegEx == 0),
	m_pRegEx (pRegEx)
{
	SetDefaults ();
}

CBaseEdit::~CBaseEdit ()
{
	if (m_bRegExOwner)
	{
		delete m_pRegEx;
		m_pRegEx = 0;
	}
}

bool CBaseEdit::SetRegEx (const TCHAR *pszRegEx)
{
	// Only Change regex if we are the owner.
	bool bSuccess = m_bRegExOwner;

	if (bSuccess)
	{
		if (!m_pRegEx)
		{
			ConstructRegEx ();
		}

		try
		{
			m_pRegEx->assign (pszRegEx);
		}
		catch (const std::exception &e)
		{
			CString strError (e.what ());

			bSuccess = false;
			strError = _T("SetRegEx failed:\r\n") + strError;
			AfxMessageBox (strError, MB_OK);
		}
	}

	return bSuccess;
}

void CBaseEdit::SetRegEx (CRegEx *pRegEx)
{
	if (m_bRegExOwner)
	{
		delete m_pRegEx;
		m_pRegEx = 0;
	}

	m_pRegEx = pRegEx;
	m_bRegExOwner = false;
}

bool CBaseEdit::IsInputValid (const bool bSetInputValid/* = true*/)
{
	bool bSuccess = true;
	CString str;

	GetWindowText (str);
	bSuccess = IsTextValid (str) || m_bAllowEmpty && str.IsEmpty ();

	if (bSetInputValid) SetInputValid (bSuccess);

	return bSuccess;
}

const CString &CBaseEdit::GetProposedText () const
{
	return m_strProposedText;
}

void CBaseEdit::SetProcessChars (const bool bProcessChars)
{
	m_bProcessChars = bProcessChars;
}

bool CBaseEdit::GetProcessChars () const
{
	return m_bProcessChars;
}

// You can do this in the regex directly, but for more complex
// controls (with multiple modes/regexes) it is cleaner to
// use this flag.
void CBaseEdit::SetAllowEmpty (const bool bAllowEmpty)
{
	m_bAllowEmpty = bAllowEmpty;
}

bool CBaseEdit::GetAllowEmpty () const
{
	return m_bAllowEmpty;
}

void CBaseEdit::EscapeString (CString &str) const
{
	int iIndex = 0;

	while (iIndex < str.GetLength ())
	{
		if (_tcschr (_T(".|*?+(){}[]^$\\"), str[iIndex]))
		{
			str.Insert (iIndex++, _T("\\"));
		}

		++iIndex;
	}
}

void CBaseEdit::SetWavyLineOnError (const bool bWavyLineOnError)
{
	m_bWavyLineOnError = bWavyLineOnError;
}

bool CBaseEdit::GetWavyLineOnError () const
{
	return m_bWavyLineOnError;
}

void CBaseEdit::SetWavyLineColour (const COLORREF crWavyLine)
{
	m_crWavyLineColour = crWavyLine;
}

void CBaseEdit::SetBackgroundColourFocus (const COLORREF crBkFocus)
{
	m_crBkFocus = crBkFocus;
	m_brFocus.DeleteObject ();
	m_brFocus.CreateSolidBrush (m_crBkFocus);
}

void CBaseEdit::SetForegroundColourFocus (const COLORREF crFgFocus)
{
	m_crFgFocus = crFgFocus;
}

void CBaseEdit::SetBackgroundColourOK (const COLORREF crBkOK)
{
	m_crBkOK = crBkOK;
	m_brOK.DeleteObject ();
	m_brOK.CreateSolidBrush (m_crBkOK);
}

void CBaseEdit::SetForegroundColourOK (const COLORREF crFgOK)
{
	m_crFgOK = crFgOK;
}

void CBaseEdit::SetBackgroundColourError (const COLORREF crBkError)
{
	m_crBkError = crBkError;
	m_brError.DeleteObject ();
	m_brError.CreateSolidBrush (m_crBkError);
}

void CBaseEdit::SetForegroundColourError (const COLORREF crFgError)
{
	m_crFgError = crFgError;
}

void CBaseEdit::CreateToolTip (CWnd *pParent, const TCHAR *pszText,
	const int iIconType/* = 1*/, const TCHAR *pszTitle/* = _T("Data Format")*/)
{
	TOOLINFO ti;
	DWORD dwStyle = TTS_BALLOON;

	m_ToolTip.Create (pParent, dwStyle);
	m_ToolTip.FillInToolInfo (ti, this, 0);
	ti.uFlags |= TTF_TRACK | TTF_CENTERTIP | TTF_ABSOLUTE;
	ti.lpszText = (LPTSTR) pszText;
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
	m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
}

void CBaseEdit::SetToolTipText (const TCHAR *pszText)
{
	m_ToolTip.UpdateTipText (pszText, this);
}

void CBaseEdit::SetToolTipTitle (const int iIconType, const TCHAR *pszTitle)
{
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
}

void CBaseEdit::SetDisplayTTonNull (const bool bDisplayTTonNull)
{
	m_bDisplayTTonNull = bDisplayTTonNull;
}

bool CBaseEdit::GetDisplayTTonNull () const
{
	return m_bDisplayTTonNull;
}

bool CBaseEdit::GetToolTipCreated () const
{
	return m_ToolTip.m_hWnd != 0;
}

// protected functions:
bool CBaseEdit::IsTextValid (const CString &strText)
{
	bool bSuccess = SyntaxCheck (strText);

	if (bSuccess)
	{
		bSuccess = SemanticCheck (strText);
	}

	return bSuccess;
}

bool CBaseEdit::SyntaxCheck (const CString &strText)
{
	bool bSuccess = true;
	boost::match_results<const TCHAR *> what;

	// This is a full match check, not a partial match
	bSuccess = boost::regex_match (static_cast<const TCHAR *>(strText),
		what, *m_pRegEx, boost::match_default);
	return bSuccess;
}

bool CBaseEdit::SemanticCheck (const CString &/*strText*/,
	CString * /*pstrErrorMsg = 0*/)
{
	return true;
}

bool CBaseEdit::ValidateChar (UINT nChar,
	const bool bDisplayToolTip/* = true*/)
{
	int iStartIndex = -1;
	int iEndIndex = -1;
	bool bCharValid = true;

	GetSel (iStartIndex, iEndIndex);
	GetWindowText (m_strProposedText);

	//  Backspace validation is done here as it has a virtual key code.
	if (nChar == VK_BACK)
	{
		if (iStartIndex == iEndIndex)
		{
			--iStartIndex;
		}

		// Remove char
		m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
		bCharValid = Match (m_strProposedText, bDisplayToolTip);
	}
	// Ignore ^X, ^C and ^V
	else if (nChar != '\x03' && nChar != '\x16' && nChar != '\x18')
	{
		m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
		m_strProposedText.Insert (iStartIndex, static_cast<TCHAR>(nChar));
		bCharValid = Match (m_strProposedText, bDisplayToolTip);
	}

	return bCharValid;
}

// NOTE: This function checks if the given string matches SO FAR.
// To check if the edit control contains a completely valid string,
// IsInputValid is called.
bool CBaseEdit::Match (const CString &strText,
	const bool bDisplayToolTip/* = true*/)
{
	bool bSuccess = true;

	// An empty string is always valid for matching a string SO FAR.
	if (strText.GetLength ())
	{
		boost::match_results<const TCHAR *> what;

		bSuccess = boost::regex_match (static_cast<const TCHAR *>(strText),
			what, *m_pRegEx, boost::match_default | boost::match_partial);

		if (bDisplayToolTip)
		{
			const bool bTT = GetDisplayTTonNull ();

			SetDisplayTTonNull (true);
			DisplayToolTip (!bSuccess);
			SetDisplayTTonNull (bTT);
		}
	}

	return bSuccess;
}

bool CBaseEdit::CanDelete ()
{
	CString strText;
	int iStartIndex = -1;
	int iEndIndex = -1;

	GetWindowText (strText);
	GetSel (iStartIndex, iEndIndex);

	if (iStartIndex == iEndIndex)
	{
		++iEndIndex;
	}

	strText.Delete (iStartIndex, iEndIndex - iStartIndex);
	return Match (strText);
}

bool CBaseEdit::CanPaste ()
{
	bool bSuccess = OpenClipboard () != 0;

	if (bSuccess)
	{
#ifdef _UNICODE
		HANDLE hClipMem = ::GetClipboardData (CF_UNICODETEXT);
#else
		HANDLE hClipMem = ::GetClipboardData (CF_TEXT);
#endif

		bSuccess = hClipMem != 0;

		if (bSuccess)
		{
			const TCHAR *lpClipMem = (const TCHAR *) ::GlobalLock (hClipMem);

			bSuccess = lpClipMem != 0;

			if (bSuccess)
			{
				CString strClipText;
				CString strText;
				int iStartIndex = -1;
				int iEndIndex = -1;

				strClipText = lpClipMem;
				::GlobalUnlock (hClipMem);
				GetWindowText (strText);
				GetSel (iStartIndex, iEndIndex);
				strText.Delete (iStartIndex, iEndIndex - iStartIndex);
				strText.Insert (iStartIndex, strClipText);

				if (strText.GetLength () > static_cast<int>(GetLimitText ()))
				{
					strText = strText.Left (GetLimitText ());
				}

				bSuccess = Match (strText);

				if (bSuccess)
				{
					bSuccess = SemanticCheck (strText);
				}
			}
		}

		::CloseClipboard ();
	}

	return bSuccess;
}

// Set control state to input OK and hide tooltip
void CBaseEdit::ResetValid ()
{
	if (!m_bInputValid)
	{
		SetInputValid (true);
		DisplayToolTip (FALSE);
	}
}

// Set control state to input OK/not OK, update the visuals and
// set the focus if the input is not valid.
void CBaseEdit::SetInputValid (const bool bInputValid)
{
	m_bInputValid = bInputValid;
	Invalidate ();

	if (!m_bInputValid) SetFocus ();
}

void CBaseEdit::DisplayToolTip (const bool bDisplay,
	const TCHAR *pszText/* = 0*/)
{
	if (m_ToolTip.m_hWnd &&
		!(GetWindowTextLength () == 0 && !m_bDisplayTTonNull))
	{
		CToolInfo ti;

		m_ToolTip.GetToolInfo (ti, this, 0);

		if (bDisplay)
		{
			RECT ClientRect;
			int iHeight = 0;
			int iOffset = 0;
			const int iBorder = ::GetSystemMetrics (SM_CXEDGE) + 1;

			if (pszText)
			{
				m_bInputValid = false;

				if (m_strPrevToolTipText.IsEmpty ())
				{
					m_strPrevToolTipText = ti.lpszText;
				}

				SetToolTipText (pszText);
			}
			else if (!m_strPrevToolTipText.IsEmpty ())
			{
				SetToolTipText (m_strPrevToolTipText);
				m_strPrevToolTipText.Empty ();
			}

			GetClientRect (&ClientRect);
			iHeight = ClientRect.bottom - ClientRect.top;
			iOffset = (ClientRect.right - ClientRect.left) / 2;
			GetWindowRect (&ti.rect);
			ti.rect.top += iHeight + iBorder;
			ti.rect.bottom += iHeight + iBorder;
			ti.rect.left += iOffset;
			ti.rect.right += iOffset;

			m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (ti.rect.left,
				ti.rect.top));
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,
				reinterpret_cast<LPARAM>(&ti));
		}
		else
		{
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
				reinterpret_cast<LPARAM>(&ti));
		}
	}
}

// private functions:
void CBaseEdit::SetDefaults ()
{
	m_bAllowEmpty = false;
	m_bInputValid = true;
	m_bWavyLineOnError = true;
	SetWavyLineColour (RGB (255, 0, 0));
	SetBackgroundColourFocus (::GetSysColor(COLOR_WINDOW));
	SetForegroundColourFocus (::GetSysColor(COLOR_WINDOWTEXT));
	SetBackgroundColourOK (::GetSysColor(COLOR_WINDOW));
	SetForegroundColourOK (::GetSysColor(COLOR_WINDOWTEXT));
	SetBackgroundColourError (::GetSysColor(COLOR_WINDOW));
	SetForegroundColourError (::GetSysColor(COLOR_WINDOWTEXT));
	m_bDisplayTTonNull = true;
}

void CBaseEdit::ConstructRegEx ()
{
	if (m_pRegEx == 0)
	{
		m_pRegEx = new CRegEx;
		m_bRegExOwner = true;
	}
}

BEGIN_MESSAGE_MAP(CBaseEdit, CEdit)
	//{{AFX_MSG_MAP(CBaseEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit message handlers

// Intercept messages that change the text and validate them.
LRESULT CBaseEdit::WindowProc (UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	bool bExecute = true;
	bool bResetValid = false;

	if (m_bProcessChars)
	{
		switch (message)
		{
			case EM_REPLACESEL:
			{
				int iStartIndex = -1;
				int iEndIndex = -1;
				CString strReplace;

				GetSel (iStartIndex, iEndIndex);
				strReplace = reinterpret_cast<LPCTSTR>(lParam);
				GetWindowText (m_strProposedText);
				m_strProposedText.Delete (iStartIndex, iEndIndex -
					iStartIndex);
				m_strProposedText.Insert (iStartIndex, strReplace);
				bExecute = bResetValid = Match (m_strProposedText);

				if (bExecute)
				{
					bExecute = SemanticCheck (m_strProposedText);
				}

				break;
			}
			case WM_CLEAR:
			case WM_CUT:
				bExecute = bResetValid = CanDelete ();
				break;
			case WM_PASTE:
				bExecute = bResetValid = CanPaste ();
				break;
			case WM_SETTEXT:
			{
				const TCHAR *psz = reinterpret_cast<const TCHAR *>
					(lParam);

				bExecute = bResetValid = !*psz || IsTextValid (psz);
				break;
			}
			default:
				break;
		}
	}
	else
	{
		bResetValid = true;
	}

	if (bExecute)
	{
		lResult = CEdit::WindowProc (message, wParam, lParam);
	}

	if (bResetValid) ResetValid ();

	return lResult;
}

void CBaseEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_bProcessChars || ValidateChar (nChar))
	{
		CEdit::OnChar (nChar, nRepCnt, nFlags);
	}
}

void CBaseEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bExecute = true;

	ResetValid ();

	// VK_DELETE never reaches OnChar, so we process it here.
	if (nChar == VK_DELETE)
	{
		if (m_bProcessChars)
		{
			bExecute = CanDelete ();
		}
	}
	else if (!(::GetKeyState (VK_SHIFT) & 0x80) &&
		(::GetKeyState (VK_CONTROL) & 0x80))
	{
		// ^X, ^C and ^V do not have virtual key codes so we cannot
		// cleanly process them in OnChar. CBaseEdit::WindowProc deals with
		// responding to WM_CUT and WM_PASTE
		if ((nChar == 'x' || nChar == 'X'))
		{
			SendMessage (WM_CUT, 0, 0);
			// Will handle cut ourselves, so don't pass char on...
			bExecute = false;
		}
		// For some reason this is not needed...
/*
		else if ((nChar == 'v' || nChar == 'V'))
		{
			SendMessage (WM_PASTE, 0, 0);
			// Will handle paste ourselves, so don't pass char on...
			bExecute = false;
		}
*/
	}

	if (bExecute)
	{
		CEdit::OnKeyDown (nChar, nRepCnt, nFlags);
	}
}

void CBaseEdit::OnKillFocus (CWnd *pNewWnd)
{
	// Pass in false so that focus is not set back
	// to this control (see function SetInputValid).
	m_bInputValid = IsInputValid (false);
	Invalidate ();
	DisplayToolTip (FALSE);

	// Don't allow cursor to disappear...
	if (pNewWnd && pNewWnd->m_hWnd != m_ToolTip.m_hWnd)
	{
		CEdit::OnKillFocus (pNewWnd);
	}
}

void CBaseEdit::OnSetFocus (CWnd *pOldWnd)
{
	CEdit::OnSetFocus (pOldWnd);
	Invalidate ();

	if (!m_bInputValid) DisplayToolTip (true);
}

HBRUSH CBaseEdit::CtlColor (CDC *pDC, UINT /*nCtlColor*/)
{
	HBRUSH hbr = 0;

	if (!(GetStyle () & ES_READONLY) && IsWindowEnabled ())
	{
		if (m_bInputValid)
		{
			if (GetFocus () == this)
			{
				hbr = m_brFocus;
				pDC->SetBkColor (m_crBkFocus);
				pDC->SetTextColor (m_crFgFocus);
			}
			else
			{
				hbr = m_brOK;
				pDC->SetBkColor (m_crBkOK);
				pDC->SetTextColor (m_crFgOK);
			}
		}
		else
		{
			hbr = m_brError;
			pDC->SetBkColor (m_crBkError);
			pDC->SetTextColor (m_crFgError);
		}
	}

	return hbr;
}

void CBaseEdit::OnPaint ()
{
	if (GetStyle () & ES_READONLY)
	{
		CEdit::OnPaint ();
	}
	else
	{
		CClientDC dc (this);

		//  This stops the control from going grey
		Default ();

		if (!m_bInputValid && m_bWavyLineOnError)
		{
			CRect rect;
			CRect rectClient;
			const int iIncrement = 2;

			// Get the rect for the entire edit control
			GetRect (&rect);
			GetClientRect (&rectClient);

			// Check that wavy line will not over-write edit control border...
			if (rectClient.bottom - rect.bottom >= iIncrement + 1)
			{
				CFont *pFont = GetFont ();
				CPen *pPen = dc.GetCurrentPen ();
				LOGPEN lp;
				CPen MyPen;
				CString strText;
				CSize size;
				int iX = 0;

				rect.bottom -= iIncrement - 2;

				// We must use the correct font when using GetTextExtent..!
				dc.SelectObject (pFont);
				// We have to create a new pen to set a new colour...
				pPen->GetLogPen (&lp);
				lp.lopnColor = m_crWavyLineColour;
				MyPen.CreatePenIndirect (&lp);
				dc.SelectObject (&MyPen);
				GetWindowText (strText);
				// This part deals with the *displayed* Text Extent
				size = dc.GetTextExtent (strText.
					Mid (CharFromPos (CPoint (rect.left + 1, rect.top))));

				// Check for case of empty string
				if (size.cx == 0) size.cx = iIncrement * 3;
				// Dont draw off the end of the edit control
				else if (size.cx > rect.right) size.cx = rect.right;

				int iOffset = 0;

				if (GetStyle () & ES_CENTER)
				{
					iOffset = (rect.Width () - size.cx) / 2;
				}
				else if (GetStyle () & ES_RIGHT)
				{
					iOffset = rect.Width () - size.cx;
				}

				rect.left += iOffset;
				// Starting x co-ordinate to start drawing from
				iX = rect.left;
				// Start at the bottom left of the edit control
				dc.MoveTo (rect.left, rect.bottom);

				// Draw the wavy line like in Microsoft Word
				while (iX < size.cx + rect.left)
				{
					iX += iIncrement;

					if (iX > size.cx + rect.left) break;

					dc.LineTo (iX, rect.bottom + iIncrement);
					iX += iIncrement;

					if (iX > size.cx + rect.left) break;

					dc.LineTo (iX, rect.bottom);
				}
			}
			else
			{
				TRACE (_T("Edit Control too small to draw wavy line!\r\n"));
			}
		}
	}
}
