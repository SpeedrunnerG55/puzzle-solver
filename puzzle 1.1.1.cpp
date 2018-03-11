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

const short delay = 50;
const short scrambleamount  = 20;
const short numberOfRoters  = 4;
const short numberOfStates  = 8;
const short numberOfButtons = 8;
const short threshhold = (numberOfStates / 1.5) *  numberOfRoters;


/* Struct to contain node information */
struct listNode;
typedef listNode *listPtr;
struct listNode {
  short solution[numberOfButtons];
  listPtr nxt; // Pointer to next node
};

struct effects{
  short change[numberOfRoters][numberOfButtons]; // 0 1 or -1 for each roter for each button
};

//misc function
void sleep(int delay);
char lookup(int arg);

void incrementArray(short (&value)[numberOfButtons], short base[numberOfButtons]);

//MAIN FUNCTIONS
void displayPuzzle(short roters[numberOfRoters], effects &effects);
void dsiplayMenue (short roters[numberOfRoters], effects effects);
void solve        (short roters[numberOfRoters], effects effects);
void editRoters   (short roters[numberOfRoters]);
void editEffects  (effects &effects);
void pressButton  (short roters[numberOfRoters], effects effects);

///SUB MAIN
void simulation();
void manual();

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
void Display_List     (listPtr start_ptr, int count); //displays whole list
void Display_Smallest (listPtr start_ptr);  //displays node with smallest value
void createlistNode   (listPtr &temp1, short solution[numberOfButtons]);
void add_start_node   (listPtr &start_ptr, listPtr temp1);
void delete_start_node(listPtr &start_ptr); //
void purge_List       (listPtr &start_ptr); // deletes entire stack

//ROTER FUNCTIONS (this could be a class probably)
void scrambleRoters   (short roters[numberOfRoters], effects effects);
void button           (short roters[numberOfRoters], effects effects, short choice);
void randomiseEffects (short roters[numberOfRoters], effects &effects);
void displayRoters    (short roters[numberOfRoters], short y);
void displayEffects   (effects &effects, short x, short y);
short checkRoters     (short roters[numberOfRoters]);
void resetEffects     (effects &effects);
void resetRoters      (short roters[numberOfRoters]);


/*  MAIN  */
int main(){

  //menue settup
  string options[]= {"1 manual solve","2 simulation","9 Quit"};
  string discription = "Solve the roter puzzle!";

  displayMenue("Puzzle Solver",discription,options,sizeof(options));


  bool running = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      switch (menue) {
        case '1': manual();        break;
        case '2': simulation();    break;
        case '9': running = false; break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key); // << shows
      }
      displayMenue("Puzzle Solver",discription,options,sizeof(options));
    }
  }while(running);
  CenterString("Bye!");
  printLine('#');
  return 0;
}

/*  simulation menue to simulate the puzzle  */
void simulation(){

  srand(time(NULL));

  effects effects;
  memset(effects.change,0,sizeof(effects.change));
  //removes uninitialized data

  short roters[numberOfRoters];
  memset(roters,0,sizeof(roters));
  //removes uninitialized data

  //menue settup
  string options[]= {"1 edit effects","2 Randomise effects","3 Scramble puzzle","4 Press Button","5 auto solve","9 Quit"};
  string discription = "Simulate the puzzle right here in software! scramble and generate effects on the fly for you or me to solve (be sure not to close the program while solving and only close it when it says its safe to close)";
  string title = "Simulation";

  displayMenue(title,discription,options,sizeof(options));
  displayPuzzle(roters,effects);

  bool running = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      switch (menue) {
        case '1': editEffects(effects);             break;
        case '2': randomiseEffects(roters,effects); break;
        case '3': scrambleRoters(roters,effects);   break;
        case '4': pressButton(roters,effects);      break;
        case '5': solve(roters,effects);            break;
        case '9': running = false;                  break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key); // << shows
      }
      displayMenue(title,discription,options,sizeof(options));
      displayPuzzle(roters,effects);
    }
  }while(running);
  cout << string(100,'\n');
}

/*  manual, used for solving the actual puzzle  */
void manual(){
  srand(time(NULL));

  effects effects;
  memset(effects.change,0,sizeof(effects.change));
  //removes uninitialized data

  short roters[numberOfRoters];
  memset(roters,0,sizeof(roters));
  //removes uninitialized data

  //instructions
  printLine('=');
  CenterString("Press each button and record its affects on each roter");
  editEffects(effects);
  printLine('=');
  CenterString("Record the state of the roters");
  editRoters(roters);

  //menue settup
  string options[]= {"1 Press Button","2 auto solve","3 reset puzzle","9 Quit"};
  string discription = "Configure and solve the roter puzzle! (be sure not to close the program while solving and only close it when it says its safe to close)";
  string title = "Puzzle Solver";

  displayMenue(title,discription,options,sizeof(options));
  displayPuzzle(roters,effects);

  bool running = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      char menue = GetChar();
      switch (menue) {
        case '1': pressButton(roters,effects); break;
        case '2': solve(roters,effects);       break;
        case '3':
        resetEffects(effects);
        printLine('=');
        CenterString("Press each button and record its affects on each roter");
        editEffects(effects);
        resetRoters(roters);
        printLine('=');
        CenterString("Record the state of the roters");
        editRoters(roters);
        break;
        case '9': running = false;             break;
        default:
        printLine('#');
        printLine(' ');
        CenterString("invalid input");
        string key;
        key.push_back(menue);
        CenterString(key); // << shows
      }
      displayMenue(title,discription,options,sizeof(options));
      displayPuzzle(roters,effects);
    }
  }while(running);
  cout << string(100,'\n');
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

/* edit the state of the roters manually  */
void editRoters(short roters[numberOfRoters]){
  bool edditing = true;
  short y = 0;

  //menue settup
  string options[]= {"W up","S down","A decrement","D increment","9 quit"};
  string discription = "Edditing roters, use the W and S keys to move selection up and down and A and D to increment and decrement the state";
  string title = "Roter edditor";

  displayMenue(title,discription,options,sizeof(options));
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
      displayMenue(title,discription,options,sizeof(options));
      displayRoters(roters,y);
    }
  }while(edditing);
  cout << string(100,'\n');
}

/* edit the state of the effects manually  */
void editEffects (effects &effects){
  bool edditing = true;
  short x = 0, y = 0;

  //menue settup
  string options[]= {"1 [0]"," 2 [1]"," 3 [-1]","W up","A left","S down","D right","9 quit"};
  string discription = "Edditing Effects, use the W and S keys to move selection up and down, and A and D to move selection left and right";
  string title = "Effects edditor";
  displayMenue(title,discription,options,sizeof(options));
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
      displayMenue(title,discription,options,sizeof(options));
      displayEffects(effects,x,y);
    }
  }while(edditing);
  cout << string(100,'\n');
}

/* press the buttons as if where the puzzle with the number keys  */
void pressButton (short roters[numberOfRoters], effects effects){

  //menue settup
  string discription = "Manually solve";
  string options[]= {"use numbers for buttons","Esc to exit"};
  string title = "Button presser";

  displayMenue(title,discription,options,sizeof(options));
  displayPuzzle(roters,effects);

  bool solving = true;
  do{
    if(KeyHit()){
      cout << string(100,'\n');
      printLine('#');
      printLine(' ');
      char menue = GetChar();
      menue = toupper(menue);
      if(menue == KEY_ESCAPE){
        return;
      }
      for(short i = 0; i < 16; i++){
        if((menue == lookup(i)) && i < numberOfButtons){
          button(roters,effects,i);
        }
      }
      //print menue
      displayMenue(title,discription,options,sizeof(options));
      displayPuzzle(roters,effects);
    }
  }while(solving);
  cout << string(100,'\n');
}

/*  used to lookup character for input and output text if needed  */
char lookup(int arg){
  char table[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  return table[arg];
}

/*  tests the sequence of buttons and returns how close they where to the solution  */
short testSequence(short sequence[numberOfButtons], short roters[numberOfRoters], effects effects){
  //get a coppy
  short copy[numberOfRoters];
  for(int j = 0; j < numberOfRoters; j++){
    copy[j] = roters[j];
  }
  for(short i = 0; i < numberOfButtons; i++){
    for(short j = 0; j < sequence[i]; j++){
      button(copy,effects,i);
    }
  }
  return checkRoters(copy);
}

/*  increments array of values with arbitrary base values  */
void incrementArray(short (&value)[numberOfButtons], short base[numberOfButtons]){
  value[0]++;
  for(int i = 0; i < numberOfButtons; i++){
    if(value[i] >= base[i]){
      if(i != numberOfButtons -1){
        value[i] = 0;
        value[i+1]++;
      }
    }
    else{
      return;
    }
  }
}

/*  compares array 1 to 2 and returns if it is greater than it -1  */
bool arrayComp(short arg1[numberOfButtons], short arg2[numberOfButtons]){
  for(unsigned int i = 0; i < numberOfButtons;i++){
    if(arg1[i] < arg2[i] - 1){
      return true;
      break;
    }
  }
  return false;
}

/*  solves the puzzle displaying all and most efficient solution  */
void solve(short roters[numberOfRoters],effects effects){
  int startTime = time(NULL);
  //keeps track of count (compleatly unesisary)
  int stack_Count = 0;

  //list pointer
  listPtr start_ptr = NULL;

  //declare bases set them all to max to do exaustive search
  short base[numberOfButtons];
  memset(base,0,sizeof(base));
  //establish all non 0 bases makes it a tad faster in some cases
  for(int i = 0; i < numberOfRoters; i++){
    for(int j = 0; j < numberOfButtons; j++){
      if(effects.change[i][j] != 0){
        base[j] = numberOfStates;
      }
    }
  }

  //just a really big for loop
  //set counter to 0
  short current[numberOfButtons];
  memset(current,0,sizeof(current));
  CenterString("Building list, dont close untill finished");
  //begin loop
  do {
    //test sequence if it passes add  it to list
    if(testSequence(current,roters,effects) == 0){
      stack_Count++;
      listPtr temp1;
      createlistNode(temp1,current);
      add_start_node(start_ptr,temp1);
      //progress bar
      cout << "| found " << setw(6) << stack_Count;
      short bar = ((current[numberOfButtons - 1] * numberOfButtons + current[numberOfButtons - 2]) * 3/4);
      short total = ((numberOfButtons * numberOfButtons) * 3/4);
      cout << "[" << string(bar,'#');
      cout << string(total - bar,' ') << "]" << bar << "|" << total << '\r';
    }
    //increment counter
    incrementArray(current,base);
    //test counter
  } while(arrayComp(current,base));



  Display_List(start_ptr, stack_Count);
  Display_Smallest(start_ptr);

  purge_List(start_ptr);
  CenterString("execution time");
  CenterString(to_string(time(NULL)-startTime));
  CenterString("seconds");
  CenterString("it is now safe to close");
}


// LIST FUNCTIONS

/* returns boolian value */
bool empty(listPtr start_ptr){
  return (start_ptr == NULL);
}

/* Displays contents of current node that temp1 is pointing at */
void displayInfo(listPtr output){
  string outstring;
  outstring.append("[ ");
  for(int i = 0; i < numberOfButtons; i++){
    outstring.push_back(lookup(output ->solution[i]));
    if(i != numberOfButtons - 1){
      outstring.push_back(',');
    }
  }
  outstring.append(" ]");
  CenterString(outstring);
}

/* will display each node untill it reaches the end */
void Display_List(listPtr start_ptr, int count){
  if(empty(start_ptr)){
    string outString;
    outString.append("* the Stack is empty! *");
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
  }
  else{
    string outstring;
    CenterString("solution Stack");
    outstring.append("found ");
    outstring.append(to_string(count));
    outstring.append(" Solutions");
    CenterString(outstring);
    listPtr temp1 = start_ptr;
    do {
      // Display details for what temp points to
      displayInfo(temp1);
      // Move to next node (if present)
      temp1 = temp1 ->nxt;
    } while (temp1 != NULL);
  }
}

/*  returns the sum of the button pressed in solution node  */
short getSum(listPtr temp){
  short sum = 0;
  for(short i = 0; i < numberOfButtons; i ++){
    sum += temp ->solution[i];
  }
  return sum;
}

/*  will display each node untill it reaches the end  */
void Display_Smallest(listPtr start_ptr){
  if(empty(start_ptr)){
    string outString;
    outString.append("* the Stack is empty! *");
    CenterString(string(outString.length(),'*'));
    CenterString(outString);
    CenterString(string(outString.length(),'*'));
  }
  else{
    CenterString("Solution with least amount of presses");
    listPtr temp1 = start_ptr, temp2 = start_ptr;
    //get sum of first node and save it
    short min = getSum(temp2);
    while (temp1 ->nxt != NULL){
      // Move to next node (if present)
      temp1 = temp1 ->nxt;
      //get sum of next node
      short sum = getSum(temp1);
      // Check to see if the sum of next node it less than previous
      if(sum < min){
        //if it is set the
        min = sum;
        temp2 = temp1;
      }
    }
    displayInfo(temp2);
    CenterString(to_string(min));
    CenterString("with Presses");
  }
}


/*  gets information for ans sets up pointers for new node  */
void createlistNode(listPtr &temp1, short solution[numberOfButtons]){
  temp1 = new listNode;
  for(int i = 0; i < numberOfButtons; i++){
    temp1 ->solution[i] = solution[i];
  }
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

/*  returns sum of all deviation from 0  */
short checkRoters(short roters[numberOfRoters]){
  short sum = 0;
  for(short i = 0; i < numberOfRoters; i++){
    sum +=(numberOfStates/2) - abs((numberOfStates/2) - roters[i]);
  }
  return(sum);
}

/*  pauses execution for x milliseconds  */
void sleep(int delay){
  this_thread::sleep_for(chrono::milliseconds(delay));
}

/*  visually displays both effects and the roters  */
void displayPuzzle(short roters[numberOfRoters], effects &effects){
  printLine('=');
  printLine(' ');

  //X axis...
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

    //Y axis

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

/*  visually displays the effects with a selector */
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

/*  visually displays the roters with a selector  */
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

/*  uses effects to change the roter values as if a button was pressed */
void button(short roters[numberOfRoters], effects effects, short choice){
  for(short i = 0; i < numberOfRoters; i++){
    roters[i] += effects.change[i][choice];
    if(roters[i] < 0){
      roters[i] = numberOfStates - 1;
    }
    roters[i] %= numberOfStates;
  }
}

/*  randomises the effects and sets roters to 0   */
void randomiseEffects(short roters[numberOfRoters],effects &effects){
  //resets values to 0
  resetRoters(roters);
  for(short i = 0; i < numberOfRoters; i++){
    //clear any previous rotter settup because not doing so may create an unsolvable puzzle
    for(short j = 0; j <numberOfButtons; j++){
      printLine('=');
      CenterString("Randomising effects");
      displayEffects(effects,j,i);
      sleep(delay);
      cout << string(100,'\n');
      effects.change[i][j] = rand() % 3 - 1;
      printLine('=');
      CenterString("Randomising effects");
      displayEffects(effects,j,i);
      sleep(delay);
      cout << string(100,'\n');
    }
  }
}

/*  scrambles the Roters using the effects  */
void scrambleRoters(short roters[numberOfRoters], effects effects){
  //resets values to 0
  resetRoters(roters);
  for(short j = 0; j < scrambleamount; j++){
    printLine('=');
    CenterString("Scrambling Roters");
    displayRoters(roters,-1);
    sleep(delay);
    cout << string(100,'\n');
    button(roters,effects,rand() % numberOfButtons);
    printLine('=');
    CenterString("Scrambling Roters");
    displayRoters(roters,-1);
    sleep(delay);
    cout << string(100,'\n');
  }
}

/*  sets effects to 0  */
void resetEffects     (effects &effects){
  for(short i = 0; i < numberOfRoters; i++){
    for(short j = 0; j < numberOfButtons; j++){
      printLine('=');
      CenterString("Clearing Roters");
      displayEffects(effects,j,i);
      sleep(delay);
      cout << string(100,'\n');
      effects.change[i][j] = 0;
      printLine('=');
      CenterString("Clearing Roters");
      displayEffects(effects,j,i);
      sleep(delay);
      cout << string(100,'\n');
    }
  }
}

/*  sets roters to 0  */
void resetRoters      (short roters[numberOfRoters]){
  for(short i = 0; i < numberOfRoters; i++){
    printLine('=');
    CenterString("Clearing Roters");
    displayRoters(roters,i);
    sleep(delay);
    cout << string(100,'\n');
    roters[i] = 0;
    printLine('=');
    CenterString("Clearing Roters");
    displayRoters(roters,i);
    sleep(delay);
    cout << string(100,'\n');
  }
}


//GRAPHICAL FUNCTIONS THEY DO NOOOTHINNGGGGG

/* Takes a long string and prints it to multiple lines while word wrapping */
void printLong(string longString){
  unsigned short pos = 0;
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
  unsigned short max = 0;
  for(unsigned short i = 0; i < sizeOfStrArray/sizeof(string); i++){
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
    for(unsigned short j = 0; j < sizeOfStrArray/sizeof(string); j++){
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
