#pragma once
#include "MyIPTool2.h"		//afxprintinfo
#include "MyIPImage.h"
#include <iostream>			//190430 YWCho CSV file
#include <fstream>			//
using namespace std;		//#pragma once


class CSV 
{
private: 
// 	CString filename;
// 	CString filepath;
	FILE* fp;

public:
	void CSVtest();
	void MakeFile(char *fn);
	void SetKey(char *fn);
	void SetNode(char *fn);
};