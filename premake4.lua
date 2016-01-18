-- A premake4 build script used to compile a basic SDL application using OpenGL for rendering

solution "SDLApp"
	configurations { "Debug", "Release" }
	
	project "SDLApp"
		targetname "SDLApp"
		kind "ConsoleApp"
		language "C++"
		
		
		files { "include/**.h", "src/**.cc" }
		includedirs { "include", "include/**" }
		links { "SDL2" }
		
		buildoptions { "--std=c++11" }
		
		includedirs { "/usr/local/include" }
		libdirs { "/usr/local/lib"}
		linkoptions { "-framework OpenGL" }
		
		
		
		configuration "Debug"
			targetdir "bin/Debug"
			objdir "obj/Debug"
			flags { "Symbols" }
			
			buildoptions { "-Wall" }
			
		configuration "Release"
			targetdir "bin/Release"
			objdir "obj/Release"
			flags { "Optimize" }