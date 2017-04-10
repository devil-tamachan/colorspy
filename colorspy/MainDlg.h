// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ColorCtrlWrapper.h"
#include "ZoomView.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>, public CDialogResize<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

  CColorCtrlWrapper m_color1;
  CColorCtrlWrapper m_color2;
  CColorCtrlWrapper m_color3;
  CZoomView m_zoomview;

  CMainDlg()
  {
  }

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}
  
  BEGIN_DDX_MAP(CMainDlg)
    DDX_CONTROL(IDC_STATIC1, m_color1)
    DDX_CONTROL(IDC_STATIC2, m_color2)
    DDX_CONTROL(IDC_STATIC3, m_color3)
    DDX_CONTROL(IDS_ZOOM, m_zoomview)
  END_DDX_MAP()
  
  BEGIN_DLGRESIZE_MAP(CMainDlg)
    BEGIN_DLGRESIZE_GROUP()
     DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_X/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDC_STATIC2, DLSZ_SIZE_X/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDC_STATIC3, DLSZ_SIZE_X/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDS_ZOOM, DLSZ_SIZE_X/*|DLSZ_REPAINT*/)
    END_DLGRESIZE_GROUP()
     DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_Y/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDC_STATIC2, DLSZ_SIZE_Y/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDC_STATIC3, DLSZ_SIZE_Y/*|DLSZ_REPAINT*/)
     DLGRESIZE_CONTROL(IDS_ZOOM, DLSZ_SIZE_Y/*|DLSZ_REPAINT*/)
  END_DLGRESIZE_MAP()


	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP_EX(CMainDlg)
		MESSAGE_HANDLER(WM_CHANGERGB, OnChangeRGB)
    MSG_WM_CLOSE(OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()


  COLORREF DivideCOLORREF(COLORREF c1, COLORREF c3)
  {
    double d1, d3;
    int r,g,b;
    d1 = GetRValue(c1);
    d3 = GetRValue(c3);
    if(d1==0.0)d3=0.0;
    else d3/=d1;
    r = min(max(d3*255.0,0.0), 255.0);

    d1 = GetGValue(c1);
    d3 = GetGValue(c3);
    if(d1==0.0)d3=0.0;
    else d3/=d1;
    g = min(max(d3*255.0,0.0), 255.0);

    d1 = GetBValue(c1);
    d3 = GetBValue(c3);
    if(d1==0.0)d3=0.0;
    else d3/=d1;
    b = min(max(d3*255.0,0.0), 255.0);
    return RGB(r,g,b);
  }
  
	LRESULT OnChangeRGB(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  {
    ATLTRACE("OnChangeRGB\n");
    COLORREF c1 = m_color1.GetRGB();
    COLORREF c3 = m_color3.GetRGB();
    m_color2.SetRGB(DivideCOLORREF(c1,c3));
    return 1;
  }
  BOOL OnEraseBkgnd(CDCHandle /*dc*/)
  {
    return 1;
  }
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

    DoDataExchange(FALSE);

    /*CIcon icon1;
    icon1.LoadIconW(IDI_PICK1);
    CImageList il;
    //il.Create(32, 32, ILC_COLOR8|ILC_MASK, 1, 0);
    //il.AddIcon(icon1);
    il.CreateFromImage(IDB_BMP_PICK1, 16, 1, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION);
    m_button_pick1.SetImageList(il);
    m_button_pick1.SetImages(0, 1, 2, 3);
    //m_button_pick1.SizeToImage();
    //m_button_pick1.SetIcon(icon1);
    LONG_PTR exstyle = m_button_pick1.GetWindowLongPtr(GWL_EXSTYLE);
    exstyle &= ~WS_EX_NOPARENTNOTIFY;
    m_button_pick1.SetWindowLongPtr(GWL_EXSTYLE, exstyle);*/

    m_color1.Init();
    m_color1.SetPickId(0);
    m_color2.Init();
    m_color2.SetPickId(1);
    m_color2.SetPickEnable(false);
    m_color3.Init();
    m_color3.SetPickId(2);

    m_zoomview.ShowWindow(SW_SHOW);
    m_zoomview.Init();

		UIAddChildWindowContainer(m_hWnd);
    DlgResize_Init(true, true, WS_THICKFRAME | WS_MAXIMIZEBOX/* | WS_CLIPCHILDREN*/);
    SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

  //LRESULT OnChangeRGB(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
  //{
  //  if(lParam!=0 && lParam!=2)return 1;

  //  {
  //    COLORREF c = (COLORREF)wParam;

  //    double hsv[3];
  //    RGBConverter::rgbToHsv(GetRValue(c), GetGValue(c), GetBValue(c), hsv);
  //    //int h = hsv[0]*255.0;
  //    //int s = hsv[1]*255.0;
  //    //int v = hsv[2]*255.0;
  //    if(lParam==0)
  //    {
  //      m_hsvview1.SetRGB(c);
  //      m_col1.SetColor(c);
  //      m_intbarCtrlSet1.SetHSV(hsv);
  //    } else if(lParam==2) {
  //      m_hsvview3.SetRGB(c);
  //      m_col3.SetColor(c);
  //      m_intbarCtrlSet3.SetHSV(hsv);
  //    }
  //    DoDataExchange(FALSE);
  //  }
  //  return 1;
  //}

  void OnClose()
  {
		DestroyWindow();
		::PostQuitMessage(0);
  }

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
