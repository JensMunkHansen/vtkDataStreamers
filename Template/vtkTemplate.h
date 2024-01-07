#ifndef vtkTemplate_h
#define vtkTemplate_h

#include "vtkTemplateModule.h"
#include "vtkImplicitFunction.h"

class VTKTEMPLATE_EXPORT vtkTemplate : public vtkImplicitFunction
{
 public:
  static vtkTemplate* New();
  vtkTypeMacro(vtkTemplate, vtkImplicitFunction)

  void PrintSelf(ostream& os, vtkIndent indent) override;

  double EvaluateFunction(double x[3]) override;
  void EvaluateGradient(double x[3], double g[3]) override;
protected:
  vtkTemplate();
  ~vtkTemplate() override;

private:
  vtkTemplate(const vtkTemplate&) = delete;
  void operator=(const vtkTemplate&) = delete;
  
};
#endif
