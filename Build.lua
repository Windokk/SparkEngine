-- premake5.lua
workspace "Spark Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SparkEngineEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "SparkEngine-Core/Build-Core.lua"
group ""

include "SparkEngine-Editor/Build-Editor.lua"