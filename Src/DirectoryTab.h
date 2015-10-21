//DirectoryTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.6 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _DIRECTORYTAB_H_66DAB78A_DD75_4760_9EEF_153D45BDF3F0
#define _DIRECTORYTAB_H_66DAB78A_DD75_4760_9EEF_153D45BDF3F0


#include"TabBase.h"



class DirectoryTab:public TabBase{
public:
	DirectoryTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_DIRECTORY,config_list){}
	~DirectoryTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_DIRECTORYTAB_H_66DAB78A_DD75_4760_9EEF_153D45BDF3F0
