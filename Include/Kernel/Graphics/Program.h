#pragma once

#include <Subframe/Subframe.h>

namespace Kernel::Graphics
{
	class Shader;

	/**
	 * @class Program
	 * @author Seth Traman
	 *
	 * @brief GPU shader program executable for use in rendering
	 **/
	class Program
	{
		/// Backend-specific handle to shader program executable
		uint32 handle;

	public:

		/// Initializes class instance
		/// @note Does NOT LINK A SHADER PROGRAM
		/// @note Destroy needn't be called unless explicitly linked
		Program();

		/// Links given shader objects into a executable shader program
		/// @param [in] shaders      Contiguous array of shader objects to link
		/// @param [in] shaderCount  Number of shaders to link
		/// @see Kernel::Graphics::Shader
		void Link(const Shader * shaders, uint32 shaderCount);

		/// Frees up resources used by shader program
		/// @note This needn't be called unless explicitly linked
		void Destroy();

		/// Bind named uniform block into binding slot
		/// @param [in] slot  Program slot to bind uniform block to
		/// @param [in] name  Name of the uniform block to bind
		void BindUniformSlot(uint32 slot, cchar name);

		/// Binds this program for usage in rendering
		void Use() const;

		/// Getter for backend-specific handle
		/// @return Backend-specific shader program handle
		inline uint32 Handle() const { return handle; }
	};
}
