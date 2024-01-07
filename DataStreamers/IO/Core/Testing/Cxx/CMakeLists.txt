vtk_add_test_cxx(
  vtkSpsIOCoreCxxTests tests
  TestMe.cxx,NO_VALID
  )
set(all_tests
  ${tests}
  )

vtk_test_cxx_executable(vtkSpsIOCoreCxxTests all_tests
  RENDERING_FACTORY
  )
