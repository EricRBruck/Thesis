#include <iostream>
#include "PM.h"
#include "QG.h"

using namespace std;

int main()
{
  PM pm;
  /*QG qg;
  question_info Qinfo;
  sentence_data curr_sentence;

  qg.GetTestData();

  for ( int i = 0; i < 10; i++ ) {
    Qinfo=pm.QuestionInfo(0);
    cout<<Qinfo.level<<" - "<<Qinfo.intent<<" - "<<Qinfo.type<<endl;
    curr_sentence=qg.Generator(Qinfo.level,Qinfo.intent,Qinfo.type);
    pm.DoThings(curr_sentence);
    sleep (((i+1)%3));  
  }
  */

  pm.START();
}
