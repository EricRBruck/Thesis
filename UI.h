#ifndef UI_H
#define UI_H
#include <string>
#include <vector>
#include "PM.h"

using namespace std;

//struct to hold diagram information for the diagrams to be displayed in the diagram selection question
struct temp_diagram {
  string link;
  string node;
  string focus;
};

class UI
{
 private:
  sentence_data curr_sentence;      //Current correct sentence information
  sentence_data student_sentence;   //Student answers
  string intent_usage[10];          //The usage of the intent (intent description)
  string intent_description[10];    //The description of the intent
  string intro_description[7];      //Lessons provided to user
  string ans;                       //student answer
  temp_diagram diagram[3];          //diagrams to be displayed in the diagram questions
  vector<string> users;             //list of users

  //Introduction Questions
  int AskIntroduction();
  int CheckIntroduction();

  //Diagram Questions
  int AskDiagram();
  int CheckDiagram();

  //Article Questions
  int AskArticle(int);
  int CheckArticle();

  //Display lessions
  int IntroDescription();

  //Save error to error log file
  void SaveErrorToLog(string);

 public:
  
  //Constructor / Destructor
  UI();
  ~UI();

  //Get the user
  string GetUser();
  
  //Display question, ask for answer, check answer
  sentence_data AskQuestion(sentence_data, int);

  //Ask for answer again, check answer
  sentence_data AskAgain();

  //Provide correct answer
  void provideCorrectAnswer(sentence_data);
};

#endif
