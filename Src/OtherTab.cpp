//OtherTab.cpp
//その他タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.5 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"OtherTab.h"

#include"Utilities.h"


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
		_T("b2e32.dll"),
#else
		_T("7-zip64.dll"),
		_T("UNBYPASS.DLL"),
		_T("unrar64j.dll"),
		_T("tar64.dll"),
		_T("ZBYPASSA.SPH"),
		_T("Total7zip.wcx64"),
		_T("b2e64.dll"),
#endif
	};
	struct CODEPAGE_DATA{
		unsigned int codepage;
		tstring name;
		CODEPAGE_DATA():codepage(0),name(){}
		CODEPAGE_DATA(unsigned int _codepage,tstring& _name):codepage(_codepage),name(_name){}
	};
	std::vector<CODEPAGE_DATA> codepage_table;
}


INT_PTR OtherTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	for(size_t i=0;i<ARRAY_SIZEOF(library_name_list);i++){
		sendItemMessage(IDC_COMBO_OTHER_LIBRARY_NAME,
						CB_ADDSTRING,
						0,
						(LPARAM)library_name_list[i]);
	}

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
				tstring cmd(format(_T("reces.exe /mS%s"),
								   path::quote(path::removeTailSlash(&library_name_str[0])).c_str()));

				if(!m_config_list[0]->cfg().general.b2e_dir.empty()){
					cmd.append(format(_T(" /Db%s"),
									  path::quote(path::removeTailSlash(m_config_list[0]->cfg().general.b2e_dir)).c_str()));
				}

				if(!m_config_list[0]->cfg().general.spi_dir.empty()){
					cmd.append(format(_T(" /Ds%s"),
									  path::quote(path::removeTailSlash(m_config_list[0]->cfg().general.spi_dir)).c_str()));
				}

				if(!m_config_list[0]->cfg().general.wcx_dir.empty()){
					cmd.append(format(_T(" /Dw%s"),
									  path::quote(path::removeTailSlash(m_config_list[0]->cfg().general.wcx_dir)).c_str()));
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

		case IDC_CHECKBOX_OTHER_SFX:
			//SFX
			m_config_list[0]->cfg().compress.b2e.sfx=getCheck(LOWORD(wparam));
			return true;

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

					if(str::isEqualStringIgnoreCase(path::getFileName(m_config_list[0]->cfg().general.selected_library_name),
#ifndef _WIN64
											 _T("b2e32.dll")
#else
											 _T("b2e64.dll")
#endif
													)){
						util::PipeRedirect pipe;
						std::list<tstring> method_list;
						tstring cmd(_T("reces "));

						if(!m_config_list[0]->cfg().general.b2e_dir.empty()){
							cmd.append(format(_T(" /Db%s "),
											  path::quote(path::removeTailSlash(m_config_list[0]->cfg().general.b2e_dir)).c_str()));
						}

						cmd.append(_T("/mv b2e"));

						pipe.launch(cmd.c_str());

						str::splitString(&method_list,pipe.result().c_str(),_T("\r\n"));

						UINT cur_sel=sendItemMessage(IDC_COMBO_OTHER_METHOD,CB_GETCURSEL,0,0);

						if(cur_sel==CB_ERR)cur_sel=0;

						sendItemMessage(IDC_COMBO_OTHER_METHOD,CB_RESETCONTENT,0,0);
						for(std::list<tstring>::iterator ite=method_list.begin(),
							end=method_list.end();
							ite!=end;
							++ite){
							if(!ite->empty()){
								sendItemMessage(IDC_COMBO_OTHER_METHOD,
												CB_ADDSTRING,
												0,
												(LPARAM)ite->c_str());
							}
						}
						sendItemMessage(IDC_COMBO_OTHER_METHOD,CB_SETCURSEL,cur_sel,0);
						::EnableWindow(getDlgItem(IDC_COMBO_OTHER_METHOD),true);
						::EnableWindow(getDlgItem(IDC_CHECKBOX_OTHER_SFX),true);
					}else{
						::EnableWindow(getDlgItem(IDC_COMBO_OTHER_METHOD),false);
						::EnableWindow(getDlgItem(IDC_CHECKBOX_OTHER_SFX),false);
						m_config_list[0]->cfg().compress.b2e.format.clear();
						m_config_list[0]->cfg().compress.b2e.method.clear();
					}
					return true;
				}

				case IDC_COMBO_OTHER_METHOD:{
					//b2eスクリプトのformatとmethodを指定
					std::vector<TCHAR> method_str(128);
					int index=sendItemMessage(LOWORD(wparam),CB_GETCURSEL,(WPARAM)0,0);

					if(HIWORD(wparam)==CBN_EDITCHANGE){
						::GetWindowText(getDlgItem(LOWORD(wparam)),
										&method_str[0],
										method_str.size());
					}else{
						sendItemMessage(LOWORD(wparam),CB_GETLBTEXT,(WPARAM)index,(LPARAM)&method_str[0]);
					}

					m_config_list[0]->cfg().compress.b2e.format.assign(&method_str[0]);
					tstring::size_type pos=m_config_list[0]->cfg().compress.b2e.format.rfind(_T(":"));

					if(pos!=tstring::npos){
						//圧縮メソッド
						m_config_list[0]->cfg().compress.b2e.method=m_config_list[0]->cfg().compress.b2e.format.substr(pos+1);
						//圧縮形式
						m_config_list[0]->cfg().compress.b2e.format=m_config_list[0]->cfg().compress.b2e.format.substr(0,pos);
					}
					return true;
				}

				case IDC_COMBO_OTHER_CODEPAGE:{
					//codepageを取得
					int index=sendItemMessage(LOWORD(wparam),CB_GETCURSEL,(WPARAM)0,0);

					if(index){
						m_config_list[0]->cfg().general.arc_codepage=codepage_table[index].codepage;
					}else{
						m_config_list[0]->cfg().general.arc_codepage=0;
					}
					return true;
				}

				default:
					break;
			}
			break;
		}

		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_OTHER_CUSTOM_PARAM:{
					//追加するパラメータ
					std::vector<TCHAR> custom_param(2048);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&custom_param[0],
									custom_param.size());
					m_config_list[0]->cfg().general.custom_param.assign(&custom_param[0]);
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

		if(str::isEqualStringIgnoreCase(path::getFileName(m_config_list[0]->cfg().general.selected_library_name),
#ifndef _WIN64
											 _T("b2e32.dll")
#else
											 _T("b2e64.dll")
#endif
										)){
			//b2e32.dllの設定
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_OTHER_LIBRARY_NAME,CBN_SELCHANGE),0);

			tstring b2e_format(m_config_list[0]->cfg().compress.b2e.format);
			std::vector<TCHAR> combo_string(MAX_PATH);

			if(!m_config_list[0]->cfg().compress.b2e.method.empty()){
				b2e_format+=_T(":")+m_config_list[0]->cfg().compress.b2e.method;
			}

			UINT sel=sendItemMessage(IDC_COMBO_OTHER_METHOD,
									 CB_FINDSTRINGEXACT,
									 0,
									 (LPARAM)b2e_format.c_str());

			sendItemMessage(IDC_COMBO_OTHER_METHOD,
							CB_SETCURSEL,
							(sel!=CB_ERR)?sel:0,
							(LPARAM)0);
		}
	}else{
		sendItemMessage(IDC_COMBO_OTHER_LIBRARY_NAME,
						CB_SETCURSEL,
						(WPARAM)0,
						0
						);
	}
	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_OTHER_LIBRARY_NAME,CBN_SELCHANGE),0);

	//SFX
	setCheck(IDC_CHECKBOX_OTHER_SFX,m_config_list[0]->cfg().compress.b2e.sfx);

	//文字コード
	util::PipeRedirect pipe;
	std::list<tstring> codepage_list;
	tstring cmd(_T("reces /mv cp"));

	pipe.launch(cmd.c_str());

	str::splitString(&codepage_list,pipe.result().c_str(),_T("\r\n"));

	//「自動選択」を追加
	CODEPAGE_DATA autodetect_all(50001,tstring(_T("自動選択")));

	codepage_table.push_back(autodetect_all);
	sendItemMessage(IDC_COMBO_OTHER_CODEPAGE,
					CB_ADDSTRING,
					0,
					(LPARAM)autodetect_all.name.c_str());

	int combo_count=1;

	for(std::list<tstring>::iterator ite=codepage_list.begin(),
		end=codepage_list.end();
		ite!=end;
		++ite){
		if(!ite->empty()){
			CODEPAGE_DATA data;

			tstring::size_type index=ite->find(_T(":"));
			data.codepage=_ttoi(ite->substr(0,index).c_str());

			//「自動選択」は追加済み
			if(data.codepage==50001)continue;

			data.name=ite->substr(index+1);
			data.name=data.name.substr(0,data.name.rfind(_T(":")));
			codepage_table.push_back(data);

			sendItemMessage(IDC_COMBO_OTHER_CODEPAGE,
							CB_ADDSTRING,
							0,
							(LPARAM)data.name.c_str());

			if(m_config_list[0]->cfg().general.arc_codepage==data.codepage){
				sendItemMessage(IDC_COMBO_OTHER_CODEPAGE,CB_SETCURSEL,combo_count,0);
			}
			combo_count++;
		}
	}
	if(sendItemMessage(IDC_COMBO_OTHER_CODEPAGE,CB_GETCURSEL,(WPARAM)0,0)==CB_ERR){
		sendItemMessage(IDC_COMBO_OTHER_CODEPAGE,CB_SETCURSEL,0,0);
	}

	//追加するパラメータ
	if(!m_config_list[0]->cfg().general.custom_param.empty()){
		setCheck(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,true);

		::SetWindowText(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),m_config_list[0]->cfg().general.custom_param.c_str());
	}else{
		setCheck(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,false);
	}
	::EnableWindow(getDlgItem(IDC_EDIT_OTHER_CUSTOM_PARAM),sendItemMessage(IDC_CHECKBOX_OTHER_CUSTOM_PARAM,BM_GETCHECK,0,0));
}
