#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <stdint.h>

using namespace std;
int main(){
	ofstream memoria("memoria.txt");
	for(int i=0;i<256;i++){
		memoria << i;
		memoria << " ";
	}
}
