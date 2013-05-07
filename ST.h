#ifndef ST_H
#define ST_H
#include <string>
#include <vector>
#include "PM.h"

using namespace std;

class ST
{
 private:
  ST_domain DomainModel;    //Domain Model
  ST_student StudentModel;  //Student Model

  //Create Domain Model
  void InitDomainModel(const char*,int);
  void PrintDomain();

  //Create Student Model
  void InitStudentModel(const char*,int);
  void PrintStudent();

  //Save error to error log
  void SaveErrorToLog(string);

 public:
  
  //Constructor / Destructor
  ST();
  ~ST();

  //Initialize the student tracker
  void initST(string);

  //Return Domain and Student Model
  ST_domain ReturnDomainModel();
  ST_student ReturnStudentModel();
};

#endif
