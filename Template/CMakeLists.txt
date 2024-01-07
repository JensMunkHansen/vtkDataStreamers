set(Template_SOURCES
  vtkTemplate.cxx
)

set(Template_HEADERS
  vtkTemplate.h
)

vtk_module_add_module(VTK::Template
  SOURCES ${Template_SOURCES}
  HEADERS ${Template_HEADERS})
