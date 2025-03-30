//============================================================================
// Name        : ELCompiler.cpp
// Author      : no
// Version     :
// Copyright   : Your copyright notice
// Description : ELCompiler, Ansi-style
//============================================================================


#include <iostream>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>
#include <algorithm>
#include "ELCompilerNasm.h"
#include "ELFileReader.h"

using namespace std;

void ReadFile(string FullFilePath);

int main() {
	Object::Initialize();
	cout << ">> program started <<" << endl;
	string FilePath = "/home/er/Desktop/Demo.txt";

	InitializeReader();

	ReadFile(FilePath);

	//CheckUnused();

	cout << "end";
	cout << endl << "Layer" << GetLayer();
	return 0;
}


void ReadFile(string FullFilePath){
	ifstream FileSt(FullFilePath);
	if(! FileSt.is_open() ){
		cout << "file opening failed, check if file exist>>" << endl;
		return;
	}else{
		cout<<"file opening success>>" << endl << endl;
	};

	int line = 0;
	string s;
	bool IsEmptyCode;
	while (getline(FileSt, s)){
		line++;
		if(s.empty()) continue;
		IsEmptyCode = Trim(s);
		if (!IsEmptyCode){
			// empty line
		}
	}
}






