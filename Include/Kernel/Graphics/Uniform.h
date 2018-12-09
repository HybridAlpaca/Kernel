#pragma once

#include <Subframe/Subframe.h>

/*
 * GAME PLAN
 *
 * - Two main categories of UBO's
 *
 * - (1) Global data
 *   - View direction
 *   - Lighting data
 *   EX: [lighting, view, misc_data]
 *
 * - (2) Object information
 *   - Varying amount depending on scene size, max of 16KB
 *   - Contains global array of data *per all objects*
 *   - Update once per scene per frame
 *   - Upload specific range of data to each object, depending on where it's data is in the buffer
 *   EX: [obj1, obj2, obj3, ...objN] -> Submit range [object_index] of buffer
 *   - May have multiple of these based off of common object data
 *   - ^^ All skeleton rigged meshes in one buffer, all reflective meshes in another
 *   - ^^ Above could introduce issues where multiple meshes could share multiple kinds of data
 */

namespace Kernel::Graphics
{
	class Uniform
	{
		/// Static counter for progressively generating slots
		static uint32 GLOBAL_SLOT;

		/// Handle to backend-specific UBO
		uint32 handle;

		/// Binding site to 'link' this UBO to shader(S)
		uint32 slot;

		/// Size in bytes of this uniform
		uint32 size;

	public:

		Uniform();

		void Create(uint32 size);

		void Destroy();

		void Bind();

		void BindRange(uint32 offset, uint32 size);

		void Write(char * data, uint32 offset, uint32 size);

		inline uint32 Handle() const { return handle; }
		inline uint32 Slot() const { return slot; }
		inline uint32 Size() const { return size; }
	};
}
