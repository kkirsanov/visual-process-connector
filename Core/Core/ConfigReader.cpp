#include "ConfigReader.h"

void ConfigReader::Save(){
	ofstream of("./config.ini");
	map<string,string>::iterator it = config.begin();
	for(;it!=config.end();it++){
		of <<(*it).first << "="<<(*it).second<<endl;
	};
	of.close();
}
ConfigReader::ConfigReader(void)
{
	ifstream conf("./config.ini");
	
	if(!conf.good()){
		//AfxMessageBox("Не могу открыть config.ini");
		throw "Error opening config.ini";
	}

	char ch;

	bool isComment = false;
	bool isValue = false;

	string param("");
	string value("");

	while(!conf.eof()){
		ch=conf.get();
		if(ch=='\r')
			continue;
		if(ch=='\n'){
			if(param.length()>0 && value.length()>0)
				config[param] = value;
			param.clear();
			value.clear();
			isComment =false;
			isValue  = false;
			continue;
		}
		if(param.length()==0 && ch=='#'){
			isComment=true;
		}

		if (!isComment){
			if(ch=='='){
				isValue=true;
				continue;
			};
			if(!isValue){
				if (ch!= -1)
					param.push_back(ch);
			}else{
				if (ch!= -1)
					value.push_back(ch);
			}
		}
	}
	if(param.length()>0 && value.length()>0)
		config[param] = value;
	//CString st(
	//	AfxMessageBox(CString(config["TABLE_NAME"].c_str()));
	Save();
}

string ConfigReader::operator[](string param){
	return config[param];
}
string ConfigReader::operator[](char* param){
	return config[string(param)];
}
ConfigReader::~ConfigReader(void)
{
}
