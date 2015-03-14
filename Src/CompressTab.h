//CompressTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.3 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0
#define _COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0


#include"TabBase.h"



class CompressTab:public TabBase{
public:
	CompressTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_COMPRESS,config_list){}
	~CompressTab(){}
private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
	tstring getExtension();
};

#endif //_COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0
