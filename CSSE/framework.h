#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// MFC classes.
#include <afxdialogex.h>
#include <afxpropertypage.h>
#include <afxpropertygridctrl.h>

// Standard template libraries.
#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <sstream>
#include <stack>
#include <stddef.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <future>
#include <optional>
#include <regex>

// Required Windows APIs.
#include <mmsystem.h>
#include <wtypes.h>
#include <psapi.h>
#include <dbghelp.h>
#include <d3d.h>
#include <commctrl.h>
#include <Windowsx.h>

// Required C includes.
#include <cmath>
#include <cstdint>
#include <cstring>
#include <ctime>

// 3rd party library: span-lite
// URL: https://github.com/martinmoene/span-lite
#include <nonstd/span.hpp>

// 3rd party library: toml11
#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT
#include <toml.hpp>

// DirectInput/Sound requirements.
#define DIRECTINPUT_VERSION 0x0800
#include <control.h>
#include <strmif.h>
#include <dinput.h>
#include <dsound.h>

// Core lua binding library.
//#include <sol/sol.hpp>

// Undefine anything we don't care about otherwise.
#undef far
#undef near
#undef PlaySound
#undef max
#undef min