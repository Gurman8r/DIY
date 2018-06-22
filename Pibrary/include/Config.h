#ifndef _ML_CONFIG_H_
#define _ML_CONFIG_H_

/*	Debug Macro
* * * * * * * * * * * * * * * * * * * */
#if defined(_DEBUG)
	#define ML_DEBUG
#endif


/*	Operating System Macro
* * * * * * * * * * * * * * * * * * * */
#if defined(_WIN32)
	// Windows
	#define ML_SYSTEM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	// Apple
	#define ML_SYSTEM_APPLE
	#error This APPLE operating system does not support memes.
#elif defined(__unix__)
	#if defined(__ANDROID__)
		// Android
		#define ML_SYSTEM_ANDROID
	#elif defined(__linux__)
		// Linux
		#define ML_SYSTEM_LINUX
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)	
		// Free BSD
		#define ML_SYSTEM_FREEBSD
	#else
		// Unsupported Unix OS
		#error This UNIX operating system does not support memes.
	#endif
#else
	// Unsupported OS
	#error This operating system does not support memes.
#endif


/*	Export / Import Macro
* * * * * * * * * * * * * * * * * * * */
#if !defined(ML_STATIC)
	#if defined(ML_SYSTEM_WINDOWS)
		// Windows compilers need different keywords for exports and imports
		#define ML_API_EXPORT __declspec(dllexport)
		#define ML_API_IMPORT __declspec(dllimport)
		// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
		#ifdef _MSC_VER
			#pragma warning(disable: 4251)
		#endif
	#else
		#if __GNUC__ >= 4
			// GCC 4 has special keywords for showing/hidding symbols,
			// the same keyword is used for both importing and exporting
			#define ML_API_EXPORT __attribute__ ((__visibility__ ("default")))
			#define ML_API_IMPORT __attribute__ ((__visibility__ ("default")))
		#else
			// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
			#define ML_API_EXPORT
			#define ML_API_IMPORT
		#endif
	#endif
#else
	#define ML_API_EXPORT
	#define ML_API_IMPORT
#endif

#endif // !_ML_CONFIG_H_
