#include <iostream>

using namespace std;

const int width = 5;

char lookup(int arg){
  char table[36] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  return table[arg];
}

void displayArray(int value[width],int base[width]){
  cout << "V >";
  for(int i = 0; i < width; i++){
    cout << lookup(value[i]) << ',';
  }
  cout << endl;
  cout << string(width * 2 + 3,'-');
  cout << endl;
  cout << "B >";
  for(int i = 0; i < width; i++){
    cout << lookup(base[i]) << ',';
  }
  cout << endl << endl;
  getchar();
}

void increment(int (&value)[width], int base[width]){
  value[0]++;
  for(int i = 0; i < width; i++){
    if(i != width -1){
      if(value[i] >= base[i]){
        value[i] = 0;
        value[i+1]++;
      }
    }
  }
}

bool arrayComp(int arg1[width], int arg2[width]){
  for(unsigned int i = 0; i < width;i++){
    if(arg1[i] < arg2[i] - 1){
      return true;
      break;
    }
  }
  return false;
}

int main(){
  //needlessly complicated for loop
  int current[width] {0,0,0,0,0};
  int base[width] {2,10,16,8,3};
  while(arrayComp(current,base)){
    displayArray(current,base);
    increment(current,base);
  }
  return 0;
}
