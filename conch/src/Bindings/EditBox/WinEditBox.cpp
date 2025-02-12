#include "WinEditBox.h"
#include "Application/App.h"
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>


extern HWND g_hWnd;

static HMENU IDL_EditBox = (HMENU) 100;

static LRESULT CALLBACK EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	laya::WinEditBox* editBox = (laya::WinEditBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!editBox)
		return true;

	switch (message)
	{
	default:
		return CallWindowProc(editBox->GetDefaultWndProc(), hWnd, message, wParam, lParam);
	}
	return true;
}

namespace laya {

	/// style
	void WinEditBox::Style::SetLeft(int val)
	{
		left = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetTop(int val)
	{
		top = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetWidth(int val)
	{
		width = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetHeight(int val)
	{
		height = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetFontSize(int val)
	{
		fontSize = val + 2;
		if (m_owner->IsFocus())
		{
			m_owner->UpdateFont();
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	void WinEditBox::Style::SetBgColor(COLORREF val)
	{
		bgColor = val;
		UpdatePaintOrDirty();
	}

	void WinEditBox::Style::SetFontColor(COLORREF val)
	{
		fontColor = val;
		UpdatePaintOrDirty();
	}

	void WinEditBox::Style::UpdatePaintOrDirty()
	{
		if (m_owner->IsFocus())
		{
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	void WinEditBox::Style::UpdateSizeOrDirty()
	{
		if (m_owner->IsFocus())
		{
			m_owner->UpdateSize();
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	/// WinEditBox

	WinEditBox::WinEditBox() :
		m_hSingleEditWnd(0),
		m_hMultiEditWnd(0),
		m_isFocus(false),
		m_isMultiLine(false),
		m_isInitialized(false),
		m_refCount(0),
		m_defaultWndProc(NULL)
	{
		m_style = new Style(this);
		//m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
		m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
		// Send Message for creating
		SendEditBoxCustomEvent([this]() {
			this->Init();
			});
	}

	WinEditBox::~WinEditBox()
	{
		SetWindowLongPtr(m_hSingleEditWnd, GWLP_USERDATA, (LONG)0);
		SetWindowLongPtr(m_hMultiEditWnd, GWLP_USERDATA, (LONG)0);

		delete m_style;
		m_style = nullptr;

		DestroyWindow(m_hSingleEditWnd);
		DestroyWindow(m_hMultiEditWnd);
	}
	static void getAdjustedPos(int left, int top, int width, int height, int& adjustedLeft, int& adjustedTop, int& adjustedWidth, int& adjustedHeight)
	{
		// ʹ�ø���ȷ�ı߿򲹳�
		int borderX = GetSystemMetrics(SM_CXEDGE);  // ʹ��3D�߿�Ŀ���
		int borderY = GetSystemMetrics(SM_CYEDGE);  // ʹ��3D�߿�ĸ߶�
		int padding = 2;  // ������ڱ߾�

		// ����λ�úʹ�С
		int adjustedX = adjustedLeft = left + borderX + padding;
		int adjustedY = adjustedTop = top + borderY + padding;
		int adjustedW = adjustedWidth = width - ((borderX + padding) * 2);
		int adjustedH = adjustedHeight = height - ((borderY + padding) * 2);

	}
	void WinEditBox::Init()
	{
		if (!m_isInitialized)
		{
			m_isInitialized = true;
			//m_hSingleEditWnd = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | ES_AUTOHSCROLL, m_style->left, m_style->top, m_style->width, m_style->height, g_hWnd, IDL_EditBox, (HINSTANCE)GetWindowLongPtr(g_hWnd, GWLP_HINSTANCE),  NULL);

			//m_hMultiEditWnd = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | ES_MULTILINE | ES_WANTRETURN, m_style->left, m_style->top, m_style->width, m_style->height, g_hWnd, IDL_EditBox, (HINSTANCE)GetWindowLongPtr(g_hWnd, GWLP_HINSTANCE), ,NULL);

			int adjustedLeft;
			int adjustedTop;
			int adjustedWidth;
			int adjustedHeight;
			getAdjustedPos(m_style->left, m_style->top, m_style->width, m_style->height, adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight);


			m_hSingleEditWnd = CreateWindowEx(
				0,
				L"EDIT",
				TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,  // �Ƴ�ES_CENTER
				adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight,
				g_hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);

			/*m_hMultiEditWnd = CreateWindowEx(
				0,
				L"EDIT",
				TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,  // �Ƴ�ES_CENTER
				m_style->left, m_style->top, m_style->width, m_style->height,
				g_hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);*/
			if (m_isMultiLine)
			{
				m_defaultWndProc = (WNDPROC)SetWindowLongPtr(m_hMultiEditWnd, GWLP_WNDPROC, (LONG_PTR)EditWndProc);
			}
			else
			{
				m_defaultWndProc = (WNDPROC)SetWindowLongPtr(m_hSingleEditWnd, GWLP_WNDPROC, (LONG_PTR)EditWndProc);
			}

			SetWindowLongPtr(m_hSingleEditWnd, GWLP_USERDATA, (LONG_PTR)this);
			SetWindowLongPtr(m_hMultiEditWnd, GWLP_USERDATA, (LONG_PTR)this);



		}
	}


	void WinEditBox::UpdateSize()
	{
		int adjustedLeft;
		int adjustedTop;
		int adjustedWidth;
		int adjustedHeight;
		getAdjustedPos(m_style->left, m_style->top, m_style->width, m_style->height, adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight);
		SetWindowPos(GetCurHWND(), HWND_TOP, adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight, SWP_SHOWWINDOW);
	}

	void WinEditBox::UpdateFont()
	{
		//m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
		m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
		SendMessage(GetCurHWND(), WM_SETFONT, (WPARAM)m_font, true);
	}
	void WinEditBox::SetFocus(bool isFocus)
	{
		SendEditBoxCustomEvent([isFocus, this]() {
			this->SetFocus_(isFocus);
			});
	}

	static BOOL ForceSetFocus(HWND hWnd)
	{
		HWND hParent = GetParent(hWnd);
		HWND hForeground = GetForegroundWindow();

		DWORD foregroundThreadId = GetWindowThreadProcessId(hForeground, NULL);
		DWORD currentThreadId = GetCurrentThreadId();

		AttachThreadInput(currentThreadId, foregroundThreadId, TRUE);

		SetActiveWindow(hParent);
		SetForegroundWindow(hParent);

		SetFocus(hWnd);
		AttachThreadInput(currentThreadId, foregroundThreadId, FALSE);

		return TRUE;
	}
	static void SetCaretToEnd(HWND hEdit)
	{
		int length = GetWindowTextLength(hEdit);
		SendMessage(hEdit, EM_SETSEL, length, length);
	}
	static std::wstring CrossThreadGetText(HWND hEdit)
	{
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		if (length == 0)
			return std::wstring();

		std::wstring buffer;
		buffer.resize(length + 1);
		SendMessage(hEdit, WM_GETTEXT, length + 1, (LPARAM)&buffer[0]);
		return buffer;
	}
	void WinEditBox::SetFocus_(bool isFocus)
	{
		if (m_isFocus == isFocus)
		{
			return;
		}
		m_isFocus = isFocus;
		if (!GetCurHWND())
			m_isFocus = false;


		if (m_style->isDirty)
		{
			UpdateSize();
			UpdateFont();
			m_style->isDirty = false;
		}


		/*if (m_isFocus)
		{
			SetText(m_text.c_str());
		}*/

		if (m_isFocus)
		{
			int adjustedLeft;
			int adjustedTop;
			int adjustedWidth;
			int adjustedHeight;
			getAdjustedPos(m_style->left, m_style->top, m_style->width, m_style->height, adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight);
			ShowWindow(GetCurHWND(), true);
			SetWindowPos(GetCurHWND(), HWND_TOP, adjustedLeft, adjustedTop, adjustedWidth, adjustedHeight, SWP_SHOWWINDOW);
			ForceSetFocus(GetCurHWND());
			SetCaretToEnd(GetCurHWND());
			
		}
		else
		{
			::SetFocus(NULL);
			ShowWindow(GetCurHWND(), SW_HIDE);
		}

		ForceUpdateWindow();
	}

	void WinEditBox::SetMutiLine(bool val)
	{
		if (m_isMultiLine == val)
			return;

		SetWindowLongPtr(GetCurHWND(), GWLP_WNDPROC, (LONG)m_defaultWndProc);
		m_isMultiLine = val;

		m_defaultWndProc = (WNDPROC) SetWindowLongPtr(GetCurHWND(), GWLP_WNDPROC, (LONG)EditWndProc);

		if (m_isFocus)
		{
			ShowWindow(m_hSingleEditWnd, !m_isMultiLine);
			ShowWindow(m_hMultiEditWnd, m_isMultiLine);
		}

		ForceUpdateWindow();
	}

	void WinEditBox::ForceUpdateWindow()
	{
		RECT r;
		GetClientRect(GetCurHWND(), &r);
		InvalidateRect(GetCurHWND(), &r, true);
		UpdateWindow(GetCurHWND());

	}

	std::string WinEditBox::GetText()
	{
		std::wstring text = CrossThreadGetText(GetCurHWND());

		return wideToUtf8(text);
	}

	void WinEditBox::SetText(const std::string& text)
	{

		SendEditBoxCustomEvent([this, text]() {
			SetWindowTextW(GetCurHWND(), utf8ToWide(text).c_str());
			SetCaretToEnd(GetCurHWND());
			if (m_isFocus)
			{
				ForceUpdateWindow();
			}
		});
	}

	void WinEditBox::Retain()
	{
		m_refCount++;
	}

	bool WinEditBox::Release()
	{
		m_refCount--;
		if (m_refCount <= 0)
		{
			delete this;
			return true;
		}

		return false;
	}
};