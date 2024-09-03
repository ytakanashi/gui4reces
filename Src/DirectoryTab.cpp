//DirectoryTab.cpp
//ディレクトリタブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"DirectoryTab.h"

#include"Utilities.h"


using namespace sslib;



namespace dir_ctrl{
	enum{
		DLL,B2E,SPI,WCX
	};
	struct ID_TABLE{
		const unsigned int chkbox_id;
		const unsigned int edit_id;
		tstring* dir;
	}table[]={
		{IDC_CHECKBOX_DIRECTORY_DLL,IDC_EDIT_DIRECTORY_DLL,NULL},
		{IDC_CHECKBOX_DIRECTORY_B2E,IDC_EDIT_DIRECTORY_B2E,NULL},
		{IDC_CHECKBOX_DIRECTORY_SPI,IDC_EDIT_DIRECTORY_SPI,NULL},
		{IDC_CHECKBOX_DIRECTORY_WCX,IDC_EDIT_DIRECTORY_WCX,NULL},
		{0,0,NULL}
	};
	const unsigned int getIdFromCheck(const unsigned int chkbox){
		int i=0;
		for(;table[i].chkbox_id!=0;i++)
			if(chkbox==table[i].chkbox_id)
				break;
		return i;
	}
	const unsigned int getIdFromEdit(const unsigned int edit){
		int i=0;
		for(;table[i].chkbox_id!=0;i++)
			if(edit==table[i].edit_id)
				break;
		return i;
	}
}

INT_PTR DirectoryTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	dir_ctrl::table[dir_ctrl::DLL].dir=&m_config_list[0]->cfg().general.dll_dir;
	dir_ctrl::table[dir_ctrl::B2E].dir=&m_config_list[0]->cfg().general.b2e_dir;
	dir_ctrl::table[dir_ctrl::SPI].dir=&m_config_list[0]->cfg().general.spi_dir;
	dir_ctrl::table[dir_ctrl::WCX].dir=&m_config_list[0]->cfg().general.wcx_dir;
	setCurrentSettings();
	return true;
}

INT_PTR DirectoryTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_DIRECTORY_DLL:
		case IDC_CHECKBOX_DIRECTORY_B2E:
		case IDC_CHECKBOX_DIRECTORY_SPI:
		case IDC_CHECKBOX_DIRECTORY_WCX:{
			//プラグインディレクトリ
			unsigned control_id=dir_ctrl::getIdFromCheck(LOWORD(wparam));

			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				FolderDialog folder_dialog;

				std::vector<TCHAR> old_dir(MAX_PATHW);
				::GetWindowText(getDlgItem(dir_ctrl::table[control_id].edit_id),&old_dir[0],old_dir.size());

				if(lstrcmp(&old_dir[0],_T(""))==0){
					//gui4reces.exeのあるディレクトリ
					lstrcpy(&old_dir[0],path::getExeDirectory().c_str());
				}

				if(folder_dialog.doModalOpen(dir_ctrl::table[control_id].dir,
											 handle(),
											 (tstring(
													 (control_id==dir_ctrl::DLL)?
													 _T("統合アーカイバライブラリ (*.dll)"):
													 (control_id==dir_ctrl::B2E)?
													 _T("B2E Script (*.b2e)"):
													 (control_id==dir_ctrl::SPI)?
#ifndef _WIN64
														 _T("Susie Plug-in (*.spi)"):
														 _T("Total Commander Plugin (*.wcx)"))+
#else
														 _T("Susie Plug-in (*.spi, *.sph)"):
														 _T("Total Commander Plugin (*.wcx, *.wcx64)"))+
#endif
											 _T("のあるディレクトリを選択してください")).c_str(),
											 &old_dir[0])){
					::SetWindowText(getDlgItem(dir_ctrl::table[control_id].edit_id),dir_ctrl::table[control_id].dir->c_str());
					::EnableWindow(getDlgItem(dir_ctrl::table[control_id].edit_id),true);
					return true;
				}
			}
			setCheck(LOWORD(wparam),false);
			dir_ctrl::table[control_id].dir->clear();
			::EnableWindow(getDlgItem(dir_ctrl::table[control_id].edit_id),false);
			return true;
		}

		case IDC_CHECKBOX_DIRECTORY_WORK:{
			//作業ディレクトリ
			if(sendItemMessage(LOWORD(wparam),BM_GETCHECK,0,0)){
				FolderDialog folder_dialog;

				std::vector<TCHAR> old_dir(MAX_PATHW);
				::GetWindowText(getDlgItem(IDC_EDIT_DIRECTORY_WORK),&old_dir[0],old_dir.size());

				if(folder_dialog.doModalOpen(&m_config_list[0]->cfg().gui4reces.work_dir,
											 handle(),
											 _T("作業ディレクトリを選択してください"),
											 &old_dir[0])){
					::SetWindowText(getDlgItem(IDC_EDIT_DIRECTORY_WORK),m_config_list[0]->cfg().gui4reces.work_dir.c_str());
					::EnableWindow(getDlgItem(IDC_EDIT_DIRECTORY_WORK),true);
					return true;
				}
			}
			setCheck(IDC_CHECKBOX_DIRECTORY_WORK,false);
			m_config_list[0]->cfg().gui4reces.work_dir.clear();
			::EnableWindow(getDlgItem(IDC_EDIT_DIRECTORY_WORK),false);
			return true;
		}

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_DIRECTORY_DLL:
				case IDC_EDIT_DIRECTORY_B2E:
				case IDC_EDIT_DIRECTORY_SPI:
				case IDC_EDIT_DIRECTORY_WCX:{
					//プラグインディレクトリ
					unsigned control_id=dir_ctrl::getIdFromEdit(LOWORD(wparam));
					std::vector<TCHAR> dir(MAX_PATHW);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&dir[0],
									dir.size());
					dir_ctrl::table[control_id].dir->assign(&dir[0]);
					break;
				}

				case IDC_EDIT_DIRECTORY_WORK:{
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

void DirectoryTab::setCurrentSettings(){
	//プラグインディレクトリ
	for(int i=0;dir_ctrl::table[i].chkbox_id!=0;i++){
		if(!dir_ctrl::table[i].dir->empty()){
			setCheck(dir_ctrl::table[i].chkbox_id,true);

			::SetWindowText(getDlgItem(dir_ctrl::table[i].edit_id),dir_ctrl::table[i].dir->c_str());
		}else{
			setCheck(dir_ctrl::table[i].chkbox_id,false);
		}
		::EnableWindow(getDlgItem(dir_ctrl::table[i].edit_id),
					   sendItemMessage(dir_ctrl::table[i].chkbox_id,BM_GETCHECK,0,0));
	}

	//作業ディレクトリ
	if(m_config_list[0]->cfg().gui4reces.work_dir!=
	   m_config_list[0]->default_cfg().gui4reces.work_dir){
		setCheck(IDC_CHECKBOX_DIRECTORY_WORK,true);

		::SetWindowText(getDlgItem(IDC_EDIT_DIRECTORY_WORK),m_config_list[0]->cfg().gui4reces.work_dir.c_str());
	}else{
		setCheck(IDC_CHECKBOX_DIRECTORY_WORK,false);
	}
	::EnableWindow(getDlgItem(IDC_EDIT_DIRECTORY_WORK),sendItemMessage(IDC_CHECKBOX_DIRECTORY_WORK,BM_GETCHECK,0,0));
}
