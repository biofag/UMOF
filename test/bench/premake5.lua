include "qt.lua"
local qt = premake.extensions.qt

solution "benchmark"
	configurations { "Release", "Debug" }
	location ( "build" )
	
	project "bench_native"
		targetname "bench_native"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_native.cpp" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			
	project "bench_umof"
		targetname "bench_umof"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_umof.cpp" }
		links { "umof" }
		includedirs { "../../src" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			libdirs { "../../bin/debug" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "../../bin/release" }
			
		configuration "gmake"
			buildoptions { "-std=c++11" }
			
	project "bench_camp"
		targetname "bench_camp"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_camp.cpp" }
		includedirs { "W:/third-party/camp-master/include", os.getenv("BOOST_ROOT") }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			-- Oops, get rid of absolute path in future
			libdirs { "W:/third-party/camp-master/Debug" }
			links { "campd" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "W:/third-party/camp-master/Release" }
			links { "camp" }
			
	project "bench_cpgf"
		targetname "bench_cpgf"
		language "C++"
		kind "ConsoleApp"
		
		files {	"bench_cpgf.cpp" }
		includedirs { "W:/third-party/cpgf-develop/include"}
		links { "cpgf" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			-- Oops, get rid of absolute path in future
			libdirs { "W:/third-party/cpgf-develop/lib/Debug" }
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"
			libdirs { "W:/third-party/cpgf-develop/lib/Release" }
			
	project "bench_qt"
		targetname "bench_qt"
		language "C++"
		kind "ConsoleApp"
		
		qt.enable()
		-- Set local Qt path
		qtpath "C:/qt5"
		qtmodules { "core" }
		qtprefix "Qt5"
		
		files {	"qt_test.h", "bench_qt.cpp" }
		
		configuration "Debug"
			targetdir "bin/debug"
			defines "_DEBUG"
			flags { "Symbols" }
			qtsuffix "d"
			
		configuration "Release"
			targetdir "bin/release"
			defines "NDEBUG"
			optimize "On"