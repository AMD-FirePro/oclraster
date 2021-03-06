/*
 *  Flexible OpenCL Rasterizer (oclraster)
 *  Copyright (C) 2012 - 2013 Florian Ziesche
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License only.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __OCLRASTER_OSX_HELPER_H__
#define __OCLRASTER_OSX_HELPER_H__

#if !defined(__MAC_10_8)
#include "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CoreGraphics.h"
#else
#include <CoreGraphics/CoreGraphics.h>
#endif

class osx_helper {
public:
	static size_t get_dpi(SDL_Window* wnd);
	static float get_scale_factor(SDL_Window* wnd);
	static float get_menu_bar_height();
	static size_t get_system_version();
	static size_t get_compiled_system_version();
	
};

#endif
