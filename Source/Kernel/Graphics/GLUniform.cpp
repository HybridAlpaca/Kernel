#include <Kernel/Graphics/Uniform.h>
#include <Kernel/Debug.h>

#include <GL/glew.h>

using namespace Kernel::Graphics;

uint32 Uniform::GLOBAL_SLOT = 0;

Uniform::Uniform()
: handle {0}
, slot   {GLOBAL_SLOT++}
, size   {0}
{
	// Epmty initializer
}

void Uniform::Create(uint32 size)
{
	// Store size for future housekeeping
	this -> size = size;

	// Create buffer and expand to size
	glGenBuffers(1, & handle);
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Uniform::Destroy()
{
	// Self-explanatory
	glDeleteBuffers(1, & handle);
}

void Uniform::Bind()
{
	// Bind our handle to our slot
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, handle);
}

void Uniform::BindRange(uint32 offset, uint32 size)
{
	// Bind specific range of our buffer to our slot
	glBindBufferRange(GL_UNIFORM_BUFFER, slot, handle, offset, size);
}

void Uniform::Write(char * data, uint32 offset, uint32 size)
{
	// Prevent overflowing the buffer
	if ((offset + size) > (this -> size))
	{
		Kernel::DebugLog("[Error] Caught overflow from Uniform::Write");
		return;
	}

	// Write data to buffer
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
