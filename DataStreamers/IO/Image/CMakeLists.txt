set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SpsIOImage_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(SpsIOImage_BUILD_TESTING ${BUILD_TESTING})

# Only needed for old compiler support - not used here
set(SpsIOImage_OVERRIDE " override")
set(SpsIOImage_DELETE " = delete")

set(classes
  vtkSpsImageFileSeriesReader
  vtkSpsRawImageFileSeriesReader)

vtk_module_add_module(VTK::SpsIOImage
  CLASSES ${classes})
