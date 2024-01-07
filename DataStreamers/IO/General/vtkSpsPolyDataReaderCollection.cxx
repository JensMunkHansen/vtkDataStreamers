/*=========================================================================

=========================================================================*/
#include "vtkSpsPolyDataReaderCollection.h"

#include "vtkObjectFactory.h"
#include "vtkAlgorithm.h"

vtkStandardNewMacro(vtkSpsPolyDataReaderCollection)

void vtkSpsPolyDataReaderCollection::AddItem(vtkAlgorithm* f)
{
  this->vtkCollection::AddItem(f);
}

vtkAlgorithm* vtkSpsPolyDataReaderCollection::GetNextItem()
{
  return static_cast<vtkAlgorithm*>(this->GetNextItemAsObject());
}

vtkAlgorithm* vtkSpsPolyDataReaderCollection::GetNextPolyDataReader(vtkCollectionSimpleIterator& cookie)
{
  return static_cast<vtkAlgorithm*>(this->GetNextItemAsObject(cookie));
}

//------------------------------------------------------------------------------
void vtkSpsPolyDataReaderCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
