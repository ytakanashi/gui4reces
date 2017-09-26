//PasswordTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.8 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _PASSWORDTAB_H_59880DE9_AB0F_4825_B3CB_20588495DCF7
#define _PASSWORDTAB_H_59880DE9_AB0F_4825_B3CB_20588495DCF7


#include"TabBase.h"



class PasswordTab:public TabBase{
public:
	PasswordTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_PASSWORD,config_list),
		m_write_password_list_menu(){}
	~PasswordTab(){}
private:
	sslib::Menu m_write_password_list_menu;
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
	bool getPasswordList(std::vector<tstring>* list);
	bool getNewPassword(tstring* new_password);
};

#endif //_PASSWORDTAB_H_59880DE9_AB0F_4825_B3CB_20588495DCF7
