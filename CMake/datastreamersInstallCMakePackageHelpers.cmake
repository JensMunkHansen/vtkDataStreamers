function (_datastreamers_package_append_variables)
  set(_datastreamers_package_variables)
  foreach (var IN LISTS ARGN)
    if (NOT DEFINED "${var}")
      continue ()
    endif ()

    get_property(type_is_set CACHE "${var}"
      PROPERTY TYPE SET)
    if (type_is_set)
      get_property(type CACHE "${var}"
        PROPERTY TYPE)
    else ()
      set(type UNINITIALIZED)
    endif ()

    string(APPEND _datastreamers_package_variables
      "if (NOT DEFINED \"${var}\" OR NOT ${var})
  set(\"${var}\" \"${${var}}\" CACHE ${type} \"Third-party helper setting from \${CMAKE_FIND_PACKAGE_NAME}\")
endif ()
")
  endforeach ()

  set(datastreamers_find_package_code
    "${datastreamers_find_package_code}${_datastreamers_package_variables}"
    PARENT_SCOPE)
endfunction ()

get_property(_datastreamers_packages GLOBAL
  PROPERTY _datastreamers_module_find_packages_DATASTREAMERS)
if (_datastreamers_packages)
  list(REMOVE_DUPLICATES _datastreamers_packages)
endif ()

# Per-package variable forwarding goes here.
set(Python3_find_package_vars
  Python3_EXECUTABLE
  Python3_INCLUDE_DIR
  Python3_LIBRARY)

set(datastreamers_find_package_code)
foreach (_datastreamers_package IN LISTS _datastreamers_packages)
  _datastreamers_package_append_variables(
    # Standard CMake `find_package` mechanisms.
    "${_datastreamers_package}_DIR"
    "${_datastreamers_package}_ROOT"

    # Per-package custom variables.
    ${${_datastreamers_package}_find_package_vars})
endforeach ()

file(GENERATE
  OUTPUT  "${datastreamers_cmake_build_dir}/datastreamers-find-package-helpers.cmake"
  CONTENT "${datastreamers_find_package_code}")
