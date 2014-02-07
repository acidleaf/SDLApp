-- A premake4 build script used to compile a basic SDL application using OpenGL for rendering

solution "SDLApp"
	configurations { "Debug", "Release" }
	
	project "SDLApp"
		targetname "SDLApp"
		kind "ConsoleApp"
		language "C++"
		
		pchheader "include/pch.h"
		
		files { "include/**.h", "src/**.cc" }
		
		includedirs { "D:/Lib/SDL/i686-w64-mingw32/include", "include", "include/**" }
		libdirs { "D:/Lib/SDL/i686-w64-mingw32/lib"}
		
		links { "SDL2", "mingw32", "SDL2main", "SDL2.dll", "opengl32" }
		linkoptions {  }
		
		buildoptions { "--std=c++11" }
		
		configuration "Debug"
			targetdir "bin/Debug"
			objdir "obj/Debug"
			flags { "Symbols" }
			buildoptions { "-mconsole" }		-- Console for logging debug output
			
		configuration "Release"
			targetdir "bin/Release"
			objdir "obj/Release"
			flags { "Optimize" }