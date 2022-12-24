find_package(Vulkan REQUIRED COMPONENTS glslc)

## find all the shader files under the shaders folder
file(GLOB_RECURSE GLSL_SOURCE_FILES
     "${PROJECT_SOURCE_DIR}/shaders/*.frag"
     "${PROJECT_SOURCE_DIR}/shaders/*.vert"
     "${PROJECT_SOURCE_DIR}/shaders/*.comp"
     )

## iterate each shader
foreach (GLSL ${GLSL_SOURCE_FILES})
	get_filename_component(FILE_NAME ${GLSL} NAME)
	set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
	##execute glslang command to compile that specific shader
	add_custom_command(
			OUTPUT ${SPIRV}
			COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${GLSL} -o ${SPIRV}
			DEPENDS ${GLSL})
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
		Shaders
		DEPENDS ${SPIRV_BINARY_FILES}
)