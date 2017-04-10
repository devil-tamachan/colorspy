#pragma once

class CHSVView :
  public CWindowImpl<CHSVView>
{
public:

  CHSVView(void)
  {
    m_rgb[0] = 0;
    m_rgb[1] = 0;
    m_rgb[2] = 0;
    m_huebarheight = -1;
    m_hsvwidth = -1;
    m_hsvheight = -1;
    m_hsvhue = -1.0;
    MakeBitmapInfoHeader();
  }

  ~CHSVView(void)
  {
  }
public:
    DECLARE_WND_CLASS(_T("HSVView"));

    // メッセージマップ
    BEGIN_MSG_MAP_EX(CHSVView)
      MSG_WM_ERASEBKGND(OnEraseBkGnd)
		  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
      MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

private:
    BYTE m_rgb[3];

    CAutoVectorPtr<BYTE> m_huebar;
    int m_huebarheight;

    CAutoVectorPtr<BYTE> m_hsv;
    int m_hsvwidth;
    int m_hsvheight;
    double m_hsvhue;

    BITMAPINFO m_bmpInfo;

	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    return (LRESULT)GetStockObject(NULL_BRUSH);
  }

    BOOL OnEraseBkGnd(CDCHandle dc)
    {
      return 1;
    }

    bool RemakeHSV(int w, int h, double width, double height, double *hsv)
    {
        if(w==m_hsvwidth && h==m_hsvheight && m_rgb[0]==m_hsvhue)return true;

        int padding = (4-((w*3) & 0x3) & 0x3);
        m_hsv.Free();
        if(!m_hsv.Allocate(((w*4)+padding)*h))return false;
        BYTE *hsvPixel = m_hsv;
        BYTE *nextPixel = hsvPixel;
        
        BYTE rgb[3];
        for(int i=0;i<h;i++)
        {
          double v = h-i;
          v/=height;
          for(int k=0;k<w;k++)
          {
            double s = k;
            s/=width;
            RGBConverter::hsvToRgb(hsv[0], s, v, rgb);
            //dc.SetPixel(k, i, RGB(rgb[0], rgb[1], rgb[2]));
          nextPixel[0] = rgb[2];
          nextPixel[1] = rgb[1];
          nextPixel[2] = rgb[0];
          nextPixel+=3;
          }
          nextPixel+=padding;
        }

        m_hsvwidth = w;
        m_hsvheight = h;
        m_hsvhue = hsv[0];
        return true;
    }
    bool RemakeHueBar(int h, double height)
    {
        if(h==m_huebarheight)return true;

        m_huebar.Free();
        if(!m_huebar.Allocate(h*4))return false;
        BYTE *huebarPixel = m_huebar;

        BYTE *nextPixel = huebarPixel;
        BYTE rgb[3];
        for(int i=0;i<h;i++)
        {
          double hue = h-i;
          hue/=height;
          RGBConverter::hsvToRgb(hue, 1.0, 1.0, rgb);
          nextPixel[0] = rgb[2];
          nextPixel[1] = rgb[1];
          nextPixel[2] = rgb[0];
          nextPixel+=3+1/*padding*/;
        }
        m_huebarheight = h;
        return true;
    }

    void MakeBitmapInfoHeader()
    {
        BITMAPINFOHEADER *bmpHeader = &(m_bmpInfo.bmiHeader);
        bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
        //bmpHeader->biWidth = w;
        //bmpHeader->biHeight = -h;
        bmpHeader->biPlanes = 1;
        bmpHeader->biBitCount = 24;
        bmpHeader->biCompression = BI_RGB;
        bmpHeader->biSizeImage = 0;
        bmpHeader->biXPelsPerMeter = 0;
        bmpHeader->biYPelsPerMeter = 0;
        bmpHeader->biClrUsed = 0;
        bmpHeader->biClrImportant = 0;
    }
    
    void OnPaint(CDCHandle /*dc*/)
    {
        CPaintDC dc(m_hWnd);
        /*CRect r1;
        dc.GetClipBox(r1);
        ATLTRACE("%d, %d, %d, %d\n", r1.left, r1.top, r1.right, r1.bottom);*/
        CRect rect;
        GetClientRect(rect);
        CRgn rgn;
        rgn.CreateRectRgnIndirect(rect);
        dc.SelectClipRgn(rgn);
        int huebarwidth = rect.Width()*0.1;
        int huebarwidthpadding = huebarwidth*0.1;
        rect.right -= huebarwidth;
        huebarwidth -= huebarwidthpadding;

        int w = rect.Width();
        double width = w;
        int h = rect.Height();
        double height = h;

        double hsv[3];
        RGBConverter::rgbToHsv(m_rgb[0], m_rgb[1], m_rgb[2], hsv);

        BITMAPINFOHEADER *bmpHeader = &(m_bmpInfo.bmiHeader);
        bmpHeader->biWidth = w;
        bmpHeader->biHeight = -h;

        if(RemakeHSV(w, h, width, height, hsv))
          dc.StretchDIBits(0, 0, w, h, 0, 0, w, h, (void*)((BYTE *)m_hsv), &m_bmpInfo, DIB_RGB_COLORS, SRCCOPY);
        

        bmpHeader->biWidth = 1;
        //bmpHeader->biHeight = -h;
        
        if(RemakeHueBar(h, height))
          dc.StretchDIBits(rect.Width()+huebarwidthpadding, 0, huebarwidth, h, 0, 0, 1, h, (void*)((BYTE *)m_huebar), &m_bmpInfo, DIB_RGB_COLORS, SRCCOPY);

        dc.FillSolidRect(w, 0, huebarwidthpadding, h, RGB(0,0,0));
        
        int oldRop2 = dc.SetROP2(R2_NOT);
        DrawHSVTarget(dc, hsv, w, width, height);
        DrawHueBarTarget(dc, hsv, rect, height, huebarwidth, huebarwidthpadding);
        dc.SetROP2(oldRop2);
    }
    void DrawHSVTarget(CPaintDC &dc, double *hsv, int w, double width, double height)
    {
        int x, y;
        x = hsv[1]*width;
        y = (1.0-hsv[2])*height;
        int size = w*0.05;

        dc.MoveTo(x-size, y-size);
        dc.LineTo(x+size, y+size);
        dc.MoveTo(x-size, y+size);
        dc.LineTo(x+size, y-size);
    }
    void DrawHueBarTarget(CPaintDC &dc, double *hsv, CRect &rect, double height, int huebarwidth, int huebarwidthpadding)
    {
        int size = huebarwidth*0.6;
        int x = rect.Width()+huebarwidthpadding;
        int y = (1.0-hsv[0])*height;

        dc.MoveTo(x, y);
        dc.LineTo(x+size, y+size);
        dc.MoveTo(x, y);
        dc.LineTo(x+size, y-size);
        dc.MoveTo(x+size, y+size);
        dc.LineTo(x+size, y-size);
    }

public:
    void SetRGB(COLORREF c)
    {
      m_rgb[0] = GetRValue(c);
      m_rgb[1] = GetGValue(c);
      m_rgb[2] = GetBValue(c);
      RedrawWindow();
      UpdateWindow();
    }
};

