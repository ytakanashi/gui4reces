//ModeTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.6 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _MODETAB_H_8CC3C9DD_32F2_4631_9DE0_B88046BD19F9
#define _MODETAB_H_8CC3C9DD_32F2_4631_9DE0_B88046BD19F9


#include"TabBase.h"



class ModeTab:public TabBase{
public:
	ModeTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_MODE,config_list){}
	~ModeTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_MODETAB_H_8CC3C9DD_32F2_4631_9DE0_B88046BD19F9
