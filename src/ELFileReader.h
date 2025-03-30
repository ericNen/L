/*
 * ELFileReader.h
 *
 *  Created on: Feb 13, 2025
 *      Author: er
 */

#ifndef ELFILEREADER_H_
#define ELFILEREADER_H_
#pragma once

#include <string.h>
#include <map>
#include <iostream>
using namespace std;


void InitializeReader();

class Object{
public:
	enum EMainTypes: short{
		EVariable, EFunction
	};

	static vector<string> MainTypes;

	static vector<string> Unused;
	static vector<string> ChildTypes;
	static vector<string> CommonPrefix;
	static vector<string> VarPrefix;
	static vector<string> FuncPrefix;

	static void Initialize();

	bool RegisterNewType(string name, vector<string> childs);

private:
	map<string, short> ClassType = {};
};


class ObjType{
public:
	string Name;
	Object::EMainTypes Type;
	string OutPutType;
};

class FunctionType: public ObjType{
public:
	vector<string> ParamsTypes;;
};

class VariableType: public ObjType{
public:
	bool IsInitialized;
};



bool LayerControl(int CharacterLabel);
int GetLayer();
void SetLayer(int number);
void IncreaseLayer();
void DecreaseLayer();

string GetLayerState();
void AddLayer(string State);
void RemoveLayer();

bool Trim(string Line); // False if the line has no valid code
void CheckTag(string Text);
void Interpret(string Text);
void DeclarVar(string Text);
void DeclarFun(string Text);
void DeclarClass(string Text);
void Assigning(string Text);
void UseFunction(string Text);
bool Contains(vector<string> &list, string item);


void AssignValue();
static void (*NewVariable) (string, string, string);
static void (*NewFunction)(string, string , map<string, string>);
static void (*Assign)(string, string);
void DeclareObj(vector<string> &Snippet);
void UseTags(vector<string> &Snippet);
bool CheckIfVarDefined(string Symbol, string Type);
string GetVarType(string VarName);
bool CheckIfFunDefined(string Symbol);

#define Others 0;
#define Bracket1Start 1;
#define Bracket1End 2;
#define Bracket2Start 3;
#define Bracket2End 4;
#define Bracket3Start 5;
#define Bracket3End 6;
#define EqualMark 7;
#define Comma 8;
#define AngleBracketStart 9;
#define AngleBracketEnd 10;

int CheckSpecialCharacter(char word);

void CheckUnused();

#endif /* ELFILEREADER_H_ */
