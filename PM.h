#ifndef PM_H
#define PM_H
#include <string>
#include <vector>
using namespace std;

//==========================================
// CURRENT QUESTION INFORMATION
//==========================================

//About The Question
typedef struct question_info {
  string level;
  string intent;
  string type;
};

//The Question and Correct Data
typedef struct sentence_data {
  int ID;
  string sentence;
  string article;
  string object1;
  string object2;
  string noun_phrase;
  string intent;
  string link;
  string focus;
  string node;
  string type;
  string question;
  string question_level;
  string question_type;
};

//==========================================
// STUDENT TRACKER
//==========================================

//Tracking what the student knows
struct ST_INTENT {
  //int count;
  bool full;
  bool all;
  bool link;
  bool focus;
  bool node;
  bool type;
  bool article;
  bool object;
  bool noun_phrase;
};

struct ST_LEVEL {
  bool full;
  ST_INTENT intent[10];
};

//Domain Model Question
typedef struct ST_question {
  int ID;
  string level;
  string question_type;
  string question;
  string sentence;
  string article;
  string object1;
  string object2;
};

//Student Model Question
typedef struct ST_student_question {
  string intent;
  string link;
  string node;
  string focus;
  string article_type;
  int ID;
  string level;
  string question_type;
  string question;
  string sentence;
  string article;
  string object1;
  string object2;
  string noun_phrase;
};

//Domain Model Intent
typedef struct ST_intent {
  string intent;
  string link;
  string node;
  string focus;
  string article_type;
  int count;
  vector<ST_question> question;
};

//Student Model Intent
typedef struct ST_student_intent {
  int count;
  string intent;
  vector<ST_student_question> question;
};

//Domain Model
typedef struct ST_domain {
  ST_intent intent[10];
};

//Student Model
typedef struct ST_student {
  ST_student_intent intent[10];
};

//Student progress
typedef struct ST_know {
  int intro1[10];
  int intro[10][8];
  int diagram[10][4];
  int article1[10][2];
  int article2[10][2];
};

//Hints
struct HintItem {
  string type;
  string correct;
  string student;
  int count;
  vector<string> hint;
  vector<int> used;
};

class PM
{
 private:
  string user;
  vector<string> users;
  string userPath;

  ST_domain DomainModel;         //Domain Model
  ST_student StudentModel;       //Student Model

  ST_LEVEL SP_introduction;      //Introduction
  ST_LEVEL SP_diagram_intent;    //Given intent, pic diagram
  ST_LEVEL SP_intent_diagram;    //Given diagram, pic intent
  ST_LEVEL SP_article_diagram;   //Given diagram, pic article
  ST_LEVEL SP_article_intent;    //Given intent, pic article
  int curr_intro_section;        //Current question type in the into section
  int disp_intro_txt;            //Indicates what into text to display
  int program_start;             //Indicates is the program has just started
  vector<HintItem> HintList;     //The list of all hints that can be provided
  string HintForStudent;         //The hint to provide

  question_info Qinfo;           //Question information

  sentence_data curr_sentence;   //Sentence/Question information
  sentence_data student_sentence;
 
  int CorrectAnswers[9];
  ST_know StudentProgress;

  //Get the question information
  question_info QuestionInfo(int);

  //Select the category
  int QuestionCategory();         //Next that has not been mastered
  //Select the intent
  int QuestionIntent(ST_LEVEL);   //Next that has not been mastered
  int QuestionIntent();           //Random
  //Select the type of area
  int QuestionType(ST_INTENT);    //Next that has not been mastered
  int QuestionType(int);          //Random
  
  //Translate number values into words
  void translate(int,int,int);
  int translate2intent();
  int translate2type();

  //Set the ID of the question
  int SetQuestionID();
  int SetNumberOfAnswers();

  //Student answer and progress
  int CheckAnswer();
  int initUpdateST();
  int UpdateST(int,int);
  void UpdateSP();

  //Hint
  int initHints();
  int selectHint();

  //Add question to domain
  int AddQuestionToDomain();
  int AddQuestionToStudent();

  //Save question to the domain
  int SaveQuestionToDomain();
  int SaveQuestionToStudent();
  void SaveErrorToLog(string);

  //User
  void AddUser();

  //Initialize files
  int InitDomainFiles();         //Domain Model Files
  int InitStudentFiles();        //Student Model Files
  int InitFile(const char*,string);    //Create the files

  //Print the sentence information
  void PrintQuestion(sentence_data);
  void PrintStudent();

 public:
  //Constructor/Destructor
  PM();
  ~PM();

  int START();

};
#endif
