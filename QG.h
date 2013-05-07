#ifndef QG_H
#define QG_H
#include <string>
#include <vector>
#include "PM.h"
using namespace std;

//Struct for each node in the sentence linked list
struct node {
  string word;
  int count;
  bool article;
  bool object1;
  bool object2;
  vector<node*> next;
};

class QG
{
 private:
  string testdata[30][9];       //For testing
  int question_counter;         //The question number - don't know if i need this
  sentence_data curr_sentence;  //The current sentence/question information
  question_info Qinfo;          //Question info needed to generate the question

  //Linked list for making a sentence
  node* intent1_front;
  node* intent1P_front;
  node* intent2_front;
  node* intent2P_front;
  node* intent3_front;
  node* intent3P_front;
  node* intent4_front;
  node* intent4P_front;
  node* intent5_front;
  node* intent5P_front;

  //Init the sentence structure
  void InitSentence();

  //Delete a linked list
  void RemoveListLoop(node*);

  //Generate linked list
  void ListGenerator(node*,string);

  //Generate the information
  int SentenceGenerator(string);           //Sentence
  int QuestionGenerator(string, string);   //Question
  node* SetIntentParams(string);           //Diagram
  
  //Print
  void PrintStruct();              //Sentence
  void PrintListLoop(node*,int);   //Linked list

  //Save error messages to the error log
  void SaveErrorToLog(string);

 public:

  //Constructor/Destructor
  QG();
  ~QG();
  
  //Get test data - REMOVE
  int GetTestData();

  //Generate the sentence and question, then save to domain
  sentence_data Generator(string,string,string);
};
#endif
