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

#ifndef __OCLRASTER_CUDACL_TRANSLATOR_H__
#define __OCLRASTER_CUDACL_TRANSLATOR_H__

#if defined(OCLRASTER_CUDA_CL)

#include "oclraster/global.h"
#include "hash/city.h"

enum class CUDACL_PARAM_ADDRESS_SPACE : unsigned int {
	NONE,
	GLOBAL,
	LOCAL,
	CONSTANT,
};
enum class CUDACL_PARAM_TYPE : unsigned int {
	NONE,
	BUFFER,
	IMAGE_1D,
	IMAGE_2D,
	IMAGE_3D,
	SAMPLER,
};
enum class CUDACL_PARAM_ACCESS : unsigned int {
	NONE,
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE,
};

struct OCLRASTER_API cudacl_kernel_info {
	// name, address space, type, access
	typedef tuple<string, CUDACL_PARAM_ADDRESS_SPACE, CUDACL_PARAM_TYPE, CUDACL_PARAM_ACCESS> kernel_param;
	
	string name;
	vector<kernel_param> parameters;
	
	const string& get_parameter_name(const size_t& index) const {
		return get<0>(parameters[index]);
	}
	const CUDACL_PARAM_ADDRESS_SPACE& get_parameter_address_space(const size_t& index) const {
		return get<1>(parameters[index]);
	}
	const CUDACL_PARAM_TYPE& get_parameter_type(const size_t& index) const {
		return get<2>(parameters[index]);
	}
	const CUDACL_PARAM_ACCESS& get_parameter_access(const size_t& index) const {
		return get<3>(parameters[index]);
	}
	
	cudacl_kernel_info(const string& kernel_name, const vector<kernel_param>& params) : name(kernel_name), parameters(params) {}
};

extern void OCLRASTER_API cudacl_translate(const string& cl_source,
										   const string& preprocess_options,
										   string& cuda_source,
										   vector<cudacl_kernel_info>& kernels,
										   const bool use_cache,
										   bool& found_in_cache,
										   uint128& kernel_hash,
										   std::function<bool(const uint128&)> hash_lookup);

#endif
#endif
