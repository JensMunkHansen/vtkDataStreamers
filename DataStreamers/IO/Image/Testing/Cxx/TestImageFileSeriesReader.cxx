#include <vtkNew.h>
#include <vtkSpsImageFileSeriesReader.h>
#include <vtkPNGReader.h>
#include <vtkImageData.h>
#include <vtkPNGWriter.h>
#include <vtkTesting.h>
#include <vtkTestUtilities.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkInformation.h>
#include <vtkImageShiftScale.h>
#include <vtkTemporalDataSetCache.h>

#include <sstream>
#include <string>
#include <vector>

const int nFiles = 8;


int TestImageFileSeriesReader(int argc, char *argv[])
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
    std::cout << "hello" << std::endl;
    std::cout << fileName << std::endl;
    fileNames.push_back(fileName);
    imageWriter->SetFileName(fileName.c_str());
    imageWriter->SetInputData(pZImage);
    imageWriter->Write();
  }


  vtkNew<vtkSpsImageFileSeriesReader> seriesReader;

  vtkNew<vtkPNGReader> reader;
  seriesReader->SetReadAsImageStack(0);
  seriesReader->SetReader(reader);

  for (const std::string& str : fileNames) {
    seriesReader->AddFileName(str.c_str());
  }

  // Calls RequestData for all times
  seriesReader->Update();

  vtkInformation* info = seriesReader->GetOutputInformation(0);
  seriesReader->UpdateInformation();

  double *steps = info->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
  int nSteps = info->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());

  for (int i = 0; i < nSteps; i++) {
    printf("%f\n", steps[i]);
  }

  double* timeRange = info->Get(vtkStreamingDemandDrivenPipeline::TIME_RANGE());

  printf("Time range: %f %f\n", timeRange[0], timeRange[1]);

  vtkNew<vtkTemporalDataSetCache> cache;
  cache->SetCacheSize(3);
  cache->SetInputConnection(seriesReader->GetOutputPort());

  vtkNew<vtkImageShiftScale> shifter;
  shifter->SetShift(1);
  shifter->SetInputConnection(cache->GetOutputPort());
  shifter->Update();

  vtkImageData* imgData = vtkImageData::SafeDownCast(shifter->GetOutput());

  shifter->UpdateInformation();
  vtkInformation* info1 = shifter->GetOutputInformation(0);
  double* steps1 = info1->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
  int nSteps1 = info1->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());

  printf("Time information at the end\n");
  for (int i = 0; i < nSteps1; i++) {
    printf("%f\n", steps1[i]);
  }

  shifter->UpdateInformation();
  int iFile = 5;
  double time = double(iFile % nFiles);
  shifter->GetOutputInformation(0)->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), time);
  shifter->Update();

  imgData = vtkImageData::SafeDownCast(shifter->GetOutput());

  // Verify that it indeed is image 3
  unsigned char expected = (iFile*wx) % 256 + 1;

  unsigned char actual = *((unsigned char *)imgData->GetScalarPointer(0, 0, 0 ));

  printf("actual: %d\n", actual);
  printf("expected: %d\n", expected);
  vtkAlgorithm::SetDefaultExecutivePrototype(nullptr);

  return 0;
}
