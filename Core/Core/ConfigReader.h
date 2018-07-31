#pragma once


#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;
class ConfigReader
{
public:	
	ConfigReader(void);
	void Save();
	~ConfigReader(void);
	string get(string param){
		return this->operator [](param);
	}
	string get(char* param){
		return this->operator [](param);
	}
	string operator[](string param);
	string operator[](char *param);
	
//private:
	map<string, string> config;
};

#endif