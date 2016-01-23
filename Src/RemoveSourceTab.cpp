//RemoveSourceTab.cpp
//処理後削除タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.7 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"RemoveSourceTab.h"


using namespace sslib;



INT_PTR RemoveSourceTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	setCurrentSettings();
	return true;
}

INT_PTR RemoveSourceTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_RADIO_REMOVESOURCE_SAVE:
			//保持する
			m_config_list[0]->cfg().general.remove_source=RMSRC_DISABLE;
			return true;

		case IDC_RADIO_REMOVESOURCE_RECYCLEBIN:
			//ごみ箱
			m_config_list[0]->cfg().general.remove_source=RMSRC_RECYCLEBIN;
			return true;

		case IDC_RADIO_REMOVESOURCE_REMOVE:
			//完全削除
			m_config_list[0]->cfg().general.remove_source=RMSRC_REMOVE;
			return true;

		default:
			return false;
	}
	return false;
}

void RemoveSourceTab::setCurrentSettings(){
	int id=0;

	setCheck(IDC_RADIO_REMOVESOURCE_SAVE,false);
	setCheck(IDC_RADIO_REMOVESOURCE_RECYCLEBIN,false);
	setCheck(IDC_RADIO_REMOVESOURCE_REMOVE,false);

	if(m_config_list[0]->cfg().general.remove_source==RMSRC_DISABLE)id=IDC_RADIO_REMOVESOURCE_SAVE;
	else if(m_config_list[0]->cfg().general.remove_source==RMSRC_RECYCLEBIN)id=IDC_RADIO_REMOVESOURCE_RECYCLEBIN;
	else if(m_config_list[0]->cfg().general.remove_source==RMSRC_REMOVE)id=IDC_RADIO_REMOVESOURCE_REMOVE;

	setCheck(id,true);
}
