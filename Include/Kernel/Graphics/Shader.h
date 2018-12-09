#pragma once

#include <Subframe/Subframe.h>

namespace Kernel::Graphics
{
	/**
	 * @enum ShaderType
	 * @brief Type of individual shader object
	 **/
	enum class ShaderType
	{
		VERTEX,   ///< Vertex shader
		FRAGMENT  ///< Fragment / Pixel shader
	};

	/**
	 * @class Shader
	 * @author Seth Traman
	 *
	 * @brief GPU shader object for linking into a full program
	 **/
	class Shader
	{
		/// Type of this shader object
		ShaderType type;

		/// Handle to backend-specific shader object
		uint32 handle;

	public:

		/// Initialize a shader with the given type
		/// @param [in] type  Type of this shader
		/// @note Does NOT CREATE OR COMPILE THE SHADER
		/// @note Destroy needn't be called unless explicitly compiled
		/// @see ShaderType
		explicit Shader(ShaderType type);

		/// Loads shader from file and compiles on the spot
		/// @param [in] path  Filesystem location of shader file
		/// @note Blocks until file is read and shader is compiled
		void CompileFromPath(cchar path);

		/// Frees up resources used by this shader
		/// @note This needn't be called unless explicitly compiled
		void Destroy();

		/// Getter for backend-specific handle
		/// @return Backend-specific shader object handle
		inline uint32 Handle() const { return handle; }
	};
}
