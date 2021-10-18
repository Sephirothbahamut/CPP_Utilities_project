#pragma once

////////////////// NOTE: Your project must include "Dwmapi.lib" and "psapi.lib"

#ifdef _WIN32

	#ifdef NOMINMAX
		#include <windows.h>
		#include <conio.h>
	#else
		#define NOMINMAX
		#include <windows.h>
		#include <conio.h>
		#undef NOMINMAX
	#endif

	#include <WinUser.h>
	#include <dwmapi.h>
	#pragma comment (lib, "dwmapi.lib")//without this dwmapi.h doesn't work :shrugs: no idea whatsoever where the compiler is taking this file from
	#include <tchar.h>
	#include <Shlwapi.h>

	#include "../../private/SetWindowCompositionAttribute.h"

	namespace utils::window
		{
		bool make_glass_CompositionAttribute(HWND hwnd)
			{
			if (HMODULE hUser = GetModuleHandleA("user32.dll"))
				{
				//Windows >= 10
				pfnSetWindowCompositionAttribute SetWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
				if (SetWindowCompositionAttribute)
					{
					ACCENT_POLICY accent = {ACCENT_ENABLE_BLURBEHIND, 0, 0, 0};

					WINDOWCOMPOSITIONATTRIBDATA data;
					data.Attrib = WCA_ACCENT_POLICY;
					data.pvData = &accent;
					data.cbData = sizeof(accent);
					SetWindowCompositionAttribute(hwnd, &data);
					return true;
					}
				}
			return false;
			}
		bool make_glass_DWM_BlurBehind(HWND hwnd)
			{
			DWM_BLURBEHIND bb = {0};
			bb.dwFlags = DWM_BB_ENABLE;
			bb.fEnable = true;
			bb.hRgnBlur = NULL;
			HRESULT result = DwmEnableBlurBehindWindow(hwnd, &bb);
			return result == S_OK;
			}
		bool make_glass_DWM_margin(HWND hwnd)
			{
			// Negative margins have special meaning to DwmExtendFrameIntoClientArea.
			// Negative margins create the "sheet of glass" effect, where the client area
			// is rendered as a solid surface with no window border.
			MARGINS margins = {-1};
			HRESULT hr = S_OK;

			// Extend the frame across the entire window.
			hr = DwmExtendFrameIntoClientArea(hwnd, &margins);
			return (SUCCEEDED(hr));
			}

		bool make_transparent_Layered(HWND hWnd, BYTE opacity = 100)
			{
			//Transparent window
			SetWindowLong(hWnd, GWL_EXSTYLE,
				GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

			//HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
			//SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)brush);

			return SetLayeredWindowAttributes(hWnd, 0, opacity, LWA_ALPHA);
			}
		}

#else
#error "window.h is Windows-only"
#endif