
#if defined(OCLRASTER_TRANSFORM_PROGRAM)
//////////////////////////////////////////////////////////////////
// transform program

oclraster_in simple_input {
	float2 vertex;
} input_attributes;

oclraster_uniforms transform_uniforms {
	mat4 modelview_matrix;
} tp_uniforms;

float4 transform_main() {
	return mat4_mul_vec4(tp_uniforms->modelview_matrix,
						 (float4)(input_attributes->vertex, 0.0f, 1.0f));
}

#elif defined(OCLRASTER_RASTERIZATION_PROGRAM)
//////////////////////////////////////////////////////////////////
// rasterization program

oclraster_uniforms rasterize_uniforms {
	float4 color;
} rp_uniforms;

oclraster_framebuffer {
	image2d color;
	depth_image depth;
};

void rasterization_main() {
	// standard and non-texture options
	float4 color = rp_uniforms->color;
	
	// -> output
	framebuffer->color.xyz = linear_blend(framebuffer->color.xyz, color.xyz, color.w);
	framebuffer->color.w = color.w + (framebuffer->color.w * (1.0f - color.w));
}

#endif
