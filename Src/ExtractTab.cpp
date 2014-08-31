//ExtractTab.cpp
//解凍タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.0 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"ExtractTab.h"


using namespace sslib;


namespace{
	const int minimum_exclude_base_dir=-1;
	const int maximum_exclude_base_dir=256;
}



bool ExtractTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	//スピンコントロール関係
	//エディットコントロールを関連付ける
	sendItemMessage(IDC_SPIN_EXTRACT_EXCLUDE_BASE_DIR,UDM_SETBUDDY,(WPARAM)getDlgItem(IDC_EDIT_EXTRACT_EXCLUDE_BASE_DIR),(LPARAM)0);
	//最大値と最小値を設定
	sendItemMessage(IDC_SPIN_EXTRACT_EXCLUDE_BASE_DIR,UDM_SETRANGE32,(WPARAM)minimum_exclude_base_dir,(LPARAM)maximum_exclude_base_dir);
	//現在サイズの位置に移動
	sendItemMessage(IDC_SPIN_EXTRACT_EXCLUDE_BASE_DIR,UDM_SETPOS,(WPARAM)0,(LPARAM)MAKELONG(m_config_list[0]->cfg().compress.exclude_base_dir,0));

	//サイズの文字数を制限
	sendItemMessage(IDC_EDIT_EXTRACT_EXCLUDE_BASE_DIR,EM_SETLIMITTEXT,(WPARAM)3*sizeof(TCHAR),(LPARAM)0);

	setCurrentSettings();
	return true;
}

bool ExtractTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_EXTRACT_CREATE_DIR:
			//作成する
			m_config_list[0]->cfg().extract.create_dir=getCheck(LOWORD(wparam));
			::EnableWindow(getDlgItem(IDC_CHECKBOX_EXTRACT_DOUBLE_DIR),m_config_list[0]->cfg().extract.create_dir);
			::EnableWindow(getDlgItem(IDC_CHECKBOX_EXTRACT_ONLY_FILE),m_config_list[0]->cfg().extract.create_dir);
			::EnableWindow(getDlgItem(IDC_CHECKBOX_EXTRACT_OMIT_TAIL_NUMBERS),m_config_list[0]->cfg().extract.create_dir);
			::EnableWindow(getDlgItem(IDC_CHECKBOX_EXTRACT_OMIT_TAIL_SYMBOLS),m_config_list[0]->cfg().extract.create_dir);
			::EnableWindow(getDlgItem(IDC_CHECKBOX_EXTRACT_COPY_TIMESTAMP),m_config_list[0]->cfg().extract.create_dir);
			return true;

		case IDC_CHECKBOX_EXTRACT_DOUBLE_DIR:
			//二重ディレクトリを作成しない
			m_config_list[0]->cfg().extract.create_dir_optimization.remove_redundant_dir.double_dir=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_EXTRACT_ONLY_FILE:
			//含まれるファイルが一つの時は作成しない
			m_config_list[0]->cfg().extract.create_dir_optimization.remove_redundant_dir.only_file=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_EXTRACT_OMIT_TAIL_NUMBERS:
			//ディレクトリ名末尾の数字を取り除く
			m_config_list[0]->cfg().extract.create_dir_optimization.omit_number_and_symbol.number=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_EXTRACT_OMIT_TAIL_SYMBOLS:
			//ディレクトリ名末尾の記号を取り除く
			m_config_list[0]->cfg().extract.create_dir_optimization.omit_number_and_symbol.symbol=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_EXTRACT_COPY_TIMESTAMP:
			//元書庫と同じ更新日時のディレクトリを作成
			m_config_list[0]->cfg().extract.create_dir_optimization.copy_timestamp=getCheck(LOWORD(wparam));
			return true;

		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_EXTRACT_EXCLUDE_BASE_DIR:
					//共通パス除外
					//除外するパスの数
					std::vector<TCHAR> exclude_base_dir_str(8);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&exclude_base_dir_str[0],
									exclude_base_dir_str.size());

					if(exclude_base_dir_str.size()){
						int exclude_base_dir=_ttoi(&exclude_base_dir_str[0]);
						if(exclude_base_dir<minimum_exclude_base_dir){
							//最小値
							m_config_list[0]->cfg().compress.exclude_base_dir=minimum_exclude_base_dir;
						}else if(exclude_base_dir>=maximum_exclude_base_dir){
							//最大値
							m_config_list[0]->cfg().compress.exclude_base_dir=maximum_exclude_base_dir;
						}
						VariableArgument va(_T("%d"),m_config_list[0]->cfg().compress.exclude_base_dir);

						::SetWindowText(getDlgItem(LOWORD(wparam)),va.get());
					}
					return true;
			}
	}
	return false;
}

bool ExtractTab::onNotify(WPARAM wparam,LPARAM lparam){
	if(wparam==IDC_SPIN_EXTRACT_EXCLUDE_BASE_DIR){
		LPNMUPDOWN ud=reinterpret_cast<LPNMUPDOWN>(lparam);

		if(ud->hdr.code==UDN_DELTAPOS){
			int exclude_base_dir=m_config_list[0]->cfg().compress.exclude_base_dir;
			if((ud->iDelta)>0&&exclude_base_dir<maximum_exclude_base_dir){
				//上が押された
				m_config_list[0]->cfg().compress.exclude_base_dir++;
			}else if((ud->iDelta)<0&&exclude_base_dir>minimum_exclude_base_dir){
				//下が押された
				m_config_list[0]->cfg().compress.exclude_base_dir--;
			}
			VariableArgument va(_T("%d"),m_config_list[0]->cfg().compress.exclude_base_dir);

			::SetWindowText(getDlgItem(IDC_EDIT_EXTRACT_EXCLUDE_BASE_DIR),va.get());
		}
	}
	return false;
}

void ExtractTab::setCurrentSettings(){
	//作成する
	setCheck(IDC_CHECKBOX_EXTRACT_CREATE_DIR,m_config_list[0]->cfg().extract.create_dir);
	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECKBOX_EXTRACT_CREATE_DIR,0),0);

	//二重ディレクトリを作成しない
	setCheck(IDC_CHECKBOX_EXTRACT_DOUBLE_DIR,m_config_list[0]->cfg().extract.create_dir_optimization.remove_redundant_dir.double_dir);

	//含まれるファイルが一つの時は作成しない
	setCheck(IDC_CHECKBOX_EXTRACT_ONLY_FILE,m_config_list[0]->cfg().extract.create_dir_optimization.remove_redundant_dir.only_file);

	//ディレクトリ名末尾の数字を取り除く
	setCheck(IDC_CHECKBOX_EXTRACT_OMIT_TAIL_NUMBERS,m_config_list[0]->cfg().extract.create_dir_optimization.omit_number_and_symbol.number);

	//ディレクトリ名末尾の記号を取り除く
	setCheck(IDC_CHECKBOX_EXTRACT_OMIT_TAIL_SYMBOLS,m_config_list[0]->cfg().extract.create_dir_optimization.omit_number_and_symbol.symbol);

	//元書庫と同じ更新日時のディレクトリを作成
	setCheck(IDC_CHECKBOX_EXTRACT_COPY_TIMESTAMP,m_config_list[0]->cfg().extract.create_dir_optimization.copy_timestamp);

	//除外するパスの数
	VariableArgument va(_T("%d"),m_config_list[0]->cfg().compress.exclude_base_dir);

	::SetWindowText(getDlgItem(IDC_EDIT_EXTRACT_EXCLUDE_BASE_DIR),va.get());
}
