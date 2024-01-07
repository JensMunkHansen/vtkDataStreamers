# Test with data
vtk_add_test_cxx(
  vtkSpsIOGeneralCxxTests btests
  TestSpsPolyDataFileSeriesReader.cxx, NO_VALID
  )
set(all_tests
  ${btests}
  )

vtk_test_cxx_executable(vtkSpsIOGeneralCxxTests all_tests
  )
