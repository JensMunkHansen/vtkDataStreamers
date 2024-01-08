if (NOT (DEFINED datastreamers_cmake_dir AND
         DEFINED datastreamers_cmake_build_dir AND
         DEFINED datastreamers_cmake_destination AND
         DEFINED datastreamers_modules))
  message(FATAL_ERROR
    "vtkDatastreamersInstallCMakePackage is missing input variables.")
endif ()

set(datastreamers_all_components)
foreach (datastreamers_module IN LISTS datastreamers_modules)
  string(REPLACE "VTK::" "" datastreamers_component "${datastreamers_module}")
  list(APPEND datastreamers_all_components
    "${datastreamers_component}")
endforeach ()

# Creates the variable vtk_module_import_prefix variable
_vtk_module_write_import_prefix("${datastreamers_cmake_build_dir}/datastreamers-prefix.cmake" "${datastreamers_cmake_destination}")

set(datastreamers_python_version "")
if (DATASTREAMERS_WRAP_PYTHON)
  set(datastreamers_python_version "${VTK_PYTHON_VERSION}")
endif()
# message("DATASTREAMERS_PYTHON_VERSION: ${datastreamers_python_version}")

configure_file(
  "${datastreamers_cmake_dir}/datastreamers-config.cmake.in"
  "${datastreamers_cmake_build_dir}/datastreamers-config.cmake"
  @ONLY)

configure_file(
  "${datastreamers_cmake_dir}/datastreamers-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/datastreamers-config.cmake"
  @ONLY)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${datastreamers_cmake_build_dir}/datastreamers-config-version.cmake"
  VERSION "${DATASTREAMERS_MAJOR_VERSION}.${DATASTREAMERS_MINOR_VERSION}.${DATASTREAMERS_PATH_VERSION}"
  COMPATIBILITY AnyNewerVersion)

# For convenience, a package is written to the top of the build tree. At some
# point, this should probably be deprecated and warn when it is used.
file(GENERATE
  OUTPUT  "${CMAKE_BINARY_DIR}/datastreamers-config.cmake"
  CONTENT "include(\"${datastreamers_cmake_build_dir}/datastreamers-config.cmake\")\n")
configure_file(
  "${datastreamers_cmake_build_dir}/datastreamers-config-version.cmake"
  "${CMAKE_BINARY_DIR}/datastreamers-config-version.cmake"
  COPYONLY)

# Any packages needed by this
set(datastreamers_cmake_module_files
#   Finddouble-conversion.cmake
#   FindEigen3.cmake
)

# Patch files (if any)
set(datastreamers_cmake_patch_files
)

set(datastreamers_cmake_files_to_install)
foreach (datastreamers_cmake_module_file IN LISTS datastreamers_cmake_module_files datastreamers_cmake_patch_files)
  configure_file(
    "${datastreamers_cmake_dir}/${datastreamers_cmake_module_file}"
    "${datastreamers_cmake_build_dir}/${datastreamers_cmake_module_file}"
    COPYONLY)
  list(APPEND datastreamers_cmake_files_to_install
    "${datastreamers_cmake_module_file}")
endforeach ()

# message("datastreamers_cmake_files_to_install: ${datastreamers_cmake_files_to_install}")

include(datastreamersInstallCMakePackageHelpers)

# message("DATASTREAMERS_RELOCATABLE_INSTALL: ${DATASTREAMERS_RELOCATABLE_INSTALL}")

if (NOT DATASTREAMERS_RELOCATABLE_INSTALL)
  list(APPEND datastreamers_cmake_files_to_install
    "${datastreamers_cmake_build_dir}/datastreamers-find-package-helpers.cmake")
endif ()

foreach (datastreamers_cmake_file IN LISTS datastreamers_cmake_files_to_install)
  if (IS_ABSOLUTE "${datastreamers_cmake_file}")
    file(RELATIVE_PATH datastreamers_cmake_subdir_root "${datastreamers_cmake_build_dir}" "${datastreamers_cmake_file}")
    get_filename_component(datastreamers_cmake_subdir "${datastreamers_cmake_subdir_root}" DIRECTORY)
    set(datastreamers_cmake_original_file "${datastreamers_cmake_file}")
  else ()
    get_filename_component(datastreamers_cmake_subdir "${datastreamers_cmake_file}" DIRECTORY)
    set(datastreamers_cmake_original_file "${datastreamers_cmake_dir}/${datastreamers_cmake_file}")
  endif ()
  install(
    FILES       "${datastreamers_cmake_original_file}"
    DESTINATION "${datastreamers_cmake_destination}/${datastreamers_cmake_subdir}"
    COMPONENT   "development")
endforeach ()

install(
  FILES       "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/datastreamers-config.cmake"
              "${datastreamers_cmake_build_dir}/datastreamers-config-version.cmake"
              "${datastreamers_cmake_build_dir}/datastreamers-prefix.cmake"
  DESTINATION "${datastreamers_cmake_destination}"
  COMPONENT   "development")

vtk_module_export_find_packages(
  CMAKE_DESTINATION "${datastreamers_cmake_destination}"
  FILE_NAME         "datastreamers-vtk-module-find-packages.cmake"
  MODULES           ${datastreamers_modules})
