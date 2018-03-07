#include <iostream>


struct numberNode;
typedef numberNode *numPtr;
struct numberNode {
  int number;
};

using namespace std;


void swap(numPtr source, numPtr destination){
  numPtr temp = destination;
  destination = source;
  source = temp;
}

void swapbyreference(numPtr &source, numPtr &destination){
  numPtr temp = destination;
  destination = source;
  source = temp;
}

void deleteNum(numPtr num){
  cout << "p before delete -> " << num << " " << num ->number << endl;
  delete num;
  cout << "p after delete  -> " << num << " " << num ->number << endl;
  num = NULL;
  cout << "p after null    -> " << num;
}

void deleteNumbyreference(numPtr &num){
  cout << "p before delete -> " << num << " " << num ->number << endl;
  delete num;
  cout << "p after delete  -> " << num << " " << num ->number << endl;
  num = NULL;
  cout << "p after null    -> " << num;
}


int main(){

  numPtr num1 = new numberNode;
  num1 ->number = 111;

  cout << "before delete" << endl << endl;

  cout << " p1 -> " << num1 << " " << num1 ->number << endl << endl;

  deleteNum(num1);

  cout << "after delete" << endl << endl;

  cout << " p1 -> " << num1 << " " << num1 ->number << endl << endl;


  num1 = new numberNode;
  num1 ->number = 111;

  cout << "before delete by reference" << endl << endl;

  cout << " p1 -> " << num1 << " " << num1 ->number << endl << endl;

  deleteNumbyreference(num1);

  cout << "after delete by reference" << endl << endl;

  cout << " p1 -> " << num1 << " " << num1 ->number << endl << endl;


  return 0;
}
