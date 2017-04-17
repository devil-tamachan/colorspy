#pragma once

class CPickerButton :
  public CWindowImpl<CPickerButton>
{
public:
    DECLARE_WND_CLASS(_T("PickerButton"));

    BEGIN_MSG_MAP_EX(CPickerButton)
      //MSG_WM_SETCURSOR(OnSetCursor)
      MSG_WM_ERASEBKGND(OnEraseBkgnd)
      MSG_WM_PAINT(OnPaint)
      MSG_WM_LBUTTONDOWN(OnLButtonDown)
      MSG_WM_MOUSEMOVE(OnMouseMove)
      MSG_WM_LBUTTONUP(OnLButtonUp)
    END_MSG_MAP()

private:
    CBitmap m_btnface;
    bool m_bCapture;
    DWORD m_pickid;
    CCursor m_cur;
    HCURSOR m_curOld;


    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
      SetFocus();
      SetCapture();
      m_curOld = SetCursor(m_cur);
      m_bCapture=true;
    }
    void ChangeRGB(CPoint& point)
    {
      ClientToScreen(&point);
      HDC hDesktop = ::GetDC(::GetDesktopWindow());
      COLORREF c = ::GetPixel(hDesktop, point.x, point.y);
      ReleaseDC(hDesktop);

      SendMessage(GetParent(), WM_CHANGERGB, c, m_pickid);
      ATLTRACE("ChangeRGB: 0x%08X\n", c);
    }

  void OnMouseMove(UINT nFlags, CPoint point)
  {
    if(m_bCapture)
      ChangeRGB(point);
  }

    void OnLButtonUp(UINT nFlags, CPoint point)
    {
      if(m_bCapture)
      {
        ChangeRGB(point);
      ::ReleaseCapture();
        SetCursor(m_curOld);
        m_bCapture = false;
      }
    }

    void OnPaint(CDCHandle /*dc*/)
    {
      CPaintDC dc(m_hWnd);
      CDC dcMem;
      dcMem.CreateCompatibleDC(dc);
      HBITMAP oldBitmap = dcMem.SelectBitmap(m_btnface);
      dc.BitBlt(0, 0, 16, 16, dcMem, 0, 0, SRCCOPY);
      dcMem.SelectBitmap(oldBitmap);
    }

public:
  void SetPickId(DWORD id)
  {
    m_pickid = id;
  }

  CPickerButton(void)
  {
    m_btnface.LoadBitmap(IDB_BMP_PICK1);
    m_bCapture = false;
    m_cur.LoadCursor(IDC_CUR_PICK);
  }

  ~CPickerButton(void)
  {
  }
};

