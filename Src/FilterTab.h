//FilterTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.1 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _FILTERTAB_H_64C870DC_3B17_41ad_A3CB_CF6E02E251FE
#define _FILTERTAB_H_64C870DC_3B17_41ad_A3CB_CF6E02E251FE


#include"TabBase.h"



class FilterTab:public TabBase{
public:
	FilterTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_FILTER,config_list),
		m_size_menu(NULL),
		m_size_sub_menu(NULL),
		m_def_filefilter(config_list[0]->default_cfg().general.filefilter),
		m_def_file_ex_filter(config_list[0]->default_cfg().general.file_ex_filter){}
	~FilterTab(){}
private:
	HMENU m_size_menu,m_size_sub_menu;
	const fileinfo::FILEFILTER& m_def_filefilter;
	const fileinfo::FILEFILTER& m_def_file_ex_filter;
private:
	//メッセージハンドラ
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onDestroy();
	bool onCommand(WPARAM wparam,LPARAM lparam);
	bool onNotify(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_FILTERTAB_H_64C870DC_3B17_41ad_A3CB_CF6E02E251FE
