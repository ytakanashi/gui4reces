//FilterTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.0 by x@rgs
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
		m_filter_ptr(&config_list[0]->cfg().general.filefilter),
		m_def_filefilter(config_list[0]->default_cfg().general.filefilter),
		m_def_file_ex_filter(config_list[0]->default_cfg().general.file_ex_filter),
		m_filter_type(0){}
	~FilterTab(){}
private:
	HMENU m_size_menu,m_size_sub_menu;
	fileinfo::FILEFILTER* m_filter_ptr;
	const fileinfo::FILEFILTER& m_def_filefilter;
	const fileinfo::FILEFILTER& m_def_file_ex_filter;
	int m_filter_type;
private:
	bool longlong2SYSTEMTIME(SYSTEMTIME* result_st,const long long date_time);
	long long SYSTEMTIME2longlong(const SYSTEMTIME st);
	//メッセージハンドラ
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onDestroy();
	bool onCommand(WPARAM wparam,LPARAM lparam);
	bool onNotify(WPARAM wparam,LPARAM lparam);
	inline bool include_filter(){return m_filter_type==0;}
public:
	void setCurrentSettings();
};

#endif //_FILTERTAB_H_64C870DC_3B17_41ad_A3CB_CF6E02E251FE
