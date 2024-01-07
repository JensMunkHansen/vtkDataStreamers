# Test with data
vtk_add_test_cxx(
  vtkSpsIOImageCxxTests btests
  TestImageFileSeriesReader.cxx, NO_VALID
  )
set(all_tests
  ${btests}
  )

vtk_test_cxx_executable(vtkSpsIOImageCxxTests all_tests
  )
