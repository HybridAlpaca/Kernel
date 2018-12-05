#include <Kernel/Context.h>

#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/glx.h>

using namespace Kernel;

namespace
{
	/// @namespace KGL
	/// Contains platform-specific SwapInterval function pointers (PFN's)
	namespace KGL
	{
		PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;

		PFNGLXSWAPINTERVALEXTPROC         glXSwapIntervalEXT;
		PFNGLXSWAPINTERVALMESAPROC        glXSwapIntervalMESA;
		PFNGLXSWAPINTERVALSGIPROC         glXSwapIntervalSGI;
	}
}

struct GraphicsContext::Driver
{
	GLXContext    context;
	XVisualInfo * visualInfo;
};

GraphicsContext::GraphicsContext(const WindowPlatformData & platformData)
: platform{platformData}
{
	driver = new Driver{};
}

GraphicsContext::~GraphicsContext()
{
	delete driver;
}

void GraphicsContext::Create(uint32 width, uint32 height)
{
	const auto display = static_cast<::Display *>(platform.nativeDisplay);
	const auto window  = reinterpret_cast<Window>(platform.nativeWindow);

	driver -> context = reinterpret_cast<GLXContext>(platform.context);

	// If we don't already have a context, create one
	if (platform.context == nullptr)
	{
		// Lock the X client for threading purposes
		XLockDisplay(display);

		// Query GLX version
		int32 major, minor;
		bool version = glXQueryVersion(display, & major, & minor);

		const int attrsGlx []
		{
			GLX_RENDER_TYPE,    GLX_RGBA_BIT,
			GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
			GLX_DOUBLEBUFFER,   true,
			GLX_RED_SIZE,       8,
			GLX_BLUE_SIZE,      8,
			GLX_GREEN_SIZE,     8,
			GLX_DEPTH_SIZE,     24,
			GLX_STENCIL_SIZE,   8,
			0
		};

		GLXFBConfig bestConfig = nullptr;

		// Get available Framebuffer configurations
		uint32 screen = DefaultScreen(display);

		int32 configCount;
		GLXFBConfig * configs = glXChooseFBConfig(display, screen, attrsGlx, & configCount);

		// Choose the best FB
		for (uint32 i = 0; i < configCount; ++i)
		{
			// Get visual info for this FB configuration
			driver -> visualInfo = glXGetVisualFromFBConfig(display, configs[i]);

			// Rate each FB based on its qualities
			if (driver -> visualInfo != nullptr)
			{
				bool valid = true;
				for (uint32 attr = 6; attr < 16 && attrsGlx[attr] != None; attr += 2)
				{
					int32 value;
					glXGetFBConfigAttrib(display, configs[i], attrsGlx[attr], & value);

					if (value < attrsGlx[attr + 1])
					{
						valid = false;
						break;
					}
				}

				if (valid)
				{
					bestConfig = configs[i];
					break;
				}
			}

			XFree(driver -> visualInfo);
			driver -> visualInfo = nullptr;
		}

		XFree(configs);

		// We want a version 3.3 GL context
		int32 contextAttribs []
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
			GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB, ///< Comment out in release build
			None
		};

		// Get the 3.0+ context creation pointer
		KGL::glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

		// Create the context
    driver -> context = KGL::glXCreateContextAttribsARB(display, bestConfig, 0, True, contextAttribs);

		// Make the display available again
		XUnlockDisplay(display);
	}

	// Attach the context to our display
	glXMakeCurrent(display, window, driver -> context);

	// Decide which swap driver we're using and set the swap interval

	KGL::glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte *) "glXSwapIntervalEXT");
	if (KGL::glXSwapIntervalEXT != nullptr)
	{
		KGL::glXSwapIntervalEXT(display, window, 0);
	}
	else
	{
		KGL::glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC) glXGetProcAddress((const GLubyte *) "glXSwapIntervalMESA");
		if (KGL::glXSwapIntervalMESA != nullptr)
		{
			KGL::glXSwapIntervalMESA(0);
		}
		else
		{
			KGL::glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress((const GLubyte *) "glXSwapIntervalSGI");
			if (KGL::glXSwapIntervalSGI != nullptr)
			{
				KGL::glXSwapIntervalSGI(0);
			}
		}
	}

	glewExperimental = true;
	glewInit();

	glViewport(0, 0, width, height);
}

void GraphicsContext::Destroy()
{
	const auto display = static_cast<::Display *>(platform.nativeDisplay);

	// Detach any contexts from our display
	glXMakeCurrent(display, 0, 0);

	// Destroy the context if it exists
	if (platform.context != nullptr)
	{
		glXDestroyContext(display, driver -> context);
		XFree(driver -> visualInfo);
	}

	driver -> visualInfo = nullptr;
}

void GraphicsContext::SwapBuffers()
{
	const auto display = static_cast<::Display *>(platform.nativeDisplay);
	const auto window  = reinterpret_cast<Window>(platform.nativeWindow);

	glXSwapBuffers(display, window);
}

bool GraphicsContext::Valid() const
{
	return driver -> context != nullptr;
}
