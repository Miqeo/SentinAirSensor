# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/mhecka/Developer/esp/esp-idf/components/bootloader/subproject"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/tmp"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/src"
  "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/mhecka/Developer/esp/projects/captive_portal/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
