//SplitTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.4 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068
#define _SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068


#include"TabBase.h"



class SplitTab:public TabBase{
public:
	SplitTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_SPLIT,config_list),
		m_size_menu(){}
	~SplitTab(){}
private:
	sslib::Menu m_size_menu;
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
	INT_PTR onNotify(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068
