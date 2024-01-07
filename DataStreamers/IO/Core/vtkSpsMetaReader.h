/*=========================================================================

  Program:   Sps
  Module:    vtkSpsMetaReader.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

     This is adapted from Paraview - client server part is removed

=========================================================================*/

/**
 * @class   vtkSpsMetaReader
 * @brief   Common functionality for meta-readers.
 *
 *
 * to another Reader.  The Reader reads from a file selected from a
 * list of files using a FileIndex.
 */

/*
 *  Adapted from vtkMetaReader.cxx
 */

#pragma once

#include "vtkDataObjectAlgorithm.h"
#include "vtkSpsIOCoreModule.h" //needed for exports

#include <string> // for std::string

class VTKSPSIOCORE_EXPORT vtkSpsMetaReader : public vtkDataObjectAlgorithm
{
public:
  static vtkSpsMetaReader* New();
  vtkTypeMacro(vtkSpsMetaReader, vtkDataObjectAlgorithm)

  vtkSpsMetaReader();
  ~vtkSpsMetaReader() override;

  //@{
  /**
   * Set/get the internal reader.
   */
  vtkSetObjectMacro(Reader, vtkAlgorithm);
  vtkGetObjectMacro(Reader, vtkAlgorithm);
  //@}

  //@{
  /**
   * Get/set the filename for the meta-file.
   * Description:
   * Get/Set the meta-file name
   */
  void SetMetaFileName(const char* name)
  {
    Set_MetaFileName(name);
    this->MetaFileNameMTime = this->vtkDataObjectAlgorithm::GetMTime();
  }
  char* GetMetaFileName()
  {
    return Get_MetaFileName();
  }
  //@}

  //@{
  /**
   * Returns the available range of file indexes. It is
   * 0, ..., GetNumberOfFiles () - 1.
   */
  vtkGetVector2Macro(FileIndexRange, vtkIdType);
  //@}

  //@{
  /**
   * Get/set the index of the file to read.
   */
  vtkIdType GetFileIndex()
  {
    return this->Get_FileIndex();
  }
  void SetFileIndex(vtkIdType i)
  {
    this->Set_FileIndex(i);
    this->FileIndexMTime = this->vtkDataObjectAlgorithm::GetMTime();
  }
  //@}

  /**
   * Return the MTime when also considering the internal reader.
   */
  vtkMTimeType GetMTime() override;

  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  int FillOutputPortInformation(int port, vtkInformation* info) override;

  vtkSetStringMacro(_MetaFileName);
  vtkGetStringMacro(_MetaFileName);

  vtkSetMacro(_FileIndex, vtkIdType);
  vtkGetMacro(_FileIndex, vtkIdType);

  void ReaderSetFileName(const char* filename);
  int ReaderCanReadFile(const char* filename);

  /**
   * 'metaFileName' to either a file path that is relative to the
   * current working directory (CWD) or to an absolute file path.
   * The choice is made based on if 'metaFileName' is relative or absolute.
   * Return the original if 'fileName' is already absolute.
   */
  std::string FromRelativeToMetaFile(const char* metaFileName, const char* fileName);

protected:
  // Reader that handles requests for the meta-reader
  vtkAlgorithm* Reader;
  // Reader modification time after changing the Reader's FileName
  // Used to ignore changing the FileName for the reader when reporting MTime
  vtkMTimeType FileNameMTime;
  // Reader modification time before changing the Reader's FileName
  // Used to ignore changing the FileName for the reader when reporting MTime
  vtkMTimeType BeforeFileNameMTime;
  // The index of the file to read.
  vtkIdType _FileIndex;
  vtkMTimeType FileIndexMTime;
  // Range for the file index
  vtkIdType FileIndexRange[2];

  // File name for the meta-reader
  char* _MetaFileName;
  // File name modification time
  vtkMTimeType MetaFileNameMTime;
  //@{
  /**
   * Records the time when the meta-file was read.
   */
  vtkTimeStamp MetaFileReadTime;

private:
  vtkSpsMetaReader(const vtkSpsMetaReader&) = delete;
  void operator=(const vtkSpsMetaReader&) = delete;
};
//@}

