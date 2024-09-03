//RenameTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _RENAMETAB_H_B2D4389D_A81E_417f_87FE_306FAF253EA5
#define _RENAMETAB_H_B2D4389D_A81E_417f_87FE_306FAF253EA5


#include"TabBase.h"



class RenameTab:public TabBase{
public:
	RenameTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_RENAME,config_list),
		m_regex_menu(){}
	~RenameTab(){}
private:
	sslib::Menu m_regex_menu;
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_RENAMETAB_H_B2D4389D_A81E_417f_87FE_306FAF253EA5
