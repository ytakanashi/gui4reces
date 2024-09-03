//OtherTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _OTHERTAB_H_14B30189_A168_4e16_BB22_D7FC72848884
#define _OTHERTAB_H_14B30189_A168_4e16_BB22_D7FC72848884


#include"TabBase.h"



class OtherTab:public TabBase{
public:
	OtherTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_OTHER,config_list){}
	~OtherTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
};

#endif //_OTHERTAB_H_14B30189_A168_4e16_BB22_D7FC72848884
