/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSpsPolyDataReaderFactory.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSpsPolyDataReaderFactory.h"

#include "vtkAlgorithm.h"
#include "vtkGLTFReader.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkObjectFactoryCollection.h"
#include "vtkPLYReader.h"
#include "vtkPolyDataReader.h"
#include "vtkSpsPolyDataReaderCollection.h"
#include "vtkXMLPolyDataReader.h"

// Destroying the prototype readers requires information keys.
// Include the manager here to make sure the keys are not destroyed
// until after the AvailableReaders singleton has been destroyed.
#include "vtkFilteringInformationKeyManager.h"

#include <sstream>

vtkStandardNewMacro(vtkSpsPolyDataReaderFactory)

  //----------------------------------------------------------------------------
  class vtkSpsPolyDataReaderFactoryCleanup
{
public:
  inline void Use() {}
  ~vtkSpsPolyDataReaderFactoryCleanup()
  {
    if (vtkSpsPolyDataReaderFactory::AvailableReaders)
    {
      vtkSpsPolyDataReaderFactory::AvailableReaders->Delete();
      vtkSpsPolyDataReaderFactory::AvailableReaders = nullptr;
    }
  }
};
static vtkSpsPolyDataReaderFactoryCleanup vtkSpsPolyDataReaderFactoryCleanupGlobal;

//----------------------------------------------------------------------------
vtkSpsPolyDataReaderCollection* vtkSpsPolyDataReaderFactory::AvailableReaders;

//----------------------------------------------------------------------------
void vtkSpsPolyDataReaderFactory::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Available Readers : ";
  if (AvailableReaders)
  {
    AvailableReaders->PrintSelf(os, indent);
  }
  else
  {
    os << "None.";
  }
}

//----------------------------------------------------------------------------
void vtkSpsPolyDataReaderFactory::RegisterReader(vtkAlgorithm* r)
{
  vtkSpsPolyDataReaderFactory::InitializeReaders();
  AvailableReaders->AddItem(r);
}

//----------------------------------------------------------------------------
vtkAlgorithm* vtkSpsPolyDataReaderFactory::CreatePolyDataReader(const char* path)
{
  vtkSpsPolyDataReaderFactory::InitializeReaders();
  vtkXMLPolyDataReader* polyReader;
  vtkPLYReader* plyReader;

  vtkAlgorithm* ret;
  vtkNew<vtkCollection> collection;
  vtkObjectFactory::CreateAllInstance("vtkAlgorithm", collection);
  // first try the current registered object factories to see
  // if one of them can
  for (collection->InitTraversal(); vtkObject* object = collection->GetNextItemAsObject();)
  {
    polyReader = vtkXMLPolyDataReader::SafeDownCast(object);
    if (polyReader && polyReader->CanReadFile(path))
    {
      return vtkAlgorithm::SafeDownCast(polyReader);
    }
    plyReader = vtkPLYReader::SafeDownCast(object);
    if (plyReader && plyReader->CanReadFile(path))
    {
      return vtkAlgorithm::SafeDownCast(plyReader);
    }
  }

  // Then try all available readers
  vtkCollectionSimpleIterator sit;
  for (vtkSpsPolyDataReaderFactory::AvailableReaders->InitTraversal(sit);
       (ret = vtkSpsPolyDataReaderFactory::AvailableReaders->GetNextPolyDataReader(sit));)
  {
    plyReader = vtkPLYReader::SafeDownCast(ret);
    if (plyReader && plyReader->CanReadFile(path))
    {
      // like a new call
      return plyReader->NewInstance();
    }
    polyReader = vtkXMLPolyDataReader::SafeDownCast(ret);
    if (polyReader && polyReader->CanReadFile(path))
    {
      return polyReader->NewInstance();
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------------
void vtkSpsPolyDataReaderFactory::InitializeReaders()
{
  if (vtkSpsPolyDataReaderFactory::AvailableReaders)
  {
    return;
  }
  vtkSpsPolyDataReaderFactoryCleanupGlobal.Use();
  vtkSpsPolyDataReaderFactory::AvailableReaders = vtkSpsPolyDataReaderCollection::New();
  vtkAlgorithm* reader;

  vtkSpsPolyDataReaderFactory::AvailableReaders->AddItem((reader = vtkXMLPolyDataReader::New()));
  reader->Delete();
  vtkSpsPolyDataReaderFactory::AvailableReaders->AddItem((reader = vtkPLYReader::New()));
  reader->Delete();
  vtkSpsPolyDataReaderFactory::AvailableReaders->AddItem((reader = vtkPolyDataReader::New()));
  reader->Delete();
  vtkSpsPolyDataReaderFactory::AvailableReaders->AddItem((reader = vtkGLTFReader::New()));
  reader->Delete();
}

//----------------------------------------------------------------------------
void vtkSpsPolyDataReaderFactory::GetRegisteredReaders(vtkSpsPolyDataReaderCollection* collection)
{
  vtkSpsPolyDataReaderFactory::InitializeReaders();
  // get all dynamic readers
  vtkObjectFactory::CreateAllInstance("vtkSpsPolyDataReaderObject", collection);
  // get the current registered readers
  vtkAlgorithm* ret;
  vtkCollectionSimpleIterator sit;
  for (vtkSpsPolyDataReaderFactory::AvailableReaders->InitTraversal(sit);
       (ret = vtkSpsPolyDataReaderFactory::AvailableReaders->GetNextPolyDataReader(sit));)
  {
    collection->AddItem(ret);
  }
}
