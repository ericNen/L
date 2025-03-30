/*
 * ELFileReader.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: er
 */
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
#include "ELFileReader.h"
#include "ELCompilerNasm.h"

using namespace std;

void InitializeReader(){
	NewVariable = NASMNewVariable;
	NewFunction = NASMNewFunction;
	Assign = NASMAssign;
}

vector<string> Object::MainTypes;
vector<string> Object::Unused;
vector<string> Object::ChildTypes;
vector<string> Object::CommonPrefix;
vector<string> Object::VarPrefix;
vector<string> Object::FuncPrefix;

vector<char> ValidObjNameChar = {
		'a','b','d'
};

void Object::Initialize(){
	ChildTypes.insert(ChildTypes.begin(), "any");
	ChildTypes.insert(ChildTypes.begin(), "string");
	ChildTypes.insert(ChildTypes.begin(), "char");
	ChildTypes.insert(ChildTypes.begin(), "int");
	ChildTypes.insert(ChildTypes.begin(), "list");


	MainTypes.insert(MainTypes.begin(), "var");
	MainTypes.insert(MainTypes.begin(), "function");

	FuncPrefix.insert(FuncPrefix.begin(), "parallel");
};

bool Object::RegisterNewType(string name, vector<string> childs){
	return 0;
};

int Layer;

bool LayerControl(int CharacterLabel){
	switch (CharacterLabel){
	case 5:
		break;
	case 6:
		RemoveLayer();
		break;
	default:
		return false;
		break;
	}
	return true;
}
int GetLayer(){ return Layer; };
void SetLayer(int number){ Layer = number;};
void IncreaseLayer(){ Layer ++; };
void DecreaseLayer(){ Layer --; };

string CurrentLayerState;
vector<string> LayerState;
string GetLayerState(){
	if (LayerState.size() == 0){ return "NoLayer";}
	return LayerState[LayerState.size()-1];
};
void AddLayer(string State){ LayerState.insert(LayerState.begin() + LayerState.size(), State); };
void RemoveLayer(){ LayerState.erase(LayerState.end() + LayerState.size()); };



bool IsCompiling = true;
enum class TrimingState:short{
	None ,String ,Number ,Byte
};
TrimingState TrimState = TrimingState::None;

//False if it is empty after triming
bool Trim(string Line){
	size_t StartPos = 0;
	size_t EndPos = Line.length();
	string Text;
	bool HasValidCode = false;

	CheckTag(Line);
	// This will check for #Start #Pause #End tag and it will effect bool {IsCompiling} Global variable
	if (IsCompiling == false){
		// #Pause used than skip triming this line
		return HasValidCode;
	}

	int Count = 0;
	for (StartPos = 0; StartPos < EndPos; StartPos++){
		if( Line[StartPos] != ' ' and Line[StartPos] != '\t'){

			if (Text.size() == 1 && Text[0] == '/'  && Line[StartPos] == '/'){
				// ensure there is at least 2 character
				// If true than it is a command
				return HasValidCode;
			}


			// "string or char"
			if (TrimState == TrimingState::String){
				if (Line[StartPos] == '\"'){
					//Last " of the string
					TrimState = TrimingState::None ;
					Text += Line[StartPos];
				}else{
					Text += Line[StartPos];
				}

			}
			else if(Line[StartPos] == '\"'){
				//First " of the string
				TrimState = TrimingState::String;
				Text += Line[StartPos];
			}
			else{
				if (CheckSpecialCharacter(Line[StartPos]) != 0){
					// Special character
					Interpret(Text);
					// Previous Text ex: "var<" in "var<int>" the "var" will be the text before "<"
					Text.clear();
					// Text become empty
					Text += Line[StartPos];
					// Add "<" to the Text
					Interpret(Text);
					Text.clear();
				}else{
					// Not Special character. Normal condition
					Text += Line[StartPos];
				}
			}


			if (StartPos != EndPos - 1){
				// If "StartPos" equals to "EndPos-1" than it is the end of the line
				continue;
			}
		}
		Interpret(Text);
		HasValidCode = true;
		Text.clear();
	}

	return HasValidCode;
}

void CheckTag(string Text){
	if(Text == "#Start"){
		IsCompiling = true;
		return;
	}else if(Text == "#Pause"){
		IsCompiling = false;
	}else if(Text == "#End"){
		cout << endl<<endl<<">>>Program Terminated ";
		abort();
	}
}

enum class InterpState:short{
	None,VarDec,FunDec,ClassDec,AssignVal,FunParams,ParamTypeDec,Other,
};
bool IsInsideFunction;
InterpState IntpState;
string PreviousUsedObjName;    // "a" in "var a = b"   or    "a" in "a = b"

bool InterpretAgain = false;
string PreviousObj;

void (*Task)(string);
void Interpret(string Text){
	if (Text.empty())return;

	Start:
	if(Task  == nullptr){
		// unknown process
		goto Check;
	}
	goto End; // If the process is already known

	Check:
	//cout << "Check";
	if(Text == "var" || Contains(Object::VarPrefix, Text)){
		Task = DeclarVar;
		//cout << "Var" << endl;
	}else if(Text == "function" || Contains(Object::FuncPrefix, Text)){
		Task = DeclarFun;
		//cout <<"Function" << endl;
	}else{
		PreviousObj = Text;
		IntpState = InterpState::Other;
	}
	goto Start;


	End:
	Task(Text);
	if(InterpretAgain){
		// when the instruction type change
		InterpretAgain = false;
		goto Start;
	}
}

string VarName;
string VarType = "any";
string Value;
bool IsWaitingForDeclare = false;
bool IsDeclaringType = false;
void DeclarVar(string Text){
	if(IsDeclaringType == true){
		// declaring type <Type>
		if(Text != ">"){
			// "Type" in "<Type>"
			VarType = Text;
			return;
		}
	}

	// special characters
	if(CheckSpecialCharacter(Text[0]) != 0){
		// It is a Special Character
		if(Text == "<"){
			// First < of declaring type
			IsDeclaringType = true;
		}else if (Text == ">"){
			// Last > of declaring type
			IsDeclaringType = false;
		}else if (Text == "="){
			IsWaitingForDeclare = true;
		}else if (Text == ";"){
			IsWaitingForDeclare = false;
			Task = 0;
		}

		return;
	}


	// words ( not a special character)

	if( !VarName.empty() && ! IsWaitingForDeclare && ! IsDeclaringType){
		// done
		NewVariable( VarName, VarType, Value);
		VarName.clear();
		InterpretAgain = true;
		Task = 0;
		Value = "";
		VarType = "any";
		return;
	}


	if(Text == "var"){
	}else if(Contains(Object::VarPrefix, Text)){
	}else{
		// not reserved symbol
		if(VarName.empty()){
			VarName = Text;
		}else if(IsWaitingForDeclare){
			// var a = b c
			// 				^ already assigned so the next symbol "c" must be in next line
			IsWaitingForDeclare = false;
			Value = Text;
		}
	}
}

string FunctionName;
string Type;
map<string,string> Params; // <Param name, type>
pair<string, string> Par;
int ParamCount = 0;
bool IsDeclaringParams  = false;
void DeclarFun(string Text){
	if(IsDeclaringType == true){
		// declaring type <Type>
		if(Text != ">"){
			// "Type" in "<Type>"
			if(IsDeclaringParams){
				Par.second = Text;
			}else{
				Type = Text;
			}
			return;
		}
	}

	// special characters
	if(CheckSpecialCharacter(Text[0]) != 0){
		// It is a Special Character
		if(Text == "<"){
			// First < of declaring type
			IsDeclaringType = true;
		}else if (Text == ">"){
			// Last > of declaring type
			IsDeclaringType = false;
		}else if (Text == "("){
			IsDeclaringParams = true;
		}else if(Text == ")"){
			if(Par.second == ""){	Par.second = "any";  };
			Params.insert(Params.end(), Par);
			NewFunction(FunctionName, Type, Params);
			IsDeclaringParams = false;
		}else if(Text == ","){
			if(Par.second == ""){	Par.second = "any";  };
			Params.insert(Params.end(), Par);
			Par.first = "";
			Par.second = "";
			ParamCount ++;
		}

		return;
	}


	// words ( not a special character)

	if (Text == "function"){

	}else if(Contains(Object::FuncPrefix, Text)){

	}else{
		if(IsDeclaringParams){
			Par.first = Text;
		}else{
			FunctionName = Text;
		}
	}
}

void UseFunction(string Text){

}

void Assigning(string Text){

}
/*
void NewVariable(string Name, string Type, string value){
	if(value.empty()) value = "NULL";
	cout << endl << "Name = " <<Name << "; Type = " << Type <<"; Value " << value <<endl;
}
*/
vector<string> PrefixKeyWords = {
	"parallel"
};

vector<string> Tags = {
	"#NoCompile"
};



vector<VariableType> DefinedVariable = {};
vector<FunctionType> DefinedFunction = {};

vector<string> ObjToLookup = {};

bool Contains(vector<string> &list, string item){
	if( find(list.begin(), list.end(), item) != list.end() ){
		return true;
	}else{return false;};
};




void AssignValue(){

}

void UseTags(vector<string> &Snippet){
	if (Snippet[0] == "#NoCompile"){
		CurrentLayerState = "NoCompile";
	}
};


int CheckSpecialCharacter(char word){
	switch (word) {
	case '(':
		return Bracket1Start;
		break;
	case ')':
		return Bracket1End;
		break;
	case '{':
		return Bracket3Start;
		break;
	case '}':
		return Bracket3End;
		break;
	case '=':
		return EqualMark;
		break;
	case ',':
		return Comma;
		break;
	case '<':
		return AngleBracketStart;
		break;
	case '>':
		return AngleBracketEnd;
		break;
	default:
		break;
	}
	return Others;
};



