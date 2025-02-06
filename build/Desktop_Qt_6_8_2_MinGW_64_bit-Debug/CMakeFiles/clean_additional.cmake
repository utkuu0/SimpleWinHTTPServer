# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SimpleWinHTTPServer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SimpleWinHTTPServer_autogen.dir\\ParseCache.txt"
  "SimpleWinHTTPServer_autogen"
  )
endif()
