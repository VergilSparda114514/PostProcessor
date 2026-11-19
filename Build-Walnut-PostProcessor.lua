-- premake5.lua
workspace "Post Processor"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "PostApp"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Build-Walnut-External.lua"
include "PostApp/Build-PostApp.lua"