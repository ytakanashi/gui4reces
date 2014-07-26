//PasswordTab.cpp
//パスワードタブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"PasswordTab.h"


using namespace sslib;



bool PasswordTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	//メニューを読み込む
	m_write_password_list_menu=::LoadMenu(inst(),MAKEINTRESOURCE(IDR_MENU_WRITE_PASSWORD_LIST));
	m_write_password_list_sub_menu=::GetSubMenu(m_write_password_list_menu,0);

	setCurrentSettings();
	return true;
}

bool PasswordTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_PASSWORD_INPUT:
			//入力
			::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_1),
						 sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_2),
						 sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_3),
						 sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			::EnableWindow(getDlgItem(IDC_BUTTON_PASSWORD_WRITE_LIST),
						 sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			return true;

		case IDC_CHECKBOX_PASSWORD_LIST:{
			//リスト
			::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_LIST),
						   sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));

			FileDialog file_dialog;

			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)&&
			   file_dialog.doModalOpen(&m_config_list[0]->cfg().extract.password_list_path,handle(),_T("全てのファイル (*.*)\0*.*\0\0"),_T("パスワードリストを選択してください"))){
				::SetWindowText(getDlgItem(IDC_EDIT_PASSWORD_LIST),m_config_list[0]->cfg().extract.password_list_path.c_str());
			}else{
				setCheck(LOWORD(wparam),false);
				m_config_list[0]->cfg().extract.password_list_path.clear();
				::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_LIST),false);
			}
			return true;
		}

		case IDC_CHECKBOX_PASSWORD_NEW:
			//新しいパスワード
			::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_NEW),
						   sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0));
			return true;

		case IDC_CHECKBOX_PASSWORD_MASK:{
			//パスワードをマスク&&非表示(&M)
			const int password_box_list[]={
				IDC_EDIT_PASSWORD_1,
				IDC_EDIT_PASSWORD_2,
				IDC_EDIT_PASSWORD_3,
				IDC_EDIT_PASSWORD_NEW
			};

			UINT password_char=0;

			if((m_config_list[0]->cfg().no_display.no_password=(sendItemMessage(LOWORD(wparam),BM_GETCHECK,(WPARAM)0,(LPARAM)0)!=0))){
				password_char=_T('*');
			}else{
				password_char=0;
			}

			for(size_t i=0;i<ARRAY_SIZEOF(password_box_list);i++){
				sendItemMessage(password_box_list[i],
								   EM_SETPASSWORDCHAR,
								   (WPARAM)password_char,0);
				RECT rc={0};

				::GetClientRect(getDlgItem(password_box_list[i]),&rc);
				::InvalidateRect(getDlgItem(password_box_list[i]),&rc,true);
			}
			return true;
		}

		case IDC_BUTTON_PASSWORD_WRITE_LIST:{
			//リスト化
			RECT rc={0};

			::GetWindowRect(getDlgItem(LOWORD(wparam)),&rc);
			if(int id=::TrackPopupMenu(m_write_password_list_sub_menu,
									   TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL|TPM_RETURNCMD,
									   rc.left,
									   rc.bottom,
									   0,
									   handle(),
									   NULL)){

				tstring password_list_path;
				FileDialog file_dialog;

				if(!file_dialog.doModalSave(&password_list_path,
											handle(),
											_T("全てのファイル (*.*)\0*.*\0\0"),
											_T("保存ファイル名を入力してください"))){
					return true;
				}


				std::vector<tstring> password_list;

				getPasswordList(&password_list);
				if(!password_list.empty()){
					File password_list_file(password_list_path.c_str(),
											(id==IDM_ADD_PASSWORD_LIST)?OPEN_ALWAYS:CREATE_ALWAYS,
											GENERIC_READ|GENERIC_WRITE,
											0,
											File::UTF16LE);

					if(id==IDM_ADD_PASSWORD_LIST){
						//追加保存
						password_list_file.seek(0,FILE_END);
						password_list_file.writeEx(_T("\r\n"));
					}

					for(std::vector<tstring>::iterator ite=password_list.begin(),end=password_list.end();ite!=end;++ite){
						password_list_file.writeEx(_T("%s\r\n"),ite->c_str());
					}
				}
				return true;
			}
		}
	}
	return false;
}

bool PasswordTab::onDestroy(){
	::DestroyMenu(m_write_password_list_menu);
	return true;
}

void PasswordTab::setCurrentSettings(){
	::SetWindowText(getDlgItem(IDC_EDIT_PASSWORD_LIST),m_config_list[0]->cfg().extract.password_list_path.c_str());
	setCheck(IDC_CHECKBOX_PASSWORD_LIST,!m_config_list[0]->cfg().extract.password_list_path.empty());
	::EnableWindow(getDlgItem(IDC_EDIT_PASSWORD_LIST),sendItemMessage(IDC_CHECKBOX_PASSWORD_LIST,BM_GETCHECK,0,0));

	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECKBOX_PASSWORD_INPUT,0),0);
	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECKBOX_PASSWORD_NEW,0),0);

	::EnableWindow(getDlgItem(IDC_BUTTON_PASSWORD_WRITE_LIST),sendItemMessage(IDC_CHECKBOX_PASSWORD_INPUT,BM_GETCHECK,(WPARAM)0,(LPARAM)0));

	setCheck(IDC_CHECKBOX_PASSWORD_MASK,m_config_list[0]->cfg().no_display.no_password);
	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECKBOX_PASSWORD_MASK,0),0);
}

bool PasswordTab::getPasswordList(std::vector<tstring>* list){
	if(!sendItemMessage(IDC_CHECKBOX_PASSWORD_INPUT,BM_GETCHECK,0,0))return false;

	std::vector<TCHAR> buffer(1024);
	const int password_box_list[]={
		IDC_EDIT_PASSWORD_1,
		IDC_EDIT_PASSWORD_2,
		IDC_EDIT_PASSWORD_3};

	for(size_t i=0;i<ARRAY_SIZEOF(password_box_list);i++){
		::GetWindowText(getDlgItem(password_box_list[i]),
						&buffer[0],
						buffer.size());

		if(lstrcmp(&buffer[0],_T(""))!=0)list->push_back(&buffer[0]);
	}
	return !list->empty();
}

bool PasswordTab::getNewPassword(tstring* new_password){
	if(!sendItemMessage(IDC_CHECKBOX_PASSWORD_NEW,BM_GETCHECK,0,0)||
	   new_password==NULL)return false;

	std::vector<TCHAR> buffer(1024);

	::GetWindowText(getDlgItem(IDC_EDIT_PASSWORD_NEW),
					&buffer[0],
					buffer.size());
	new_password->assign(&buffer[0]);
	return !new_password->empty();
}
