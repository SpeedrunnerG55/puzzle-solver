#include <iostream>

using namespace std;

const int width = 5;

char lookup(int arg){
  char table[36] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  return table[arg];
}

void displayArray(int value[width], int check){
  for(int i = 0; i < width; i++){
    if(i == check){
      cout << "[";
    }
    else{
      cout << " ";
    }
    cout << lookup(value[i]);
    if(i == check){
      cout << "]";
    }
    else{
      cout << " ";
    }
    if(i != width - 1){
      cout << ",";
    }
  }
  cout << endl;
}

void increment(int (&value)[width], int base[width]){
  cout << string(width * 4 + 4,'=') << endl;
  cout << " start incrementing " << endl;
  value[0]++;
  cout << " increment first term" << endl;
  displayArray(value,0);
  cout << string(width * 4 + 4,'-') << endl;
  getchar();
  cout << string(100,'\n');
  cout << " Start loop " << endl;
  cout << string(width * 4 + 4,'~') << endl;
  for(int i = 0; i < width; i++){
    cout << " is value[" << i << "]" << endl;
    displayArray(value,i);
    cout << " >= to base[" << i << "] " << endl;
    displayArray(base,i);

    cout << string(width * 4 + 4,'-') << endl;
    getchar();
    cout << string(100,'\n');
    if(value[i] >= base[i]){
      cout << " yes" << endl;
      cout << " is this not the last term? " << endl;
      cout << string(width * 4 + 4,'+') << endl;
      getchar();
      cout << string(100,'\n');
      if(i != width -1){
        cout << " yes" << endl;
        cout << "set value["<< i << "] to 0" << endl;
        value[i] = 0;
        displayArray(value,i);
        cout << "increment value[" << i+1 << "]" << endl;
        value[i+1]++;
        displayArray(value,i+1);
        cout << string(width * 4 + 4,'+') << endl;
        getchar();
        cout << string(100,'\n');
      }
      else{
        cout << " no" << endl;
        cout << string(width * 4 + 4,'+') << endl;
      }
    }
    else{
      cout << " no" << endl;
      cout << " exit loop " << endl;
      cout << string(width * 4 + 4,'~') << endl;
      cout << " finished incrementing " << endl;
      cout << string(width * 4 + 4,'=') << endl;
      return;
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
  int base[width] {4,4,4,0,3};
  cout << string(100,'\n');
  while(arrayComp(current,base)){
    displayArray(current,-1);
    displayArray(base,-1);
    cout << string(width * 4 + 4,'-') << endl;
    getchar();
    cout << string(100,'\n');
    increment(current,base);
    displayArray(current,-1);
    displayArray(base,-1);
    cout << string(width * 4 + 4,'-') << endl;
    getchar();
    cout << string(100,'\n');
  }
  return 0;
}
