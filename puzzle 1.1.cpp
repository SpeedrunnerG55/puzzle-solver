#include <iostream>
#include <iomanip>

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
const short scrambleamount  = 20;
const short numberOfRoters  = 4;
const short numberOfStates  = 8;
const short numberOfButtons = 8;
const short threshhold = (numberOfStates / 1.5) *  numberOfRoters;


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

//sleep function
void sleep(int delay);

//MAIN FUNCTIONS
void displayPuzzle(short roters[numberOfRoters], effects &effects);
void dsiplayMenue (short roters[numberOfRoters], effects effects);
void solve        (short roters[numberOfRoters], effects effects);
void editRoters   (short (&roters)[numberOfRoters]);
void editEffects  (effects &effects);
void pressButton  (short (&roters)[numberOfRoters], effects effects);

//custom graphical functions (this could be a class probably)
const short display_Width = 70;
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

//ROTER FUNCTIONS (this could be a class probably)
void scrambleRoters   (short (&roters)[numberOfRoters], effects effects);
void button           (short (&roters)[numberOfRoters], effects effects, short choice);
void randomiseEffects (short (&roters)[numberOfRoters], effects &effects);
void displayRoters    (short roters[numberOfRoters], short y);
void displayEffects   (effects &effects, short x, short y);
short checkRoters     (short roters[numberOfRoters]);

//keeps track of count (compleatly unesisary)
short Stack_Count = 0;

//keeps track of how manny nodes are made
int level = 0;


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
        case '1': editEffects(effects);             break;
        case '2': editRoters(roters);               break;
        case '3': randomiseEffects(roters,effects); break;
        case '4': scrambleRoters(roters,effects);   break;
        case '5': pressButton(roters,effects);      break;
        case '6': solve(roters,effects);            break;
        case '9': running = false;                  break;
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

void increment(short (&value)[numberOfButtons], short base[numberOfButtons]){
  value[0]++;
  for(int i = 0; i < numberOfButtons; i++){
    if(i != numberOfButtons -1){
      if(value[i] >= base[i]){
        value[i] = 0;
        value[i+1]++;
      }
    }
  }
}

bool arrayComp(short arg1[numberOfButtons], short arg2[numberOfButtons]){
  for(unsigned int i = 0; i < numberOfButtons;i++){
    if(arg1[i] < arg2[i] - 1){
      return true;
      break;
    }
  }
  return false;
}

void display2dArrayByButton(short arg[numberOfButtons][numberOfRoters], short y, short x){
  for(short i = 0; i < numberOfButtons; i++){
    string outstring;
    for(short j = 0; j < numberOfRoters; j++){
      if(!(arg[i][j] < 0)){
        outstring.push_back(' ');
      }
      if((i == y) && (j == x)){
        outstring.push_back('[');
      }
      else{
        outstring.push_back(' ');
      }
      outstring.append(to_string(arg[i][j]));
      if(j < numberOfRoters - 1){
        outstring.push_back(',');
      }
      if((i == y) && (j == x)){
        outstring.push_back(']');
      }
      else{
        outstring.push_back(' ');
      }
    }
    CenterString(outstring);
  }
  printLine('-');
}

void display2dArrayByRoter(short arg[numberOfRoters][numberOfButtons]){
  for(int i = 0; i < numberOfRoters; i++){
    string outstring;
    for(int j = 0; j < numberOfButtons; j++){
      if(!(arg[i][j] < 0)){
        outstring.push_back(' ');
      }
      outstring.append(to_string(arg[i][j]));
      outstring.push_back(',');
    }
    CenterString(outstring);
  }
  printLine('-');
}

void display2dArrayByRoter(bool arg[numberOfRoters][numberOfButtons]){
  for(int i = 0; i < numberOfRoters; i++){
    string outstring;
    for(int j = 0; j < numberOfButtons; j++){
      if(!(arg[i][j] < 0)){
        outstring.push_back(' ');
      }
      outstring.append(to_string(arg[i][j]));
      outstring.push_back(',');
    }
    CenterString(outstring);
  }
  printLine('-');
}

void solve(short roters[numberOfRoters],effects effects){
  printLine('#');
  printLine(' ');
  CenterString("Solve");
  printLine(' ');
  printLine('=');
  printLine(' ');
  CenterString("Settup Procedure...");

  //find out which buttons effects each roter
  printLong("find out which buttons effects each roter and save it to doesbuttoneffectroter[numberOfRoters][numberOfButtons] V ");

  bool doesbuttoneffectroter[numberOfRoters][numberOfButtons];
  memset(doesbuttoneffectroter,false,sizeof(doesbuttoneffectroter));
  for(int i = 0; i < numberOfRoters; i++){
    for(int j = 0; j < numberOfButtons; j++){
      if(effects.change[i][j] != 0){
        doesbuttoneffectroter[i][j] = true;
      }
      else{
        doesbuttoneffectroter[i][j] = false;
      }
    }
  }

  display2dArrayByRoter(doesbuttoneffectroter);

  //calculate the number of presses it takes to return the rotors to its original state.
  printLong("calculate the number of presses it takes to return the rotors to its original state and save it to getback[numberOfRoters][numberOfButtons] V (this value is not used)");

  short getback[numberOfRoters][numberOfButtons];
  for(int i = 0; i < numberOfRoters; i++){
    for(int j = 0; j < numberOfButtons; j++){
      if(doesbuttoneffectroter[i][j]){
        getback[i][j] = numberOfStates;
      }
      else{
        getback[i][j] = -1;
      }
    }
  }

  display2dArrayByRoter(getback);

  // find the amount of times each button must be pressed to turn each rotor to 0
  printLong("find the amount of times each button must be pressed to turn each rotor to 0 and save it to numberOfPresses[numberOfRoters][numberOfButtons] V");

  short numberOfPresses[numberOfRoters][numberOfButtons];
  for(int i = 0; i < numberOfRoters; i++){
    for(int j = 0; j < numberOfButtons; j++){
      if(effects.change[i][j] > 0){
        if(floor((numberOfStates - roters[i]) / effects.change[i][j]) == (numberOfStates - roters[i]) / effects.change[i][j]){
          numberOfPresses[i][j] = (numberOfStates - roters[i]) / effects.change[i][j];
        }
        else{
          numberOfPresses[i][j] = -1;
        }
      }
      else if(effects.change[i][j] < 0){
        if(floor(roters[i] / abs(effects.change[i][j])) == roters[i] / abs(effects.change[i][j])){
          numberOfPresses[i][j] = roters[i] / abs(effects.change[i][j]);
        }
        else{
          numberOfPresses[i][j] = -1;
        }
      }
      else{
        numberOfPresses[i][j] = -1;
      }
    }
  }

  display2dArrayByRoter(numberOfPresses);

  printLong("declare base[numberOfButtons] and set all of its values to 0");

  short base[numberOfButtons];
  memset(base,0,sizeof(base));

  printLong("declare amount[numberOfButtons][numberOfRoters] V and set all of its values to -1");

  short amount[numberOfButtons][numberOfRoters];
  memset(amount,-1,sizeof(amount));

  display2dArrayByButton(amount,-1,-1);

  printLong("remove douplicates and leading -1's while counting the amount each one has putting result into amount[numberOfButtons][numberOfRoters] V");

  //remove douplicates and leading -1's while counting the amount each one has putting result into amount[numberOfButtons][numberOfRoters]
  for(int j = 0; j < numberOfButtons; j++){
    cout << " button  [" << j + 1 << "] ";
    short check[numberOfRoters];
    memset(check,-1,sizeof(check));
    for(int i = 0; i < numberOfRoters; i++){
      if(numberOfPresses[i][j] != -1){
        check[i] = numberOfPresses[i][j];
        bool duplicate = false;
        for(int k = 0; k < i; k++){
          if(numberOfPresses[i][j] == check[k]){
            numberOfPresses[i][j] = -1;
            duplicate = true;
            break;
          }
        }
        if(!duplicate){
          cout << " " << numberOfPresses[i][j] << ",";
          amount[j][base[j]] = numberOfPresses[i][j];
          base[j]++;
        }
        else{
          cout << "   ";
        }
      }
      else{
        cout << "   ";
      }
    }
    cout << " ... base[" << j << "] = " << base[j] << endl;
  }

  display2dArrayByButton(amount,-1,-1);

  short current[numberOfButtons];
  memset(current,0,sizeof(current));

  int solutionsfound = 0;

  printLine(' ');
  printLine('=');
  printLine(' ');
  CenterString("Looping Procedure...");

  //really just a big forloop
  while(arrayComp(current,base)){


    //get a copy
    short tempRoters[numberOfRoters];
    for(int i = 0; i < numberOfRoters; i++){
      tempRoters[i] = roters[i];
    }

    //do the buttons cor current configuration index
    for(int i = 0; i < numberOfButtons; i++){

      printLine('=');
      CenterString("get copy of roters into tempRoters[numberOfRoters] V");
      displayRoters(roters,-1);

      string outstring;
      printLine('-');
      outstring.append("index values...");
      LeftString(outstring); outstring.erase();
      outstring.append("y axis    | i = "); outstring.append(to_string(i));
      LeftString(outstring); outstring.erase();
      outstring.append("x axis    | current["); outstring.append(to_string(i)); outstring.append("] = "); outstring.append(to_string(current[i]));
      LeftString(outstring); outstring.erase();
      printLine(' ');
      outstring.append("normal values...");
      LeftString(outstring); outstring.erase();
      outstring.append("x axis limmit| base["); outstring.append(to_string(i)); outstring.append("] = "); outstring.append(to_string(base[i]));
      LeftString(outstring); outstring.erase();
      printLine('-');

      //print array of amounts so i can see whats going on
      display2dArrayByButton(amount,i,current[i]);

      outstring.append("press |"); outstring.append(to_string(i + 1)); outstring.append("| amount["); outstring.append(to_string(i)); outstring.append("]] ^ times ");
      LeftString(outstring); outstring.erase();
      for(int j = 0; j < amount[i][current[i]]; j++){
        outstring.append("[press] ");
        button(tempRoters,effects,i);
      }
      printLong(outstring); outstring.erase();
      displayRoters(tempRoters,-1);
      sleep(20);
      cout << string(100,'\n');
    }
    printLine('=');
    CenterString("Check Roters");
    if(checkRoters(tempRoters) == 0){
      solutionsfound++;
      CenterString("FOUND ONE! :D");
    }
    string outstring;
    outstring.append("found "); outstring.append(to_string(solutionsfound));
    LeftString(outstring);
    increment(current,base);
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




// ROTER FUNCTIONS

short checkRoters(short roters[numberOfRoters]){
  short sum = 0;
  for(short i = 0; i < numberOfRoters; i++){
    sum +=(numberOfStates/2) - abs((numberOfStates/2) - roters[i]);
  }
  return(sum);
}

void sleep(int delay){
  this_thread::sleep_for(chrono::milliseconds(delay));
}

void randomiseEffects(short (&roters)[numberOfRoters],effects &effects){
  //clear any previous rotter settup because not doing so may create an unsolvable puzzle
  memset(roters,0,sizeof(roters));

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
      button(roters,effects,rand() % numberOfButtons);
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
