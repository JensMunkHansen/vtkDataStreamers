/*=========================================================================

  Program:   ParaView
  Module:    vtkSpsPolyDataFileSeriesReader.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSpsPolyDataFileSeriesReader.h"

#include "vtkAppendPolyData.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStringArray.h"

#include "vtkXMLPolyDataReader.h"
#include "vtkPolyDataReader.h"
#include "vtkPLYReader.h"
#include "vtkGLTFReader.h"

vtkStandardNewMacro(vtkSpsPolyDataFileSeriesReader)
//----------------------------------------------------------------------------
vtkSpsPolyDataFileSeriesReader::vtkSpsPolyDataFileSeriesReader()
{
  // TODO: Support multiple passes - merge functionality of vtkStreamerBase
  // Relevant if files are read as one partition
  //this->NumberOfPasses = 1;
}

//----------------------------------------------------------------------------
vtkSpsPolyDataFileSeriesReader::~vtkSpsPolyDataFileSeriesReader() = default;

//----------------------------------------------------------------------------
int vtkSpsPolyDataFileSeriesReader::ProcessRequest(
  vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
  {
    this->ResetTimeRanges();
    this->UpdateMetaData();
    this->UpdateFileNames();
  }

  // TODO: We need to make the reader set it's name. Here it doesn't.
  return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

void vtkSpsPolyDataFileSeriesReader::UpdateReaderDataExtent()
{
  if (!this->Reader)
  {
    return;
  }
  /*
   * // Used by image reader to know what slices to load. Here it doesn't work
   * int ext[6] = { 0, 0, 0, 0, 0, 0 };
   * this->Reader->SetDataExtent(ext);
   */
}

//----------------------------------------------------------------------------
void vtkSpsPolyDataFileSeriesReader::UpdateFileNames()
{

  vtkXMLPolyDataReader* pPolyDataReader = vtkXMLPolyDataReader::SafeDownCast(this->Reader);

  if (!pPolyDataReader)  {
    return;
  }
  this->BeforeFileNameMTime = this->GetMTime();

  pPolyDataReader->SetFileName(this->GetFileName(this->_FileIndex));
  this->FileNameMTime = pPolyDataReader->GetMTime();

  this->UpdateReaderDataExtent();
}

//----------------------------------------------------------------------------
void vtkSpsPolyDataFileSeriesReader::PrintSelf(ostream& os, vtkIndent indent)
{
  os << "File Index: " << this->_FileIndex;
  this->Superclass::PrintSelf(os, indent);
  
}
