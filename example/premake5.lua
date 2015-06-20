solution "example"
	location "build"
	configurations {"debug", "release"}

project "example"
	kind "ConsoleApp"
	location "build"
	language "C++"
	targetdir "build/bin_%{cfg.buildcfg}"

	links {"User32", "Gdi32"}

	includedirs {".."}
	files {"../**.h", "../**.c", "../**.cpp", "example.manifest"}

	filter "configurations:debug"
		defines {"DEBUG"}
		flags {"Symbols"}

	filter "configurations:release"
		defines {"NDEBUG"}
		optimize "On"
