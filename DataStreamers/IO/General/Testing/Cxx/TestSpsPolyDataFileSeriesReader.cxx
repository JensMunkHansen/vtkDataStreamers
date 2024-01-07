#include "vtkSmartPointer.h"
#include "vtkNew.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkSphereSource.h"
#include "vtkSpsPolyDataFileSeriesReader.h"
#include <vtkTesting.h>
#include <vtkTestUtilities.h>
#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <sstream>
#include <string>
#include <vector>

int TestSpsPolyDataFileSeriesReader(int argc, char* argv[])
{
  const int nFiles = 8;

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

  vtkNew<vtkSphereSource> sphereSource;

  vtkNew<vtkXMLPolyDataWriter> polyWriter;

  std::stringstream sstream;
  std::vector<std::string> fileNames;

  for (int iFile = 0 ; iFile < nFiles ; iFile++) {
    sphereSource->SetThetaResolution(3 + iFile);
    sphereSource->SetPhiResolution(3 + iFile);
    sphereSource->Update();
    
    std::stringstream().swap(sstream);
    sstream << "testFile" << std::setfill('0') << std::setw(2) << iFile;
    sstream << ".vtk";

    std::string fileName = sstream.str();
    fileName = outpath + fileName;
    fileNames.push_back(fileName);
    polyWriter->SetFileName(fileName.c_str());
    polyWriter->SetInputData(sphereSource->GetOutput());
    polyWriter->Write();
  }

  vtkNew<vtkSpsPolyDataFileSeriesReader> seriesReader;

  vtkNew<vtkXMLPolyDataReader> reader;
  seriesReader->SetReader(reader);

  for (const std::string& str : fileNames) {
    seriesReader->AddFileName(str.c_str());
  }
  seriesReader->Update();

  vtkInformation* info = seriesReader->GetOutputInformation(0);

  int expectedNumberOfPoints[nFiles] = { 5, 10, 17, 26, 37, 50, 65, 82 };

  vtkPolyData* polyOutput;

  int retval = EXIT_SUCCESS;

  for (int i = 0; i < nFiles; i++)
  {
    info->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), (double) i);
    seriesReader->Update();
    polyOutput = vtkPolyData::SafeDownCast(seriesReader->GetOutput());
    int nPoints = polyOutput->GetNumberOfPoints();
    retval |= (nPoints != expectedNumberOfPoints[i]);
  }
  // 3 -> 5
  // 4 -> 13
  // 5 -> 17
  return EXIT_SUCCESS;
}

