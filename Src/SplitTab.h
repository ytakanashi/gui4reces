//SplitTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068
#define _SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068


#include"TabBase.h"



class SplitTab:public TabBase{
public:
	SplitTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_SPLIT,config_list){}
	~SplitTab(){}
private:
	HMENU m_size_menu,m_size_sub_menu;
private:
	//メッセージハンドラ
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onDestroy();
	bool onCommand(WPARAM wparam,LPARAM lparam);
	bool onNotify(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_SPLITTAB_H_1DF76AF8_1DB9_4b8e_AD3F_F6EDBDF4C068
