// stdafx.cpp : source file that includes just the standard includes
//	colorspy.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

const TCHAR* strDiffMode[] = { _T("RAW"), _T("Diff"), _T("Diff(%)"), _T("%") };
const TCHAR* strColorSpaceMode[] = { _T("HSV"), _T("RGB"), _T("RGBL") };

double clampDouble(double d, double max /* = 1.0 */)
{
  if(d<0.0)return 0.0;
  else if(d>max)return max;
  return d;
}

int clampBYTE(int i)
{
  if(i<0)return 0;
  else if(i>255)return 255;
  return i;
}
