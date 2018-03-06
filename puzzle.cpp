#include <iostream>

//for tolower()
#include <ctype.h>

/*!**********************************************************************
@file    main.cpp
@author  rwmc
@date    4/6/2016

@brief
Testing main for the console-input.h file. A reasonable reference for
using this application on a few different systems.

@copyright See LICENSE.md
************************************************************************/
#include "console-input.h" // KeyHit() and GetKey() and others


//for delays
#include <chrono>
#include <thread>
#include <math.h>

//for memset
#include <cstring>

/*
uses elements from c++11
if std::to_string is not declared in scope
add -std=c++11 as a compiler option in g++ or gcc
*/

using namespace std;

const short delay = 6;
const short scrambleamount  = 3;
const short numberOfRoters  = 4;
const short numberOfStates  = 8;
const short numberOfButtons = 8;


void sleep(short delay);

/* Struct to contain node information */
struct treeNode;
typedef treeNode *treePtr;
struct treeNode {
  short choice;
  short roters[numberOfRoters];
  treePtr choices[numberOfButtons]; // Pointers to children
};

/* Struct to contain node information */
struct listNode;
typedef listNode *listPtr;
struct listNode {
  short branch;
  listPtr nxt; // Pointer to next node
};

struct effects{
  short change[numberOfRoters][numberOfButtons]; // 0 1 or -1 for each roter for each button
};

//combine string and short and string
string build(string sA, short sB, string sC) {
  if(sB != -1) { //none of the values will ever be -1 so use it as a null value
    sA.append(to_string(sB));
  }
  sA.append(sC);
  return sA;
}

//custom input function
#define getInput(output,input) cout << "| " << output << " >"; cin>>input

//MAIN FUNCTIONS
void displayPuzzle(short roters[numberOfRoters], effects &effects);
void dsiplayMenue (short roters[numberOfRoters], effects effects);
void solve        (short roters[numberOfRoters], effects effects);
void editRoters   (short (&roters)[numberOfRoters]);
void editEffects  (effects &effects);
void pressButton  (short (&roters)[numberOfRoters], effects effects);

//custom graphical functions (this could be a class probably)
const short display_Width = 60;
void printMultiString(string strArray[],short sizeOfStrArray);
void printLong       (string longString);
void displayMenue    (string title, string discription, string options[],short sizeofOptions);
void printLine       (char weight);
void titleLine       (string str, char weight);
void LeftString      (string str);
void CenterString    (string str);

//LIST FUNCTIONS (this could be a class probably)
bool empty            (listPtr start_ptr); //checks if stack is empty
void Display_List     (listPtr start_ptr); //displays whole list
void createlistNode   (listPtr &temp1, short branch);
void add_start_node   (listPtr &start_ptr, listPtr temp1);
void delete_start_node(listPtr &start_ptr); //
void purge_List       (listPtr &start_ptr); // deletes entire stack

//TREE FUNCTIONS (this could be a class probably)
void displayTree      (treePtr root, short currentdepth);  //recurcivly displays entire tree
bool createTreeNode   (treePtr &current, short roters[numberOfRoters], short branch, effects effects);
short populateDecendance(treePtr current, effects effects, listPtr &start_ptr);
short populateNextLevel (treePtr root,    effects effects, listPtr &start_ptr);
bool checkRoters      (short roters[numberOfRoters]);
void purge_Tree       (treePtr &root); // recurcivlry delets all of the nodes untill the tree is gone

//ROTER FUNCTIONS (this could be a class probably)
void scrambleRoters   (short (&roters)[numberOfRoters], effects effects);
void button(short       (&roters)[numberOfRoters], effects effects, short choice);
void randomiseEffects (effects &effects);
void displayRoters    (short roters[numberOfRoters], short y);
void displayEffects   (effects &effects, short x, short y);

//keeps track of count (compleatly unesisary)
short Stack_Count = 0;








//MAIN FUNCTIONS

int main(){
  srand(time(NULL));

  effects effects;
  memset(effects.change,0,sizeof(effects.change));
  //removes uninitialized data

  short roters[numberOfRoters];
  memset(roters,0,sizeof(roters));
  //removes uninitialized data


  //menue settup
  string options[]= {"1 edit effects","2 edit roters","3 Randomise effects","4 Scramble puzzle","5 Press Button","6 Solve Puzzle","9 Quit"};
  string discription = "Configure and solve the roter puzzle! (be sure not to close the program while solving and only close it when it says its safe to close)";

  displayMenue("Puzzle Solver",discription,options,sizeof(options));

  displayPuzzle(roters,effects);

  bool running = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      switch (menue) {
        case '1': editEffects(effects);           break;
        case '2': editRoters(roters);             break;
        case '3': randomiseEffects(effects);      break;
        case '4': scrambleRoters(roters,effects); break;
        case '5': pressButton(roters,effects);    break;
        case '6': solve(roters,effects);          break;
        case '9': running = false;                break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key); // << shows
      }
      displayMenue("Main Menue",discription,options,sizeof(options));
      displayPuzzle(roters,effects);
    }
  }while(running);
  CenterString("Bye!");
  printLine('#');
  return 0;
}

/* displays a menue with options and controlls */
void displayMenue(string title, string discription, string options[],short sizeofOptions){
  printLine('=');
  CenterString(title);
  printLine('-');
  printLong(discription);
  printLine('-');
  printMultiString(options,sizeofOptions);
}

void editRoters(short (&roters)[numberOfRoters]){
  bool edditing = true;
  short y = 0;

  //menue settup
  string options[]= {"W move up","S move down","A decrement","D increment","9 quit"};
  string discription = "Edditing roters, use the W and S keys to move selection up and down and A and D to increment and decrement the state";

  displayMenue("Roter edditor",discription,options,sizeof(options));
  displayRoters(roters,y);

  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      menue = tolower(menue);
      switch (menue) {
        case 'w':
        y--;
        y %= numberOfRoters;
        if(y < 0){
          y = numberOfRoters - 1;
        }
        break;

        case 'a':
        roters[y]--;
        if(roters[y] < 0){
          roters[y] = numberOfStates - 1;
        }
        roters[y] %= numberOfStates;
        break;

        case 's':
        y++;
        y %= numberOfRoters;
        break;

        case 'd':
        roters[y]++;
        roters[y] %= numberOfStates;
        break;

        case '9': edditing = false; break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key);
      }
      displayMenue("Roter edditor",discription,options,sizeof(options));
      displayRoters(roters,y);
    }
  }while(edditing);
  cout << string(100,'\n');
}

void editEffects (effects &effects){
  bool edditing = true;
  short x = 0, y = 0;

  //menue settup
  string options[]= {"1 [0]"," 2 [1]"," 3 [-1]","W move up","A move left","S move down","D move right","9 quit"};
  string discription = "Edditing Effects, use the W and S keys to move selection up and down, and A and D to move selection left and right";

  displayMenue("Roter edditor",discription,options,sizeof(options));
  displayEffects(effects,x,y);

  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      menue = tolower(menue);
      switch (menue) {
        case 'w':
        y--;
        y %= numberOfRoters;
        if(y < 0){
          y = numberOfRoters - 1;
        }
        break;

        case 'a':
        x--;
        x %= numberOfButtons;
        if(x < 0){
          x = numberOfButtons - 1;
        }
        break;

        case 's':
        y++;
        y %= numberOfRoters;
        break;

        case 'd':
        x++;
        x %= numberOfButtons;
        break;

        case '1': effects.change[y][x] = 0; break;
        case '2': effects.change[y][x] = 1; break;
        case '3': effects.change[y][x] = -1; break;
        case '9': edditing = false; break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key); // << shows
      }
      displayMenue("Roter edditor",discription,options,sizeof(options));
      displayEffects(effects,x,y);
    }
  }while(edditing);
  cout << string(100,'\n');
}

void pressButton (short (&roters)[numberOfRoters], effects effects){

  //menue settup
  string discription = "Manually solve";
  string options[]= {"use numbers for buttons","Esc to exit"};

  displayMenue("Button presser",discription,options,sizeof(options));
  displayPuzzle(roters,effects);

  //15 lookup characters
  char table[16] = {'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

  bool solving = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      printLine('#');
      printLine(' ');
      char menue = GetChar();
      menue = tolower(menue);
      if(menue == KEY_ESCAPE){
        return;
      }
      for(short i = 0; i < 16; i++){
        if((menue == table[i]) && i < numberOfButtons){
          button(roters,effects,i);
        }
      }
      //print menue
      displayMenue("Button presser",discription,options,sizeof(options));
      displayPuzzle(roters,effects);
    }
  }while(solving);
  cout << string(100,'\n');
}

void solve(short roters[numberOfRoters],effects effects){
  if(checkRoters(roters)){
    CenterString("wat, the puzzle solved itself...");
  }
  else{
    treePtr root = NULL;
    createTreeNode(root,roters,-1,effects);//set the initial conditions for root node and check if its alreaddy solved
    listPtr start_ptr = NULL;
    Stack_Count = 0;
    short toList;
    do{
      toList = populateNextLevel(root, effects, start_ptr);
    }while(toList == -1);
    //solved
    Display_List(start_ptr);

    purge_List(start_ptr);
    purge_Tree(root);

    string outString;
    outString.append("* the Tree is Purged! *");
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
    CenterString("it is now save to close");
  }
}






// LIST FUNCTIONS

/* returns boolian value */
bool empty(listPtr start_ptr){
  return (start_ptr == NULL);
}

/* Displays contents of current node that temp1 is pointing at */
void displayInfo(listPtr output){
  //initialise output
  string outString;
  //build output
  outString.append("branch ");
  outString.append(to_string(output ->branch + 1));
  //display output
  CenterString(outString);
}

void displayCount(){
  CenterString(build("Number of Steps in Solution ",Stack_Count,""));
}

//title information for item of list
void displayHeadder(){
  printLine('=');
  CenterString("solution Stack");
  //initialise output
  string outString;
  //build output
  //display output
  CenterString(outString);
}

/* will display each node untill it reaches the end */
void Display_List(listPtr start_ptr){
  listPtr temp1;
  if(empty(start_ptr)){
    string outString;
    outString.append("* the Stack is empty! *");
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
  }
  else{
    displayHeadder();
    displayCount();
    temp1 = start_ptr;
    do {
      // Display details for what temp points to
      displayInfo(temp1);
      // Move to next node (if present)
      temp1 = temp1 ->nxt;
    } while (temp1 != NULL);
  }
}

void CopyNodeInfo(listPtr &destination, listPtr Source){
  destination = new listNode; //make new node
  *destination = *Source;  //store information being passed to function
  destination ->nxt = NULL;
}

/* gets information for ans sets up pointers for new node */
void createlistNode(listPtr &temp1, short branch){
  temp1 = new listNode;
  temp1 ->branch = branch;
  temp1 ->nxt = NULL;
}

/* creates a node and if empty, point the start pointer to it,
else, point it to where the start pointer is pointing to
then points the start pointer to it (insert at front)      */
void add_start_node(listPtr &start_ptr, listPtr temp1){
  if(empty(start_ptr)){
    start_ptr = temp1;
  }
  else{
    temp1 ->nxt = start_ptr;
    start_ptr = temp1;
  }
}

/* sets temp node to start pointer then makes the start pointer
point to the next node then deletes node wtih temp           */
void delete_start_node(listPtr &start_ptr){
  if(!empty(start_ptr)){
    listPtr temp1;
    temp1 = start_ptr;
    start_ptr = start_ptr ->nxt;
    delete temp1;
  }
}

/* if not empty will remove the first element untill it is empty */
void purge_List(listPtr &start_ptr){
  if(empty(start_ptr)){
    string outString;
    outString.append("* the Stack is alreaddy empty! *");
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
  }
  else{
    string outString;
    outString.append("* the Stack is purged! *");
    do{
      delete_start_node(start_ptr);
    }while(!empty(start_ptr));
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
  }
}




















// TREE FUCNTIONS

void displayNode(treePtr node,short currentdepth){
  string outString;
  outString.push_back('>');
  outString.append(string(currentdepth * 2,' '));
  outString.push_back(' ');
  if(node != NULL){
    outString.append(to_string(node ->choice + 1));
  }
  else{
    outString.push_back('E');
    cout << "boop";
  }
  LeftString(outString);
}

void displayTree(treePtr root, short currentdepth){
  if(root != NULL){
    displayNode(root,currentdepth);
    currentdepth++;
    if(root->choices[0] != NULL){
      for(short i =0; i < numberOfButtons; i++){
        displayTree(root->choices[i],currentdepth);
      }
    }
  }
}


short populateNextLevel(treePtr root, effects effects, listPtr &start_ptr){
  if(root -> choices[0] != NULL){ //checks for children nodes
    treePtr temp1;
    short toList;
    for(short i =0; i < numberOfButtons; i++){
      temp1 = root->choices[i];
      toList = populateNextLevel(temp1,effects,start_ptr);
      if(toList != -1){
        //solved
        listPtr listtemp1;
        createlistNode(listtemp1,i);
        add_start_node(start_ptr,listtemp1);
        Stack_Count++;
        return toList;
      }
    }
    return toList;
  }
  else{
    return populateDecendance(root,effects,start_ptr);
  }
}

short populateDecendance(treePtr current, effects effects, listPtr &start_ptr){
  for(short i = 0; i < numberOfButtons; i++){
    if(createTreeNode(current ->choices[i],current->roters,i,effects)){
      listPtr listtemp1;
      createlistNode(listtemp1,i);
      add_start_node(start_ptr,listtemp1);
      Stack_Count++;
      printLine('#');
      printLine(' ');
      CenterString("Found Solution!!!! :D");
      return(i);
    }
  }
  return -1;
}

bool createTreeNode(treePtr &current, short roters[numberOfRoters], short branch, effects effects){
  current = new treeNode;
  current ->choice = branch;
  //copy the roters from parrent
  for(short i = 0; i < numberOfRoters; i++){
    current ->roters[i] = roters[i];
  }
  if(branch != -1){ //dont apply button affects to root
    button(current ->roters,effects,branch); //aply button effects
    if(checkRoters(current ->roters)){
      return true; //checks if all roters are 0 if so solution is found
    }
  }
  //asign children pointers NULL
  for(short i = 0; i < numberOfButtons; i++){
    current ->choices[i] = NULL;
  }
  return false;
}

void purge_Tree(treePtr &root){
  if(root != NULL){ //check to see if tree or node is empty
    for(short i = 0; i < numberOfButtons; i++){//goes to each decendant node and calls this function of it as if it where root
      purge_Tree(root->choices[i]); //recursion
    }
    //when node has no children, it is safe to delete, even though it is still called root
    delete root;
    root = NULL;
  }
}







// ROTER FUNCTIONS

bool checkRoters(short roters[numberOfRoters]){
  return(!(roters[0] || roters[1] || roters[2] || roters[3]));
}

void sleep(short delay){
  this_thread::sleep_for(chrono::milliseconds(delay));
}

void randomiseEffects(effects &effects){
  for(short i = 0; i < numberOfRoters; i++){
    for(short j = 0; j <numberOfButtons; j++){
      effects.change[i][j] = rand() % 3 - 1;
    }
  }
}

void displayPuzzle(short roters[numberOfRoters], effects &effects){
  printLine('=');
  printLine(' ');

  //x axis label
  string xAxis = "   Button ";
  for(short j = 0; j < numberOfButtons; j++){
    xAxis.push_back('[');
    xAxis.append(to_string(j + 1));
    xAxis.push_back(']');
  }

  xAxis.append("  State ");

  for(short j = 0; j < numberOfStates; j++){
    xAxis.append(to_string(j + 1));
    if(j != numberOfStates - 1){
      xAxis.push_back(' ');
    }
  }

  LeftString(xAxis);

  // For each roter...
  for(short i = 0; i < numberOfRoters; i++){
    //start string
    string outString;

    //y axis label
    //button effects
    outString.append("Roter (");
    outString.append(to_string(i + 1));
    outString.append(") ");


    //button effects
    for(short j = 0; j < numberOfButtons; j++){
      if(effects.change[i][j] >= 0){
        outString.push_back(' ');
      }
      outString.append(to_string(effects.change[i][j]));
      outString.push_back(' ');
    }

    //seperation
    outString.append("      ");

    //state of roter
    outString.append(to_string(roters[i]));
    outString.push_back(' ');
    for(short j = 0; j < numberOfStates; j++){
      if(roters[i] == j){
        outString.push_back('#');
      }
      else{
        outString.push_back(' ');
      }
      if(j != numberOfStates - 1){
        outString.push_back('|');
      }
    }
    LeftString(outString);
  }
}

void displayEffects(effects &effects, short x, short y){
  printLine('=');
  printLine(' ');

  //x axis label
  string xAxis = "   Button  ";
  for(short j = 0; j < numberOfButtons; j++){
    xAxis.push_back('[');
    xAxis.append(to_string(j + 1));
    xAxis.push_back(']');
    xAxis.push_back(' ');
  }

  LeftString(xAxis);

  for(short i = 0; i < numberOfRoters; i++){

    //start string
    string outString;

    //y axis label
    outString.append("Roter (");
    outString.append(to_string(i + 1));
    outString.append(") ");

    //button effects
    for(short j = 0; j < numberOfButtons; j++){
      if(effects.change[i][j] >= 0){
        outString.push_back(' ');
      }
      if((x == j) && (y == i)){
        outString.push_back('[');
      }
      else{
        outString.push_back(' ');
      }
      outString.append(to_string(effects.change[i][j]));
      if((x == j) && (y == i)){
        outString.push_back(']');
      }
      else{
        outString.push_back(' ');
      }
    }
    LeftString(outString);
  }
}


void displayRoters(short roters[numberOfRoters], short y){
  printLine('=');
  printLine(' ');
  for(short i = 0; i < numberOfRoters; i++){
    string output;
    output.append(to_string(roters[i]));
    if(i == y){
      output.append("[");
    }
    else{
      output.append(" ");
    }
    for(short j = 0; j < numberOfStates; j++){
      if(roters[i] == j){
        output.push_back('#');
      }
      else{
        output.push_back(' ');
      }
      if(j != numberOfStates - 1){
        output.push_back('|');
      }
    }
    if(i == y){
      output.append("]");
    }
    else{
      output.append(" ");
    }
    CenterString(output);
  }
}

void button(short (&roters)[numberOfRoters], effects effects,short choice){
  for(short i = 0; i < numberOfRoters; i++){
    roters[i] += effects.change[i][choice];
    if(roters[i] < 0){
      roters[i] = numberOfStates - 1;
    }
    roters[i] %= numberOfStates;
  }
}

void scrambleRoters(short (&roters)[numberOfRoters], effects effects){
  //resets values to 0
  memset(roters,0,sizeof(roters));
  for(short i = 0; i < numberOfRoters; i++){
    for(short j = 0; j < scrambleamount; j++){
      button(roters,effects,rand() % numberOfStates);
    }
  }
}












//GRAPHICAL FUNCTIONS THEY DO NOOOTHINNGGGGG

/* Takes a long string and prints it to multiple lines while word wrapping */
void printLong(string longString){
  short pos = 0;
  bool writing = true;
  do{
    string outString;
    short wordlength = 0;
    for(short i = 0; i < display_Width - 4; i++){
      if(pos >= longString.length()){
        writing = false;
      }
      else{
        outString.push_back(longString[pos]);
        if(longString[pos] == ' '){
          wordlength = 0;
        }
        else{
          wordlength++;
        }
        pos++;
      }
    }
    if((writing) && (wordlength > 0)){
      pos -= wordlength;
      for(short j = 0; j < wordlength; j++){
        outString.pop_back();
      }
    }
    LeftString(outString);
  }while(writing);
}

void printMultiString(string strArray[],short sizeOfStrArray){
  short max = 0;
  for(short i = 0; i < sizeOfStrArray/sizeof(string); i++){
    if(strArray[i].length() > max){
      max = strArray[i].length();
    }
  }
  //if the array is too big
  if((max + 5) * sizeOfStrArray/sizeof(string) > display_Width){
    short amountThatFits = 0;
    for(short i = (sizeOfStrArray/sizeof(string)) - 1; i > 0; i--){
      if((max + 5) * i > display_Width){
        continue;
      }
      amountThatFits = i;
      break;
    }
    //print what fits
    string outString;
    for(short j = 0; j < amountThatFits; j++){
      outString.append("  ");
      outString.append(strArray[j]);
      outString.append(string(max - strArray[j].length(),' '));
      outString.append("  ");
    }
    LeftString(outString);
    //truncate array for recurcive call
    short SizeOfNewStrArray = (sizeOfStrArray/sizeof(string)) - amountThatFits;
    string newStrArray[SizeOfNewStrArray];
    for(short j = 0; j < SizeOfNewStrArray; j++){
      newStrArray[j] = strArray[j + amountThatFits];
    }
    //recurcive call
    printMultiString(newStrArray,SizeOfNewStrArray * sizeof(string));
  }
  //print array of strings
  else{
    string outString;
    for(short j = 0; j < sizeOfStrArray/sizeof(string); j++){
      outString.append("  ");
      outString.append(strArray[j]);
      outString.append(string(max - strArray[j].length(),' '));
      outString.append("  ");
    }
    LeftString(outString);
  }
}



/* prints a chatacter string across the display */
void printLine(char weight){
  cout << "|";
  cout << string(display_Width + 2, weight);
  cout << "|" << endl;
}

/* prints a title then a character string across the display */
void titleLine(string str, char weight){
  cout << "| ";
  short str_length = str.length();
  cout << str << " ";
  cout << string(display_Width - str_length, weight);
  cout << "|" << endl;
}

/* contains a message within the display boundries centered */
void LeftString(string str){
  cout << "| ";
  short str_length = str.length();
  cout << str;
  cout << string(display_Width - (str_length - 1),' ');
  cout << "|" << endl;
}

/* contains a message within the display boundries centered */
void CenterString(string str){
  cout << "| ";
  short str_length = str.length() / 2;
  if(display_Width % 2 != 0)
  cout << " ";
  if(str.length() % 2)
  cout << string(display_Width / 2 - str_length,' ');
  else
  cout << string(display_Width / 2 - str_length + 1,' ');
  cout << str;
  cout << string(display_Width / 2 - str_length,' ');
  cout << "|" << endl;
}
