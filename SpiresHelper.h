#include <iostream>
#include <fstream>
#include <unordered_map>

#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/tuple/tuple.hpp>

#define MAX_HANDLERS 400

#if defined(_MSC_VER)
//  Windows 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#elif defined(__GNUC__)
//  GCC/G++
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  Do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown OS, linking may be touchy
#endif

using namespace std;

namespace SpiresHelper {

	extern "C" EXPORT void *LogText(int argc, char *args[]);

	extern "C" EXPORT void *Unregister(int argc, char *args[]);

	extern "C" EXPORT void *UnregisterAll(int argc, char *args[]);
}
