-- premake5.lua
workspace "PostProcessor"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "PostProcessor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Build-PostProcessor-External.lua"
include "PPApp/Build-PPApp.lua"