#ifndef NC_PLATFORM_H
#define NC_PLATFORM_H

// Set platform

#if defined(_WIN64)
	#define WIN64
#endif

#if defined(_WIN32) && !defined(_WIN64)
	#define WIN32
#endif

#ifdef WIN32
	#define WINDOWS
#endif

#ifdef WIN64
	#define WINDOWS
#endif

// Set platform
#if defined(__linux__) || defined(MACOS)
	#ifndef UNIX
		#define UNIX
	#endif
#endif

#if defined(WINDOWS) && defined(UNIX)
	#error "Too many platforms defined"
#elif !defined(WINDOWS) && !defined(UNIX)
	#error "No platform defined"
#endif

// Set build
#if defined(_DEBUG) || !defined(NDEBUG)
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif

#if defined(NDEBUG)
	#ifndef RELEASE
		#define RELEASE
	#endif
#endif

#if defined(DEBUG) && defined(RELEASE)
	#error "Too many build flags defined"
#endif

#if defined(RELEASE) && !defined(NDEBUG)
	#define NDEBUG
#endif

#endif	// NC_PLATFORM_H
