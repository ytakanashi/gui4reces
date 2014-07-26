//SplitTab.cpp
//分割タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"SplitTab.h"


using namespace sslib;


namespace{
	//最小分割数
	const int minimum_split_chunk=2;
	//最大分割数(適当)
	const int maximum_split_chunk=999999;

	const TCHAR* split_size_list[]={
		_T("分割しない"),
		_T("任意のサイズ"),
		_T("2MB"),
		_T("4MB"),
		_T("20MB"),
		_T("30MB"),
		_T("60MB"),
		_T("100MB"),
		_T("150MB"),
		_T("300MB"),
		_T("1GB")
	};

	struct SIZE_UNITS_TABLE{
		const int id;
		const TCHAR* units;
	}const size_units_table[]={
		{IDM_SIZE_B,_T("B")},
		{IDM_SIZE_KB,_T("KB")},
		{IDM_SIZE_MB,_T("MB")},
		{IDM_SIZE_GB,_T("GB")},
		{IDM_SIZE_TB,_T("TB")}
	};
}


bool SplitTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	for(size_t i=0;i<ARRAY_SIZEOF(split_size_list);i++){
		sendItemMessage(IDC_COMBO_SPLIT_SIZE,
						CB_ADDSTRING,
						0,
						(LPARAM)split_size_list[i]);
	}

	//スピンコントロール関係
	//エディットコントロールを関連付ける
	sendItemMessage(IDC_SPIN_SPLIT_CHUNK,UDM_SETBUDDY,(WPARAM)getDlgItem(IDC_EDIT_SPLIT_CHUNK),(LPARAM)0);
	//最大値と最小値を設定
	sendItemMessage(IDC_SPIN_SPLIT_CHUNK,UDM_SETRANGE32,(WPARAM)minimum_split_chunk,(LPARAM)maximum_split_chunk);
	//現在サイズの位置に移動
	sendItemMessage(IDC_SPIN_SPLIT_CHUNK,UDM_SETPOS,(WPARAM)0,(LPARAM)MAKELONG(_ttoi(m_config_list[0]->cfg().compress.split_value.c_str()),0));

	//サイズの文字数を制限
	sendItemMessage(IDC_EDIT_SPLIT_SIZE,EM_SETLIMITTEXT,(WPARAM)20*sizeof(TCHAR),(LPARAM)0);

	//サイズメニューを読み込む
	m_size_menu=::LoadMenu(inst(),MAKEINTRESOURCE(IDR_MENU_FILTER_SIZE));
	m_size_sub_menu=::GetSubMenu(m_size_menu,0);

	setCurrentSettings();
	return true;
}

bool SplitTab::onDestroy(){
	::DestroyMenu(m_size_menu);
	return true;
}

bool SplitTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_BUTTON_SPLIT_SIZE:{
			//サイズボタン
			RECT rc={0};

			::GetWindowRect(getDlgItem(LOWORD(wparam)),&rc);
			if(int id=::TrackPopupMenu(m_size_sub_menu,
									   TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL|TPM_RETURNCMD,
									   rc.left,
									   rc.bottom,
									   0,
									   handle(),
									   NULL)){

				size_t i=0;

				for(;i<ARRAY_SIZEOF(size_units_table);i++){
					if(size_units_table[i].id==id){
						::SetWindowText(getDlgItem(LOWORD(wparam)),size_units_table[i].units);
						break;
					}
				}

				std::vector<TCHAR> size(64);

				::GetWindowText(getDlgItem(IDC_EDIT_SPLIT_SIZE),
								&size[0],
								size.size());
				if(size.size()){
					m_config_list[0]->cfg().compress.split_value.assign(&size[0]);
					m_config_list[0]->cfg().compress.split_value.append(size_units_table[i].units);
				}
			}
			return true;
		}

		case IDC_RADIO_SPLIT_SIZE:
			//分割サイズ指定
			m_config_list[0]->cfg().compress.split_value.clear();
			::EnableWindow(getDlgItem(IDC_COMBO_SPLIT_SIZE),true);
			::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),true);
			::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_CHUNK),false);
			::EnableWindow(getDlgItem(IDC_SPIN_SPLIT_CHUNK),false);
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_SPLIT_SIZE,CBN_SELCHANGE),0);
			return true;

		case IDC_RADIO_SPLIT_CHUNK:{
			//分割数指定
			std::vector<TCHAR> chunk(64);

			::GetWindowText(getDlgItem(IDC_EDIT_SPLIT_CHUNK),
							&chunk[0],
							chunk.size());

			if(lstrcmp(&chunk[0],_T(""))==0){
				m_config_list[0]->cfg().compress.split_value.assign(_T("2"));
				::SetWindowText(getDlgItem(IDC_EDIT_SPLIT_CHUNK),
								m_config_list[0]->cfg().compress.split_value.c_str());
			}else{
				m_config_list[0]->cfg().compress.split_value.assign(&chunk[0]);
			}

			::EnableWindow(getDlgItem(IDC_COMBO_SPLIT_SIZE),false);
			::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),false);
			::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_CHUNK),true);
			::EnableWindow(getDlgItem(IDC_SPIN_SPLIT_CHUNK),true);
			return true;
		}

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case CBN_SELCHANGE:{
			switch(LOWORD(wparam)){
				case IDC_COMBO_SPLIT_SIZE:{
					std::vector<TCHAR> split_size_str(12);

					int index=sendItemMessage(LOWORD(wparam),CB_GETCURSEL,(WPARAM)0,0);
					sendItemMessage(LOWORD(wparam),CB_GETLBTEXT,(WPARAM)index,(LPARAM)&split_size_str[0]);

					::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),false);
					::EnableWindow(getDlgItem(IDC_BUTTON_SPLIT_SIZE),false);

					if(lstrcmp(&split_size_str[0],_T("分割しない"))==0){
						m_config_list[0]->cfg().compress.split_value.clear();
					}else if(lstrcmp(&split_size_str[0],_T("任意のサイズ"))==0){
						std::vector<TCHAR> buffer(1024);

						::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),true);
						::GetWindowText(getDlgItem(IDC_EDIT_SPLIT_SIZE),&buffer[0],buffer.size());
						::EnableWindow(getDlgItem(IDC_BUTTON_SPLIT_SIZE),true);
					}else{
						m_config_list[0]->cfg().compress.split_value.assign(&split_size_str[0]);
					}
					return true;
				}

				default:
					break;
			}
		}

		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_SPLIT_SIZE:{
					std::vector<TCHAR> size(64),unit(12);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&size[0],
									size.size());
					::GetWindowText(getDlgItem(IDC_BUTTON_SPLIT_SIZE),
									&unit[0],
									unit.size());


					if(size.size()){
						m_config_list[0]->cfg().compress.split_value.assign(&size[0]);
						m_config_list[0]->cfg().compress.split_value.append(&unit[0]);
					}
					return true;
				}

				case IDC_EDIT_SPLIT_CHUNK:{
					std::vector<TCHAR> chunk_str(64);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&chunk_str[0],
									chunk_str.size());

					if(chunk_str.size()){
						int chunk=_ttoi(&chunk_str[0]);
						if(chunk<minimum_split_chunk){
							//最小値
							m_config_list[0]->cfg().compress.split_value.assign(_T("2"));
							::SetWindowText(getDlgItem(LOWORD(wparam)),m_config_list[0]->cfg().compress.split_value.c_str());
						}else if(chunk>=maximum_split_chunk){
							//最大値
							m_config_list[0]->cfg().compress.split_value.assign(_T("999999"));
							::SetWindowText(getDlgItem(LOWORD(wparam)),m_config_list[0]->cfg().compress.split_value.c_str());
						}else{
							m_config_list[0]->cfg().compress.split_value.assign(&chunk_str[0]);
						}
					}
					return true;
				}

				default:
					break;
			}
	}
	return false;
}

bool SplitTab::onNotify(WPARAM wparam,LPARAM lparam){
	if(wparam==IDC_SPIN_SPLIT_CHUNK){
		LPNMUPDOWN ud=reinterpret_cast<LPNMUPDOWN>(lparam);

		if(ud->hdr.code==UDN_DELTAPOS){
			int chunk=_ttoi(m_config_list[0]->cfg().compress.split_value.c_str());
			if((ud->iDelta)>0&&chunk<maximum_split_chunk){
				//上が押された
				chunk++;
			}else if((ud->iDelta)<0&&chunk>minimum_split_chunk){
				//下が押された
				chunk--;
			}
			VariableArgument va(_T("%d"),chunk);

			m_config_list[0]->cfg().compress.split_value.assign(va.get());
			::SetWindowText(getDlgItem(IDC_EDIT_SPLIT_CHUNK),va.get());
		}
	}
	return false;
}

void SplitTab::setCurrentSettings(){
	bool by_count=!m_config_list[0]->cfg().compress.split_value.empty()&&
		isdigit(m_config_list[0]->cfg().compress.split_value[m_config_list[0]->cfg().compress.split_value.length()-1]);
	//30MB,150MBなど...
	bool normal_item=false;

	if(!by_count){
		for(size_t i=2;i<ARRAY_SIZEOF(split_size_list);i++){
			if(m_config_list[0]->cfg().compress.split_value==split_size_list[i]){
				sendItemMessage(IDC_COMBO_SPLIT_SIZE,
								   CB_SETCURSEL,
								   (WPARAM)i,
								   0
								   );
				normal_item=true;
				break;
			}
		}
	}

	if(!normal_item){
		if(m_config_list[0]->cfg().compress.split_value.empty()||
		   by_count){
			//分割しない
			sendItemMessage(IDC_COMBO_SPLIT_SIZE,
							   CB_SETCURSEL,
							   (WPARAM)0,
							   0
							  );
		}else{
			//任意のサイズ
			tstring size_str(m_config_list[0]->cfg().compress.split_value);
			size_t size_index=0;

			while(size_index<size_str.length()&&
				  !isalpha(size_str.c_str()[++size_index]));

			tstring size_unit(str::toUpper(size_str.substr(size_index)));
			size_str.assign(size_str.substr(0,size_index));

			for(size_t i=0;i<ARRAY_SIZEOF(size_units_table);i++){
				if(str::isEqualStringIgnoreCase(size_units_table[i].units,size_unit)){
					//順番注意
					::SetWindowText(getDlgItem(IDC_BUTTON_SPLIT_SIZE),size_unit.c_str());
					::SetWindowText(getDlgItem(IDC_EDIT_SPLIT_SIZE),size_str.c_str());
					break;
				}
			}

			sendItemMessage(IDC_COMBO_SPLIT_SIZE,
							   CB_SETCURSEL,
							   (WPARAM)1,
							   0
							  );
		}
	}

	setCheck(IDC_RADIO_SPLIT_SIZE,false);

	setCheck(IDC_RADIO_SPLIT_CHUNK,false);

	if(m_config_list[0]->cfg().compress.split_value.empty()||
		!by_count){
		::SetWindowText(getDlgItem(IDC_EDIT_SPLIT_CHUNK),_T("2"));
		setCheck(IDC_RADIO_SPLIT_SIZE,true);
		setCheck(IDC_RADIO_SPLIT_CHUNK,false);
		::EnableWindow(getDlgItem(IDC_COMBO_SPLIT_SIZE),sendItemMessage(IDC_RADIO_SPLIT_SIZE,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),sendItemMessage(IDC_RADIO_SPLIT_SIZE,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_CHUNK),!sendItemMessage(IDC_RADIO_SPLIT_SIZE,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_SPIN_SPLIT_CHUNK),!sendItemMessage(IDC_RADIO_SPLIT_SIZE,BM_GETCHECK,0,0));

		sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_SPLIT_SIZE,CBN_SELCHANGE),0);
	}else{
		if(m_config_list[0]->cfg().compress.split_value.empty()){
			m_config_list[0]->cfg().compress.split_value.assign(_T("2"));
		}

		::SetWindowText(getDlgItem(IDC_EDIT_SPLIT_CHUNK),m_config_list[0]->cfg().compress.split_value.c_str());
		setCheck(IDC_RADIO_SPLIT_SIZE,false);
		setCheck(IDC_RADIO_SPLIT_CHUNK,true);
		::EnableWindow(getDlgItem(IDC_COMBO_SPLIT_SIZE),!sendItemMessage(IDC_RADIO_SPLIT_CHUNK,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_SIZE),!sendItemMessage(IDC_RADIO_SPLIT_CHUNK,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_EDIT_SPLIT_CHUNK),sendItemMessage(IDC_RADIO_SPLIT_CHUNK,BM_GETCHECK,0,0));
		::EnableWindow(getDlgItem(IDC_SPIN_SPLIT_CHUNK),sendItemMessage(IDC_RADIO_SPLIT_CHUNK,BM_GETCHECK,0,0));
	}
}
