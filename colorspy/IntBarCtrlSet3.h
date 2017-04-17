#pragma once

#include "IntBarCtrl.h"

class CIntBarCtrlSet3 :
  public CWindowImpl<CIntBarCtrlSet3>
{
public:
    DECLARE_WND_CLASS(_T("IntBarCtrlSet3"));

    BEGIN_MSG_MAP_EX(CIntBarCtrlSet3)
      //MSG_WM_ERASEBKGND(OnEraseBkgnd)
      //MSG_WM_PAINT(OnPaint)
      MSG_WM_SIZE(OnSize)
    END_MSG_MAP()

    CIntBarCtrl m_intbar1;
    CIntBarCtrl m_intbar2;
    CIntBarCtrl m_intbar3;
    int m_modeColorSpace; //0-HSV, 1-RGB, 2-RGB(Linear)
    int m_modeDiff; //0-raw, 1-diff, 2-diff(%)
    COLORREF m_color;
    COLORREF m_colorDiffBase;
    
    BOOL OnEraseBkgnd(CDCHandle /*dc*/)
    {
      return 1;
    }
    void OnSize(UINT nType, CSize size)
    {
      //m_bar1.ResizeClient(size.cx, 14);
      m_intbar1.SetWindowPos(NULL, 0, 0, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
      m_intbar2.SetWindowPos(NULL, 0, 24, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
      m_intbar3.SetWindowPos(NULL, 0, 48, size.cx, 23, SWP_NOMOVE|SWP_NOZORDER);
    }

    void SetLabels()
    {
      m_intbar1.SetLabel(m_modeColorSpace==0 ? _T("H:") : _T("R:"));
      m_intbar2.SetLabel(m_modeColorSpace==0 ? _T("S:") : _T("G:"));
      m_intbar3.SetLabel(m_modeColorSpace==0 ? _T("V:") : _T("B:"));
    }

  void Init()
  {
    CRect rect;
    GetClientRect(rect);
    int w = rect.Width();
    m_intbar1.Create(m_hWnd);
    m_intbar1.SetWindowPos(NULL, 0, 0, w, 23, SWP_NOMOVE|SWP_NOZORDER);
    m_intbar2.Create(m_hWnd);
    m_intbar2.SetWindowPos(NULL, 0, 24, w, 23, SWP_NOZORDER);
    m_intbar3.Create(m_hWnd);
    m_intbar3.SetWindowPos(NULL, 0, 48, w, 23, SWP_NOZORDER);
    SetLabels();
  }

  void SetCOLORREF(COLORREF c)
  {
    m_color = c;
    ATLTRACE("SetCOLORREF\n");
    UpdateColor();
  }

  void SetDiffCOLORREF(COLORREF c)
  {
    m_colorDiffBase = c;
    ATLTRACE("SetDiffCOLORREF\n");
    UpdateColor();
  }
  ATL::CString GetPercentStr(double d)
  {
    ATL::CString str;
    str.Format(_T("%.2f%%"), d);
    return str;
  }
  int diffOptInt(int i, int num)
  {
    int m = num/2;
    int a = abs(i);
    if(a<m)return i;
    
    if(i>0)return i-num;
    else return i+num;
  }
  double diffOptDouble(double d)
  {
    double a = abs(d);
    if(a<0.5)return d;
    
    if(d>0.0)return d-1.0;
    else return d+1.0;
  }
  void UpdateColorHSV()
  {
      double hsv[3];
      RGBConverter::rgbToHsv(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color), hsv);
      switch(m_modeDiff)
      {
      case 0:
        {
        m_intbar1.SetDoubleValue(hsv[0], 359.0);
        m_intbar2.SetDoubleValue(hsv[1]);
        m_intbar3.SetDoubleValue(hsv[2]);
        }
        break;
      case 1: 
        {
        double hsv2[3];
        RGBConverter::rgbToHsv(GetRValue(m_colorDiffBase), GetGValue(m_colorDiffBase), GetBValue(m_colorDiffBase), hsv2);
        m_intbar1.SetDoubleValueDiff(hsv[0], diffOptInt((hsv[0]-hsv2[0])*359.0, 360));
        m_intbar2.SetDoubleValueDiff(hsv[1], diffOptInt((hsv[1]-hsv2[1])*255.0, 256));
        m_intbar3.SetDoubleValueDiff(hsv[2], diffOptInt((hsv[2]-hsv2[2])*255.0, 256));
        }
        break;
      case 2:
        {
        double hsv2[3];
        RGBConverter::rgbToHsv(GetRValue(m_colorDiffBase), GetGValue(m_colorDiffBase), GetBValue(m_colorDiffBase), hsv2);
        double d = diffOptDouble(hsv[0]-hsv2[0]);
        m_intbar1.SetStrValue(hsv[0], GetPercentStr(d));
        d = diffOptDouble(hsv[1]-hsv2[1]);
        m_intbar2.SetStrValue(hsv[1], GetPercentStr(d));
        d = diffOptDouble(hsv[2]-hsv2[2]);
        m_intbar3.SetStrValue(hsv[2], GetPercentStr(d));
        }
        break;
      }
  }
  void UpdateColorRGB()
  {
      switch(m_modeDiff)
      {
      case 0:
        m_intbar1.SetIntValue(GetRValue(m_color));
        m_intbar2.SetIntValue(GetGValue(m_color));
        m_intbar3.SetIntValue(GetBValue(m_color));
        break;
      case 1:
        {
        int c = GetRValue(m_color);
        int c2 = GetRValue(m_colorDiffBase);
        m_intbar1.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        ATLTRACE("%d, %d\n", c, c2);
        c = GetGValue(m_color);
        c2 = GetGValue(m_colorDiffBase);
        m_intbar2.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        c = GetBValue(m_color);
        c2 = GetBValue(m_colorDiffBase);
        m_intbar3.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        }
        break;
      case 2:
        {
        double d2 = GetRValue(m_color);
        d2/=255.0;
        double d = GetRValue(m_colorDiffBase);
        d/=255.0;
        m_intbar1.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));

        d2 = GetGValue(m_color);
        d2/=255.0;
        d = GetGValue(m_colorDiffBase);
        d/=255.0;
        m_intbar2.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));

        d2 = GetBValue(m_color);
        d2/=255.0;
        d = GetBValue(m_colorDiffBase);
        d/=255.0;
        m_intbar3.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));
        }
        break;
      }
  }
  int GetLinearByte(BYTE c)
  {
    double d = c;
    d/=255.0;
    d = pow(d, 2.2);
    d*=255.0;
    return clampDouble(d, 255.0);
  }
  void UpdateColorRGBL()
  {
      switch(m_modeDiff)
      {
      case 0:
        {
        m_intbar1.SetIntValue(GetLinearByte(GetRValue(m_color)));
        m_intbar2.SetIntValue(GetLinearByte(GetGValue(m_color)));
        m_intbar3.SetIntValue(GetLinearByte(GetBValue(m_color)));
        }
        break;
      case 1:
        {
        int c = GetLinearByte(GetRValue(m_color));
        int c2 = GetLinearByte(GetRValue(m_colorDiffBase));
        m_intbar1.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        ATLTRACE("%d, %d\n", c, c2);
        c = GetLinearByte(GetGValue(m_color));
        c2 = GetLinearByte(GetGValue(m_colorDiffBase));
        m_intbar2.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        c = GetLinearByte(GetBValue(m_color));
        c2 = GetLinearByte(GetBValue(m_colorDiffBase));
        m_intbar3.SetIntValueDiff(c, diffOptInt(c - c2, 256));
        }
        break;
      case 2:
        {
        double d2 = GetLinearByte(GetRValue(m_color));
        d2/=255.0;
        double d = GetLinearByte(GetRValue(m_colorDiffBase));
        d/=255.0;
        m_intbar1.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));

        d2 = GetLinearByte(GetGValue(m_color));
        d2/=255.0;
        d = GetLinearByte(GetGValue(m_colorDiffBase));
        d/=255.0;
        m_intbar2.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));

        d2 = GetLinearByte(GetBValue(m_color));
        d2/=255.0;
        d = GetLinearByte(GetBValue(m_colorDiffBase));
        d/=255.0;
        m_intbar3.SetStrValue(d2, GetPercentStr(diffOptDouble(d2-d)));
        }
        break;
      }
  }

  void UpdateColor()
  {
    switch(m_modeColorSpace)
    {
    case 0:
      UpdateColorHSV();
      break;
    case 1:
      UpdateColorRGB();
      break;
    case 2:
      UpdateColorRGBL();
      break;
    }
  }

  void SetColorSpace(int modeColorSpace)
  {
    m_modeColorSpace = modeColorSpace;
    SetLabels();
    UpdateColor();
  }

  void SetDiffMode(int diffMode)
  {
    m_modeDiff = diffMode;
    SetLabels();
    UpdateColor();
  }

  CIntBarCtrlSet3(void) : m_modeColorSpace(0), m_modeDiff(0), m_color(0), m_colorDiffBase(0)
  {
  }

  ~CIntBarCtrlSet3(void)
  {
  }
};

