//RenameTab.cpp
//リネームタブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"RenameTab.h"


using namespace sslib;



INT_PTR RenameTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	//正規表現メニューを読み込む
	m_regex_menu.load(IDR_MENU_FILTER_REGEX);

	setCurrentSettings();
	return true;
}

INT_PTR RenameTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_RENAME_STRING:
			//検索する文字列/置換する文字列
			::EnableWindow(getDlgItem(IDC_EDIT_RENAME_STRING),sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			::EnableWindow(getDlgItem(IDC_CHECKBOX_RENAME_REGEX),sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));

			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				sendMessage(WM_COMMAND,MAKEWPARAM(IDC_EDIT_RENAME_STRING,EN_CHANGE),0);
				m_config_list[0]->cfg().rename.regex=!!sendItemMessage(IDC_CHECKBOX_RENAME_REGEX,BM_GETCHECK,0,0);
			}else{
				::EnableWindow(getDlgItem(IDC_BUTTON_RENAME_REGEX),false);
				m_config_list[0]->cfg().rename.pattern_list.clear();
				m_config_list[0]->cfg().rename.regex=false;
			}
			::EnableWindow(getDlgItem(IDC_BUTTON_RENAME_REGEX),m_config_list[0]->cfg().rename.regex);
			::EnableWindow(getDlgItem(IDC_CHECKBOX_GENERAL_TEST),m_config_list[0]->cfg().rename.regex);
			break;

		case IDC_CHECKBOX_RENAME_REGEX:
			//正規表現を使用する
			m_config_list[0]->cfg().rename.regex=!!sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0);
			::EnableWindow(getDlgItem(IDC_BUTTON_RENAME_REGEX),m_config_list[0]->cfg().rename.regex);
			break;

		case IDC_BUTTON_RENAME_REGEX:
			//正規表現ボタン
			if(int id=m_regex_menu.popup(handle(),getDlgItem(LOWORD(wparam)))){
				tstring menu_str(m_regex_menu.string(id));

				sendItemMessage(IDC_EDIT_RENAME_STRING,
								EM_REPLACESEL,
								1,
								(LPARAM)menu_str.substr(0,menu_str.find_first_of(_T("\t"))).c_str());
				::SetFocus(getDlgItem(IDC_EDIT_RENAME_STRING));
			}
			break;

		case IDC_CHECKBOX_GENERAL_TEST:
			//結果をテスト表示(実際にはリネームしません)
			m_config_list[0]->cfg().general.test=getCheck(LOWORD(wparam));
			return true;

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_RENAME_STRING:{
					//検索する文字列/置換する文字列
					std::vector<TCHAR> buffer(1024);

					::GetWindowText(getDlgItem(LOWORD(wparam)),&buffer[0],buffer.size());
					if(!tstring(&buffer[0]).empty()){
						m_config_list[0]->cfg().rename.pattern_list.clear();
						std::list<tstring> pattern_list;

						//';'で分割
						str::splitString(&pattern_list,&buffer[0],';');
						//重複を削除
						misc::undupList(&pattern_list);

						for(std::list<tstring>::const_iterator ite=pattern_list.begin(),
							end=pattern_list.end();
							ite!=end;++ite){
							std::list<tstring> pattern;

							str::splitString(&pattern,ite->c_str(),':');
							if(pattern.front().empty())continue;
							if(pattern.size()<2)pattern.push_back(_T(""));
							m_config_list[0]->cfg().rename.pattern_list.push_back(RENAME::pattern(pattern.front(),pattern.back()));
						}
					}
					break;
				}

				default:
					break;
			}
			break;

		default:
			break;
	}
	return false;
}

void RenameTab::setCurrentSettings(){
	//検索する文字列/置換する文字列
	tstring pattern;

	for(std::list<RENAME::pattern>::const_iterator ite=m_config_list[0]->cfg().rename.pattern_list.begin(),
		end=m_config_list[0]->cfg().rename.pattern_list.end();
		ite!=end;
		++ite){
		if(ite!=m_config_list[0]->cfg().rename.pattern_list.begin())pattern+=_T(";");
		pattern+=ite->first;
		if(!ite->second.empty())pattern+=_T(":")+ite->second;
	}

	::SetWindowText(getDlgItem(IDC_EDIT_RENAME_STRING),pattern.c_str());
	setCheck(IDC_CHECKBOX_RENAME_STRING,!pattern.empty());
	::EnableWindow(getDlgItem(IDC_EDIT_RENAME_STRING),sendItemMessage(IDC_CHECKBOX_RENAME_STRING,BM_GETCHECK,0,0));
	if(pattern.empty())m_config_list[0]->cfg().rename.regex=false;
	setCheck(IDC_CHECKBOX_RENAME_REGEX,m_config_list[0]->cfg().rename.regex);
	setCheck(IDC_CHECKBOX_GENERAL_TEST,m_config_list[0]->cfg().general.test);
	::EnableWindow(getDlgItem(IDC_CHECKBOX_RENAME_REGEX),sendItemMessage(IDC_CHECKBOX_RENAME_STRING,BM_GETCHECK,0,0));
	::EnableWindow(getDlgItem(IDC_BUTTON_RENAME_REGEX),sendItemMessage(IDC_CHECKBOX_RENAME_REGEX,BM_GETCHECK,0,0));
	::EnableWindow(getDlgItem(IDC_CHECKBOX_GENERAL_TEST),sendItemMessage(IDC_CHECKBOX_RENAME_REGEX,BM_GETCHECK,0,0));
}
