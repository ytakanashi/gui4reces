//OutputTab.cpp
//出力タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.0 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"OutputTab.h"

#include<shlobj.h>


using namespace sslib;



namespace{
	const TCHAR* const combo_output_file_list[]={
		_T("ファイル名を指定しない"),
		_T("ファイル名を指定する"),
		_T("実行時に指定する")
	};
	enum{
		OUTPUT_FILE_DEFAULT=0,
		OUTPUT_FILE_CHOOSE,
		OUTPUT_FILE_CHOOSE_EACH_TIME
	};
}

bool OutputTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	for(size_t i=0;i<ARRAY_SIZEOF(combo_output_file_list);i++){
		sendItemMessage(IDC_COMBO_OUTPUT_FILE,
						CB_ADDSTRING,
						0,
						(LPARAM)combo_output_file_list[i]);
	}

	setCurrentSettings();
	return true;
}

bool OutputTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_RADIO_OUTPUT_SOURCE:
			//対象と同じ
			::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),false);
			m_config_list[0]->cfg().general.output_dir.clear();
			return true;

		case IDC_RADIO_OUTPUT_DESKTOP:{
			//デスクトップ
			::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),false);
			m_config_list[0]->cfg().general.output_dir.clear();

			std::vector<TCHAR> desktop_path(MAX_PATH);

			::SHGetSpecialFolderPath(NULL,&desktop_path[0],CSIDL_DESKTOP,false);

			m_config_list[0]->cfg().general.output_dir.assign(&desktop_path[0]);
			return true;
		}

		case IDC_RADIO_OUTPUT_SPECIFIC_DIR:{
			//ディレクトリを指定
			FolderDialog folder_dialog;

			std::vector<TCHAR> old_dir(MAX_PATHW);
			::GetWindowText(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),&old_dir[0],old_dir.size());

			if(!folder_dialog.doModalOpen(&m_config_list[0]->cfg().general.output_dir,
										  handle(),
										  _T("全てのディレクトリ (*.:)\0*.:\0\0"),
										  _T("保存先ディレクトリを選択してください"),
										  &old_dir[0])){
				m_config_list[0]->cfg().general.output_dir.clear();
				return false;
			}
			::SetWindowText(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),m_config_list[0]->cfg().general.output_dir.c_str());
			::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),true);
			return true;
		}

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_OUTPUT_SPECIFIC_DIR:{
					//ディレクトリを指定
					std::vector<TCHAR> dir(MAX_PATHW);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&dir[0],
									dir.size());
					m_config_list[0]->cfg().general.output_dir.assign(&dir[0]);
					break;
				}

				case IDC_EDIT_OUTPUT_FILE:{
					//ファイルを指定
					std::vector<TCHAR> file(MAX_PATHW);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&file[0],
									file.size());
					m_config_list[0]->cfg().compress.output_file.assign(&file[0]);
					break;
				}

				default:
					break;
			}
			break;

		case CBN_SELCHANGE:
			switch(LOWORD(wparam)){
				case IDC_COMBO_OUTPUT_FILE:
					//ファイル名
					switch(sendItemMessage(LOWORD(wparam),CB_GETCURSEL,0,0)){
						case OUTPUT_FILE_DEFAULT:
							//ファイル名を指定しない
							m_config_list[0]->cfg().compress.choose_output_file_each_time=false;
							::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_FILE),false);
							m_config_list[0]->cfg().compress.output_file.clear();
							break;

						case OUTPUT_FILE_CHOOSE:{
							//ファイル名を指定する
							FileDialog file_dialog;

							std::vector<TCHAR> old_file(MAX_PATHW);
							::GetWindowText(getDlgItem(IDC_EDIT_OUTPUT_FILE),&old_file[0],old_file.size());

							if(file_dialog.doModalSave(&m_config_list[0]->cfg().compress.output_file,
													   handle(),
													   _T("全てのファイル (*.*)\0*.*\0\0"),
													   _T("保存ファイル名を入力してください"),
													   path::getParentDirectory(&old_file[0]).c_str(),
													   path::getFileName(&old_file[0]).c_str())){
								::SetWindowText(getDlgItem(IDC_EDIT_OUTPUT_FILE),m_config_list[0]->cfg().compress.output_file.c_str());
								m_config_list[0]->cfg().compress.choose_output_file_each_time=false;
								::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_FILE),true);
								return true;
							}else{
								sendItemMessage(LOWORD(wparam),
												CB_SETCURSEL,
												(WPARAM)OUTPUT_FILE_DEFAULT,
												0
												);
							}
							break;
						}

						case OUTPUT_FILE_CHOOSE_EACH_TIME:
							//ファイル名を実行時に指定する
							m_config_list[0]->cfg().compress.choose_output_file_each_time=true;
							::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_FILE),false);
							m_config_list[0]->cfg().compress.output_file.clear();
							break;
					}
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return false;
}

void OutputTab::setCurrentSettings(){
	sendItemMessage(IDC_RADIO_OUTPUT_SOURCE,
					BM_SETCHECK,
					(WPARAM)BST_UNCHECKED,
					0
					);
	sendItemMessage(IDC_RADIO_OUTPUT_DESKTOP,
					BM_SETCHECK,
					(WPARAM)BST_UNCHECKED,
					0
					);
	sendItemMessage(IDC_RADIO_OUTPUT_SPECIFIC_DIR,
					BM_SETCHECK,
					(WPARAM)BST_UNCHECKED,
					0
					);

	std::vector<TCHAR> desktop_path(MAX_PATH);

	::SHGetSpecialFolderPath(NULL,&desktop_path[0],CSIDL_DESKTOP,false);

	if(m_config_list[0]->cfg().general.output_dir.empty()){
		//対象と同じ
		sendItemMessage(IDC_RADIO_OUTPUT_SOURCE,
						BM_SETCHECK,
						(WPARAM)BST_CHECKED,
						0
						);
		::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),false);
	}else if(m_config_list[0]->cfg().general.output_dir==&desktop_path[0]){
		//デスクトップ
		sendItemMessage(IDC_RADIO_OUTPUT_DESKTOP,
						BM_SETCHECK,
						(WPARAM)BST_CHECKED,
						0
						);
		::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),false);
	}else{
		//ディレクトリを指定
		sendItemMessage(IDC_RADIO_OUTPUT_SPECIFIC_DIR,
						BM_SETCHECK,
						(WPARAM)BST_CHECKED,
						0
						);
		::SetWindowText(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),m_config_list[0]->cfg().general.output_dir.c_str());
		::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_SPECIFIC_DIR),true);
	}

	//ファイルを指定
	if(m_config_list[0]->cfg().compress.choose_output_file_each_time){
		sendItemMessage(IDC_COMBO_OUTPUT_FILE,
						CB_SETCURSEL,
						(WPARAM)OUTPUT_FILE_CHOOSE_EACH_TIME,
						0
						);
	}else{
		if(!m_config_list[0]->cfg().compress.output_file.empty()){
			sendItemMessage(IDC_COMBO_OUTPUT_FILE,
							CB_SETCURSEL,
							(WPARAM)OUTPUT_FILE_CHOOSE,
							0
							);
			::SetWindowText(getDlgItem(IDC_EDIT_OUTPUT_FILE),m_config_list[0]->cfg().compress.output_file.c_str());
		}else{
			sendItemMessage(IDC_COMBO_OUTPUT_FILE,
							CB_SETCURSEL,
							(WPARAM)OUTPUT_FILE_DEFAULT,
							0
							);
		}
	}
	::EnableWindow(getDlgItem(IDC_EDIT_OUTPUT_FILE),
				   !m_config_list[0]->cfg().compress.choose_output_file_each_time&&
				   !m_config_list[0]->cfg().compress.output_file.empty());
}
