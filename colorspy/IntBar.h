#pragma once

class CIntBar :
  public CWindowImpl<CIntBar>
{
public:
    DECLARE_WND_CLASS(_T("IntBar"));

    BEGIN_MSG_MAP_EX(CIntBar)
      MSG_WM_ERASEBKGND(OnEraseBkgnd)
      MSG_WM_PAINT(OnPaint)
      MSG_WM_LBUTTONDOWN(OnLButtonDown)
      MSG_WM_LBUTTONUP(OnLButtonUp)
      MSG_WM_MOUSEMOVE(OnMouseMove)
    END_MSG_MAP()

private:
    int m_value;
    DWORD m_barid;
    bool m_bCapture;
    bool m_bCanChange;

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
      if(m_bCanChange)
      {
        SetCapture();
        m_bCapture = true;
        ValueUpdate(point);
      }
    }
    void ValueUpdate(CPoint &point)
    {
      CRect rect;
      GetClientRect(rect);
      int v = min(max(point.x, rect.left), rect.right-1);
      v -= rect.left;
      double width = rect.Width()-1;
      double value = v;
      v = value / width * 255.0;
      ATLTRACE("v: %d\n",v);
      v=min(max(v, 0), 255);
      if(m_value!=v)
      {
        m_value = v;
        ::SendMessage(GetParent(), WM_CHANGEBARVALUE, v, m_barid);
      }
      RedrawWindow();
      UpdateWindow();
    }
    void OnMouseMove(UINT nFlags, CPoint point)
    {
      if(m_bCapture)ValueUpdate(point);
    }
    void OnLButtonUp(UINT nFlags, CPoint point)
    {
      if(m_bCapture)
      {
        ReleaseCapture();
        ValueUpdate(point);
        m_bCapture = false;
      }
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
      dc.FillSolidRect(rect, RGB(0,0,0));
      rect.right -= 1;
      rect.left += 1;
      rect.top += 1;
      rect.bottom -= 1;
      dc.FillSolidRect(rect, RGB(255,255,255));
      double width = rect.Width();
      double v = m_value;
      v /= 255.0;
      v *= width;
      int l = v;
      rect.right = rect.left+l;
      dc.FillSolidRect(rect, RGB(135,153,180));
    }

public:
    void SetIntValue(int val)
    {
      m_value = val;
      RedrawWindow();
      UpdateWindow();
    }

    void SetBarId(int barid)
    {
      m_barid = barid;
    }
    void SetCanChange(bool bCanChange)
    {
      m_bCanChange = bCanChange;
    }
  CIntBar(void) : m_value(0), m_bCapture(false), m_bCanChange(false), m_barid(-1)
  {
  }
  ~CIntBar(void)
  {
  }
};

