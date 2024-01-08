option(STANDALONE "Stand alone" ON)

if (STANDALONE)
  cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)
  
  # Extra policies (not covering all introduced after 3.12)
  foreach(policy
      CMP0083 # CMake 3.14
      CMP0127
      # Add more here
      )
    if(POLICY ${policy})
      cmake_policy(SET ${policy} NEW)
    endif()
  endforeach()

  set(DATASTREAMERS_INSTALL_SDK ON)
  
  include(CMakeDependentOption)
  
  project(datastreamers
    LANGUAGES
    CXX)

  # Allow custom cmake overrides
  set(datastreamers_cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
  set(CMAKE_MODULE_PATH ${datastreamers_cmake_dir} ${CMAKE_MODULE_PATH})
  
  find_package(VTK 9 REQUIRED)

  option(DATASTREAMERS_RELOCATABLE_INSTALL "Do not embed hard-coded paths into the install" ON)
  mark_as_advanced(DATASTREAMERS_RELOCATABLE_INSTALL)
  
  option(BUILD_SHARED_LIBS "Build shared libraries." ON)
  
  include(GNUInstallDirs)
  
  option(BUILD_TESTING "Build the tests" ON)
  
  set(DATASTREAMERS_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
  set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  
  # Set up our directory structure for output libraries and binaries
  # (Note: these are the build locations, not the install locations)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DataStreamers_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
  endif()
  
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    if(UNIX)
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DataStreamers_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
    else()
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DataStreamers_BINARY_DIR}${CMAKE_INSTALL_BINDIR}")
    endif()
  endif()
  
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DataStreamers_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
  endif()
  
  set(datastreamers_requested_modules
    "VTK::SpsIOImage"
  )
  
  option(DATASTREAMERS_BUILD_ALL_MODULES "Build all modules by default" ON)
  mark_as_advanced(DATASTREAMERS_BUILD_ALL_MODULES)
  
  set(datastreamers_source_directories
    "${CMAKE_CURRENT_SOURCE_DIR}/DataStreamers/Misc"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataStreamers/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataStreamers/IO/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataStreamers/IO/Image"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataStreamers/IO/General"
    "${CMAKE_CURRENT_SOURCE_DIR}/Template"
  )
  
  vtk_module_find_modules(datastreamers_module_files ${datastreamers_source_directories})
  
  vtk_module_find_kits(datastreamers_kit_files ${datastreamers_source_directories})

  if (NOT DEFINED datastreamers_cmake_destination)
    set(datastreamers_cmake_destination
      "${CMAKE_INSTALL_LIBDIR}/cmake/datastreamers${datastreamers_version_suffix}")
  endif()
  
  vtk_module_scan(
    MODULE_FILES        ${datastreamers_module_files}
    KIT_FILES           ${datastreamers_kit_files} # empty right now
    REQUEST_MODULES     ${datastreamers_requested_modules}
    REJECT_MODULES      ${datastreamers_rejected_modules}
    PROVIDES_MODULES    datastreamers_modules
    PROVIDES_KITS       datastreamers_kits
    WANT_BY_DEFAULT     "${DATASTREAMERS_BUILD_ALL_MODULES}"
    ENABLE_TESTS        "${BUILD_TESTING}"
  )
  
  vtk_module_build(
    MODULES               ${datastreamers_modules}
    KITS                  ${datastreamers_kits}
    INSTALL_EXPORT        "${datastreamers_install_export}"
    ARCHIVE_DESTINATION   "lib"
    HEADERS_DESTINATION   "${CMAKE_INSTALL_INCLUDEDIR}/datastreamers${datastreamers_version_suffix}"
    CMAKE_DESTINATION     "${datastreamers_cmake_destination}"
    LICENSE_DESTINATION   "${CMAKE_INSTALL_LIBDIR}/datastreamers"
    HIERARCHY_DESTINATION "${CMAKE_INSTALL_LIBDIR}/vtk/hierarchy/${CMAKE_PROJECT_NAME}"
    LIBRARY_NAME_SUFFIX   "${DataStreamers_CUSTOM_LIBRARY_SUFFIX}"
    VERSION               "${DataStreamers_VERSION}"
    SOVERSION             "1" # If the library is small and should work as a remote module to VTK
    TEST_DATA_TARGET      DataStreamersData
    INSTALL_HEADERS       "${DATASTREAMERS_INSTALL_SDK}"
    USE_EXTERNAL          "${DataStreamers_USE_EXTERNAL}"
    TEST_INPUT_DATA_DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/Testing"
    TEST_OUTPUT_DATA_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/ExternalData/Testing")

  # Used for potential new way of installation
  if (DATASTREAMERS_INSTALL_SDK)
    set(datastreamers_cmake_build_dir
      "${CMAKE_CURRENT_BINARY_DIR}/${datastreamers_cmake_destination}")
    include(datastreamersInstallCMakePackage)
  endif ()
  
  if (NOT TARGET UNINSTALL)
    add_custom_target(UNINSTALL
      COMMAND
        "${CMAKE_COMMAND}"
          "-DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}"
          -P "${CMAKE_CURRENT_LIST_DIR}/CMake/datastreamersUninstall.cmake"
      USES_TERMINAL
      COMMENT "Uninstalling DataStreamers")
  endif ()
else()
  add_subdirectory(DataStreamers)
endif()
