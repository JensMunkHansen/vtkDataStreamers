set(classes
#  vtkCLIOptions
#  vtkCommandOptions
#  vtkCommandOptionsXMLParser
#  vtkCommunicationErrorCatcher
#  vtkDistributedTrivialProducer
  vtkFileSequenceParser
#  vtkLogRecorder
#  vtkMultiProcessControllerHelper
#  vtkPVCompositeDataPipeline
#  vtkPVDataUtilities
#  vtkPVInformationKeys
#  vtkPVLogger
#  vtkPVNullSource
#  vtkPVPostFilter
#  vtkPVPostFilterExecutive
#  vtkPVTestUtilities
#  vtkPVTrivialProducer
#  vtkPVXMLElement
#  vtkPVXMLParser
#  vtkStringList
#  vtkUndoElement
#  vtkUndoSet
#  vtkUndoStack
  )

set(headers
  vtkMemberFunctionCommand.h)

set(private_headers
  vtkUndoStackInternal.h)

set(nowrap_classes
  vtkPVStringFormatter)

vtk_module_add_module(VTK::SpsCore
  CLASSES ${classes}
  #NOWRAP_CLASSES ${nowrap_classes}
  HEADERS ${headers}
  PRIVATE_HEADERS ${private_headers})

