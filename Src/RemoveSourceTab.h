//RemoveSourceTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.6 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _REMOVESOURCETAB_H_40151108_2A1E_4b2b_8F38_54313098227F
#define _REMOVESOURCETAB_H_40151108_2A1E_4b2b_8F38_54313098227F


#include"TabBase.h"



class RemoveSourceTab:public TabBase{
public:
	RemoveSourceTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_REMOVESOURCE,config_list){}
	~RemoveSourceTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_REMOVESOURCETAB_H_40151108_2A1E_4b2b_8F38_54313098227F
