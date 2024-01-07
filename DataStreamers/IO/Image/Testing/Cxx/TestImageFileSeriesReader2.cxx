#include <vtkNew.h>
#include <vtkFuseImageFileSeriesReader.h>
// #include <vtkPNGReader.h>
#include <vtkImageData.h>
#include <vtkPNGWriter.h>
#include <vtkTesting.h>
#include <vtkTestUtilities.h>

#include <sstream>
#include <string>
#include <vector>

const int nFiles = 8;


int TestImageFileSeriesReader2(int argc, char *argv[])
{
  char* tempDir =
    vtkTestUtilities::GetArgOrEnvOrDefault("-T", argc, argv, "VTK_TEMP_DIR", "Testing/Temporary");
  if (!tempDir)
  {
    cerr << "Could not determine temporary directory.\n";
    return 1;
  }

  std::string outpath = tempDir;
  outpath += "/";
  delete[] tempDir;

  vtkNew<vtkCompositeDataPipeline> prototype;
  vtkAlgorithm::SetDefaultExecutivePrototype(prototype);

  const int wx = 32, wy = 32;

  vtkNew<vtkPNGWriter> imageWriter;
  std::stringstream sstream;
  std::vector<std::string> fileNames;

  for (int iFile = 0 ; iFile < nFiles ; iFile++) {
    vtkNew<vtkImageData> pZImage;
    pZImage->SetExtent(0, wx-1, 0, wy-1, 0, 0);
    pZImage->SetDimensions(wx, wy, 1);
    pZImage->SetOrigin( 0.0, 0.0, 0.0 );
    pZImage->SetSpacing( 1.0, 1.0, 1.0 );
    pZImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    for( int i = 0 ; i < wx ; i++ ) {
      for( int j = 0 ; j < wy ; j++ ) {
        // get scalar pointer to current pixel
        unsigned char *zPtr = (unsigned char *) pZImage->GetScalarPointer( i, j, 0 );

        // set scalar value accordingly
        *zPtr = static_cast<unsigned char>((iFile*wx + j * wx + i) % 256);
      }
    }
    std::stringstream().swap(sstream);
    sstream << "testFile" << std::setfill('0') << std::setw(2) << iFile;
    sstream << ".png";

    std::string fileName = sstream.str();
    fileName = outpath + fileName;
    fileNames.push_back(fileName);
    imageWriter->SetFileName(fileName.c_str());
    imageWriter->SetInputData(pZImage);
    imageWriter->Write();
  }
  return 0;
}
