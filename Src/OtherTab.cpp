//OtherTab.cpp
//その他タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.5 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"OtherTab.h"


using namespace sslib;


namespace{
	const TCHAR* library_name_list[]={
		_T("指定しない"),
#ifndef _WIN64
		_T("7-zip32.dll"),
		_T("Unlha32.dll"),
		_T("unrar32.dll"),
		_T("tar32.dll"),
		_T("UnIso32.dll"),
		_T("XacRett.dll"),
		_T("Total7zip.wcx"),
#else
		_T("7-zip64.dll"),
		_T("UNBYPASS.DLL"),
		_T("unrar64j.dll"),
		_T("tar64.dll"),
		_T("ZBYPASSA.SPH"),
		_T("Total7zip.wcx64"),
#endif
	};

	const TCHAR* plugin_type_list[]={
		_T("SPI"),
		_T("WCX")
	};
	tstring* plugin_dir_ptr=NULL;
	int plugin_type=0;
	inline bool susie_plugin(){return plugin_type==0;}
}


INT_PTR OtherTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	for(size_t i=0;i<ARRAY_SIZEOF(library_name_list);i++){
		sendItemMessage(IDC_COMBO_OTHER_LIBRARY_NAME,
						CB_ADDSTRING,
						0,
						(LPARAM)library_name_list[i]);
	}

	plugin_dir_ptr=NULL;
	plugin_type=0;
	::SetWindowText(getDlgItem(IDC_BUTTON_PLUGIN_DIR),plugin_type_list[plugin_type]);

	setCurrentSettings();
	return true;
}

INT_PTR OtherTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_BUTTON_OTHER_LIBRARY_SETTINGS:{
			//ライブラリの設定
			std::vector<TCHAR> library_name_str(MAX_PATH);

			::GetWindowText(getDlgItem(IDC_COMBO_OTHER_LIBRARY_NAME),
							&library_name_str[0],
							library_name_str.size());

			if(lstrlen(&library_name_str[0])){
				tstring cmd(format(_T("reces.exe /mS%s%s%s"),
								   str::containsWhiteSpace(&library_name_str[0])?_T("\""):_T(""),
								   path::removeTailSlash(&library_name_str[0]).c_str(),
								   (str::containsWhiteSpace(&library_name_str[0]))?_T("\""):_T("")));

				if(!m_config_list[0]->cfg().general.spi_dir.empty()){
					cmd.append(format(_T(" /Ds%s%s%s"),
									  (str::containsWhiteSpace(m_config_list[0]->cfg().general.spi_dir))?_T("\""):_T(""),
									  path::removeTailSlash(m_config_list[0]->cfg().general.spi_dir).c_str(),
									  (str::containsWhiteSpace(m_config_list[0]->cfg().general.spi_dir))?_T("\""):_T("")));
				}

				if(!m_config_list[0]->cfg().general.wcx_dir.empty()){
					cmd.append(format(_T(" /Dw%s%s%s"),
									  (str::containsWhiteSpace(m_config_list[0]->cfg().general.wcx_dir))?_T("\""):_T(""),
									  path::removeTailSlash(m_config_list[0]->cfg().general.wcx_dir).c_str(),
									  (str::containsWhiteSpace(m_config_list[0]->cfg().general.wcx_dir))?_T("\""):_T("")));
				}

				TCHAR* cmd_buffer=new TCHAR[cmd.length()+1];

				lstrcpy(cmd_buffer,cmd.c_str());

				STARTUPINFO startup_info={};
				startup_info.cb=sizeof(STARTUPINFO);
				PROCESS_INFORMATION process_info={};
				startup_info.dwFlags=STARTF_USESHOWWINDOW;
				startup_info.wShowWindow=SW_SHOWNORMAL;
				::CreateProcess(NULL,cmd_buffer,NULL,NULL,false,0,NULL,NULL,&startup_info,&process_info);
				::WaitForSingleObject(process_info.hProcess,INFINITE);
				SAFE_CLOSE(process_info.hThread);
				SAFE_CLOSE(process_info.hProcess);
			}
			return true;
		}

		case IDC_CHECKBOX_OTHER_PLUGIN_DIR:{
			//プラグインディレクトリ
			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				FolderDialog folder_dialog;

				std::vector<TCHAR> old_dir(MAX_PATHW);
				::GetWindowText(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),&old_dir[0],old_dir.size());

				if(folder_dialog.doModalOpen(plugin_dir_ptr,
											 handle(),
											 (susie_plugin())?
											 _T("Susie Plug-inのあるディレクトリを選択してください"):
											 _T("Total Commander Pluginのあるディレクトリを選択してください"),
											 &old_dir[0])){
					::SetWindowText(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),plugin_dir_ptr->c_str());
					::EnableWindow(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),true);
					return true;
				}
			}
			setCheck(IDC_CHECKBOX_OTHER_PLUGIN_DIR,false);
			plugin_dir_ptr->clear();
			::EnableWindow(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),false);
			return true;
		}

		case IDC_BUTTON_PLUGIN_DIR:
			//プラグインの種類
			plugin_type^=1;
			::SetWindowText(getDlgItem(IDC_BUTTON_PLUGIN_DIR),plugin_type_list[plugin_type]);
			plugin_dir_ptr=(susie_plugin())?
				&m_config_list[0]->cfg().general.spi_dir:
				&m_config_list[0]->cfg().general.wcx_dir;
			setCurrentSettings();
			break;

		case IDC_CHECKBOX_OTHER_CUSTOM_PARAM:{
			//追加するパラメータ
			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				::EnableWindow(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),true);
				return true;
			}
			setCheck(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,false);
			m_config_list[0]->cfg().general.custom_param.clear();
			::EnableWindow(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),false);
			return true;
		}

		case IDC_CHECKBOX_OTHER_WORK_DIR:{
			//作業ディレクトリ
			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				FolderDialog folder_dialog;

				std::vector<TCHAR> old_dir(MAX_PATHW);
				::GetWindowText(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),&old_dir[0],old_dir.size());

				if(folder_dialog.doModalOpen(&m_config_list[0]->cfg().gui4reces.work_dir,
											 handle(),
											 _T("作業ディレクトリを選択してください"),
											 &old_dir[0])){
					::SetWindowText(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),m_config_list[0]->cfg().gui4reces.work_dir.c_str());
					::EnableWindow(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),true);
					return true;
				}
			}
			setCheck(IDC_CHECKBOX_OTHER_WORK_DIR,false);
			m_config_list[0]->cfg().gui4reces.work_dir.clear();
			::EnableWindow(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),false);
			return true;
		}

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case CBN_EDITCHANGE:
		case CBN_SELCHANGE:{
			switch(LOWORD(wparam)){
				case IDC_COMBO_OTHER_LIBRARY_NAME:{
					//使用するライブラリを指定
					std::vector<TCHAR> library_name_str(MAX_PATH);
					int index=sendItemMessage(LOWORD(wparam),CB_GETCURSEL,(WPARAM)0,0);

					if(HIWORD(wparam)==CBN_EDITCHANGE){
						::GetWindowText(getDlgItem(LOWORD(wparam)),
										&library_name_str[0],
										library_name_str.size());
					}else{
						sendItemMessage(LOWORD(wparam),CB_GETLBTEXT,(WPARAM)index,(LPARAM)&library_name_str[0]);
					}
					if(lstrcmp(&library_name_str[0],library_name_list[0])==0){
						lstrcpy(&library_name_str[0],_T(""));
					}
					m_config_list[0]->cfg().general.selected_library_name.assign(&library_name_str[0]);
					return true;
				}

				default:
					break;
			}
			break;
		}

		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_OTHER_PLUGIN_DIR:{
					//プラグインディレクトリ
					std::vector<TCHAR> dir(MAX_PATHW);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&dir[0],
									dir.size());
					plugin_dir_ptr->assign(&dir[0]);
					break;
				}

				case IDC_EDIT_OTHER_CUSTOM_PARAM:{
					//追加するパラメータ
					std::vector<TCHAR> custom_param(2048);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&custom_param[0],
									custom_param.size());
					m_config_list[0]->cfg().general.custom_param.assign(&custom_param[0]);
					break;
				}

				case IDC_EDIT_OTHER_WORK_DIR:{
					//作業ディレクトリ
					std::vector<TCHAR> dir(MAX_PATHW);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&dir[0],
									dir.size());
					m_config_list[0]->cfg().gui4reces.work_dir.assign(&dir[0]);
					break;
				}

				default:
					break;
			}
			break;
	}

	return false;
}

void OtherTab::setCurrentSettings(){
	//使用するライブラリを指定
	if(!m_config_list[0]->cfg().general.selected_library_name.empty()){
		bool found=false;

		for(size_t i=0;i<ARRAY_SIZEOF(library_name_list);i++){
			if(m_config_list[0]->cfg().general.selected_library_name==library_name_list[i]){
				sendItemMessage(IDC_COMBO_OTHER_LIBRARY_NAME,
								CB_SETCURSEL,
								(WPARAM)i,
								0
								);
				found=true;
			}
		}

		if(!found){
			::SetWindowText(getDlgItem(IDC_COMBO_OTHER_LIBRARY_NAME),
							m_config_list[0]->cfg().general.selected_library_name.c_str());
		}
	}else{
		sendItemMessage(IDC_COMBO_OTHER_LIBRARY_NAME,
						CB_SETCURSEL,
						(WPARAM)0,
						0
						);
	}


	//プラグインディレクトリ
	plugin_dir_ptr=(susie_plugin())?
		&m_config_list[0]->cfg().general.spi_dir:
		&m_config_list[0]->cfg().general.wcx_dir;
	if(!plugin_dir_ptr->empty()){
		setCheck(IDC_CHECKBOX_OTHER_PLUGIN_DIR,true);

		::SetWindowText(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),plugin_dir_ptr->c_str());
	}else{
		setCheck(IDC_CHECKBOX_OTHER_PLUGIN_DIR,false);
	}
	::EnableWindow(getDlgItem(IDC_EDIT_OTHER_PLUGIN_DIR),sendItemMessage(IDC_CHECKBOX_OTHER_PLUGIN_DIR,BM_GETCHECK,0,0));

	//追加するパラメータ
	if(!m_config_list[0]->cfg().general.custom_param.empty()){
		setCheck(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,true);

		::SetWindowText(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),m_config_list[0]->cfg().general.custom_param.c_str());
	}else{
		setCheck(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,false);
	}
	::EnableWindow(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),sendItemMessage(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,BM_GETCHECK,0,0));

	//作業ディレクトリ
	if(m_config_list[0]->cfg().gui4reces.work_dir!=
	   m_config_list[0]->default_cfg().gui4reces.work_dir){
		setCheck(IDC_CHECKBOX_OTHER_WORK_DIR,true);

		::SetWindowText(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),m_config_list[0]->cfg().gui4reces.work_dir.c_str());
	}else{
		setCheck(IDC_CHECKBOX_OTHER_WORK_DIR,false);
	}
	::EnableWindow(getDlgItem(IDC_EDIT_OTHER_WORK_DIR),sendItemMessage(IDC_CHECKBOX_OTHER_WORK_DIR,BM_GETCHECK,0,0));
}
