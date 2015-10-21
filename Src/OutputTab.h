//OutputTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.6 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _OUTPUTTAB_H_9360AA74_9E35_4189_9763_A9CC9497824D
#define _OUTPUTTAB_H_9360AA74_9E35_4189_9763_A9CC9497824D


#include"TabBase.h"



class OutputTab:public TabBase{
public:
	OutputTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_OUTPUT,config_list){}
	~OutputTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_OUTPUTTAB_H_9360AA74_9E35_4189_9763_A9CC9497824D
