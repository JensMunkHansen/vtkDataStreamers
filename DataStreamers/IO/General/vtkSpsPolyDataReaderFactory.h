/*=========================================================================


=========================================================================*/
/**
 * @class   vtkSpsPolyDataReaderFactory
 * @brief   Superclass of polydata file readers.
 *
 * vtkSpsPolyDataReaderFactory: This class is used to create a vtkAlgorithm
 * object given a path name to a file.  It calls CanReadFile on all
 * available readers until one of them returns true.  The available reader
 * list comes from three places.  In the InitializeReaders function of this
 * class, built-in VTK classes are added to the list, users can call
 * RegisterReader, or users can create a vtkObjectFactory that has
 * CreateObject method that returns a new vtkAlgorithm sub class when
 * given the string "vtkPolyDataReaderObject".  This way applications can be
 * extended with new readers via a plugin dll or by calling RegisterReader.
 * Of course all of the readers that are part of the vtk release are made
 * automatically available.
 *
 */

#ifndef vtkSpsPolyDataReaderFactory_h
#define vtkSpsPolyDataReaderFactory_h

#include "vtkSpsIOGeneralModule.h" // For export macro
#include "vtkObject.h"

class vtkAlgorithm;
class vtkSpsPolyDataReaderCollection;
class vtkSpsPolyDataReaderFactoryCleanup;

class VTKSPSIOGENERAL_EXPORT vtkSpsPolyDataReaderFactory : public vtkObject
{
public:
  static vtkSpsPolyDataReaderFactory* New();
  vtkTypeMacro(vtkSpsPolyDataReaderFactory, vtkObject)
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * registered readers will be queried in CreateImageReader2 to
   * see if they can load a given file.
   */
  static void RegisterReader(vtkAlgorithm* r);

  /**
   * open the image file, it is the callers responsibility to call
   * Delete on the returned object. If no reader is found, nullptr
   * is returned.
   */
  VTK_NEWINSTANCE
  static vtkAlgorithm* CreatePolyDataReader(const char* path);


  /**
   * The caller must allocate the vtkAlgorithmCollection and pass in the
   * pointer to this method.
   */
  static void GetRegisteredReaders(vtkSpsPolyDataReaderCollection*);


protected:
  vtkSpsPolyDataReaderFactory() = default;
  ~vtkSpsPolyDataReaderFactory() override = default;

  static void InitializeReaders();

private:
  static vtkSpsPolyDataReaderCollection* AvailableReaders;
  vtkSpsPolyDataReaderFactory(const vtkSpsPolyDataReaderFactory&) = delete;
  void operator=(const vtkSpsPolyDataReaderFactory&) = delete;

  friend class vtkSpsPolyDataReaderFactoryCleanup;
};

#endif
