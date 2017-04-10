#pragma once

class CZoomView :
  public CWindowImpl<CZoomView>
{
public:
    DECLARE_WND_CLASS(_T("ZoomView"));

    BEGIN_MSG_MAP_EX(CZoomView)
      MSG_WM_TIMER(OnTimer)
      MSG_WM_ERASEBKGND(OnEraseBkgnd)
      MSG_WM_PAINT(OnPaint)
      MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    void OnTimer(UINT_PTR nIDEvent)
    {
      RedrawWindow();
      UpdateWindow();
    }

    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }
    void OnPaint(CDCHandle /*dc*/)
    {
      CPaintDC dc(m_hWnd);
      CRect rect;
      GetClientRect(rect);
      CMemoryDC memdc(dc, rect);
      memdc.FillSolidRect(rect, RGB(0,0,0));
      HDC hDesktop = ::GetDC(::GetDesktopWindow());
      POINT pt;
      GetCursorPos(&pt);
      int w = rect.Width();
      int h = rect.Height();
      int w2 = w/13;
      int h2 = h/13;
      int targetsize, targetleft, targettop;
      if(w2>h2)
      {
        targetsize = w2;
      } else {
        targetsize = h2;
      }
      targetleft = w/2 - targetsize/2;
      targettop = h/2 - targetsize/2;
      int dx = targetleft-6*targetsize;
      int dy = targettop -6*targetsize;
      StretchBlt(memdc, dx, dy, targetsize*13, targetsize*13, hDesktop, pt.x-6, pt.y-6, 13, 13, SRCCOPY);
      ReleaseDC(hDesktop);
      //dc.FillSolidRect(rect, m_color);
      int oldRop2 = memdc.SetROP2(R2_NOT);
      memdc.MoveTo(targetleft, targettop);
      memdc.LineTo(targetleft+targetsize, targettop);
      memdc.MoveTo(targetleft, targettop);
      memdc.LineTo(targetleft, targettop+targetsize);
      memdc.MoveTo(targetleft+targetsize, targettop+targetsize);
      memdc.LineTo(targetleft, targettop+targetsize);
      memdc.MoveTo(targetleft+targetsize, targettop+targetsize);
      memdc.LineTo(targetleft+targetsize, targettop);
      memdc.SetROP2(oldRop2);
    }
    void OnDestroy()
    {
      SetMsgHandled(FALSE);
      KillTimer(ID_TIMER0);
    }
    void Init()
    {
      SetTimer(ID_TIMER0, 100, NULL);
    }

  CZoomView(void)
  {
  }

  ~CZoomView(void)
  {
  }
};

