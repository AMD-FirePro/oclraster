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

#include <Cocoa/Cocoa.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "core/vector2.h"
#include "core/logger.h"
#include "core/util.h"
#include "osx_helper.h"

size_t osx_helper::get_dpi(SDL_Window* wnd) {
	float2 display_res(float2(CGDisplayPixelsWide(CGMainDisplayID()),
							  CGDisplayPixelsHigh(CGMainDisplayID())) * get_scale_factor(wnd));
	const CGSize display_phys_size(CGDisplayScreenSize(CGMainDisplayID()));
	const float2 display_dpi((display_res.x / display_phys_size.width) * 25.4f,
							 (display_res.y / display_phys_size.height) * 25.4f);
	return floorf(std::max(display_dpi.x, display_dpi.y));
}

float osx_helper::get_scale_factor(SDL_Window* wnd) {
	SDL_SysWMinfo wm_info;
	SDL_VERSION(&wm_info.version);
	if(SDL_GetWindowWMInfo(wnd, &wm_info) == 1) {
		return [wm_info.info.cocoa.window backingScaleFactor];
	}
	return 1.0f;
}

float osx_helper::get_menu_bar_height() {
	return [[[NSApplication sharedApplication] mainMenu] menuBarHeight];
}

size_t osx_helper::get_system_version() {
	// add a define for the run time os x version
	string osrelease(16, 0);
	size_t size = osrelease.size() - 1;
	sysctlbyname("kern.osrelease", &osrelease[0], &size, nullptr, 0);
	osrelease.back() = 0;
	const size_t major_dot = osrelease.find(".");
	const size_t minor_dot = (major_dot != string::npos ? osrelease.find(".", major_dot + 1) : string::npos);
	if(major_dot != string::npos && minor_dot != string::npos) {
		const size_t major_version = string2size_t(osrelease.substr(0, major_dot));
		const size_t osx_minor_version = string2size_t(osrelease.substr(major_dot + 1, major_dot - minor_dot - 1));
		const size_t osx_major_version = major_version - 4; // osrelease = kernel version, not os x version -> substract 4
		const string osx_major_version_str = size_t2string(osx_major_version);
		
		// mimic the compiled version string (1xxy, xx = major, y = minor)
		size_t condensed_version = 1000;
		// this should work for all 10.x releases and possibly also for future 11.x or 10.10+ releases
		// -> double digit major version (09 for 10.9, 08 for 10.8, ...); future: 10 (10.10/11.0), ...
		condensed_version += osx_major_version * 10;
		// -> single digit minor version (cut off at 9, this should probably suffice)
		condensed_version += (osx_minor_version < 10 ? osx_minor_version : 9);
		return condensed_version;
	}
	
	oclr_error("unable to retrieve os x version!");
	return 1070; // just return lowest version
}

size_t osx_helper::get_compiled_system_version() {
	// this is a number: 1090 (10.9), 1080 (10.8), ...
	return MAC_OS_X_VERSION_MAX_ALLOWED;
}
