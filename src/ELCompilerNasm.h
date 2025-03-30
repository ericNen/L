/*
 * ELCompilerNasm.h
 *
 *  Created on: Feb 2, 2025
 *      Author: nobody
 */

#ifndef ELCOMPILERNASM_H_
#define ELCOMPILERNASM_H_
#pragma once



#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

void NASMNewVariable(string Name, string Type, string value);
void NASMNewFunction(string Name, string Type, map<string, string> Params);

void NASMAssign(string Variable, string Value);



#endif /* ELCOMPILERNASM_H_ */
