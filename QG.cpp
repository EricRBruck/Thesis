#include <iostream>     // print
#include <fstream>      // file
#include <string>       // string
#include <vector>       // vector
#include <sstream>      // stringstream
#include <stdlib.h>     // srand, rand
#include <time.h>       // time
#include <algorithm>
#include "QG.h"

using namespace std;

//=================================================================
//          CONSTRUCTOR / DESTRUCTOR
//=================================================================

QG::QG()
{
  for ( int i = 0; i < 30; i++)
    {
      for ( int j = 0; j < 9; j++ )
	{ testdata[i][j] = ""; }
    }
  question_counter = 0;

  InitSentence();

  intent1_front = new node;
  intent1_front->word = "INTENT1";
  intent1_front->count = 0;
  intent1_front->article = false;
  intent1_front->object1 = false;
  intent1_front->object2 = false;
  ListGenerator(intent1_front, "Intent1.xml");
  //PrintListLoop(intent1_front,0);

  intent1P_front = new node;
  intent1P_front->word = "INTENT1P";
  intent1P_front->count = 0;
  intent1P_front->article = false;
  intent1P_front->object1 = false;
  intent1P_front->object2 = false;
  ListGenerator(intent1P_front, "Intent1P.xml");
  //PrintListLoop(intent1P_front,0);

  intent2_front = new node;
  intent2_front->word = "INTENT2";
  intent2_front->count = 0;
  intent2_front->article = false;
  intent2_front->object1 = false;
  intent2_front->object2 = false;
  ListGenerator(intent2_front, "Intent2.xml");
  //PrintListLoop(intent2_front,0);

  intent2P_front = new node;
  intent2P_front->word = "INTENT2P";
  intent2P_front->count = 0;
  intent2P_front->article = false;
  intent2P_front->object1 = false;
  intent2P_front->object2 = false;
  ListGenerator(intent2P_front, "Intent2P.xml");
  //PrintListLoop(intent2P_front,0);

  intent3_front = new node;
  intent3_front->word = "INTENT3";
  intent3_front->count = 0;
  intent3_front->article = false;
  intent3_front->object1 = false;
  intent3_front->object2 = false;
  ListGenerator(intent3_front, "Intent3.xml");
  //PrintListLoop(intent3_front,0);

  intent3P_front = new node;
  intent3P_front->word = "INTENT3P";
  intent3P_front->count = 0;
  intent3P_front->article = false;
  intent3P_front->object1 = false;
  intent3P_front->object2 = false;
  ListGenerator(intent3P_front, "Intent3P.xml");
  //PrintListLoop(intent3P_front,0);

  intent4_front = new node;
  intent4_front->word = "INTENT4";
  intent4_front->count = 0;
  intent4_front->article = false;
  intent4_front->object1 = false;
  intent4_front->object2 = false;
  ListGenerator(intent4_front, "Intent4.xml");
  //PrintListLoop(intent4_front,0);

  intent4P_front = new node;
  intent4P_front->word = "INTENT4P";
  intent4P_front->count = 0;
  intent4P_front->article = false;
  intent4P_front->object1 = false;
  intent4P_front->object2 = false;
  ListGenerator(intent4P_front, "Intent4P.xml");
  //PrintListLoop(intent4P_front,0);

  intent5_front = new node;
  intent5_front->word = "INTENT5";
  intent5_front->count = 0;
  intent5_front->article = false;
  intent5_front->object1 = false;
  intent5_front->object2 = false;
  ListGenerator(intent5_front, "Intent5.xml");
  //PrintListLoop(intent5_front,0);

  intent5P_front = new node;
  intent5P_front->word = "INTENT5P";
  intent5P_front->count = 0;
  intent5P_front->article = false;
  intent5P_front->object1 = false;
  intent5P_front->object2 = false;
  ListGenerator(intent5P_front, "Intent5P.xml");
  //PrintListLoop(intent5P_front,0);
}

QG::~QG()
{
    for ( int i = 0; i < 30; i++)
    {
      for ( int j = 0; j < 9; j++ )
	{ testdata[i][j] = ""; }
    }

    RemoveListLoop(intent1_front);
    RemoveListLoop(intent1P_front);
    RemoveListLoop(intent2_front);
    RemoveListLoop(intent2P_front);
    RemoveListLoop(intent3_front);
    RemoveListLoop(intent3P_front);
    RemoveListLoop(intent4_front);
    RemoveListLoop(intent4P_front);
    RemoveListLoop(intent5_front);
    RemoveListLoop(intent5P_front);
}

//=================================================================
//          INITIALIZE
//=================================================================

/*****************************************************************/
/** Initialize the sentence                                     **/
/**                                                             **/
/** Description:                                                **/
/** Initialize the sentence structure vars                      **/
/*****************************************************************/
void QG::InitSentence()
{
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
}

//=================================================================
//          DESTROY LINKED LIST
//=================================================================

/*****************************************************************/
/** Remove linked list nodes                                    **/
/**                                                             **/
/** Description:                                                **/
/** Remove all nodes from the linked list                       **/
/**                                                             **/
/** Parameters:                                                 **/
/** curr - the node to remove                                   **/
/*****************************************************************/
void QG::RemoveListLoop(node* curr)
{
  for ( int i = 0; i < curr->count; i++ )
    { RemoveListLoop(curr->next[i]); }
  delete curr;
}

//=================================================================
//          FOR TESTING
//=================================================================

/*****************************************************************/
/** Get Test Data                                               **/
/**                                                             **/
/** Description:                                                **/
/** Get test data from a file.  This function is only used for  **/
/** testing with predefined sentences and data.                 **/
/*****************************************************************/
int QG::GetTestData()
{
  string line;
  string part;
  int i = 0;
  int j = 0;
  int m = 0;
  ifstream myfile;

  myfile.open("test.txt");

  if ( myfile.is_open() )
    {
      getline (myfile,line);
      
      while ( myfile.good() )
        {
	  j = 0;
      	  question_counter++;
	  getline (myfile,line);
          for ( m = 0; m < line.length(); m++ )
            {
              if ( line[m] != '|' ) { part += line[m]; }
	      else if (line[m] == '|' ) 
		{
		  testdata[i][j] = part;
		  part = "";
		  j++;
		}
	    }
          i++;
        }
      myfile.close();
    }
  else { SaveErrorToLog("Error QG_001: cannot open file: test.txt"); return -1;}
  return 0;
}

//=================================================================
//          GENERATE LINKED LIST USED FOR GENERATING SENTENCES
//=================================================================

/*****************************************************************/
/** Generate sentence linked list                               **/
/**                                                             **/
/** Description:                                                **/
/** Read words from files to generate a linked list that is     **/
/** used to create the sentences for each question.  Each node  **/
/** contains one word that points to all optional next words.   **/
/**                                                             **/
/** Parameters:                                                 **/
/** front - the node pointer to the front of the lined list     **/
/** filename - the name of the file to get the content of the   **/
/** linked list from.                                           **/
/*****************************************************************/
void QG::ListGenerator(node* front, string filename)
{
  ifstream myfile;
  string next_word;
  string tag = "";
  int level;
  int curr_level = 0;
  string value;
  stringstream ss;//create a stringstream
  string ss_level;
  node* curr = new node;
  curr = front;
  string errortxt;

  filename = "intent/"+filename;
  myfile.open(filename.c_str());
  if ( myfile.is_open() )
    {
      myfile >> next_word >> level >> next_word;
      while ( myfile.good() )
	{
	  curr = front;
	  myfile >> next_word;

	  if ( next_word[0] == '<' )
	    {
	      if ( next_word[1] == '/' )
		{
		  --curr_level;
		  tag = "";
		}
	      else
		{ 
		  curr_level++;
		  tag = next_word;
		}
	    }
	  else
	    {
	      for ( int j = 1; j < curr_level; j++ )
		{
		  curr = curr->next[curr->count-1];
		}

	      node* temp = new node;
	      temp->word = next_word;
	      temp->count = 0;
	      temp->article = false;
	      temp->object1 = false;
	      temp->object2 = false;

	      if ( tag == "<article>" ) { temp->article = true; }
	      else if ( tag == "<object1>" ) { temp->object1 = true; }
	      else if ( tag == "<object2>" ) { temp->object2 = true; }

	      curr->next.push_back(temp);
	      curr->count++;
	    } 
	}
      myfile.close();
    }
  else { errortxt = "ERROR QG_006: cannot open file: "+filename; SaveErrorToLog(errortxt); }
}

//=================================================================
//          SENTENCE AND QUESTION GENERATOR
//=================================================================

/*****************************************************************/
/** Generate sentence and question                              **/
/**                                                             **/
/** Description:                                                **/
/** call the functions to generate the sentence and question    **/
/**                                                             **/
/** Parameters:                                                 **/
/** INTENT - The intent of the sentence.  Used to generate the  **/
/**   sentence.                                                 **/
/** LEVEL - The category of the question.  Used to generate     **/
/**   the questions                                             **/
/** TYPE - The type of question.  Used to generate the question **/
/*****************************************************************/
sentence_data QG::Generator(string LEVEL, string INTENT, string TYPE)
{
  SentenceGenerator(INTENT);
  QuestionGenerator(LEVEL,TYPE);
  
  return curr_sentence;
}

/*****************************************************************/
/** Sentence Generator                                          **/
/**                                                             **/
/** Description:                                                **/
/** Generate the sentence based on the intent.                  **/
/**                                                             **/
/** Parameters:                                                 **/
/** INTENT - the intent of the sentence                         **/
/*****************************************************************/
int QG::SentenceGenerator(string INTENT)
{
  node* curr = new node;
  int choice = 0;
  
  InitSentence();
  curr = SetIntentParams(INTENT);
  srand (time(NULL));
  while ( curr->count != 0 )
    {
      choice = rand() % curr->count;
      curr = curr->next[choice];
      
      if ( curr->article ) { curr_sentence.article = curr->word; }
      else if ( curr->object1 ) { curr_sentence.object1 = curr->word; }
      else if ( curr->object2 ) { curr_sentence.object2 = curr->word; }
      else if ( curr->word == "NONE" ) { continue; }
      
      curr_sentence.sentence += curr->word;
      if ( curr->count != 0 ) { curr_sentence.sentence += "_"; }
    }
  
  std::transform(curr_sentence.article.begin(), curr_sentence.article.end(), curr_sentence.article.begin(), ::toupper);
  std::transform(curr_sentence.object1.begin(), curr_sentence.object1.end(), curr_sentence.object1.begin(), ::toupper);
  std::transform(curr_sentence.object2.begin(), curr_sentence.object2.end(), curr_sentence.object2.begin(), ::toupper);
  
  curr_sentence.noun_phrase = curr_sentence.article + "_" + curr_sentence.object1;
  
  return 0;
  
  /***************************************
  / USED FOR TESTING - START
  /***************************************
  //using predefined sentence
  int i = 0; 
  int j = 0;
  //  cout<<TYPE<<" - "<<VALUE<<" - "<<INTENT<<endl;
  cout<<"Generating Sentence"<<endl;
  if ( TYPE == "ARTICLE" ) { j = 1; }
  else if ( TYPE == "INTENT" ) { j = 4; }
  else if ( TYPE == "LINK" ) { j =5; }
  else if ( TYPE == "FOCUS" ) { j = 6; }
  else if ( TYPE == "NODE" ) { j = 7; }
  else if ( TYPE == "TYPE" ) { j = 8; }
  
  while (testdata[i][0] != "" && i < 30)
    {
      //cout<<testdata[i][j]<<" == "<<VALUE<<endl;
      //cout<<testdata[i][4]<<" == "<<INTENT<<endl;
      if ( testdata[i][j] == VALUE && testdata[i][4] == INTENT ) { break; }
      i++;
    }
  //cout<<"i = "<<i<<endl;
  if ( i >= question_counter )
    { cout<<"ERROR: No test data with those parameters"<<endl; return -1; }
  else
    {
      j = 0;
      curr_sentence.sentence = testdata[i][j]; j++;
      curr_sentence.article = testdata[i][j]; j++;
      curr_sentence.object1 = testdata[i][j]; j++;
      curr_sentence.object2 = testdata[i][j]; j++;
      curr_sentence.intent = testdata[i][j]; j++;
      curr_sentence.link = testdata[i][j]; j++;
      curr_sentence.focus = testdata[i][j]; j++;
      curr_sentence.node = testdata[i][j]; j++;
      curr_sentence.type = testdata[i][j]; j++;
    }
  return 0;
  /***************************************
  / USED FOR TESTING - END
  /***************************************/
}

/*****************************************************************/
/** Question Generator                                          **/
/**                                                             **/
/** Description:                                                **/
/** Generate the question based on the question category and    **/
/** question type.                                              **/
/**                                                             **/
/** Parameters:                                                 **/
/** LEVEL - The category of the questions                       **/
/** TYPE - The type of question                                 **/
/*****************************************************************/
int QG::QuestionGenerator(string LEVEL, string TYPE)
{
  string errortxt;

  if ( LEVEL == "INTRODUCTION" )
    {
      if ( TYPE == "ARTICLE" )
	{
	  curr_sentence.question = "What_is_the_article?";
	}
      else if ( TYPE == "INTENT" )
	{
	  curr_sentence.question = "What_is_the_intent?";
	}
      else if ( TYPE == "LINK" )
	{
	  curr_sentence.question = "What_is_the_link?";
	}
      else if ( TYPE == "FOCUS" )
	{
	  curr_sentence.question = "What_is_the_focus?";
	}
      else if ( TYPE == "NODE" )
	{
	  curr_sentence.question = "What_is_the_node?";
	}
      else if ( TYPE == "TYPE" )
	{
	  curr_sentence.question = "What_is_the_type_of_article?";
	}
      else if ( TYPE == "OBJECT" )
	{
	  curr_sentence.question = "What_is_the_object?";
	}
      else if ( TYPE == "NOUNPHRASE" )
	{
	  curr_sentence.question = "What_is_the_noun_phrase?";
	}
      else if ( TYPE == "ALL" )
	{
	  curr_sentence.question = "";
	}
      else 
	{
	  errortxt = "ERROR QG_002: Not a valid type; type = "+TYPE;
	  SaveErrorToLog(errortxt);
	  return -1;
	}
    }  
  else if ( LEVEL == "DIAGRAM" )
    { curr_sentence.question = "What_is_the_diagram?"; }
  else if ( LEVEL == "INTENT" )
    { curr_sentence.question = "What_is_the_intent?"; }
  else if ( LEVEL == "ARTICLE_DIAGRAM" )
    { 
      if ( TYPE == "TYPE" )
	{
	  curr_sentence.question = "What_is_the_type_of_article?";
	}
      else if ( TYPE == "ARTICLE" )
	{
	  curr_sentence.question = "What_is_the_article?";
	}
      else 
	{
	  errortxt = "ERROR QG_003: Not a valid type; type = "+TYPE;
	  SaveErrorToLog(errortxt);
	  return -1;
	}
    }
  else if ( LEVEL == "ARTICLE_INTENT" )
    { 
      if ( TYPE == "TYPE" )
	{
	  curr_sentence.question = "What_is_the_type_of_article?";
	}
      else if ( TYPE == "ARTICLE" )
	{
	  curr_sentence.question = "What_is_the_article?";
	}
      else 
	{
	  errortxt = "ERROR QG_004: Not a valid type; type = "+TYPE;
	  SaveErrorToLog(errortxt);
	  return -1;
	}
    }
  else
    {
      errortxt = "ERROR QG_005: Not a valid level; level = "+LEVEL;
      SaveErrorToLog(errortxt);
      return -1;
    }
  
  curr_sentence.question_level = LEVEL;
  curr_sentence.question_type = TYPE;
  return 0;
}

/*****************************************************************/
/** Set intent parameters                                       **/
/**                                                             **/
/** Description:                                                **/
/** Set the basic static information based on the intent        **/
/**                                                             **/
/** Parameters:                                                 **/
/** INTENT - The intent of the sentence                         **/
/*****************************************************************/
node* QG::SetIntentParams(string INTENT)
{
  if ( INTENT == "1" )
    {
      curr_sentence.intent = "1";
      curr_sentence.link = "IS";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "ELEMENT";
      curr_sentence.type = "THE";
      return intent1_front;
    }
  else if ( INTENT == "1P" )
    {
      curr_sentence.intent = "1P";
      curr_sentence.link = "IS-ALL-OF";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "SET";
      curr_sentence.type = "THE";
      return intent1P_front;
    }
  else if ( INTENT == "2" )
    {
      curr_sentence.intent = "2";
      curr_sentence.link = "IS";
      curr_sentence.focus = "SPEAKER";
      curr_sentence.node = "ELEMENT";
      curr_sentence.type = "A-AN";
      return intent2_front;
    }
  else if ( INTENT == "2P" )
    {
      curr_sentence.intent = "2P";
      curr_sentence.link = "IS";
      curr_sentence.focus = "SPEAKER";
      curr_sentence.node = "SET";
      curr_sentence.type = "NONE";
      return intent2P_front;
    }
  else if ( INTENT == "3" )
    {
      curr_sentence.intent = "3";
      curr_sentence.link = "IS";
      curr_sentence.focus = "LISTENER";
      curr_sentence.node = "ELEMENT";
      curr_sentence.type = "A-AN";
      return intent3_front;
    }
  else if ( INTENT == "3P" )
    {
      curr_sentence.intent = "3P";
      curr_sentence.link = "IS";
      curr_sentence.focus = "LISTENER";
      curr_sentence.node = "SET";
      curr_sentence.type = "NONE";
      return intent3P_front;
    }
  else if ( INTENT == "4" )
    {
      curr_sentence.intent = "4";
      curr_sentence.link = "IS-REP";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "CLASS";
      curr_sentence.type = "A-AN";
      return intent4_front;
    }
  else if ( INTENT == "4P" )
    {
      curr_sentence.intent = "4P";
      curr_sentence.link = "IS";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "CLASS";
      curr_sentence.type = "NONE";
      return intent4P_front;
    }
  else if ( INTENT == "5" )
    {
      curr_sentence.intent = "5";
      curr_sentence.link = "IS-IN";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "CLASS";
      curr_sentence.type = "A-AN";
      return intent5_front;
    }
  else if ( INTENT == "5P" )
    {
      curr_sentence.intent = "5P";
      curr_sentence.link = "IS-IN";
      curr_sentence.focus = "SHARED";
      curr_sentence.node = "CLASS";
      curr_sentence.type = "NONE";
      return intent5P_front;
    }
}

//=================================================================
//          PRINT
//=================================================================

/*****************************************************************/
/** Print sentnece struct                                       **/
/**                                                             **/
/** Description:                                                **/
/** Print the sentence and question information                 **/
/*****************************************************************/
void QG::PrintStruct()
{

  cout<<"\n================="<<endl;
  cout<<"current structure:"<<endl;
  cout<<"================="<<endl;
  cout<<curr_sentence.sentence<<endl;
  cout<<curr_sentence.article<<endl;
  cout<<curr_sentence.object1<<endl;
  cout<<curr_sentence.object2<<endl;
  cout<<curr_sentence.intent<<endl;
  cout<<curr_sentence.link<<endl;
  cout<<curr_sentence.focus<<endl;
  cout<<curr_sentence.node<<endl;
  cout<<curr_sentence.type<<endl;
  cout<<curr_sentence.question<<endl;
  cout<<curr_sentence.question_level<<endl;
  cout<<curr_sentence.question_type<<endl;
  cout<<"=================\n"<<endl;
}

/*****************************************************************/
/** Print the Linked List                                       **/
/**                                                             **/
/** Description:                                                **/
/** Print the linked list of the snetences data                 **/
/**                                                             **/
/** Parameters:                                                 **/
/** curr - the node pointer to the current node                 **/
/** l - the depth of the linked list                            **/
/*****************************************************************/
void QG::PrintListLoop(node* curr, int l)
{
  for ( int i = 0; i < l; i++ ) { cout<<"\t"; }
  l++;
  cout<<curr->word<<" - "<<curr->count;
  if ( curr->article == true ) { cout<<" - article"<<endl; }
  else if ( curr->object1 == true ) { cout<<" - object1"<<endl; }
  else if ( curr->object2 == true ) { cout<<" - object2"<<endl; }
  else { cout<<endl; }
  for ( int i = 0; i < curr->count; i++ )
  { PrintListLoop(curr->next[i], l); }
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
void QG::SaveErrorToLog(string error)
{
  ofstream myError;
  myError.open("error_log.txt", fstream::app);
  if ( !myError.is_open() )
    { cout<<"ERROR: Cannot open Error Log"<<endl; }
  myError<<error<<"\n";
  myError.close();
}
