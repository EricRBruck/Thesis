#include <iostream>
#include <fstream>
#include <vector>
#include "ST.h"
#include "PM.h"

using namespace std;

//=================================================================
//          CONSTRUCTOR / DESTRUCTOR
//=================================================================

/*****************************************************************/
/** Constructor - Student Tracker                               **/
/**                                                             **/
/** Initalize the domain model and student model                **/
/*****************************************************************/
ST::ST()
{
  ST_intent temp_intent;
  temp_intent.intent = "-";
  temp_intent.link = "-";
  temp_intent.node = "-";
  temp_intent.focus = "-";
  temp_intent.article_type = "-";
  temp_intent.count = 0;

  for ( int i = 0; i < 10; i++ )
    { DomainModel.intent[i] = temp_intent; }

  ST_student_intent temp_student_intent;
  temp_student_intent.count = 0;

  for ( int i = 0; i < 10; i++ )
    { StudentModel.intent[i] = temp_student_intent; }
}

/*****************************************************************/
/** Destructor - Student Tracker                                **/
/*****************************************************************/
ST::~ST()
{
  ;
}

/*****************************************************************/
/** initialize student tracker                                  **/
/**                                                             **/
/** Description:                                                **/
/** initialize the students progress when the program first     **/
/** starts                                                      **/
/*****************************************************************/
void ST::initST(string path)
{
  
  string filepath = path + "/domain/DM_1.txt";
  InitDomainModel(filepath.c_str(),0);
  filepath = path + "/domain/DM_1P.txt";
  InitDomainModel(filepath.c_str(),1);
  filepath = path + "/domain/DM_2.txt";
  InitDomainModel(filepath.c_str(),2);
  filepath = path + "/domain/DM_2P.txt";
  InitDomainModel(filepath.c_str(),3);
  filepath = path + "/domain/DM_3.txt";
  InitDomainModel(filepath.c_str(),4);
  filepath = path + "/domain/DM_3P.txt";
  InitDomainModel(filepath.c_str(),5);
  filepath = path + "/domain/DM_4.txt";
  InitDomainModel(filepath.c_str(),6);
  filepath = path + "/domain/DM_4P.txt";
  InitDomainModel(filepath.c_str(),7);
  filepath = path + "/domain/DM_5.txt";
  InitDomainModel(filepath.c_str(),8);
  filepath = path + "/domain/DM_5P.txt";
  InitDomainModel(filepath.c_str(),9);
  //PrintDomain();

  filepath = path + "/student/SM_1.txt";
  InitStudentModel(filepath.c_str(),0);
  filepath = path + "/student/SM_1P.txt";
  InitStudentModel(filepath.c_str(),1);
  filepath = path + "/student/SM_2.txt";
  InitStudentModel(filepath.c_str(),2);
  filepath = path + "/student/SM_2P.txt";
  InitStudentModel(filepath.c_str(),3);
  filepath = path + "/student/SM_3.txt";
  InitStudentModel(filepath.c_str(),4);
  filepath = path + "/student/SM_3P.txt";
  InitStudentModel(filepath.c_str(),5);
  filepath = path + "/student/SM_4.txt";
  InitStudentModel(filepath.c_str(),6);
  filepath = path + "/student/SM_4P.txt";
  InitStudentModel(filepath.c_str(),7);
  filepath = path + "/student/SM_5.txt";
  InitStudentModel(filepath.c_str(),8);
  filepath = path + "/student/SM_5P.txt";
  InitStudentModel(filepath.c_str(),9);
  //PrintStudent();
}

//=================================================================
//          DOMAIN MODEL
//=================================================================

/*****************************************************************/
/** Initialize Domain Model                                     **/
/**                                                             **/
/** Description:                                                **/
/** Get all the questions the student has been asked in         **/
/** previous sessions and save them to the domain model linked  **/
/** list                                                        **/
/**                                                             **/
/** Parameters:                                                 **/
/** fiename: The name of the file to get domain knowledge from  **/
/** INTENT: Number representing the intent.  The numbers relate **/
/**   the each intent as follows:                               **/
/**   INTENT == 0 -> intent 1                                   **/
/**   INTENT == 1 -> intent 1P                                  **/
/**   INTENT == 2 -> intent 2                                   **/
/**   INTENT == 3 -> intent 2P                                  **/
/**   INTENT == 4 -> intent 3                                   **/
/**   INTENT == 5 -> intent 3P                                  **/
/**   INTENT == 6 -> intent 4                                   **/
/**   INTENT == 7 -> intent 4P                                  **/
/**   INTENT == 8 -> intent 5                                   **/
/**   INTENT == 9 -> intent 5P                                  **/
/*****************************************************************/
void ST::InitDomainModel(const char* filename, int INTENT)
{
  ST_intent temp_intent;
  temp_intent.count = -1;
  ST_question temp_question;

  int word_count = 0;
  int prevID = 0;
  string line = "";
  string word = "";
  string errortxt;

  ifstream myfile;
  myfile.open(filename);
  if ( !myfile.is_open() ) { errortxt = "ERROR ST_001: Domain file could not be opened."; SaveErrorToLog(errortxt); return; }

  while ( myfile >> line )
    {
      temp_intent.count++;

      if ( temp_intent.count == 0 )
	{
	  word_count = 0;
	  for ( int i = 0; i < line.length(); i++ )
	    {
	      if ( line[i] != '|' ) { word += line[i]; }
	      else
		{
		  switch (word_count) {
		  case 0: temp_intent.intent = word; break;
		  case 1: temp_intent.link = word; break;
		  case 2: temp_intent.focus = word; break;
		  case 3: temp_intent.node = word; break;
		  case 4: temp_intent.article_type = word; break; }
		  word_count++;
		  word = "";
		}
	    }
	}
      else
	{
	  word_count = 0;
	  for ( int i = 0; i < line.length(); i++ )
	    {
	      if ( line[i] != '|' ) { word += line[i]; }
	      else
		{
		  if ( word_count == 0 )
		    {
		      if ( atoi(word.c_str()) == prevID )
			{ errortxt = "ERROR ST_002: repeated question ID - "+prevID; SaveErrorToLog(errortxt); temp_intent.count--; word = ""; break; }
		    }
		  switch (word_count) {
		  case 0: temp_question.ID = atoi(word.c_str()); prevID = atoi(word.c_str()); break;
		  case 1: temp_question.level = word; break;
		  case 2: temp_question.question_type = word; break;
		  case 3: temp_question.question = word; break;
		  case 4: temp_question.sentence = word; break;
		  case 5: temp_question.article = word; break;
		  case 6: temp_question.object1 = word; break;
		  case 7: temp_question.object2 = word; break; }
		  word_count++;
		  word = "";
		}
	    }
	  temp_intent.question.push_back(temp_question);
	}
    }
  DomainModel.intent[INTENT] = temp_intent;
  myfile.close();
}

/*****************************************************************/
/** Print Domain Model                                          **/
/**                                                             **/
/** Description:                                                **/
/** Print the domain model for each intent                      **/
/*****************************************************************/
void ST::PrintDomain()
{
  for ( int j = 0; j < 10; j++ )
    {
      cout<<"Intent: "<<DomainModel.intent[j].intent<<endl;
      cout<<"Link: "<<DomainModel.intent[j].link<<endl;
      cout<<"Node: "<<DomainModel.intent[j].node<<endl;
      cout<<"Focus: "<<DomainModel.intent[j].focus<<endl;
      cout<<"Article Type: "<<DomainModel.intent[j].article_type<<endl;
      cout<<"Count: "<<DomainModel.intent[j].count<<endl;
      for ( int i = 0; i < DomainModel.intent[j].count; i++ )
	{
	  cout<<"\tID: "<<DomainModel.intent[j].question[i].ID<<endl;
	  cout<<"\tLevel: "<<DomainModel.intent[j].question[i].level<<endl;
	  cout<<"\tQ Typle: "<<DomainModel.intent[j].question[i].question_type<<endl;
	  cout<<"\tQuestion: "<<DomainModel.intent[j].question[i].question<<endl;
	  cout<<"\tSentence: "<<DomainModel.intent[j].question[i].sentence<<endl;
	  cout<<"\tArticle: "<<DomainModel.intent[j].question[i].article<<endl;
	  cout<<"\tObject 1: "<<DomainModel.intent[j].question[i].object1<<endl;
	  cout<<"\tObject 2: "<<DomainModel.intent[j].question[i].object2<<endl;
	}
    }
}

/*****************************************************************/
/** Return Domain Model                                         **/
/**                                                             **/
/** Description:                                                **/
/** Return the domain model.  This is used to send the domain   **/
/** model the the pedogogical model.                            **/
/*****************************************************************/
ST_domain ST::ReturnDomainModel()
{
  return DomainModel;
}

//=================================================================
//          STUDENT MODEL
//=================================================================

/*****************************************************************/
/** Initialize Student Model                                    **/
/**                                                             **/
/** Description:                                                **/
/** Get all the ansers the student has provided in previous     **/
/** sessions and save them to the student model linked list     **/
/**                                                             **/
/** Parameters:                                                 **/
/** fiename: The name of the file to get domain knowledge from  **/
/** INTENT: Number representing the intent.  The numbers relate **/
/**   the each intent as follows:                               **/
/**   INTENT == 0 -> intent 1                                   **/
/**   INTENT == 1 -> intent 1P                                  **/
/**   INTENT == 2 -> intent 2                                   **/
/**   INTENT == 3 -> intent 2P                                  **/
/**   INTENT == 4 -> intent 3                                   **/
/**   INTENT == 5 -> intent 3P                                  **/
/**   INTENT == 6 -> intent 4                                   **/
/**   INTENT == 7 -> intent 4P                                  **/
/**   INTENT == 8 -> intent 5                                   **/
/**   INTENT == 9 -> intent 5P                                  **/
/*****************************************************************/
void ST::InitStudentModel(const char* filename, int INTENT)
{
  ST_student_intent temp_intent;
  temp_intent.count = -1;
  ST_student_question temp_question;
  int word_count = 0;
  int prevID = 0;
  string line = "";
  string word = "";
  string errortxt;
  
  ifstream myfile;
  myfile.open(filename);
  if ( !myfile.is_open() ) { errortxt = "ERROR ST_003: Student file could not be opened."; SaveErrorToLog(errortxt); return; }
  
  while ( myfile >> line )
  {
    temp_intent.count++;

    if ( temp_intent.count == 0 )
      {
	temp_intent.intent = line;
      }
    else
      {
	word_count = 0;
	for ( int i = 0; i < line.length(); i++ )
	  {
	    if ( line[i] != '|' ) { word += line[i]; }
	    else
	      {
		switch (word_count) {
		case 0: temp_question.ID = atoi(word.c_str()); prevID = atoi(word.c_str()); break;
		case 1: temp_question.level = word; break;
		case 2: temp_question.question_type = word; break;
		case 3: temp_question.question = word; break;
		case 4: temp_question.sentence = word; break;
		case 5: temp_question.article = word; break;
		case 6: temp_question.object1 = word; break;
		case 7: temp_question.object2 = word; break;
		case 8: temp_question.noun_phrase = word; break; 
		case 9: temp_question.intent = word; break;
		case 10: temp_question.link = word; break;
		case 11: temp_question.node = word; break;
		case 12: temp_question.focus = word; break; 
		case 13: temp_question.article_type = word; break; }
		word_count++;
		word = "";
	      }
	  }
	temp_intent.question.push_back(temp_question);
      }
  }
  StudentModel.intent[INTENT] = temp_intent;
  myfile.close();
}

/*****************************************************************/
/** Print Student Model                                         **/
/**                                                             **/
/** Description:                                                **/
/** Print the student model for each intent                     **/
/*****************************************************************/
void ST::PrintStudent()
{
  for ( int j = 0; j < 10; j++ )
    {
      if(j==0){cout<<"intent1"<<endl;}
      if(j==1){cout<<"intent1P"<<endl;}
      if(j==2){cout<<"intent2"<<endl;}
      if(j==3){cout<<"intent2P"<<endl;}
      if(j==4){cout<<"intent3"<<endl;}
      if(j==5){cout<<"intent3P"<<endl;}
      if(j==6){cout<<"intent4"<<endl;}
      if(j==7){cout<<"intent4P"<<endl;}
      if(j==8){cout<<"intent5"<<endl;}
      if(j==9){cout<<"intent5P"<<endl;}
      for ( int i = 0; i < StudentModel.intent[j].count; i++ )
	{
	  cout<<"\tIntent: "<<StudentModel.intent[j].question[i].intent<<endl;
	  cout<<"\tLink: "<<StudentModel.intent[j].question[i].link<<endl;
	  cout<<"\tNode: "<<StudentModel.intent[j].question[i].node<<endl;
	  cout<<"\tFocus: "<<StudentModel.intent[j].question[i].focus<<endl;
	  cout<<"\tArticle Type: "<<StudentModel.intent[j].question[i].article_type<<endl;
	  cout<<"\tID: "<<StudentModel.intent[j].question[i].ID<<endl;
	  cout<<"\tLevel: "<<StudentModel.intent[j].question[i].level<<endl;
	  cout<<"\tQ Type: "<<StudentModel.intent[j].question[i].question_type<<endl;
	  cout<<"\tQuestion: "<<StudentModel.intent[j].question[i].question<<endl;
	  cout<<"\tSentence: "<<StudentModel.intent[j].question[i].sentence<<endl;
	  cout<<"\tArticle: "<<StudentModel.intent[j].question[i].article<<endl;
	  cout<<"\tObject1: "<<StudentModel.intent[j].question[i].object1<<endl;
	  cout<<"\tObject2: "<<StudentModel.intent[j].question[i].object2<<endl;
	  cout<<"\tNoun Phrase: "<<StudentModel.intent[j].question[i].noun_phrase<<endl;
	}
    }
}

/*****************************************************************/
/** Return Student Model                                        **/
/**                                                             **/
/** Description:                                                **/
/** Return the student model.  This is used to send the student **/
/** model the the pedogogical model.                            **/
/*****************************************************************/
ST_student ST::ReturnStudentModel()
{
  return StudentModel;
}

/*****************************************************************/
/** Save error message to error log                             **/
/**                                                             **/
/** Description:                                                **/
/** Save error message to the error_log.txt file                **/
/**                                                             **/
/** Parameters:                                                 **/
/** error - the error message to save                           **/
/*****************************************************************/
void ST::SaveErrorToLog(string error)
{
  ofstream myError;
  myError.open("error_log.txt", fstream::app);
  if ( !myError.is_open() )
    { cout<<"ERROR: Cannot open Error Log"<<endl; }
  myError<<error<<"\n";
  myError.close();
}
