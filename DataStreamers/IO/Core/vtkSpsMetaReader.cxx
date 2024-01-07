/*=========================================================================

  Program:   ParaView
  Module:    vtkSpsMetaReader.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/*
 *  Adapted from vtkMetaReader.cxx
 */

#include "vtkSpsMetaReader.h"

#include "vtkDataObject.h"
#include "vtkInformation.h"
#include "vtkObjectFactory.h"
#include "vtkImageReader2.h"  // Added, since we do not use client-server

#include <vtksys/SystemTools.hxx>

//=============================================================================
vtkStandardNewMacro(vtkSpsMetaReader)

//----------------------------------------------------------------------------
vtkSpsMetaReader::vtkSpsMetaReader()
  : Reader(nullptr)
  , FileNameMTime(0)
  , BeforeFileNameMTime(0)
  , _FileIndex(0)
  , FileIndexMTime(0)
  , _MetaFileName(nullptr)
  , MetaFileNameMTime(0)
{
  this->FileIndexRange[0] = 0;
  this->FileIndexRange[1] = 0;
  this->SetNumberOfInputPorts(0);
}

vtkSpsMetaReader::~vtkSpsMetaReader()
{
  this->SetMetaFileName(nullptr);
  if (this->Reader)
  {
    this->Reader->Delete();
  }
}
//----------------------------------------------------------------------------
void vtkSpsMetaReader::ReaderSetFileName(const char* name)
{
  if (this->Reader)
  {
    // Paraview was using a client-server interpreter here!!!!
    // We simply cast to vtkImageReader2
    vtkImageReader2* pReader = vtkImageReader2::SafeDownCast(this->Reader);
    if (pReader) {
      pReader->SetFileName(name);
    } else {
      // TODO: Handle this
    }
  }
}

//-----------------------------------------------------------------------------
int vtkSpsMetaReader::ReaderCanReadFile(const char* filename)
{
  if (this->Reader)
  {
    vtkImageReader2* pReader = vtkImageReader2::SafeDownCast(this->Reader);

    int canRead = 1;

    canRead = pReader->CanReadFile(filename);
    return canRead;
  }
  return 0;
}

//----------------------------------------------------------------------------
vtkMTimeType vtkSpsMetaReader::GetMTime()
{
  vtkMTimeType mTime = this->vtkObject::GetMTime();

  if (this->Reader)
  {
    // In general, we want changes in Reader to be reflected in this object's
    // MTime.  However, we will also be making modifications to the Reader (such
    // as changing the filename) that we want to suppress from the reporting.
    // When this happens, we save the timestamp before our modification into
    // this->BeforeFileNameMTime and capture the resulting MTime in
    // this->FileNameMTime.  If we run into that modification,
    // suppress it by reporting the saved modification.
    vtkMTimeType readerMTime;
    if (this->Reader->GetMTime() == this->FileNameMTime)
    {
      readerMTime = this->BeforeFileNameMTime;
    }
    else
    {
      readerMTime = this->Reader->GetMTime();
    }
    mTime = (readerMTime > mTime ? readerMTime : mTime);
  }

  return mTime;
}

//-----------------------------------------------------------------------------
int vtkSpsMetaReader::FillOutputPortInformation(int port, vtkInformation* info)
{
  vtkInformation* rinfo = this->Reader->GetOutputPortInformation(port);
  info->CopyEntry(rinfo, vtkDataObject::DATA_TYPE_NAME());
  return 1;
}

//-----------------------------------------------------------------------------
std::string vtkSpsMetaReader::FromRelativeToMetaFile(const char* metaFileName, const char* filePath)
{
  std::string root;
  vtksys::SystemTools::SplitPathRootComponent(filePath, &root);
  if (root.empty())
  {
    // relative filePath, considered relative to the metafile.
    std::string metaFileDir = vtksys::SystemTools::GetFilenamePath(metaFileName);
    if (!metaFileDir.empty())
    {
      metaFileDir += "/";
    }
    return metaFileDir + filePath;
  }
  else
  {
    return filePath;
  }
}

//-----------------------------------------------------------------------------
void vtkSpsMetaReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  if (this->Reader)
  {
    os << indent << "Reader:" << endl;
    this->Reader->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << indent << "Reader: (null)" << endl;
  }
  os << indent << "FileIndexRange: " << this->FileIndexRange << endl;
}
