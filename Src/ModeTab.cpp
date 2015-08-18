//ModeTab.cpp
//動作詳細タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.5 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"ModeTab.h"


using namespace sslib;



INT_PTR ModeTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	setCurrentSettings();
	return true;
}

INT_PTR ModeTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_MODE_IGNORE_DIRECTORY_STRUCTURES:
			//ディレクトリ階層を無視する
			m_config_list[0]->cfg().general.ignore_directory_structures=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_MODE_BACKGROUND:
			//バックグラウンドで動作
			m_config_list[0]->cfg().general.background_mode=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_MODE_LOG:
			//ログウインドウを表示
			m_config_list[0]->cfg().gui4reces.log=getCheck(LOWORD(wparam));
			return true;

		default:
			return false;
	}
	return false;
}

void ModeTab::setCurrentSettings(){
	//ディレクトリ階層を無視する
	sendItemMessage(IDC_CHECKBOX_MODE_IGNORE_DIRECTORY_STRUCTURES,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().general.ignore_directory_structures)?BST_CHECKED:BST_UNCHECKED,
					0
					);

	//バックグラウンドで動作
	sendItemMessage(IDC_CHECKBOX_MODE_BACKGROUND,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().general.background_mode)?BST_CHECKED:BST_UNCHECKED,
					0
					);

	//ログウインドウを表示
	sendItemMessage(IDC_CHECKBOX_MODE_LOG,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().gui4reces.log)?BST_CHECKED:BST_UNCHECKED,
					0
					);
}
