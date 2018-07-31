#include "Core.h"


#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <algorithm>

string GetCurTimeString(){		
	char tmpbuf1[128], tmpbuf2[128];
	_strdate_s( tmpbuf1, 128 );
	_strtime_s( tmpbuf2, 128 );

	string s = string(tmpbuf1) + string("  ") + string(tmpbuf2);
	return s;
};

void Core::LogIt(string str){
//#ifdef _DEBUG
	boost::mutex::scoped_lock scoped_lock(IsWritingMutex);
	Log <<GetCurTimeString() << "\t" << str<<"\n";
	Log.flush();
//#endif
}

void Core::AddPinRequest(Plugin *pPlg, InPin *pPin){
	//Есть ли такой вектор, если нет - создать, если есть - получить адрес и добавить в него
	//if(WorkMap[pPlg]

	boost::mutex::scoped_lock lock(workMapMutex);
	int size = WorkMap[pPlg].size(); //Создать если нет
	
	LogIt(string("Request Added: ") + pPlg->GetName()+ 	string(" -> ") + pPin->GetLink()->m_plug->GetName());
	//Подготавливаем структуру для ввода в масив
	WorkMap[pPlg].push_back(pPin);
	
};
struct PinRrquestProcessor{
	InPin* _pin;
	PinRrquestProcessor(InPin* pin){_pin = pin;}

	void operator()(){
		_pin->pLinkedPin->RequestData();		
	}
};


Plugin::Plugin(Core* pCore){
	Plugin::pCore = pCore;
	group = 1;
	groupData=1;
	resultTime=0;
	IsWorking=false;
	name = "plugin";
	description = "description";
	perfDiffMs=0;
};

void Core::RunPinRequest(Plugin *plug){

	boost::mutex::scoped_lock scoped_lock(workMapMutex);
	int size = WorkMap[plug].size();

	if (size==0){
		WorkMap.erase(plug);
		scoped_lock.unlock();
		return;
	}

	vector<InPin*> pVec = WorkMap[plug];
	scoped_lock.unlock();

	if (pVec.size()==1){

		LogIt (string("Processing non-parallel request: ") + pVec[0]->m_plug->GetName());

		(pVec[0])->pLinkedPin->RequestData();

		boost::mutex::scoped_lock lock(workMapMutex);

		WorkMap[plug].resize(0);
		LogIt (string("Non-parallel done: ") + pVec[0]->m_plug->GetName());
	}else{
		//Пробежаться по всем группам
		for(int i=1;i<=9;i++){
			vector<int> CompliteVector;//счетчик готовности группы для паралельности
			boost::thread_group thG;
			//проверяем задания
			for(vector<InPin*>::iterator it = pVec.begin(); it!= pVec.end();it++){	
				if ((*it))
					if ((*it)->GetLink()!=NULL)
						if( (*it)->GetLink()->m_plug->group==i){
							CompliteVector.push_back(1);
							LogIt (string("Processing parallel request: ") + (*it)->m_plug->GetName());
					}
			}
			if (CompliteVector.empty())
				continue;
			//проверить каждый элемент
			int parCounter=0; //счестчик паралельности
			
			for(vector<InPin*>::iterator it = pVec.begin(); it!= pVec.end();it++){
				//если нужная группа				
				if((*it))
					if((*it)->GetLink())
						if((*it)->GetLink()->m_plug)
				if( (*it)->GetLink()->m_plug->group == i){
					//Запустить в поток					
					
					LogIt (string("Processing parallel request: ") + (*it)->m_plug->GetName());
					PinRrquestProcessor pr(*it);					

					thG.create_thread(pr);
				}
			}
			//ждать завершения всех потоков для данной группы
			thG.join_all();
		}
		boost::mutex::scoped_lock scoped_lock(workMapMutex);
		WorkMap[plug].resize(0);
	}
};

void Core::AddModule(string path){
	HMODULE hm = LoadLibrary(path.c_str());

	if ( !hm ){
		string s("couldn't load - ");
		s+= path.c_str();
		LogIt(s);
	}else{ 
		int *acceptDDL = (int*)GetProcAddress((HINSTANCE)hm, "isCBIRPlugin");
		if (acceptDDL != NULL){
			//load an `init` function return a pointer to new plugin
			FUNCTION_pPlugin_pCore *init = (FUNCTION_pPlugin_pCore*)GetProcAddress((HINSTANCE)hm, "init");
			//call init funcion
			Plugin *plg = (init)(this);
			ASSERT(AfxCheckMemory());
			if(plg==NULL){
				throw string("error!");				
			};
			PluginEntry *plgE = new PluginEntry(init, this);
			plgE->name = plg->GetName();
			plgE->type = plg->GetDataType();
			
			LogIt(string ("Plugin Recognized  : ") +plgE->name);

			vector<PluginEntry*>::iterator it, plgIt;
			bool isOk = false;

			for(it = plugins.begin();it!=plugins.end();it++){
				if ((*it)->GetName() == plgE->GetName()){
					plgIt = it;
					isOk=true;
					break;
				};
			}
			if (isOk){
				//replacePlugin
				//delete (*plgIt);
				//delete plg;

				*plgIt = plgE;
			}else{
				//addPlugin
				plugins.push_back(plgE);
			};
		}else{
		FreeLibrary(hm);
		}
	}
}

Core::Core(string pluginPath){

	Log.open("systemlog.log");
	
	Log <<"\n======================================================\n";
	Log << "Staring at " << GetCurTimeString();		
	Log <<"\n======================================================\n";
	Log.flush();

	//load other plugins
	//scan directory /plugins for *.dll files
	using namespace boost;

//	try{ 

		string p = pluginPath + string("Plugins/");
		string DirPath(p);
		filesystem::path fullPath (DirPath, filesystem::native);

		filesystem::directory_iterator end;
		for (filesystem::directory_iterator it(fullPath);it != end; ++it){
			std::string dllpath = it->string();

			string::size_type t = dllpath.find(string(".dll"));
			if (t!=string::npos)
				AddModule(dllpath);
		}
		/*
	}catch(std::exception &ex){
		int z=10;
	}*/

};
	

pair<OutPin*, InPin*> Plugin::Link(int pin/*out*/, Plugin* plg, int remotePin/*in*/){
	//TODO:: add check like in Link method
	pair<OutPin*, InPin*> p;



	p.first = NULL;
	p.second = NULL;
	if(!plg->inPins.at(remotePin)->GetLink() && outPins.size()>pin){
		plg->inPins.at(remotePin)->Link(outPins.at(pin));
		p.first =outPins.at(pin);
		p.second = plg->inPins.at(remotePin);
		
		CString s1;
		s1.Format("%s (%d) -> %s (%d)",plg->GetName().c_str(), remotePin, outPins.at(pin)->m_plug->GetName().c_str(), remotePin);
		pCore->LogIt(string ("Plugins Linked  : ") + string(s1.GetBuffer()));
	}

	return p;
}

void Plugin::Link(Plugin *plg){

OutPin *pin1 = outPins[0];
	vector<InPin*>::iterator it = plg->inPins.begin();
	
	if(!(*it)->GetLink() && ((*it)->GetDataType() == pin1->GetDataType())){
		(*it)->Link(pin1);
	}else{
		while((*it)->GetLink() && it!=plg->inPins.end() && ((*it)->GetDataType()==pin1->GetDataType())){
			it++;
			bool a1 = (*it)->GetLink();
			if(!(*it)->GetLink() && ((*it)->GetDataType() == pin1->GetDataType())){
				(*it)->Link(pin1);
				break;
			}
		};
	};
};
