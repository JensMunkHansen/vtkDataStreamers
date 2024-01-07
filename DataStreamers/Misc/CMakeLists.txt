set(classes
#   vtkAppendRectilinearGrid
#   vtkAttributeDataReductionFilter
#   vtkAttributeDataToTableFilter
#   vtkCleanArrays
#   vtkCompleteArrays
#   vtkEnvironmentAnnotationFilter
#   vtkExtractHistogram
#   vtkMergeBlocks
   vtkMinMax
#   vtkPExtractHistogram
#   vtkPVBox
#   vtkPVChangeOfBasisHelper
#   vtkPVCylinder
#   vtkPVMergeTables
#   vtkPVMergeTablesMultiBlock
#   vtkPVPlane
#   vtkPVTransform
#   vtkPVRotateAroundOriginTransform
#   vtkReductionFilter
#   vtkSelectionSerializer
  )

vtk_module_add_module(VTK::SpsMisc
  CLASSES ${classes})

