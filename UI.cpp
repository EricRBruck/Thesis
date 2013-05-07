#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "UI.h"
#include "PM.h"

using namespace std;

//=================================================================
//          CONSTRUCTOR / DESTRUCTOR
//=================================================================

/*****************************************************************/
/** Constructor - User Interface                                **/
/*****************************************************************/
UI::UI()
{
  intent_usage[0] = "Intent 1 Singular: Referring to an element or object in the current focus";
  intent_description[0] = "\tArticle THE with a singular noun referring to an element\n";
  intent_description[0] += "\tor object in the current focus of the listener.  The object\n";
  intent_description[0] += "\tmay be in the current focus via entailment.";

  intent_usage[1] = "Intent 1 Plural: Referring to an element of a set or group in the current focus";
  intent_description[1] = "\tArticle THE with a plural noun referring to all members\n";
  intent_description[1] += "\tof a group node in the current focus of the listener.  The group\n";
  intent_description[1] += "\tmay be in the current focus via entailment.";

  intent_usage[2] = "Intent 2 Singular: Introducing an element or object to the listener";
  intent_description[2] = "\tArticle A/AN with a singular noun referring to it object node\n";
  intent_description[2] += "\tnot yet in the current focus of the listener.";

  intent_usage[3] = "Intent 2 Plural: Introducing a group or set to the listener";
  intent_description[3] = "\tNo article with a plural noun referring to its group node not\n";
  intent_description[3] += "\tyet in the current focus of the listener.";

  intent_usage[4] = "Intent 3 Singular: Asking for knowledge or awareness of an object or element";
  intent_description[4] = "\tArticle A/AN with a singular noun referring to its object\n";
  intent_description[4] += "\tnode in a question.";

  intent_usage[5] = "Intent 3 Plural: Asking for knowledge or awareness of a set or group";
  intent_description[5] = "\tNo article with a plural noun referring to its group node in\n";
  intent_description[5] += "\ta question.";

  intent_usage[6] = "Intent 4 Singular: Making a general statement about a class or category\n";
  intent_usage[6] += "through a representative";
  intent_description[6] = "\tArticle A/AN with a singular noun referring to it large\n";
  intent_description[6] += "\tcategory through a representative.";

  intent_usage[7] = "Intent 4 Plural: Making a general statement about a class or category";
  intent_description[7] = "\tNo article with a plural noun referring to its category node itself.";

  intent_usage[8] = "Intent 5 Singular: What is important is the class or category";
  intent_description[8] = "\tArticle A/AN with a singular noun referring to an unspecified\n";
  intent_description[8] += "\tmember of a group or category.";

  intent_usage[9] = "Intent 5 Plural: What is important is the class or category";
  intent_description[9] = "\tNo article with a plural noun referring to an unspecified subset\n";
  intent_description[9] += "\tof a class or category.";

  intro_description[0] = "An article in English defines the type of reference being made to a noun.  \nThe English articles are a, an, the, or no article at all.  ";

  intro_description[1] = "The noun names the referent object of the sentence.  ";

  intro_description[2] = "The combination of the article and noun is noun phrase.  ";

  intro_description[3] = "The link connects the nouns / noun phrase and the nodes in the diagram.  ";
  intro_description[3] += "This represents how the reference object is being referred to.  There are four types of links.";
  intro_description[3] += "\n\t1. IS-link: Represents the noun phrase referring to the node itself.\n\t\tPICTURE";
  intro_description[3] += "\n\t2. IS-ALL-OF-link: Represents the noun phrase referring to all members of a group.\n\t\tPICTURE";
  intro_description[3] += "\n\t3. IS-REP-link: Represents the noun phrase referring to a representative of the category.\n\t\tPICTURE";
  intro_description[3] += "\n\t4. IS-IN-link: Represents the noun phrase referring to some, unspecified, member of a group.\n\t\tPICTURE";

  intro_description[4] = "The node represents the number of objects being referred to.  ";
  intro_description[4] += "There are three types of nodes.";
  intro_description[4] += "\n\t1. Element: Represents a single element / object.\n\t\tPICTURE";
  intro_description[4] += "\n\t2. Set: Represents a group of elements / objects.\n\t\tPICTURE";
  intro_description[4] += "\n\t3. Class: Represents the full category.\n\t\tPICTURE";

  intro_description[5] = "The focus represents what the speaker and listener have knowledge of.  ";
  intro_description[5] += "There are three types of focus.";
  intro_description[5] += "\n\t1. Shared: Represents the listener and speaker having knowledge of what is being talked about.\n\t\tPICTURE";
  intro_description[5] += "\n\t2. Speaker: Represents the speaker having knowledge of what is being talked about.  The listener does not know what is being talked about.\n\t\tPICTURE";
  intro_description[5] += "\n\t3. Listener: Represents the listener having knowledge of what is being talked about.  The speaker does not know what is being talked about.\n\t\tPICTURE";

  intro_description[6] = "Each sentence has an intent type that specifies this reference being made to the noun.  ";
  intro_description[6] += "The intent type describes what a speaker is trying to convey to the listener.";
  intro_description[6] += "\n\nEach intent type is represented as a diagram with three parts.";
  intro_description[6] += "\n\t1. Link";
  intro_description[6] += "\n\t2. Node";
  intro_description[6] += "\n\t3. Focus";

  temp_diagram temp_diag;
  temp_diag.link = "";
  temp_diag.node = "";
  temp_diag.focus = "";
  diagram[0] = temp_diag;
  diagram[1] = temp_diag;
  diagram[2] = temp_diag;

}

/*****************************************************************/
/** Destructor - User Interface                                 **/
/*****************************************************************/
UI::~UI()
{
  ;
}

/*****************************************************************/
/** Get User                                                    **/
/**                                                             **/
/** Description:                                                **/
/** Get the user name                                           **/
/*****************************************************************/
string UI::GetUser()
{
  string ans = "";
  string user = "";
  string line = "";

  //print all users
  cout<<"User List"<<endl;
  ifstream myfile;
  myfile.open("users/user_names.txt");
  while ( myfile >> line ) {
    cout<<"\t"<<line<<endl;
    users.push_back(line);
  }
  myfile.close();
  if ( users.size() == 0 )
    { cout<<"\tNo Users"<<endl; }

  do {
    cout<<"Enter Name: ";
    cin>>user;

    for ( int j = 0; j < user.length(); j++ )
      { user[j] = toupper(user[j]); }

    //look for user
    int i = 0;
    while ( i < users.size() )
      {
	if ( users[i] == user ) break;
	i++;
      }

    if ( i == users.size() ) 
      {
	cout<<"New User? (Y/N): ";
	cin>>ans;
	ans = tolower(ans[0]);
	if ( ans == "y" ) { 
	  user += "-";
	  break; }
	else if ( ans == "n" ) { ; }
	else { cout<<"Invalid Answer (Y/N)"<<endl; }
      }
    else
      { break; }
  } while(1);

  return user;
}

//=================================================================
//          Ask The Question
//=================================================================

/*****************************************************************/
/** Ask the Question                                            **/
/**                                                             **/
/** Description:                                                **/
/** dispay the question and any lession the the user            **/
/**                                                             **/
/** Parameters:                                                 **/
/** sentence_info - the correct current sentence and question   **/
/**   information                                               **/
/** start - indicates what, if any, lesses will be presented    **/
/**   to the user                                               **/
/*****************************************************************/
sentence_data UI::AskQuestion(sentence_data sentence_info, int start)
{
  curr_sentence = sentence_info;
  student_sentence = sentence_info;

  for (int j = 0; j < curr_sentence.sentence.length(); j++) {
    if ( curr_sentence.sentence[j] == '_' ) {
      curr_sentence.sentence[j] = ' '; }
  }
  for (int j = 0; j < curr_sentence.question.length(); j++) {
    if ( curr_sentence.question[j] == '_' ) {
      curr_sentence.question[j] = ' '; }
  }

  if ( curr_sentence.question_level == "INTRODUCTION" )
    {
      if ( start == 0 ) { IntroDescription(); }
      if ( AskIntroduction() == -1 ) { SaveErrorToLog("ERROR UI_001: Problem print INTRODUCTION question"); }
    }
  else if ( curr_sentence.question_level == "DIAGRAM" )
    {
      if ( AskDiagram() == -1 ) { SaveErrorToLog("ERROR UI_002: Problem print DIAGRAM question"); }
    }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" )
    {
      if ( AskArticle(1) == -1 ) { SaveErrorToLog("ERROR UI_003: Problem print ARTICLE DIAGRAM question"); }
    }
  else if ( curr_sentence.question_level == "ARTICLE_INTENT" )
    {
      if ( AskArticle(0) == -1 ) { SaveErrorToLog("ERROR UI_004: Problem print ARTICLE INTENT question"); }
    }
  else { return student_sentence; }

  //replace " " with "_"
  for (int j = 0; j < curr_sentence.sentence.length(); j++) {
    if ( curr_sentence.sentence[j] == ' ' ) {
      curr_sentence.sentence[j] = '_'; }
    curr_sentence.sentence[j] = tolower(curr_sentence.sentence[j]); }
  for (int j = 0; j < curr_sentence.question.length(); j++) {
    if ( curr_sentence.question[j] == ' ' ) {
      curr_sentence.question[j] = '_'; }
    curr_sentence.question[j] = tolower(curr_sentence.question[j]); }

  return student_sentence;
}

/*****************************************************************/
/** Ask Again                                                   **/
/**                                                             **/
/** Description:                                                **/
/** Ask the user to try anagin                                  **/
/*****************************************************************/
sentence_data UI::AskAgain()
{
  student_sentence = curr_sentence;
  string errortxt;

  if ( curr_sentence.question_level == "INTRODUCTION" )
    {
      if ( CheckIntroduction() == -1 ) { errortxt = "ERROR UI_005: Problem print INTRODUCTION question - "+curr_sentence.question_type; SaveErrorToLog(errortxt); }
    }
  else if ( curr_sentence.question_level == "DIAGRAM" )
    {
      if ( CheckDiagram() == -1 ) { SaveErrorToLog("ERROR UI_006: Problem print DIAGRAM question"); }
    }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" )
    {
      if ( CheckArticle() == -1 ) { SaveErrorToLog("ERROR UI_007: Problem print ARTICLE DIAGRAM question"); }
    }
  else if ( curr_sentence.question_level == "ARTICLE_INTENT" )
    {
      if ( CheckArticle() == -1 ) { SaveErrorToLog("ERROR UI_008: Problem print ARTICLE INTENT question"); }
    }
  else { return student_sentence; }

  return student_sentence;
}

/*****************************************************************/
/** Provide Correct Answer                                      **/
/**                                                             **/
/** Description:                                                **/
/** Display the correct answer to the question                  **/
/*****************************************************************/
void UI::provideCorrectAnswer(sentence_data curr_sentence)
{
  if ( curr_sentence.question_level == "INTRODUCTION" )
    {
      if ( curr_sentence.question_type == "TYPE" )
	{ cout<<"The correct article Type is "<<curr_sentence.type<<endl; }
      else if ( curr_sentence.question_type == "ARTICLE" )
	{ cout<<"The correct article is "<<curr_sentence.article<<endl; }
      else if ( curr_sentence.question_type == "OBJECT" )
	{ cout<<"The correct object is "<<curr_sentence.object1<<endl; }
      else if ( curr_sentence.question_type == "NOUNPHRASE" )
	{ 
	    for (int j = 0; j < curr_sentence.noun_phrase.length(); j++) {
	      if ( curr_sentence.noun_phrase[j] == '_' ) {
		curr_sentence.noun_phrase[j] = ' '; }
	    }
	    //cout<<"article = "<<curr_sentence.article<<endl;
	    if ( curr_sentence.article == "NONE" )
	      { curr_sentence.noun_phrase = curr_sentence.object1; }
	    cout<<"The correct noun phrase is "<<curr_sentence.noun_phrase<<endl; 
	}
      else if ( curr_sentence.question_type == "LINK" )
	{ cout<<"The correct link is "<<curr_sentence.link<<"-link"<<endl; }
      else if ( curr_sentence.question_type == "NODE" )
	{ cout<<"The correct node is "<<curr_sentence.node<<endl; }
      else if ( curr_sentence.question_type == "FOCUS" )
	{ cout<<"The correct focus is "<<curr_sentence.focus<<endl; }
    }
  else if ( curr_sentence.question_level == "DIAGRAM" )
    {
      if ( curr_sentence.question_type == "ALL" )
	{ 
	  cout<<"The correct link is "<<curr_sentence.link<<"-link"<<endl;
	  cout<<"The correct node is "<<curr_sentence.node<<endl;
	  cout<<"The correct focus is "<<curr_sentence.focus<<endl;
	}
      else if ( curr_sentence.question_type == "LINK" )
	{ cout<<"The correct link is "<<curr_sentence.link<<"-link"<<endl; }
      else if ( curr_sentence.question_type == "NODE" )
	{ cout<<"The correct node is "<<curr_sentence.node<<endl; }
      else if ( curr_sentence.question_type == "FOCUS" )
	{ cout<<"The correct focus is "<<curr_sentence.focus<<endl; }
    }
  else if ( curr_sentence.question_level == "ARTICLE_DIAGRAM" || curr_sentence.question_level == "ARTICLE_INTENT" )
    {
      if ( curr_sentence.question_type == "TYPE" )
	{ cout<<"The correct article Type is "<<curr_sentence.type<<endl; }
      else if ( curr_sentence.question_type == "ARTICLE" )
	{ cout<<"The correct article is "<<curr_sentence.article<<endl; }
    }
}

/*****************************************************************/
/** Introduction Description                                    **/
/**                                                             **/
/** Description:                                                **/
/** Display applicable lessons to user                          **/
/*****************************************************************/
int UI::IntroDescription()
{
  cout<<"\nLESSON"<<endl;
  cout<<"========================================="<<endl;
  if ( curr_sentence.question_type == "ALL" ) { 
    cout<<"\n"<<intro_description[0];
    cout<<"\n"<<intro_description[1];
    cout<<"\n"<<intro_description[2];
    cout<<"\n\n"<<intro_description[6]<<"\n\nPress Enter to continue.";
    cin.ignore();
    cout<<"\n"<<intro_description[3]<<"\n\nPress Enter to continue.";
    cin.ignore();
    cout<<"\n"<<intro_description[4]<<"\n\nPress Enter to continue.";
    cin.ignore();
    cout<<"\n"<<intro_description[5]<<"\n\nPress Enter to continue.";
    cin.ignore(); }
  else if ( curr_sentence.question_type == "TYPE" ) { cout<<intro_description[0]; }
  else if ( curr_sentence.question_type == "ARTICLE" ) { cout<<intro_description[0]; }
  else if ( curr_sentence.question_type == "OBJECT" ) { cout<<intro_description[0]<<"\n"<<intro_description[1]; }
  else if ( curr_sentence.question_type == "NOUNPHRASE" ) { cout<<intro_description[0]<<"\n"<<intro_description[1]<<"\n"<<intro_description[2]; }
  else if ( curr_sentence.question_type == "LINK" ) { cout<<intro_description[6]<<"\n\n"<<intro_description[3]; }
  else if ( curr_sentence.question_type == "NODE" ) { cout<<intro_description[6]<<"\n\n"<<intro_description[4]; }
  else if ( curr_sentence.question_type == "FOCUS" ) { cout<<intro_description[6]<<"\n\n"<<intro_description[5]; }
  else { return -1; }
  cout<<"\n========================================="<<endl;
  return 0;
}

/*****************************************************************/
/** Ask the introduction question                               **/
/**                                                             **/
/** Description:                                                **/
/** Display the introduction question to the user               **/
/*****************************************************************/
int UI::AskIntroduction()
{
  int i = -1;

  //Remove NONE
  int len = curr_sentence.article.length() + 1;
  string temp = "";
  string comp = curr_sentence.article;
  string temp_sentence = curr_sentence.sentence;

  if ( curr_sentence.article == "NONE" ) {
    temp_sentence = "";
    comp = "none ";
    for (int j = 0; j < curr_sentence.sentence.length(); j++) {
      temp = "";
      for (int m = 0; m < len; m++) {
	temp += tolower(curr_sentence.sentence[m+j]); }
      if ( temp == comp ) {
	for (int m = 0; m < j; m++) {
	  temp_sentence += curr_sentence.sentence[m]; }
	for (int m = j+len; m < curr_sentence.sentence.length(); m++) {
	  temp_sentence += tolower(curr_sentence.sentence[m]); } } } }

  temp_sentence[0] = toupper(temp_sentence[0]);
  if ( curr_sentence.intent == "3" || curr_sentence.intent == "3P" )
    { temp_sentence += "?"; }
  else { temp_sentence += "."; }

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
  else { return -1; }

  cout<<"\nINTRODUCTION QUESTION"<<endl;
  cout<<intent_usage[i]<<endl;
  if ( curr_sentence.question_type != "TYPE" && curr_sentence.question_type != "ARTICLE" && curr_sentence.question_type != "NOUNPHRASE" ) { cout<<intent_description[i]<<endl; }
  cout<<"\t(NOTE: intent will be augmented with a story.)"<<endl;
  if ( curr_sentence.question_type != "LINK" ) { cout<<"\tLink: "<<curr_sentence.link<<endl; }
  if ( curr_sentence.question_type != "NODE" ) { cout<<"\tNode: "<<curr_sentence.node<<endl; }
  if ( curr_sentence.question_type != "FOCUS" ) { cout<<"\tFocus: "<<curr_sentence.focus<<endl; }
  if ( curr_sentence.question_type != "TYPE" && curr_sentence.question_type != "ARTICLE" && curr_sentence.question_type != "NOUNPHRASE" ) { cout<<"\tArticle Type: "<<curr_sentence.type<<endl; }
  
  cout<<"Example:"<<endl;
  cout<<"\t"<<temp_sentence<<endl;
  if ( curr_sentence.question_type != "TYPE" && curr_sentence.question_type != "ARTICLE" && curr_sentence.question_type != "NOUNPHRASE" ) { cout<<"\tArticle: "<<curr_sentence.article<<endl; }
  if ( curr_sentence.question_type != "OBJECT" && curr_sentence.question_type != "NOUNPHRASE" ) { cout<<"\tObject: "<<curr_sentence.object1<<endl; }
  if ( curr_sentence.object2 != "" && curr_sentence.question_type != "OBJECT" && curr_sentence.question_type != "NOUNPHRASE" ) { cout<<"\t\tObject: "<<curr_sentence.object2<<endl; }
  if ( curr_sentence.question_type != "TYPE" && curr_sentence.question_type != "ARTICLE" && curr_sentence.question_type != "OBJECT" && curr_sentence.question_type != "NOUNPHRASE" ) {
    cout<<"\tNoun Phrase: ";
    if ( curr_sentence.article != "NONE" ) { cout<<curr_sentence.article<<" "; }
    cout<<curr_sentence.object1;
    if ( curr_sentence.object2 != "" ) { cout<<" "<<curr_sentence.object2<<endl; }
    else { cout<<endl; } }

  if ( curr_sentence.question_type != "ALL" ) {
    cout<<"\n"<<curr_sentence.question<<endl;
    if ( curr_sentence.question_type == "TYPE" )
      { cout<<"\nA) THE\nB) A / AN\nC) NONE"<<endl; }
    else if ( curr_sentence.question_type == "ARTICLE" )
      { cout<<"\nA) THE\nB) A\nC) AN\nD) NONE"<<endl; }
    else if ( curr_sentence.question_type == "LINK" )
      { cout<<"\nA) IS-link\nB) IS-ALL-OF-link\nC) IS-REP-link\nD) IS-IN-link"<<endl; }
    else if ( curr_sentence.question_type == "NODE" )
      { cout<<"\nA) Element / Object\nB) Set / Group\nC) Class / Category"<<endl; }
    else if ( curr_sentence.question_type == "FOCUS" )
      { cout<<"\nA) Shared\nB) Speaker\nC) Listener"<<endl; }
    else if ( curr_sentence.question_type == "OBJECT" || curr_sentence.question_type == "NOUNPHRASE" )
      { ; }
    else { return -1; }

    return CheckIntroduction();
  }
  return 0;
}

/*****************************************************************/
/** Check Introduction                                          **/
/**                                                             **/
/** Description:                                                **/
/** Check if the student answer to the intorduction question    **/
/** is correct                                                  **/
/*****************************************************************/
int UI::CheckIntroduction()
{
    int valid = 0;

    while (valid == 0) {

      valid = 1;

      cout<<"ANSWER ==> ";
      getline(cin,ans);

      for (int j = 0; j < ans.length(); j++) {
	ans[j] = tolower(ans[j]); }
      
      if ( curr_sentence.question_type == "TYPE" ) {
	if ( ans == "a" ) {
	  student_sentence.type = "THE";
	  student_sentence.article = "THE";
	  student_sentence.noun_phrase = "THE_" + curr_sentence.object1; }
	else if ( ans == "b" ) {
	  student_sentence.type = "A-AN";
	  if ( curr_sentence.article != "A" && curr_sentence.article != "AN" ) {
	    student_sentence.article = "A";
	    student_sentence.noun_phrase = "A_" + curr_sentence.object1; } }
	else if ( ans == "c" ) {
	  student_sentence.type = "NONE";
	  student_sentence.article = "NONE";
	  student_sentence.noun_phrase = "NONE_" + curr_sentence.object1; }
	else {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0;
	}
      }
      else if ( curr_sentence.question_type == "ARTICLE" ) {
	if ( ans == "a" ) {
	  student_sentence.article = "THE";
	  student_sentence.type = "THE";
	  student_sentence.noun_phrase = student_sentence.article + "_" + curr_sentence.object1; }
	else if ( ans == "b" ) {
	  student_sentence.article = "A";
	  student_sentence.type = "A-AN";
	  student_sentence.noun_phrase = student_sentence.article + "_" + curr_sentence.object1; }
	else if ( ans == "c" ) {
	  student_sentence.article = "AN";
	  student_sentence.type = "A-AN";
	  student_sentence.noun_phrase = student_sentence.article + "_" + curr_sentence.object1; }
	else if ( ans == "d" ) {
	  student_sentence.article = "NONE";
	  student_sentence.type = "NONE";
	  student_sentence.noun_phrase = student_sentence.article + "_" + curr_sentence.object1; }
	else {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0;
	}
      }
      else if ( curr_sentence.question_type == "LINK" ) {
	if ( ans == "a" ) {
	  student_sentence.link = "IS"; }
	else if ( ans == "b" ) {
	  student_sentence.link = "IS-ALL-OF"; }
	else if ( ans == "c" ) {
	  student_sentence.link = "IS-REP"; }
	else if ( ans == "d" ) {
	  student_sentence.link = "IS-IN"; }
	else {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0;
	}    }
      else if ( curr_sentence.question_type == "NODE" ) {
	if ( ans == "a" ) {
	  student_sentence.node = "ELEMENT"; }
	else if ( ans == "b" ) {
	  student_sentence.node = "SET"; }
	else if ( ans == "c" ) {
	  student_sentence.node = "CLASS"; }
	else {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0;
	}
      }
      else if ( curr_sentence.question_type == "FOCUS" ) {
	if ( ans == "a" ) {
	  student_sentence.focus = "SHARED"; }
	else if ( ans == "b" ) {
	  student_sentence.focus = "SPEAKER"; }
	else if ( ans == "c" ) {
	  student_sentence.focus = "LISTENER"; }
	else {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0;
	}
      }
      else if ( curr_sentence.question_type == "OBJECT" ) {
	if ( ans == "" ) {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0; }
	else {
	  for (int j = 0; j < ans.length(); j++) {
	    ans[j] = toupper(ans[j]); } 
	  student_sentence.object1 = ans;
	  student_sentence.noun_phrase = curr_sentence.article + "_" + ans;
	}
      }
      else if ( curr_sentence.question_type == "NOUNPHRASE" ) {
	int flag = 0;
	string obj = "";
	string art = "";
	
	if ( ans == "" ) {
	  cout<<"Invalid Answer"<<endl;
	  valid = 0; }
	else {
	  for (int j = 0; j < ans.length(); j++) {
	    ans[j] = toupper(ans[j]); } 
	  
	  student_sentence.noun_phrase = ans;
	  for (int j = 0; j < student_sentence.noun_phrase.length(); j++) {
	    if ( student_sentence.noun_phrase[j] == ' ' ) {
	      student_sentence.noun_phrase[j] = '_'; }
	  }
	}
	
	for (int j = 0; j < ans.length(); j++) {
	  if ( ans[j] != ' ' ) {
	    if ( flag == 0 ) {
	      art += ans[j]; }
	    else if ( flag == 1 ) {
	      obj += ans[j]; } }
	  else { flag++; }
	}
	
	if ( obj == "" ) { obj = art; art = ""; student_sentence.noun_phrase = "NONE_" + student_sentence.noun_phrase; }
	
	student_sentence.object1 = obj;
	
	if ( art == "A" ) {
	  student_sentence.article = "A";
	  student_sentence.type = "A-AN"; }
	else if ( art == "AN" ) {
	  student_sentence.article = "AN";
	  student_sentence.type = "A-AN"; }
	else if ( art == "THE" ) {
	  student_sentence.article = "THE";
	  student_sentence.type = "THE"; }
	else if ( art == "" ) {
	  student_sentence.article = "NONE";
	  student_sentence.type = "NONE"; }
	else {
	  student_sentence.article = art;
	  student_sentence.type = art; }
      }
      else { return -1; }
    }
    
    cout<<endl;
    return 0;
}

/*****************************************************************/
/** Ask Diagram                                                 **/
/**                                                             **/
/** Description:                                                **/
/** Display the diagram question to the user                    **/
/*****************************************************************/
int UI::AskDiagram()
{
  int i = -1;
  int link[4] = {0,0,0,0};
  int node[3] = {0,0,0};
  int focus[3] = {0,0,0};

  //Remove NONE
  int len = curr_sentence.article.length() + 1;
  string tempw = "";
  string comp = curr_sentence.article;
  string temp_sentence = curr_sentence.sentence;

  if ( curr_sentence.article == "NONE" ) {
    temp_sentence = "";
    comp = "none ";
    for (int j = 0; j < curr_sentence.sentence.length(); j++) {
      tempw = "";
      for (int m = 0; m < len; m++) {
	tempw += tolower(curr_sentence.sentence[m+j]); }
      if ( tempw == comp ) {
	for (int m = 0; m < j; m++) {
	  temp_sentence += curr_sentence.sentence[m]; }
	for (int m = j+len; m < curr_sentence.sentence.length(); m++) {
	  temp_sentence += tolower(curr_sentence.sentence[m]); } } } }

  temp_sentence[0] = toupper(temp_sentence[0]);
  if ( curr_sentence.intent == "3" || curr_sentence.intent == "3P" )
    { temp_sentence += "?"; }
  else { temp_sentence += "."; }

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
  else { return -1; }

  cout<<"\nDIAGRAM QUESTION"<<endl;
  cout<<intent_usage[i]<<endl;
  cout<<intent_description[i]<<endl;
  cout<<"\t(NOTE: intent will be augmented with a story.)"<<endl;
  cout<<"\n"<<curr_sentence.question<<endl;

  srand(time(NULL));
  int correct = rand() % 3;
  int temp = 0;

  for ( int j = 0; j < 3; j++ ) {
    srand(time(NULL)+j);

    if ( (curr_sentence.question_type == "LINK" || curr_sentence.question_type == "ALL") && j != correct ) {
      //select different link
      temp = (rand()+j) % 4;
      while(1) {
	if ( temp == 0 && curr_sentence.link != "IS" && link[0] == 0 )
	  { diagram[j].link = "IS"; link[0]=1; break; }
	else if ( temp == 1 && curr_sentence.link != "IS-ALL-OF" && link[1] == 0 )
	  { diagram[j].link = "IS-ALL-OF"; link[1]=1; break; }
	else if ( temp == 2 && curr_sentence.link != "IS-REP" && link[2] == 0 )
	  { diagram[j].link = "IS-REP"; link[2]=1; break; }
	else if ( temp == 3 && curr_sentence.link != "IS-IN" && link[3] == 0 )
	  { diagram[j].link = "IS-IN"; link[3]=1; break; }
	else { temp = (temp + 1) % 4; }
      }
    }
    else { diagram[j].link = curr_sentence.link; }
    if ( (curr_sentence.question_type == "NODE" || curr_sentence.question_type == "ALL") && j != correct ) {
      //select different node
      temp = (rand()+j) % 3;
      while(1) {
	if ( temp == 0 && curr_sentence.node != "ELEMENT" && node[0] == 0 )
	  { diagram[j].node = "ELEMENT"; node[0]=1; break; }
	else if ( temp == 1 && curr_sentence.node != "SET" && node[1] == 0 )
	  { diagram[j].node  = "SET"; node[1]=1; break; }
	else if ( temp == 2 && curr_sentence.node != "CLASS" && node[2] == 0 )
	  { diagram[j].node = "CLASS"; node[2]=1; break; }
	else { temp = (temp + 1) % 3; }
      }
    }
    else { diagram[j].node = curr_sentence.node; }
    if ( (curr_sentence.question_type == "FOCUS" || curr_sentence.question_type == "ALL") && j != correct ) {
      //select different focus
      temp = (rand()+j) % 3;
      while(1) {
	if ( temp == 0 && curr_sentence.focus != "SHARED" && focus[0] == 0 )
	  { diagram[j].focus = "SHARED"; focus[0]=1; break; }
	else if ( temp == 1 && curr_sentence.focus != "SPEAKER" && focus[1] == 0 )
	  { diagram[j].focus = "SPEAKER"; focus[1]=1; break; }
	else if ( temp == 2 && curr_sentence.focus != "LISTENER" && focus[2] == 0 )
	  { diagram[j].focus = "LISTENER"; focus[2]=1; break; }
	else { temp = (temp + 1) % 3; }
      }
    }
    else { diagram[j].focus = curr_sentence.focus; }

    if ( j == 0 ) { cout<<"Diagram A)"<<endl; }
    if ( j == 1 ) { cout<<"Diagram B)"<<endl; }
    if ( j == 2 ) { cout<<"Diagram C)"<<endl; }
    cout<<"\t"<<temp_sentence<<endl;
    cout<<"\t"<<"Link: "<<diagram[j].link<<endl;
    cout<<"\t"<<"Node: "<<diagram[j].node<<endl;
    cout<<"\t"<<"Focus: "<<diagram[j].focus<<endl;
  }

  return CheckDiagram();
  
}

/*****************************************************************/
/** Check Diagram Answer                                        **/
/**                                                             **/
/** Description:                                                **/
/** Check if the student answer to the diagram question is      **/
/** correct                                                     **/
/*****************************************************************/
int UI::CheckDiagram()
{
  temp_diagram diagram_ans;
  while (1)
    {
      cout<<"ANSWER ==> ";
      cin>>ans;

      if ( ans == "a" || ans == "A" ) { diagram_ans = diagram[0]; break; }
      else if ( ans == "b" || ans == "B" ) { diagram_ans = diagram[1]; break; }
      else if ( ans == "c" || ans == "C" ) { diagram_ans = diagram[2]; break; }
      else { cout<<"Invalid Answer"<<endl; }
    }

  student_sentence.link = diagram_ans.link;
  student_sentence.node = diagram_ans.node;
  student_sentence.focus = diagram_ans.focus;

  cout<<endl;
  return 0;
}

/*****************************************************************/
/** Ask Article Question                                        **/
/**                                                             **/
/** Description:                                                **/
/** Display the article question to the user                    **/
/**                                                             **/
/** Parameters:                                                 **/
/** display - indicates if the diagram is displayed to the user **/
/*****************************************************************/
int UI::AskArticle(int display)
{
  int i = -1;

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
  else { return -1; }

  cout<<endl;
  if ( display == 1 ) { cout<<"ASSISTED "; }
  cout<<"ARTICLE SELECTION QUESTION"<<endl;

  //remove article from sentence
  int len = curr_sentence.article.length() + 1;
  string temp = "";
  string comp = curr_sentence.article;
  string part1 = "";
  string part2 = "";
  string temp_sentence = "";

  for (int j = 0; j < comp.length(); j++) {
    comp[j] = tolower(comp[j]); }
  comp += ' ';

  for (int j = 0; j < curr_sentence.sentence.length(); j++) {
    temp = "";
    for (int m = 0; m < len; m++) {
      temp += tolower(curr_sentence.sentence[m+j]); }
    if ( temp == comp ) {
      for (int m = 0; m < j; m++) {
	part1 += curr_sentence.sentence[m]; }
      for (int m = j+len; m < curr_sentence.sentence.length(); m++) {
	part2 += tolower(curr_sentence.sentence[m]); }
      temp_sentence = part1 + "____ " + part2; break; } }

  temp_sentence[0] = toupper(temp_sentence[0]);
  if ( curr_sentence.intent == "3" || curr_sentence.intent == "3P" )
    { temp_sentence += "?"; }
  else { temp_sentence += "."; }

  cout<<intent_usage[i]<<endl;
  cout<<"\t(NOTE: intent will be augmented with a story.)"<<endl;
  cout<<"\n"<<curr_sentence.question<<endl;
  cout<<"\n"<<temp_sentence<<endl;

  if ( display == 1 )
    {
      cout<<"\tLink: "<<curr_sentence.link<<endl;
      cout<<"\tNode: "<<curr_sentence.node<<endl;
      cout<<"\tFocus: "<<curr_sentence.focus<<endl;
    }

  if ( curr_sentence.question_type == "TYPE" )
    { cout<<"\nA) THE\nB) A / AN\nC) NONE"<<endl; }
  else if ( curr_sentence.question_type == "ARTICLE" )
    { cout<<"\nA) THE\nB) A\nC) AN\nD) NONE"<<endl; }
  else { return -1; }

  return CheckArticle();
}

/*****************************************************************/
/** Check Article Answer                                        **/
/**                                                             **/
/** Description:                                                **/
/** Check if the student answer to the article question is      **/
/** correct                                                     **/
/*****************************************************************/
int UI::CheckArticle()
{
  while (1)
    {
      cout<<"ANSWER ==> ";
      cin>>ans;

      if ( curr_sentence.question_type == "TYPE" ) {
	if ( ans == "a" || ans == "A" ) {
	  student_sentence.article = "THE";
	  student_sentence.type = "THE";
	  student_sentence.noun_phrase = "THE_" + curr_sentence.object1;
	  break;
	}
	else if ( ans == "b" || ans == "B" ) {
	  if ( curr_sentence.article != "A" && curr_sentence.article != "AN" )
	    { student_sentence.article = "A"; }
	  student_sentence.type = "A-AN";
      	  student_sentence.noun_phrase = student_sentence.article + "_" + curr_sentence.object1;
	  break;
	}
	else if ( ans == "c" || ans == "C" ) {
	  student_sentence.article = "NONE";
	  student_sentence.type = "NONE";
	  student_sentence.noun_phrase = "NONE_" + curr_sentence.object1;
	  break;
	}
	else { cout<<"Invalid Answer"<<endl; }
      }
      else if ( curr_sentence.question_type == "ARTICLE" ) {
	if ( ans == "a" || ans == "A" ) {
	  student_sentence.article = "THE";
	  student_sentence.type = "THE";
	  student_sentence.noun_phrase = "THE_" + curr_sentence.object1;
	  break;
	}
	else if ( ans == "b" || ans == "B" ) {
	  student_sentence.article = "A";
	  student_sentence.type = "A-AN";
	  student_sentence.noun_phrase = "A_" + curr_sentence.object1;
	  break;
	}
	else if ( ans == "c" || ans == "C" ) {
	  student_sentence.article = "AN";
	  student_sentence.type = "A-AN";
	  student_sentence.noun_phrase = "AN_" + curr_sentence.object1;
	  break;
	}
	else if ( ans == "d" || ans == "D" ) {
	  student_sentence.article = "NONE";
	  student_sentence.type = "NONE";
	  student_sentence.noun_phrase = "NONE_" + curr_sentence.object1;
	  break;
	}
	else { cout<<"Invalid Answer"<<endl; }
      }
      else { return -1; }
    }
  cout<<endl;
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
void UI::SaveErrorToLog(string error)
{
  ofstream myError;
  myError.open("error_log.txt", fstream::app);
  if ( !myError.is_open() )
    { cout<<"ERROR: Cannot open Error Log"<<endl; }
  myError<<error<<"\n";
  myError.close();
}
