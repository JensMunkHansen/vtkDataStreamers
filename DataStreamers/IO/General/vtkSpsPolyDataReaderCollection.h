/*=========================================================================

=========================================================================*/
/**
 * @class   vtkSpsPolyDataReaderCollection
 * @brief   maintain a list of polydata readers
 *
 * vtkSpsPolyDataReaderCollection is an object that creates and manipulates
 * lists of objects of type vtkAlgorithm and its subclasses.
 */

#ifndef vtkSpsPolyDataReaderCollection_h
#define vtkSpsPolyDataReaderCollection_h

#include "vtkCollection.h"
#include "vtkSpsIOGeneralModule.h" // For export macro

class vtkAlgorithm;

class VTKSPSIOGENERAL_EXPORT vtkSpsPolyDataReaderCollection : public vtkCollection
{
public:
  vtkTypeMacro(vtkSpsPolyDataReaderCollection, vtkCollection)
  static vtkSpsPolyDataReaderCollection* New();
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Add an image reader to the list.
   */
  void AddItem(vtkAlgorithm*);

  /**
   * Get the next image reader in the list.
   */
  vtkAlgorithm* GetNextItem();

  /**
   * Reentrant safe way to get an object in a collection. Just pass the
   * same cookie back and forth.
   */
  vtkAlgorithm* GetNextPolyDataReader(vtkCollectionSimpleIterator& cookie);

protected:
  vtkSpsPolyDataReaderCollection() = default;
  ~vtkSpsPolyDataReaderCollection() override = default;

private:
  // hide the standard AddItem from the user and the compiler.
  void AddItem(vtkObject* o) {
    this->vtkCollection::AddItem(o);
  }

private:
  vtkSpsPolyDataReaderCollection(const vtkSpsPolyDataReaderCollection&) = delete;
  void operator=(const vtkSpsPolyDataReaderCollection&) = delete;
};

#endif
