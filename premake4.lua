solution "metasystem"
	configurations { "Release", "Debug" }
	location ( "build" )
	
	project "umof"
		targetname "umof"
		language "C++"
		kind "SharedLib"
		
		files
		{
			"src/**.h",
			"src/**.cpp"
		}
		
		configuration "Debug"
			targetdir "bin/debug"
			defines { "_DEBUG", "UMOF_LIBRARY" }
			flags { "Symbols" }
			
		configuration "Release"
			targetdir "bin/release"
			defines { "NDEBUG", "UMOF_LIBRARY" }
			flags { "Optimize" }
			
		configuration "gmake"
			buildoptions { "-std=c++11" }
