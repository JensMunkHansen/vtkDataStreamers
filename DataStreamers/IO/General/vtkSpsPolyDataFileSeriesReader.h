#pragma once

#include "vtkSpsFileSeriesReader.h"  // Steal from IOCore

#include "vtkSpsIOGeneralModule.h" //needed for exports

class vtkAppendPolyData;

class VTKSPSIOGENERAL_EXPORT vtkSpsPolyDataFileSeriesReader : public vtkSpsFileSeriesReader
{
public:
  static vtkSpsPolyDataFileSeriesReader* New();
  vtkTypeMacro(vtkSpsPolyDataFileSeriesReader, vtkSpsFileSeriesReader)
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Overridden to directly call the internal reader after passing it the
   * correct filenames when ReadAsImageStack is true.
   */
  int ProcessRequest(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  //@}

protected:
  vtkSpsPolyDataFileSeriesReader();
  ~vtkSpsPolyDataFileSeriesReader() override;

  void UpdateReaderDataExtent();
  
  void UpdateFileNames();

private:
  vtkSpsPolyDataFileSeriesReader(const vtkSpsPolyDataFileSeriesReader&) = delete;
  void operator=(const vtkSpsPolyDataFileSeriesReader&) = delete;

  // TODO: Support multiple passes to save memory
  //  unsigned int NumberOfPasses;

  vtkAppendPolyData* Append;
};

