#pragma once
#include <afx.h>
#include "core.h"

namespace CBIR{
	enum NodeType {
      NODE_IMAGE,
      NODE_INDEX,
	  NODE_LOGIC	  
   };
	enum NodeDirection {
      NODE_INPUT,
      NODE_OUTPUT
   };
	class Node{
		virtual CString GetName();
		virtual NodeType GetType();
		virtual NodeDirection GetDirection();
	};

	class WorkGraph :
		public CObject
	{
	public:
		WorkGraph();
		void Run();
		void Stop();
		void Next();
		void Insert();
		void Remove();
		void Link();
	//	void Serialize(CArchive& ar);
	public:
		~WorkGraph();
	};

};