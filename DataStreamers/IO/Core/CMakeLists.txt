set(classes
  vtkSpsFileSeriesReader
  vtkSpsFileSeriesWriter
  vtkSpsMetaReader
)

vtk_module_add_module(VTK::SpsIOCore
  CLASSES ${classes})
