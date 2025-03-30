/*
 * ELCompilerNasm.cpp
 *
 *  Created on: Feb 2, 2025
 *      Author: nobady
 */

#include "ELCompilerNasm.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <future>
#include <map>

using namespace std;


string Section;

// var a or var a = ...
void NASMNewVariable(string Name, string Type, string value){
	if(value.empty()) {
		// reserved byte
		if(Section != "bss"){
			Section = "bss";
			cout << endl << "section .bss" <<endl;
		}
		cout << "	" << Name << " resb 64" <<endl;
	}else{
		//  byte with data
		if(Section != "data"){
			Section = "data";
			cout << endl << "section .data" <<endl;
		}
		cout << "	" << Name << " db "<<  value<<", 0h" <<  "   ; type: "<< Type<<endl;
	}
}


// function a()
void NASMNewFunction(string Name, string Type, map<string, string> Params){
	if(Section != "text"){
		Section = "text";
		cout <<endl;
		cout << "section .text" << endl;
	}

	cout << Name << ":"<<endl;
	cout<<" ; type: " <<Type<<endl;
	cout<<" ; Parameters "<<Params.size()<< endl;
	int i = 0;
	for(auto Par: Params){
		i++;
		cout << " ; " << i <<"	   " << Par.first<< "(" << Par.second <<")"<<endl;
	}
}


// a = b
void NASMAssign(string Variable, string Value){
	cout << "push rax" << endl;
	cout << "mov rax, [" << Value <<"]"<< endl;
	cout << "mov [" << Variable << "], rax"<<endl;
	cout << "pop rax" << endl;
}
