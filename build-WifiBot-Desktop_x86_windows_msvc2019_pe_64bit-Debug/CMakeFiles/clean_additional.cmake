# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\WifiBot_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\WifiBot_autogen.dir\\ParseCache.txt"
  "WifiBot_autogen"
  )
endif()
