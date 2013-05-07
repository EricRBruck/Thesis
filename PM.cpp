#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "PM.h"
#include "QG.h"
#include "ST.h"
#include "UI.h"

using namespace std;

//================================================================= 
//          CONSTRUCTOR / DESTRUCTOR
//=================================================================

PM::PM()
{
  user = "";
  userPath = "";

  //Students knowledge
  ST_INTENT init_intent;
  init_intent.full = false;
  init_intent.all = false;
  init_intent.link = false;
  init_intent.focus = false;
  init_intent.node = false;
  init_intent.type = false;
  init_intent.article = false;
  init_intent.object = false;
  init_intent.noun_phrase = false;
  
  ST_LEVEL init_level;
  init_level.full = false;
  for ( int i = 0; i < 10; i++ )
    { init_level.intent[i] = init_intent; }

  SP_introduction = init_level;

  init_intent.type = true;
  init_intent.article = true;
  init_intent.object = true;
  init_intent.noun_phrase = true;
  for ( int i = 0; i < 10; i++ )
    { init_level.intent[i] = init_intent; }

  SP_diagram_intent = init_level;
  SP_intent_diagram = init_level;

  init_intent.all = true;
  init_intent.article = false;
  init_intent.type = false;
  init_intent.link = true;
  init_intent.node = true;
  init_intent.focus = true;
  init_intent.object = true;
  init_intent.noun_phrase = true;
  for ( int i = 0; i < 10; i++ )
    { init_level.intent[i] = init_intent; }

  SP_article_intent = init_level;
  SP_article_diagram = init_level;  

  curr_intro_section = 0;
  disp_intro_txt = 0;
  program_start = 0;

  curr_sentence.ID = 0;
  curr_sentence.sentence = "";
  curr_sentence.article = "";
  curr_sentence.object1 = "";
  curr_sentence.object2 = "";
  curr_sentence.intent = "";
  curr_sentence.link = "";
  curr_sentence.focus = "";
  curr_sentence.node = "";
  curr_sentence.type = "";
  curr_sentence.question = "";
  curr_sentence.question_level = "";
  curr_sentence.question_type = "";

  InitFile((char*)"users/user_names.txt","");

  Qinfo.level = "";
  Qinfo.type = "";
  Qinfo.intent = "";

  for ( int j = 0; j < 9; j++ ) { CorrectAnswers[j] = 1; }

  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 8; i++ ) { StudentProgress.intro[j][i] = 0; } }
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 4; i++ ) { StudentProgress.diagram[j][i] = 0; } }
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 2; i++ ) { StudentProgress.article1[j][i] = 0; } }
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 2; i++ ) { StudentProgress.article2[j][i] = 0; } }

}

PM::~PM()
{
  ;
}

//================================================================= 
//          START THE SYSTEM
//=================================================================

/*****************************************************************/
/** System Start                                                **/
/**                                                             **/
/** Description:                                                **/
/** Call all functions to run the system.  This is the function **/
/** called by the client.  This is the only public function in  **/
/** the PM.                                                     **/
/**                                                             **/
/** Parameters:                                                 **/
/**                                                             **/
/** Algorithm:                                                  **/
/** 1. Get User                                                 **/
/** 2. Initialize all user information                          **/
/** 3. while student wants to continue                          **/
/**  3.a. get question and sentence                             **/
/**  3.b. save question to domain model                         **/
/**  3.c. display question to user                              **/
/**  3.d. get answer from user                                  **/
/**  3.e. while answer is wrong and student gets another try    **/
/**   3.e.i. save question and answer to student model          **/
/**   3.e.ii. provide hint and ask again                        **/
/**   3.e.iii. get answer from user                             **/
/**  3.f. save question and answer to student model             **/
/**  3.g. if answer is not correct provide hint                 **/
/**  3.h. if answer is correct, inform studetn                  **/
/**  3.i. Update student progress                               **/
/**  3.j. Ask if the user wants to continue                     **/
/** 4. quit                                                     **/
/*****************************************************************/
int PM::START()
{
  ST st;
  QG qg;
  UI ui;

  int ans = -1;
  string action = "";
  int correct = 0;
  int answer_limit = 3;

  //qg.GetTestData(); //test data - FOR TESTING

  //Get User
  user = ui.GetUser();
  if ( user[user.length()-1] == '-' )
    { AddUser(); cout<<endl; }
  else { userPath = "users/" + user; }

  //init files
  InitDomainFiles();
  InitStudentFiles();
  initHints();

  //init student tracker
  st.initST(userPath);
  
  //Generate the domain and student models
  //cout<<"\nGenerating Domain Model Linked List"<<endl;
  DomainModel = st.ReturnDomainModel();
  //cout<<"Generating Student Model Linked List"<<endl;
  StudentModel = st.ReturnStudentModel();
  
  //Update student tracker and student progress
  initUpdateST();
  
  while ( action != "Q" && action != "q" ) {

    //get the question info and store it in Qinfo
    //cout<<"Generate Type of Question"<<endl;
    QuestionInfo(0);

    //Generate the sentence and sentence data
    //cout<<"Generate Sentence Data"<<endl;
    curr_sentence = qg.Generator(Qinfo.level,Qinfo.intent,Qinfo.type);
    
    //Set question ID
    SetQuestionID();
    
    //Save the question to the domain
    //cout<<"Save Question to Domain Model"<<endl;
    SaveQuestionToDomain();

    //Add question to domain linked list
    //cout<<"Add Question to Domain Model Linked List"<<endl;
    AddQuestionToDomain();

    //Get the number of trys the student has
    answer_limit = SetNumberOfAnswers();

    //Set values if the user has just started the program
    if ( program_start == 0 ) { cin.ignore(); disp_intro_txt = 0; program_start=1; }

    //Get student answer
    student_sentence = ui.AskQuestion(curr_sentence,disp_intro_txt);

    //Add question to student linked list and save it to the student model file
    //cout<<"Save Question to Student Model"<<endl;
    SaveQuestionToStudent();
    //if ( AddQuestionToStudent() == -1 ) { exit(0); }
    AddQuestionToStudent();
  
    //Print correct answer and student answer  - FOR TESTING
    //cout<<"\nDOMAIN QUESTION"<<endl;
    //PrintQuestion(curr_sentence);
    //cout<<"\nSTUDNET QUESTION"<<endl;
    //PrintQuestion(student_sentence);

    //check if answer is correct
    correct = CheckAnswer();
    //Update student tracker and student progress
    UpdateST(translate2intent(),translate2type());
    UpdateSP();
    answer_limit--;

    while ( correct == 0 && answer_limit > 0 ) {

      //Get a hint
      selectHint();
      cout<<"***********************************************"<<endl;
      cout<<"HINT: "<<HintForStudent<<endl;
      cout<<"***********************************************\n"<<endl;
      
      //Ask for answer
      student_sentence = ui.AskAgain();

      //Add question to student linked list and save it to the student model file
      //cout<<"Save Question to Student Model"<<endl;
      SaveQuestionToStudent();
      //cout<<"Add Question to Student Model Linked List"<<endl;
      AddQuestionToStudent();
      
      //Print correct answer and student answer  - FOR TESTING
      //cout<<"\nDOMAIN QUESTION"<<endl;
      //PrintQuestion(curr_sentence);
      //cout<<"\nSTUDNET QUESTION"<<endl;
      //PrintQuestion(student_sentence);
      
      //check is answer is correct
      correct = CheckAnswer();
      if ( correct == 1 ) { break; }
      answer_limit--;
    }

    if ( correct == 1 && curr_intro_section > 0 ) {
      cout<<"CORRECT ANSWER"<<endl;
    }
    else if ( curr_intro_section > 0 ) {
      selectHint();
      cout<<"***********************************************"<<endl;
      cout<<"HINT: "<<HintForStudent<<endl;
      cout<<"***********************************************\n"<<endl;
      ui.provideCorrectAnswer(curr_sentence);
    }

    cin.clear();
    sleep(1);

    //Check if done
    if ( SP_introduction.full == true &&
	 SP_diagram_intent.full == true &&
	 SP_article_diagram.full == true &&
	 SP_article_intent.full == true )
      { cout<<"PROGRAM COMPLETE!!!"<<endl; break; }

    cout<<"\nEnter 'Q' to quit or 'C' to continue: ";
    cin>>action;
    cin.ignore();
    cout<<endl;
  }
}

//================================================================= 
//          QUESTION INFORMATION SELECTION
//=================================================================

/*****************************************************************/
/** Question Information                                        **/
/**                                                             **/
/** Description:                                                **/
/** Pick the type of question to ask based on either the        **/
/** students progress or at random.                             **/
/**                                                             **/
/** Parameters:                                                 **/
/** order: this value determins if the question type will be    **/
/**   selected based on the student progress or at random.      **/
/**   order = 0: Question type based on student progress        **/
/**   order = 1: Question type selected at random               **/
/**                                                             **/
/** Algorithm:                                                  **/
/** if selecting question type based on student progress.       **/
/**   If the student does not have a full understanging of the  **/
/**   introduction, set the level, type, and intent             **/
/**   If the student does not have a full understanging of the  **/
/**   diagrams relation to the intents, set the level, type,    **/
/**   and intent                                                **/
/**   If the student does not have a full understanging of the  **/
/**   intents relation to the diagrams, set the level, type,    **/
/**   and intent                                                **/
/**   If the student does not have a full understanging of the  **/
/**   diagrams relation to the articles, set the level, type,   **/
/**   and intent                                                **/
/**   If the student does not have a full understanging of the  **/
/**   intents relation to the articles, set the level, type,    **/
/**   and intent                                                **/
/**   if the level, type or intent was not set, then select it  **/
/**   at random.                                                **/
/** If selecting the question type at random.                   **/
/**   If the level does not have a value, select it at random.  **/
/**   If the intent does not have a value, select it at random. **/
/**   If the type does not have a value, selectit at random.    **/
/** Translate the numbers into words.  This is the data that is **/
/** sent to the Question Generator.                             **/
/*****************************************************************/
question_info PM::QuestionInfo(int order)
{
  int level = -1;
  int type = -1;
  int intent = -1;
  int all_intents[10] = {0};
  if ( order == 0 )
    {
      if ( SP_introduction.full == false ) {
	level = 1;
	intent = QuestionIntent(SP_introduction);
	if ( SP_introduction.intent[intent].all == false ) { type = 0; }
	else if ( SP_introduction.intent[intent].type == false ) { type = 4; }
	else if ( SP_introduction.intent[intent].article == false ) { type = 5; }
	else if ( SP_introduction.intent[intent].object == false ) { type = 6; }
	else if ( SP_introduction.intent[intent].noun_phrase == false ) { type = 7; }
	else if ( SP_introduction.intent[intent].link == false ) { type = 1; }
	else if ( SP_introduction.intent[intent].node == false ) { type = 2; }
	else if ( SP_introduction.intent[intent].focus == false ) { type = 3; }
	else { type = QuestionType(SP_introduction.intent[intent]); }
      }
      else if ( SP_diagram_intent.full == false ) {
	level = 2;
	intent = QuestionIntent(SP_diagram_intent);
	if ( SP_diagram_intent.intent[intent].link == false ) { type = 1; }
	else if ( SP_diagram_intent.intent[intent].node == false ) { type = 2; }
	else if ( SP_diagram_intent.intent[intent].focus == false ) { type = 3; }
	else if ( SP_diagram_intent.intent[intent].all == false ) { type = 0; }
	else { type = QuestionType(SP_diagram_intent.intent[intent]); }
      }
      else if ( SP_article_diagram.full == false ){
	level = 4;
	intent = QuestionIntent(SP_article_diagram);
	if ( SP_article_diagram.intent[intent].type == false ) { type = 4; }
	else if ( SP_article_diagram.intent[intent].article == false ) { type = 5; }
      }
      else if ( SP_article_intent.full == false ) {
	level = 5;
	intent = QuestionIntent(SP_article_intent);
	if ( SP_article_intent.intent[intent].type == false ) { type = 4; }
	else if ( SP_article_intent.intent[intent].article == false ) { type = 5; }
      }
      else { order = 1; }

      if ( level == -1 ) { order = 1;
	SaveErrorToLog("ERROR PM_001: level was not selected -> making random level selection"); }
      if (intent == -1 ) { order = 1; 
	SaveErrorToLog("ERROR PM_002: intent was not selected -> making random intent selection"); }
      if ( type == -1 ) { order = 1;
	SaveErrorToLog("ERROR PM_003: type was not selected -> making random type selection"); }
    }
  if ( order == 1 )
    {
      if ( level == -1 ) { level = QuestionCategory(); }
      if ( intent == -1 ) { intent = QuestionIntent(); }
      if ( type == -1 ) { type = QuestionType(level); }
    }
  
  translate(level,type,intent);
  return Qinfo;
}

/*****************************************************************/
/** Question Intent                                             **/
/**                                                             **/
/** Description:                                                **/
/** Pick the question Intent based on the level of the question **/
/**                                                             **/
/** Parameters:                                                 **/
/** level: The structure containing the information about what  **/
/** the student knows for that level of questions.  This is one **/
/** of the following structures.                                **/
/**   SP_introduction                                           **/
/**   SP_diagram_intent                                         **/
/**   SP_intent_diagram                                         **/
/**   SP_article_diagram                                        **/
/**   SP_diagram_article                                        **/
/**                                                             **/
/** Algorithm:                                                  **/
/** Select an intent at random.  Check if the student has       **/
/** mastered that intent.  If the student has mastered the      **/
/** intent then select the next intent untill one is selected   **/
/** that has not been mastered by the student.                  **/
/*****************************************************************/
int PM::QuestionIntent(ST_LEVEL level)
{
  int start,num;
  srand (time(NULL));
  num = ( rand() % 10 );
  start = num;

  do
    {
      if ( curr_intro_section == 0 ) {
	if ( level.intent[num].all == false ) { return num; } }
      else if ( curr_intro_section == 1 ) {
	if ( level.intent[num].type == false ) { return num; } }
      else if ( curr_intro_section == 2 ) {
	if ( level.intent[num].article == false ) { return num; } }
      else if ( curr_intro_section == 3 ) {
	if ( level.intent[num].object == false ) { return num; } }
      else if ( curr_intro_section == 4 ) {
	if ( level.intent[num].noun_phrase == false ) { return num; } }
      else if ( curr_intro_section == 5 ) {
	if ( level.intent[num].link == false ) { return num; } }
      else if ( curr_intro_section == 6 ) {
	if ( level.intent[num].node == false ) { return num; } }
      else if ( curr_intro_section == 7 ) {
	if ( level.intent[num].focus == false ) { return num; } }
      else if ( curr_intro_section == 8 ) {
	if ( level.intent[num].full == false ) { return num; } }
      num = (num+1)%10;
      } while ( start != num );
  SaveErrorToLog("ERROR PM_004: unable to generate intent");
  return -1;
}

/*****************************************************************/
/** Question Intent                                             **/
/**                                                             **/
/** Description:                                                **/
/** Select the next intent at random.                           **/
/*****************************************************************/
int PM::QuestionIntent()
{
  srand (time(NULL));
  return (rand() % 10);
}

/*****************************************************************/
/** Question Type                                               **/
/**                                                             **/
/** Description:                                                **/
/** Generate the type of question                               **/
/**                                                             **/
/** Parameters:                                                 **/
/** SP_intent - the struct indicating what the student knows    **/
/**                                                             **/
/** Algorithm:                                                  **/
/** Find a type of question that has not been mastered through  **/
/** the use of a predefined order.                              **/
/**                                                             **/
/*****************************************************************/
int PM::QuestionType(ST_INTENT SP_intent)
{
  if ( SP_intent.link == false ) { return 1; }
  if ( SP_intent.node == false ) { return 2; }
  if ( SP_intent.focus == false ) { return 3; }
  if ( SP_intent.type == false ) { return 4; }
  SaveErrorToLog("ERROR PM_004: unable to generate type");
  return -1;
}

/*****************************************************************/
/** Question Type                                               **/
/**                                                             **/
/** Description:                                                **/
/** Generate the type of question at random                     **/
/**                                                             **/
/** Parameters:                                                 **/
/** level - level of question being asked                       **/
/**                                                             **/
/** Algorithm:                                                  **/
/** Select the type at random.                                  **/
/**                                                             **/
/*****************************************************************/
int PM::QuestionType(int level)
{
  int num = 5;
  if ( level < 4 ) { num = 4; }
  srand (time(NULL));
  return (rand() % num) + 1;
}

/*****************************************************************/
/** Question Category                                           **/
/**                                                             **/
/** Description:                                                **/
/** Select the level of the question at random                  **/
/**                                                             **/
/** Parameters:                                                 **/
/**                                                             **/
/** Algorithm:                                                  **/
/** Select the level at random                                  **/
/**                                                             **/
/*****************************************************************/
int PM::QuestionCategory()
{
  int count = 0;
  int array[5] = {-1,-1,-1,-1,-1};

  if ( SP_introduction.full == false ) { array[count] = 1; count++; }
  if ( SP_diagram_intent.full == false ) { array[count] = 2; count++; }
  if ( SP_intent_diagram.full == false ) { array[count] = 3; count++; }
  if ( SP_article_diagram.full == false ) { array[count] = 4; count++; }
  if ( SP_article_intent.full == false ) { array[count] = 5; count++; }

  if ( count == 0 ) { for(count; count<5; count++) array[count]=count+1; }

  srand (time(NULL));
  int num = rand() % count;
  return array[num];
}

/*****************************************************************/
/** Translate numbers to data                                   **/
/**                                                             **/
/** Description:                                                **/
/** Translate numbers to words                                  **/
/**                                                             **/
/** Parameters:                                                 **/
/** level - the level of the question                           **/
/** type - the type of question                                 **/
/** intent - the intent of the question                         **/
/*****************************************************************/
void PM::translate(int level, int type, int intent)
{
  srand(time(NULL));
  int num = 0;
  string errortxt = "";
  if (level == 1 ) { Qinfo.level = "INTRODUCTION"; }
  else if (level == 2 ) { Qinfo.level = "DIAGRAM"; }
  else if (level == 3 ) { Qinfo.level = "INTENT"; }
  else if (level == 4 ) { Qinfo.level = "ARTICLE_DIAGRAM"; }
  else if (level == 5 ) { Qinfo.level = "ARTICLE_INTENT"; }
  else { errortxt = "ERROR PM_006: invalid level; level = "+level; SaveErrorToLog(errortxt); Qinfo.level = "INTRODUCTION"; }

  if ( intent == 0 ) { Qinfo.intent="1"; }
  else if ( intent == 1 ) { Qinfo.intent="1P"; }
  else if ( intent == 2 ) { Qinfo.intent="2"; }
  else if ( intent == 3 ) { Qinfo.intent="2P"; } 
  else if ( intent == 4 ) { Qinfo.intent="3"; }
  else if ( intent == 5 ) { Qinfo.intent="3P"; } 
  else if ( intent == 6 ) { Qinfo.intent="4"; }
  else if ( intent == 7 ) { Qinfo.intent="4P"; } 
  else if ( intent == 8 ) { Qinfo.intent="5"; }
  else if ( intent == 9 ) { Qinfo.intent="5P"; }
  else { errortxt = "ERROR PM_007: invalid intent; intent = "+intent; SaveErrorToLog(errortxt); Qinfo.intent = "1"; }

  if ( type == 0 ) { Qinfo.type = "ALL"; }
  else if ( type == 1 ) { Qinfo.type = "LINK"; }
  else if ( type == 2 ) { Qinfo.type = "NODE"; }
  else if ( type == 3 ) { Qinfo.type = "FOCUS"; }
  else if ( type == 4 ) { Qinfo.type = "TYPE"; }
  else if ( type == 5 ) { Qinfo.type = "ARTICLE"; }
  else if ( type == 6 ) { Qinfo.type = "OBJECT"; }
  else if ( type == 7 ) { Qinfo.type = "NOUNPHRASE"; }
  else { errortxt = "ERROR PM_008: invalid type; type = "+type; SaveErrorToLog(errortxt); Qinfo.type = "ALL"; }
}

/*****************************************************************/
/** Translate intent to number                                  **/
/**                                                             **/
/** Description:                                                **/
/** Translate word intent to number                             **/
/*****************************************************************/
int PM::translate2intent()
{
  if ( curr_sentence.intent == "1" ) return 0; 
  else if ( curr_sentence.intent == "1P" ) return 1; 
  else if ( curr_sentence.intent == "2" ) return 2; 
  else if ( curr_sentence.intent == "2P" ) return 3;
  else if ( curr_sentence.intent == "3" ) return 4; 
  else if ( curr_sentence.intent == "3P" ) return 5;
  else if ( curr_sentence.intent == "4" ) return 6; 
  else if ( curr_sentence.intent == "4P" ) return 7;
  else if ( curr_sentence.intent == "5" ) return 8; 
  else if ( curr_sentence.intent == "5P" ) return 9;
  return -1;
}

/*****************************************************************/
/** Translate type of question to number                        **/
/**                                                             **/
/** Description:                                                **/
/** Translate word question type to number                      **/
/*****************************************************************/
int PM::translate2type()
{
  if ( curr_sentence.question_level == "INTRODUCTION" )
    {
      if ( curr_sentence.question_type == "ALL" ) return 0;
      else if ( curr_sentence.question_type == "TYPE" ) return 1;
      else if ( curr_sentence.question_type == "ARTICLE" ) return 2;
      else if ( curr_sentence.question_type == "OBJECT" ) return 3;
      else if ( curr_sentence.question_type == "NOUNPHRASE" ) return 4;
      else if ( curr_sentence.question_type == "LINK" ) return 5;
      else if ( curr_sentence.question_type == "NODE" ) return 6;
      else if ( curr_sentence.question_type == "FOCUS" ) return 7;
      return -1;
    }
  else if ( curr_sentence.question_level == "DIAGRAM" )
    {
      if ( curr_sentence.question_type == "LINK" ) return 0;
      else if ( curr_sentence.question_type == "NODE" ) return 1;
      else if ( curr_sentence.question_type == "FOCUS" ) return 2;
      else if ( curr_sentence.question_type == "ALL" ) return 3;
      return -1;
    }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" || curr_sentence.question_level == "ARTICLE_INTENT" )
    {
      if ( curr_sentence.question_type == "TYPE" ) return 0;
      else if ( curr_sentence.question_type == "ARTICLE" ) return 1;
      return -1;
    }
  return -1;
}

/*****************************************************************/
/** Set The Question ID                                         **/
/**                                                             **/
/** Description:                                                **/
/** Create ID for the question                                  **/
/*****************************************************************/
int PM::SetQuestionID()
{
  int i = 0;
  int ID = 0;

  if ( curr_sentence.intent == "1" ) { i = 0; ID = 100000; }
  else if ( curr_sentence.intent == "1P" ) { i = 1; ID = 110000; }
  else if ( curr_sentence.intent == "2" ) { i = 2; ID = 200000; }
  else if ( curr_sentence.intent == "2P" ) { i = 3; ID = 210000; }
  else if ( curr_sentence.intent == "3" ) { i = 4; ID = 300000; }
  else if ( curr_sentence.intent == "3P" ) { i = 5; ID = 310000; }
  else if ( curr_sentence.intent == "4" ) { i = 6; ID = 400000; }
  else if ( curr_sentence.intent == "4P" ) { i = 7; ID = 410000; }
  else if ( curr_sentence.intent == "5" ) { i = 8; ID = 500000; }
  else if ( curr_sentence.intent == "5P" ) { i = 9; ID = 510000; }
  else { SaveErrorToLog("ERROR PM_009: Intent data error"); return -1; }

  ID += DomainModel.intent[i].count;
  curr_sentence.ID = ID;
}

/*****************************************************************/
/** Set the number of trys                                      **/
/**                                                             **/
/** Description:                                                **/
/** Set the number of trys the user has to answer the question  **/
/*****************************************************************/
int PM::SetNumberOfAnswers()
{
  string errortxt;
  if ( curr_sentence.question_level == "INTRODUCTION" ) {
    if ( curr_sentence.question_type == "ALL" ) { return 1; }
    else if ( curr_sentence.question_type == "TYPE" ) { return 2; }
    else if ( curr_sentence.question_type == "ARTICLE" ) { return 3; }
    else if ( curr_sentence.question_type == "OBJECT" ) { return 3; }
    else if ( curr_sentence.question_type == "NOUNPHRASE" ) { return 3; }
    else if ( curr_sentence.question_type == "LINK" ) { return 3; }
    else if ( curr_sentence.question_type == "NODE" ) { return 2; }
    else if ( curr_sentence.question_type == "FOCUS" ) { return 2; }
  }
  else if ( curr_sentence.question_level == "DIAGRAM" ) {
    return 2;
  }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" || curr_sentence.question_level == "ARTICLE_INTENT" ) {
    if ( curr_sentence.question_type == "TYPE" ) { return 2; }
    else if ( curr_sentence.question_type == "ARTICLE" ) { return 3; }
  }
  errortxt = "ERROR PM_010: Invalid question level: "+curr_sentence.question_level+" "+curr_sentence.question_type;
  SaveErrorToLog(errortxt);
  return -1;
}

//================================================================= 
//          CHECK STUDENT ANSWER
//=================================================================

/*****************************************************************/
/** Check that answer                                           **/
/**                                                             **/
/** Description:                                                **/
/** Check if the student answer is correct                      **/
/*****************************************************************/
int PM::CheckAnswer()
{
  int i = 1;
  for ( int j = 0; j < 9; j++ ) { CorrectAnswers[j] = 1; }

  if ( student_sentence.article != curr_sentence.article ) { CorrectAnswers[0]=0; i=0; }
  if ( student_sentence.object1 != curr_sentence.object1 ) { CorrectAnswers[1]=0; i=0; }
  if ( student_sentence.object2 != curr_sentence.object2 ) { CorrectAnswers[2]=0; i=0; }
  if ( student_sentence.noun_phrase != curr_sentence.noun_phrase ) { CorrectAnswers[3]=0; i=0; }
  if ( student_sentence.intent != curr_sentence.intent ) { CorrectAnswers[4]=0; i=0; }
  if ( student_sentence.link != curr_sentence.link ) { CorrectAnswers[5]=0; i=0; }
  if ( student_sentence.node != curr_sentence.node ) { CorrectAnswers[6]=0; i=0; }
  if ( student_sentence.focus != curr_sentence.focus ) { CorrectAnswers[7]=0; i=0; }
  if ( student_sentence.type != curr_sentence.type ) { CorrectAnswers[8]=0; i=0; }
  return i;
}

/*****************************************************************/
/** initialize the student tracker                              **/
/**                                                             **/
/** Description:                                                **/
/** Update the student tracker based on the questions the user  **/
/** has already answered in previous sessions                   **/
/*****************************************************************/
int PM::initUpdateST()
{
  int loc = 0;
  for ( int i = 0; i < 10; i++ )
   {
     loc = 0;
     for ( int j = 0; j < DomainModel.intent[i].count; j++ )
       {
	 curr_sentence.ID = DomainModel.intent[i].question[j].ID;
	 curr_sentence.sentence = DomainModel.intent[i].question[j].sentence;
	 curr_sentence.article = DomainModel.intent[i].question[j].article;
	 curr_sentence.object1 = DomainModel.intent[i].question[j].object1;
	 curr_sentence.object2 = DomainModel.intent[i].question[j].object2;
	 curr_sentence.noun_phrase = curr_sentence.article+"_"+curr_sentence.object1;
	 curr_sentence.intent = DomainModel.intent[i].intent;
	 curr_sentence.link = DomainModel.intent[i].link;
	 curr_sentence.focus = DomainModel.intent[i].focus;
	 curr_sentence.node = DomainModel.intent[i].node;
	 curr_sentence.type = DomainModel.intent[i].article_type;
	 curr_sentence.question = DomainModel.intent[i].question[j].question;
	 curr_sentence.question_level = DomainModel.intent[i].question[j].level;
	 curr_sentence.question_type = DomainModel.intent[i].question[j].question_type;
	 
	 for ( ; loc < StudentModel.intent[i].count; loc++ )
	   {
	     if ( StudentModel.intent[i].question[loc].ID == curr_sentence.ID ) {
	       student_sentence.ID = StudentModel.intent[i].question[loc].ID;
	       student_sentence.sentence = StudentModel.intent[i].question[loc].sentence;
	       student_sentence.article = StudentModel.intent[i].question[loc].article;
	       student_sentence.object1 = StudentModel.intent[i].question[loc].object1;
	       student_sentence.object2 = StudentModel.intent[i].question[loc].object2;
	       student_sentence.noun_phrase = StudentModel.intent[i].question[loc].noun_phrase;
	       student_sentence.intent = StudentModel.intent[i].intent;
	       student_sentence.link = StudentModel.intent[i].question[loc].link;
	       student_sentence.focus = StudentModel.intent[i].question[loc].focus;
	       student_sentence.node = StudentModel.intent[i].question[loc].node;
	       student_sentence.type = StudentModel.intent[i].question[loc].article_type;
	       student_sentence.question = StudentModel.intent[i].question[loc].question;
	       student_sentence.question_level = StudentModel.intent[i].question[loc].level;
	       student_sentence.question_type = StudentModel.intent[i].question[loc].question_type;
	       break;
	     }
	     if ( StudentModel.intent[i].question[loc].ID > curr_sentence.ID ) { break; }
	   }

	 if ( student_sentence.ID == curr_sentence.ID ) {
	   UpdateST(translate2intent(),translate2type());
	 }
       }
   }
  UpdateSP();
}

/*****************************************************************/
/** Update the student tracker                                  **/
/**                                                             **/
/** Description:                                                **/
/** Update the student tracker based on if the student got the  **/
/** question right.  If it is right, add one to the level/type. **/
/** If it is wrong, delete one from the level/type.             **/
/*****************************************************************/
int PM::UpdateST(int intent, int type)
{
  int valid = 1;
  for ( int j = 0; j < 9; j++ ) {
    if ( !CorrectAnswers[j] ) valid = -1; }

  if ( curr_sentence.question_level == "INTRODUCTION" )
    {
      StudentProgress.intro[intent][type] += valid; 
      if ( StudentProgress.intro[intent][type] < 0 ) { StudentProgress.intro[intent][type] = 0; }
    }
  else if ( curr_sentence.question_level == "DIAGRAM" )
    {
      StudentProgress.diagram[intent][type] += valid;
      if ( StudentProgress.diagram[intent][type] < 0 ) { StudentProgress.diagram[intent][type] = 0; }
    }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" )
    {
      StudentProgress.article1[intent][type] += valid;
      if ( StudentProgress.article1[intent][type] < 0 ) { StudentProgress.article1[intent][type] = 0; }
    }
  else if ( curr_sentence.question_level == "ARTICLE_INTENT" )
    {
      StudentProgress.article2[intent][type] += valid;
      if ( StudentProgress.article2[intent][type] < 0 ) { StudentProgress.article2[intent][type] = 0; }
    }
    
  //Print the student tracker
  /*
  cout<<"intro"<<endl;
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 8; i++ ) { cout<<StudentProgress.intro[j][i]<<" - "; } cout<<endl; }
  cout<<"diagram"<<endl;
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 4; i++ ) { cout<<StudentProgress.diagram[j][i]<<" - "; } cout<<endl; }
  cout<<"article1"<<endl;
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 2; i++ ) { cout<<StudentProgress.article1[j][i]<<" - "; } cout<<endl; }
  cout<<"article2"<<endl;
  for ( int j = 0; j < 10; j++ ) { for ( int i = 0; i < 2; i++ ) { cout<<StudentProgress.article2[j][i]<<" - "; } cout<<endl; }
  */

  return 0;
}

/*****************************************************************/
/** Update the student progress                                 **/
/**                                                             **/
/** Description:                                                **/
/** Update the student progress based on the student tracker.   **/
/** if the level/type has reached the understanding limit then  **/
/** let the student progress for that level/type to true.       **/
/*****************************************************************/
void PM::UpdateSP()
{
  int valid = 1;

  //Introduction
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][0] >= 1 ) { SP_introduction.intent[i].all = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][1] >= 3 ) { SP_introduction.intent[i].type = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][2] >= 3 ) { SP_introduction.intent[i].article = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][3] >= 3 ) { SP_introduction.intent[i].object = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][4] >= 3 ) { SP_introduction.intent[i].noun_phrase = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][5] >= 3 ) { SP_introduction.intent[i].link = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][6] >= 3 ) { SP_introduction.intent[i].node = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.intro[i][7] >= 3 ) { SP_introduction.intent[i].focus = true; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    valid = 1;
    if ( SP_introduction.intent[i].all == false ) { valid = 0; }
    if ( SP_introduction.intent[i].type == false ) { valid = 0; }
    if ( SP_introduction.intent[i].article == false ) { valid = 0; }
    if ( SP_introduction.intent[i].object == false ) { valid = 0; }
    if ( SP_introduction.intent[i].noun_phrase == false ) { valid = 0; }
    if ( SP_introduction.intent[i].link == false ) { valid = 0; }
    if ( SP_introduction.intent[i].node == false ) { valid = 0; }
    if ( SP_introduction.intent[i].focus == false ) { valid = 0; } 
    
    if ( valid == 1 ) { SP_introduction.intent[i].full = true; }
  }
  
  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].full == false ) { valid = 0; } }
  if ( valid == 1 ) { SP_introduction.full = true; curr_intro_section = 8; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].all == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 1; disp_intro_txt = 0; }//{ intro_complete = true; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].type == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 2; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].article == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 3; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].object == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 4; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].noun_phrase == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 5; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].link == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 6; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].node == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 7; disp_intro_txt = 0; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_introduction.intent[i].focus == false ) { valid = 0; } }
  if ( valid == 1 ) { curr_intro_section = 8; disp_intro_txt = 0; }

  if ( curr_intro_section < 8 ) {
    for ( int j = 0; j < 10; j++ ) { disp_intro_txt += StudentProgress.intro[j][curr_intro_section]; } }

  //Diagram
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.diagram[i][0] >= 3 ) { SP_diagram_intent.intent[i].link = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.diagram[i][1] >= 3 ) { SP_diagram_intent.intent[i].node = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.diagram[i][2] >= 3 ) { SP_diagram_intent.intent[i].focus = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.diagram[i][3] >= 3 ) { SP_diagram_intent.intent[i].all = true; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    valid = 1;
    if ( SP_diagram_intent.intent[i].link == false ) { valid = 0; }
    if ( SP_diagram_intent.intent[i].node == false ) { valid = 0; }
    if ( SP_diagram_intent.intent[i].focus == false ) { valid = 0; } 
    if ( SP_diagram_intent.intent[i].all == false ) { valid = 0; }
    
    if ( valid == 1 ) { SP_diagram_intent.intent[i].full = true; }
  }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_diagram_intent.intent[i].full == false ) { valid = 0; } }
  if ( valid == 1 ) { SP_diagram_intent.full = true; }

  //Article Diagram
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.article1[i][0] >= 3 ) { SP_article_diagram.intent[i].type = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.article1[i][1] >= 3 ) { SP_article_diagram.intent[i].article = true; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    valid = 1;
    if ( SP_article_diagram.intent[i].type == false ) { valid = 0; }
    if ( SP_article_diagram.intent[i].article == false ) { valid = 0; }
    
    if ( valid == 1 ) { SP_article_diagram.intent[i].full = true; }
  }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_article_diagram.intent[i].full == false ) { valid = 0; } }
  if ( valid == 1 ) { SP_article_diagram.full = true; }

  //Article
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.article2[i][0] >= 3 ) { SP_article_intent.intent[i].type = true; }
  for ( int i = 0; i < 10; i++ )
    if ( StudentProgress.article2[i][1] >= 3 ) { SP_article_intent.intent[i].article = true; }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    valid = 1;
    if ( SP_article_intent.intent[i].type == false ) { valid = 0; }
    if ( SP_article_intent.intent[i].article == false ) { valid = 0; }
    
    if ( valid == 1 ) { SP_article_intent.intent[i].full = true; }
  }

  valid = 1;
  for ( int i = 0; i < 10; i++ ) {
    if ( SP_article_intent.intent[i].full == false ) { valid = 0; } }
  if ( valid == 1 ) { SP_article_intent.full = true; }

  
  //Print
  /*
  cout<<"intro - "<<SP_introduction.full<<endl;
  for ( int i = 0; i < 10; i++ ) {
    cout<<SP_introduction.intent[i].full<<" - ";
    cout<<SP_introduction.intent[i].all<<" - ";
    cout<<SP_introduction.intent[i].type<<" - ";
    cout<<SP_introduction.intent[i].article<<" - ";
    cout<<SP_introduction.intent[i].object<<" - ";
    cout<<SP_introduction.intent[i].noun_phrase<<" - ";
    cout<<SP_introduction.intent[i].link<<" - ";
    cout<<SP_introduction.intent[i].node<<" - ";
    cout<<SP_introduction.intent[i].focus<<endl; }
  
  cout<<"diagram - "<<SP_diagram_intent.full<<endl;
  for ( int i = 0; i < 10; i++ ) {
    cout<<SP_diagram_intent.intent[i].full<<" - ";
    cout<<SP_diagram_intent.intent[i].link<<" - ";
    cout<<SP_diagram_intent.intent[i].node<<" - ";
    cout<<SP_diagram_intent.intent[i].focus<<" - ";
    cout<<SP_diagram_intent.intent[i].all<<endl; }

  cout<<"article1 - "<<SP_article_diagram.full<<endl;
  for ( int i = 0; i < 10; i++ ) {
    cout<<SP_article_diagram.intent[i].full<<" - ";
    cout<<SP_article_diagram.intent[i].type<<" - ";
    cout<<SP_article_diagram.intent[i].article<<endl; }

  cout<<"article2 - "<<SP_article_intent.full<<endl;
  for ( int i = 0; i < 10; i++ ) {
    cout<<SP_article_intent.intent[i].full<<" - ";
    cout<<SP_article_intent.intent[i].type<<" - ";
    cout<<SP_article_intent.intent[i].article<<endl; }
  //*/
}

//================================================================= 
//          USER
//=================================================================

/*****************************************************************/
/** Add new user                                                **/
/**                                                             **/
/** Description:                                                **/
/** Make new directory for user and add them to the user list   **/
/*****************************************************************/
void PM::AddUser()
{
  string tempUser;
  string line = "";
  int count = 1;
  string number;

  ifstream infile;
  infile.open("users/user_names.txt");
  while ( infile >> line ) {
    users.push_back(line);
  }
  infile.close();

  for ( int i = 0; i < users.size(); i++ )
    {
      if ( user.length() > users[i].length() )
	{ continue; }

      tempUser = "";
      for ( int j = 0; j < user.length(); j++ )
	{
	  tempUser += users[i][j];
	}

      if ( tempUser == user )
	{ count++; }
    }

  std::stringstream ss;
  ss << count;
  string str = ss.str();
  number += str;

  for ( int i = number.length(); i < 4; i++ )
    { number = "0"+number; }

  user += number;

  ofstream outfile;
  outfile.open("users/user_names.txt", fstream::app);
  if ( !outfile.is_open() )
    { SaveErrorToLog("ERROR PM_016: Cannot open User List"); }
  outfile << user << "\n";
  outfile.close();

  userPath = "users/" + user;
  mkdir(userPath.c_str(),0777);

  userPath = "users/" + user + "/domain";
  mkdir(userPath.c_str(),0777);

  userPath = "users/" + user + "/student";
  mkdir(userPath.c_str(),0777);

  userPath = "users/" + user;
}

//================================================================= 
//          HINTS
//=================================================================

/*****************************************************************/
/** init the hints                                              **/
/**                                                             **/
/** Description:                                                **/
/** initialize hint structure with content from misconception   **/
/** library                                                     **/
/*****************************************************************/

int PM::initHints()
{
  HintItem temphint;
  int loc = 0;
  string word = "";
  string line = "";
  ifstream infile;
  infile.open("misconception_library.txt");
  if ( !infile.is_open() )
    { SaveErrorToLog("ERROR PM_015: Cannot open Misconception Library"); }
  else {
    while ( infile >> line )
      {
	temphint.type = "";
	temphint.correct = "";
	temphint.student = "";
	temphint.count = 0;
	temphint.hint.clear();
	temphint.used.clear();
	loc = 0;
	word = "";
	for ( int i = 0; i < line.length(); i++ ) {
	  if ( line[i] == '|' )
	    {
	      if ( loc == 0 )
		{ 
		  temphint.type = word; }
	      else if ( loc == 1 )
		{ 
		  temphint.correct = word; }
	      else if ( loc == 2 )
		{ 
		  temphint.student = word; }
	      else if ( loc > 2 )
		{ 
		  temphint.hint.push_back(word); 
		  temphint.used.push_back(0); 
		  temphint.count++; }
	      loc++;
	      word = "";
	    }
	  else if ( line[i] == '_' ) { word += ' '; }
	  else { word += line[i]; }
	}
	HintList.push_back(temphint);
      }
  }
  infile.close();
}

/*****************************************************************/
/** select the hints                                            **/
/**                                                             **/
/** Description:                                                **/
/** Select a hint to display based on student answer and        **/
/** correct answer.                                             **/
/*****************************************************************/

int PM::selectHint()
{
  string correctAns = "";
  string studentAns = "";
  string Qtype = "";

  if ( CorrectAnswers[8] == 0 ) { Qtype="TYPE"; correctAns = curr_sentence.type; studentAns = student_sentence.type; }
  else if ( CorrectAnswers[0] == 0 ) { Qtype="ARTICLE"; correctAns = curr_sentence.article; studentAns = student_sentence.article; }
  else if ( CorrectAnswers[1] == 0 ) { Qtype="OBJECT"; correctAns = curr_sentence.object1; studentAns = student_sentence.object1; }
  else if ( CorrectAnswers[2] == 0 ) { Qtype="OBJECT"; correctAns = curr_sentence.object2; studentAns = student_sentence.object2; }
  else if ( CorrectAnswers[3] == 0 ) { Qtype="NOUNPHRASE"; correctAns = curr_sentence.noun_phrase; studentAns = student_sentence.noun_phrase; }
  else if ( CorrectAnswers[4] == 0 ) { Qtype="INTENT"; correctAns = curr_sentence.intent; studentAns = student_sentence.intent; }
  else if ( CorrectAnswers[5] == 0 ) { Qtype="LINK"; correctAns = curr_sentence.link; studentAns = student_sentence.link; }
  else if ( CorrectAnswers[6] == 0 ) { Qtype="NODE"; correctAns = curr_sentence.node; studentAns = student_sentence.node; }
  else if ( CorrectAnswers[7] == 0 ) { Qtype="FOCUS" ; correctAns = curr_sentence.focus; studentAns = student_sentence.focus; }

  HintForStudent = "";
  for ( int i = 0; i < HintList.size(); i++ ) {
    if ( Qtype == HintList[i].type ) {
      if ( Qtype == "OBJECT" || Qtype == "NOUNPHRASE" ) {
	for ( int j = 0; j < HintList[i].hint.size(); j++ ) {
	  if ( HintList[i].used[j] == 0 ) {
	    HintForStudent = HintList[i].hint[j];
	    HintList[i].used[j] = 1;
	  }
	}
	if ( HintForStudent == "" ) {
	 for ( int j = 0; j < HintList[i].hint.size(); j++ )
	   { HintList[i].used[j] = 0; }
	  HintForStudent = HintList[i].hint[0];
	  HintList[i].used[0] = 1;
	}
      }
      else {
	if ( correctAns == HintList[i].correct ) {
	  if ( studentAns == HintList[i].student ) {
	    for ( int j = 0; j < HintList[i].hint.size(); j++ ) {
	      if ( HintList[i].used[j] == 0 ) {
		HintForStudent = HintList[i].hint[j];
		HintList[i].used[j] = 1;
	      }
	    }
	    if ( HintForStudent == "" ) {
	      for ( int j = 0; j < HintList[i].hint.size(); j++ )
		{ HintList[i].used[j] = 0; }
	      HintForStudent = HintList[i].hint[0];
	      HintList[i].used[0] = 1;
	    }
	  }
	}
      }
    }
  }
}

//================================================================= 
//          INITIALIZE FILES
//=================================================================

/*****************************************************************/
/** Create Domain Model Files                                   **/
/**                                                             **/
/** Description:                                                **/
/** initialize domain model files if new user                   **/
/*****************************************************************/
int PM::InitDomainFiles()
{
  string filepath = userPath + "/domain/DM_1.txt";
  InitFile(filepath.c_str(),"1|IS|SHARED|ELEMENT|THE|");
  filepath = userPath + "/domain/DM_1P.txt";
  InitFile(filepath.c_str(),"1P|IS-ALL-OF|SHARED|SET|THE|");
  filepath = userPath + "/domain/DM_2.txt";
  InitFile(filepath.c_str(),"2|IS|SPEAKER|ELEMENT|A-AN|");
  filepath = userPath + "/domain/DM_2P.txt";
  InitFile(filepath.c_str(),"2P|IS|SPEAKER|SET|NONE|");
  filepath = userPath + "/domain/DM_3.txt";
  InitFile(filepath.c_str(),"3|IS|LISTENER|ELEMENT|A-AN|");
  filepath = userPath + "/domain/DM_3P.txt";
  InitFile(filepath.c_str(),"3P|IS|LISTENER|SET|NONE|");
  filepath = userPath + "/domain/DM_4.txt";
  InitFile(filepath.c_str(),"4|IS-REP|SHARED|CLASS|A-AN|");
  filepath = userPath + "/domain/DM_4P.txt";
  InitFile(filepath.c_str(),"4P|IS|SHARED|CLASS|NONE|");
  filepath = userPath + "/domain/DM_5.txt";
  InitFile(filepath.c_str(),"5|IS-IN|SHARED|CLASS|A-AN|");
  filepath = userPath + "/domain/DM_5P.txt";
  InitFile(filepath.c_str(),"5P|IS-IN|SHARED|CLASS|NONE|");
}

/*****************************************************************/
/** Create Student Model Files                                  **/
/**                                                             **/
/** Description:                                                **/
/** initialize student model files                              **/
/*****************************************************************/
int PM::InitStudentFiles()
{
  string filepath = userPath + "/student/SM_1.txt";
  InitFile(filepath.c_str(),"1");
  filepath = userPath + "/student/SM_1P.txt";
  InitFile(filepath.c_str(),"1P");
  filepath = userPath + "/student/SM_2.txt";
  InitFile(filepath.c_str(),"2");
  filepath = userPath + "/student/SM_2P.txt";
  InitFile(filepath.c_str(),"2P");
  filepath = userPath + "/student/SM_3.txt";
  InitFile(filepath.c_str(),"3");
  filepath = userPath + "/student/SM_3P.txt";
  InitFile(filepath.c_str(),"3P");
  filepath = userPath + "/student/SM_4.txt";
  InitFile(filepath.c_str(),"4");
  filepath = userPath + "/student/SM_4P.txt";
  InitFile(filepath.c_str(),"4P");
  filepath = userPath + "/student/SM_5.txt";
  InitFile(filepath.c_str(),"5");
  filepath = userPath + "/student/SM_5P.txt";
  InitFile(filepath.c_str(),"5P");
}

/*****************************************************************/
/** Create a File                                               **/
/**                                                             **/
/** Description:                                                **/
/** initialize a file                                           **/
/**                                                             **/
/** Parameters:                                                 **/
/** fname - file name                                           **/
/** header - content to be placed in file                       **/
/*****************************************************************/
int PM::InitFile(const char* fname, string header)
{
  ifstream DMfile(fname);
  if ( !DMfile )
    {
      ofstream filename;
      filename.open(fname);
      filename<<header<<endl;
      filename.close();
    }
}

//================================================================= 
//          ADD TO LINKED LIST
//=================================================================

/*****************************************************************/
/** Add Question to Domain Model                                **/
/**                                                             **/
/** Description:                                                **/
/** Add the current question with correct answer to domain      **/
/** model linked list                                           **/
/*****************************************************************/
int PM::AddQuestionToDomain()
{
  ST_question temp_question;
  int i = 0;

  if ( curr_sentence.intent == "1" ) { i = 0; }
  else if ( curr_sentence.intent == "1P" ) { i = 1; }
  else if ( curr_sentence.intent == "2" ) { i = 2; }
  else if ( curr_sentence.intent == "2P" ) { i = 3; }
  else if ( curr_sentence.intent == "3" ) { i = 4; }
  else if ( curr_sentence.intent == "3P" ) { i = 5; }
  else if ( curr_sentence.intent == "4" ) { i = 6; }
  else if ( curr_sentence.intent == "4P" ) { i = 7; }
  else if ( curr_sentence.intent == "5" ) { i = 8; }
  else if ( curr_sentence.intent == "5P" ) { i = 9; }
  else { SaveErrorToLog("ERROR PM_011: Intent data error"); return -1; }

  temp_question.ID = curr_sentence.ID;
  temp_question.level = curr_sentence.question_level;
  temp_question.question_type = curr_sentence.question_type;
  temp_question.question = curr_sentence.question;
  temp_question.sentence = curr_sentence.sentence;
  temp_question.article = curr_sentence.article;
  temp_question.object1 = curr_sentence.object1;
  temp_question.object2 = curr_sentence.object2;

  DomainModel.intent[i].question.push_back(temp_question);
  DomainModel.intent[i].count++;

  return 0;
}

/*****************************************************************/
/** Add Question to Student Model                               **/
/**                                                             **/
/** Description:                                                **/
/** Add current question and student answer to student model    **/
/** linked list                                                 **/
/*****************************************************************/
int PM::AddQuestionToStudent()
{
  ST_student_question temp_question;
  int i = 0;

  if ( curr_sentence.intent == "1" ) { i = 0; }
  else if ( curr_sentence.intent == "1P" ) { i = 1; }
  else if ( curr_sentence.intent == "2" ) { i = 2; }
  else if ( curr_sentence.intent == "2P" ) { i = 3; }
  else if ( curr_sentence.intent == "3" ) { i = 4; }
  else if ( curr_sentence.intent == "3P" ) { i = 5; }
  else if ( curr_sentence.intent == "4" ) { i = 6; }
  else if ( curr_sentence.intent == "4P" ) { i = 7; }
  else if ( curr_sentence.intent == "5" ) { i = 8; }
  else if ( curr_sentence.intent == "5P" ) { i = 9; }
  else { SaveErrorToLog("ERROR PM_012: Intent data error"); return -1; }

  temp_question.ID = student_sentence.ID;
  temp_question.level = student_sentence.question_level;
  temp_question.question_type = student_sentence.question_type;
  temp_question.question = student_sentence.question;
  temp_question.sentence = student_sentence.sentence;
  temp_question.article = student_sentence.article;
  temp_question.object1 = student_sentence.object1;
  temp_question.object2 = student_sentence.object2;
  temp_question.intent = student_sentence.intent;
  temp_question.link = student_sentence.link;
  temp_question.node = student_sentence.node;
  temp_question.focus = student_sentence.focus;
  temp_question.article_type = student_sentence.type;
  temp_question.noun_phrase = student_sentence.noun_phrase;

  StudentModel.intent[i].question.push_back(temp_question);
  StudentModel.intent[i].count++;

  return 0;
}

//================================================================= 
//          SAVE TO FILES
//=================================================================

/*****************************************************************/
/** Save Question to Domain Model                               **/
/**                                                             **/
/** Description:                                                **/
/** Save current question with correct answer to domain model   **/
/** file                                                        **/
/*****************************************************************/
int PM::SaveQuestionToDomain()
{
  string filename = userPath+"/domain/DM_"+curr_sentence.intent+".txt";

  ofstream myDomain;
  myDomain.open(filename.c_str(), fstream::app);
  if ( !myDomain.is_open() )
    { SaveErrorToLog("ERROR PM_013: Cannot open Domain Model"); return -1; }
  myDomain << curr_sentence.ID << "|"
	   << curr_sentence.question_level << "|"
	   << curr_sentence.question_type << "|"
           << curr_sentence.question << "|"
           << curr_sentence.sentence << "|"
           << curr_sentence.article << "|"
           << curr_sentence.object1 << "|"
           << curr_sentence.object2 << "|\n";
  myDomain.close();
  return 0;
}

/*****************************************************************/
/** Save Question to Student Model                              **/
/**                                                             **/
/** Description:                                                **/
/** Save question and student answers to student model file     **/
/*****************************************************************/
int PM::SaveQuestionToStudent()
{
  string filename = userPath+"/student/SM_"+student_sentence.intent+".txt";

  ofstream myStudent;
  myStudent.open(filename.c_str(), fstream::app);
  if ( !myStudent.is_open() )
    { SaveErrorToLog("ERROR PM_014: Cannot open Student Model"); return -1; }
  myStudent << student_sentence.ID << "|"
	    << student_sentence.question_level << "|"
	    << student_sentence.question_type << "|"
	    << student_sentence.question << "|"
	    << student_sentence.sentence << "|"
	    << student_sentence.article << "|"
	    << student_sentence.object1 << "|"
	    << student_sentence.object2 << "|"
	    << student_sentence.noun_phrase << "|"
	    << student_sentence.intent << "|"
	    << student_sentence.link << "|"
	    << student_sentence.node << "|"
	    << student_sentence.focus << "|"
	    << student_sentence.type << "|\n";
  myStudent.close();
  return 0;
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
void PM::SaveErrorToLog(string error)
{
  ofstream myError;
  myError.open("error_log.txt", fstream::app);
  if ( !myError.is_open() )
    { cout<<"ERROR: Cannot open Error Log"<<endl; }
  myError<<error<<"\n";
  myError.close();
}

//================================================================= 
//          PRINT
//=================================================================

/*****************************************************************/
/** Print Question Data                                         **/
/**                                                             **/
/** Description:                                                **/
/** Print the question information                              **/
/**                                                             **/
/** Parameters:                                                 **/
/** info_sentence - the sentence and question information to    **/
/** print                                                       **/
/**                                                             **/
/*****************************************************************/
void PM::PrintQuestion(sentence_data info_sentence)
{ 
  cout<<"========================================="<<endl;
  cout<<"ID:\t"<<info_sentence.ID<<endl;
  cout<<"-----------------------------------------"<<endl;
  cout<<"SENTENCE"<<endl;
  cout<<"sentence\t"<<info_sentence.sentence<<endl;
  cout<<"article\t\t"<<info_sentence.article<<endl;
  cout<<"object1\t\t"<<info_sentence.object1<<endl;
  if ( info_sentence.object2 != "" )
    { cout<<"object2\t"<<info_sentence.object2<<endl; }
  cout<<"noun phrase\t"<<info_sentence.noun_phrase<<endl;
  cout<<"intent\t\t"<<info_sentence.intent<<endl;
  cout<<"link\t\t"<<info_sentence.link<<endl;
  cout<<"node\t\t"<<info_sentence.node<<endl;
  cout<<"focus\t\t"<<info_sentence.focus<<endl;
  cout<<"type\t\t"<<info_sentence.type<<endl;
  cout<<"QUESTION"<<endl;
  cout<<"question\t"<<info_sentence.question<<endl;
  cout<<"level\t\t"<<info_sentence.question_level<<endl;
  cout<<"type\t\t"<<info_sentence.question_type<<endl;
}

/*****************************************************************/
/** Print Student Model                                         **/
/**                                                             **/
/** Description:                                                **/
/** Print the student model for each intent                     **/
/*****************************************************************/
void PM::PrintStudent()
{
  cout<<endl;
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
	  cout<<"\t========================================="<<endl;
	  cout<<"\tID\t"<<StudentModel.intent[j].question[i].ID<<endl;
	  cout<<"\t-----------------------------------------"<<endl;
	  cout<<"\tSENTENCE"<<endl;
          cout<<"\tintent\t\t"<<StudentModel.intent[j].question[i].intent<<endl;
          cout<<"\tsentence\t"<<StudentModel.intent[j].question[i].sentence<<endl;
          cout<<"\tarticle\t\t"<<StudentModel.intent[j].question[i].article<<endl;
          cout<<"\tobject1\t\t"<<StudentModel.intent[j].question[i].object1<<endl;
          if ( StudentModel.intent[j].question[i].object2 != "" ) 
	    { cout<<"\tobject2\t\t"<<StudentModel.intent[j].question[i].object2<<endl; }
          cout<<"\tnoun phrase\t"<<StudentModel.intent[j].question[i].noun_phrase<<endl;
          cout<<"\tlink\t\t"<<StudentModel.intent[j].question[i].link<<endl;
          cout<<"\tnode\t\t"<<StudentModel.intent[j].question[i].node<<endl;
          cout<<"\tfocus\t\t"<<StudentModel.intent[j].question[i].focus<<endl;
          cout<<"\ttype\t\t"<<StudentModel.intent[j].question[i].article_type<<endl;
	  cout<<"\tQUESTION"<<endl;
          cout<<"\tquestion\t"<<StudentModel.intent[j].question[i].question<<endl;          
          cout<<"\tlevel\t\t"<<StudentModel.intent[j].question[i].level<<endl;
          cout<<"\ttype\t\t"<<StudentModel.intent[j].question[i].question_type<<endl;
	}
    }
}
