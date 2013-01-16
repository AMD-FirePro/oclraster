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

#ifndef __OCLRASTER_H__
#define __OCLRASTER_H__

#include "oclraster/global.h"
#include "core/core.h"
#include "core/file_io.h"
#include "core/event.h"
#include "core/xml.h"
#include "core/vector3.h"
#include "core/matrix4.h"
#include "core/unicode.h"

class opencl_base;
extern opencl_base* ocl;

class camera;
class OCLRASTER_API oclraster {
public:
	static void init(const char* callpath_, const char* datapath_);
	static void destroy();
	
	// graphic control functions
	static void start_draw();
	static void stop_draw();
	static void start_2d_draw();
	static void start_2d_draw(const unsigned int width, const unsigned int height);
	static void stop_2d_draw();
	static void init_gl();
	static void resize_window();
	static void swap();
	static const string get_version();
	
	// class return functions
	static event* get_event();
	static xml* get_xml();

	// miscellaneous control functions
	static void set_caption(const char* caption);
	static const char* get_caption();

	static void set_cursor_visible(const bool& state);
	static bool get_cursor_visible();
	
	static void set_data_path(const char* data_path = "../data/");
	static string get_data_path();
	static string get_call_path();
	static string get_kernel_path();
	static string data_path(const string& str);
	static string kernel_path(const string& str);
	static string strip_data_path(const string& str);
	
	static void reload_kernels();
	
	static void acquire_context();
	static void release_context();
	
	// misc position/rotation/matrix functions
	struct camera_setup {
		float3 position;
		float3 origin;
		float3 forward;
		float3 x_vec;
		float3 y_vec;
	};
	static const camera_setup& get_camera_setup();
	static void push_projection_matrix();
	static void pop_projection_matrix();
	static void push_modelview_matrix();
	static void pop_modelview_matrix();
	static matrix4f& get_projection_matrix();
	static matrix4f& get_modelview_matrix();
	static matrix4f& get_mvp_matrix();
	static matrix4f& get_translation_matrix();
	static matrix4f& get_rotation_matrix();
	static void set_position(const float& xpos, const float& ypos, const float& zpos);
	static float3& get_position();
	static void set_rotation(const float& xrot, const float& yrot);
	static float3& get_rotation();
	
	static void set_camera(camera* cam);
	static camera* get_camera();
	static void run_camera(); // TODO: move to protected again (-> add better way of manipulating the camera during a frame)
	
	// fps functions
	static unsigned int get_fps();
	static float get_frame_time();
	static bool is_new_fps_count();

	// config functions
	static xml::xml_doc& get_config_doc();
	
	// screen/window
	static SDL_Window* get_window();
	static unsigned int get_width();
	static unsigned int get_height();
	static uint2 get_screen_size();
	static bool get_fullscreen();
	static bool get_vsync();
	static bool get_stereo();
	static const size_t& get_dpi();
	
	static void set_width(const unsigned int& width);
	static void set_height(const unsigned int& height);
	static void set_screen_size(const uint2& screen_size);
	static void set_fullscreen(const bool& state);
	static void set_vsync(const bool& state);
	
	// projection
	static const float& get_fov();
	static const float2& get_near_far_plane();
	static void set_fov(const float& fov);
	static void set_upscaling(const float& upscaling);
	static const float& get_upscaling();
	
	// input
	static unsigned int get_key_repeat();
	static unsigned int get_ldouble_click_time();
	static unsigned int get_mdouble_click_time();
	static unsigned int get_rdouble_click_time();

protected:
	oclraster(const char* callpath_, const char* datapath_) = delete;
	~oclraster() = delete;
	oclraster& operator=(const oclraster&) = delete;
	
	static event* evt;
	static xml* x;
	
	static void init_internal();
	
	static struct oclraster_config {
		// screen
		size_t width = 1280, height = 720, dpi = 72;
		bool fullscreen = false, vsync = false, stereo = false;
		
		// projection
		float fov = 72.0f;
		float2 near_far_plane { 0.1f, 1000.0f }; // TODO: still necessary?
		float upscaling = 1.0f;
		
		// input
		size_t key_repeat = 200;
		size_t ldouble_click_time = 200;
		size_t mdouble_click_time = 200;
		size_t rdouble_click_time = 200;
		
		// opencl
		string opencl_platform = "0";
		bool clear_cache = false;
		set<string> cl_device_restriction;

		// sdl
		SDL_Window* wnd = nullptr;
		SDL_GLContext ctx = nullptr;
		unsigned int flags = 0;
		recursive_mutex ctx_lock;
		atomic<unsigned int> ctx_active_locks { 0 };
		
		oclraster_config() {}
	} config;
	static xml::xml_doc config_doc;
	
	// path variables
	static string datapath;
	static string rel_datapath;
	static string callpath;
	static string kernelpath;

	// transformation/positioning/rotation
	//static void run_camera();
	static camera* cam;
	static camera_setup cam_setup;
	static float3 position;
	static float3 rotation;
	static matrix4f projection_matrix;
	static matrix4f modelview_matrix;
	static matrix4f mvp_matrix;
	static matrix4f translation_matrix;
	static matrix4f rotation_matrix;
	static deque<matrix4f*> projm_stack;
	static deque<matrix4f*> mvm_stack;

	// fps counting
	static unsigned int fps;
	static unsigned int fps_counter;
	static unsigned int fps_time;
	static float frame_time;
	static unsigned int frame_time_sum;
	static unsigned int frame_time_counter;
	static bool new_fps_count;
	
	// cursor
	static bool cursor_visible;
	
	// window event handlers
	static event::handler* window_handler;
	static bool window_event_handler(EVENT_TYPE type, shared_ptr<event_object> obj);
	
	// misc
	static atomic<bool> reload_kernels_flag;

};

#endif