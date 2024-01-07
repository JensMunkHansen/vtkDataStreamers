#include "vtkTemplate.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkTemplate)

vtkTemplate::vtkTemplate()
{
}

vtkTemplate::~vtkTemplate()
{
}

double vtkTemplate::EvaluateFunction(double x[3])
{
  return 0.0;
}

void vtkTemplate::EvaluateGradient(double x[3], double n[3])
{
}
