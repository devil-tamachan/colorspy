#pragma once

class CColorView :
  public CWindowImpl<CColorView>
{
public:
    DECLARE_WND_CLASS(_T("ColorView"));

    BEGIN_MSG_MAP_EX(CColorView)
      MSG_WM_ERASEBKGND(OnEraseBkgnd)
      MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

private:
    COLORREF m_color;

    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }
    void OnPaint(CDCHandle /*dc*/)
    {
      CPaintDC dc(m_hWnd);
      CRect rect;
      GetClientRect(rect);
      dc.FillSolidRect(rect, m_color);
    }

public:
    void SetColor(COLORREF c)
    {
      m_color = c;
      RedrawWindow();
      UpdateWindow();
    }

  CColorView(void): m_color(0)
  {
  }

  ~CColorView(void)
  {
  }
};

