//CompressTab.cpp
//再圧縮/圧縮タブ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.4 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"CompressTab.h"


using namespace sslib;



namespace{
	const TCHAR* const combo_compression_level_list[]={
		_T("標準"),
		_T("最大圧縮"),
		_T("最小圧縮"),
		_T("0"),
		_T("1"),
		_T("2"),
		_T("3"),
		_T("4"),
		_T("5"),
		_T("6"),
		_T("7"),
		_T("8"),
		_T("9")
	};

	struct{
		const TCHAR* type;
		const TCHAR* ext;
	}const combo_compression_list[]={
		{_T("入力と同じ"),_T(".zip")},
		{_T("zip"),_T(".zip")},
		{_T("7z"),_T(".7z")},
		{_T("lzh"),_T(".lzh")},
		{_T("tar"),_T(".tar")},
		{_T("gzip"),_T(".gz")},
		{_T("targzip"),_T(".tar.gz")},
		{_T("bzip2"),_T(".bz2")},
		{_T("tarbzip2"),_T(".tar.bz2")},
		{_T("xz"),_T(".xz")},
		{_T("tarxz"),_T(".tar.xz")},
		{_T("lzma"),_T(".lzma")},
		{_T("tarlzma"),_T(".tar.lzma")}
	};

	const TCHAR* const combo_run_command_list[]={
		_T("無効"),
		_T("インタラクティブ"),
		_T("バッチ")
	};
	enum{
		RUN_COMMAND_DISABLE=0,
		RUN_COMMAND_INTERACTIVE,
		RUN_COMMAND_BATCH
	};

	struct COMPRESS_CONFIG{
		//圧縮形式
		tstring type;
		//パスワード
		bool password;
		//ヘッダ暗号化
		bool header_encryption;
		//自己解凍
		bool sfx;
		COMPRESS_CONFIG():
			type(),password(false),header_encryption(false),sfx(false){}
		enum{
			NO_PASSWORD=1<<0,
			NO_HEADER_ENCRYPTION=1<<1,
			NO_SFX=1<<2
		};
		tstring getType()const{
			return type;
		}
		tstring getOptions(int filter=0)const{
			return tstring(((password&&!header_encryption&&!(filter&NO_PASSWORD))?_T("pw"):_T("")))+
				tstring(((header_encryption&&!(filter&NO_HEADER_ENCRYPTION))?_T("he"):_T("")))+
					tstring(((sfx&&!(filter&NO_SFX))?_T("sfx"):_T("")));
		}
		void setType(const tstring& type_){
			type=type_;
			//パスワード
			tstring::size_type pos=type.find(_T("pw"));

			if(pos!=tstring::npos){
				password=true;
				type.erase(pos,2);
			}else{
				password=false;
			}

			//ヘッダ暗号化
			pos=type.find(_T("he"));

			if(pos!=tstring::npos){
				header_encryption=true;
				type.erase(pos,2);
			}else{
				header_encryption=false;
			}

			//自己解凍
			pos=type.find(_T("sfx"));

			if(pos!=tstring::npos){
				sfx=true;
				type.erase(pos,3);
			}else{
				sfx=false;
			}
		}
	}compress_config;
}


INT_PTR CompressTab::onInitDialog(WPARAM wparam,LPARAM lparam){
	for(size_t i=0;i<ARRAY_SIZEOF(combo_compression_list);i++){
		sendItemMessage(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,
						CB_ADDSTRING,
						0,
						(LPARAM)combo_compression_list[i].type);
	}

	for(size_t i=0;i<ARRAY_SIZEOF(combo_compression_level_list);i++){
		sendItemMessage(IDC_COMBO_COMPRESS_COMPRESSION_LEVEL,
						CB_ADDSTRING,
						0,
						(LPARAM)combo_compression_level_list[i]);
	}

	for(size_t i=0;i<ARRAY_SIZEOF(combo_run_command_list);i++){
		sendItemMessage(IDC_COMBO_COMPRESS_RUN_COMMAND,
						CB_ADDSTRING,
						0,
						(LPARAM)combo_run_command_list[i]);
	}


	setCurrentSettings();
	return true;
}

INT_PTR CompressTab::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_CHECKBOX_COMPRESS_PASSWORD:
			//パスワード
			compress_config.password=getCheck(LOWORD(wparam));
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,CBN_SELCHANGE),0);
			return true;

		case IDC_CHECKBOX_COMPRESS_HEADER_ENCRYPTION:
			//ヘッダ暗号化
			compress_config.header_encryption=getCheck(LOWORD(wparam));
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,CBN_SELCHANGE),0);
			return true;

		case IDC_CHECKBOX_COMPRESS_SFX:
			//自己解凍
			compress_config.sfx=getCheck(LOWORD(wparam));
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,CBN_SELCHANGE),0);
			return true;

		case IDC_CHECKBOX_COMPRESS_EACH_FILE:
			//個別に圧縮
			m_config_list[0]->cfg().compress.each_file=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_COMPRESS_CREATE_NEW:
			//新規作成
			m_config_list[0]->cfg().compress.create_new=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_COMPRESS_COPY_TIMESTAMP:
			//更新日時を保持
			m_config_list[0]->cfg().compress.copy_timestamp=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_COMPRESS_EXCLUDE_BASE_DIR:
			//基底ディレクトリを除外
			m_config_list[0]->cfg().compress.exclude_base_dir=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_COMPRESS_AUTO_RENAME:
			//自動リネーム
			m_config_list[0]->cfg().general.auto_rename=getCheck(LOWORD(wparam));
	}

	switch(HIWORD(wparam)){
		case CBN_SELCHANGE:
			switch(LOWORD(wparam)){
				case IDC_COMBO_COMPRESS_COMPRESSION_TYPE:{
					//圧縮形式
					::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_PASSWORD),false);
					::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_HEADER_ENCRYPTION),false);
					::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_SFX),false);

					int options_filter=0;

					std::vector<TCHAR> combo_string(MAX_PATH);

					sendItemMessage(LOWORD(wparam),
									CB_GETLBTEXT,
									(WPARAM)sendItemMessage(LOWORD(wparam),CB_GETCURSEL,0,0),
									(LPARAM)&combo_string[0]);

					tstring compression_type(&combo_string[0]);

					if(compression_type==_T("入力と同じ")){
						compress_config.type.assign(_T("@"));
					}else{
						compress_config.type.assign(compression_type.c_str());
					}

					if(compress_config.type==_T("@")||
					   compress_config.type==_T("7z")){
						::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_PASSWORD),true);
						::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_HEADER_ENCRYPTION),true);
						::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_SFX),true);
					}else if(compress_config.type==_T("zip")){
						//ヘッダ暗号化,sfxを無効に
						options_filter|=COMPRESS_CONFIG::NO_HEADER_ENCRYPTION|COMPRESS_CONFIG::NO_SFX;
						::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_PASSWORD),true);
					}else if(compress_config.type==_T("lzh")){
						//パスワード,ヘッダ暗号化を無効に
						options_filter|=COMPRESS_CONFIG::NO_PASSWORD|COMPRESS_CONFIG::NO_HEADER_ENCRYPTION;
						::EnableWindow(getDlgItem(IDC_CHECKBOX_COMPRESS_SFX),true);
					}else{
						//オプションを全て無効に
						options_filter|=COMPRESS_CONFIG::NO_PASSWORD|COMPRESS_CONFIG::NO_HEADER_ENCRYPTION|COMPRESS_CONFIG::NO_SFX;
					}

					m_config_list[0]->cfg().compress.compression_type=
						compress_config.getType()+
						compress_config.getOptions(options_filter);
					return true;
				}

				case IDC_COMBO_COMPRESS_COMPRESSION_LEVEL:{
					//圧縮率
					std::vector<TCHAR> combo_string(MAX_PATH);

					sendItemMessage(LOWORD(wparam),
									CB_GETLBTEXT,
									(WPARAM)sendItemMessage(LOWORD(wparam),CB_GETCURSEL,0,0),
									(LPARAM)&combo_string[0]);

					tstring compression_level(&combo_string[0]);

					if(compression_level==_T("標準")){
						m_config_list[0]->cfg().compress.compression_level=default_compressionlevel;
					}else if(compression_level==_T("最大圧縮")){
						m_config_list[0]->cfg().compress.compression_level=maximum_compressionlevel;
					}else if(compression_level==_T("最小圧縮")){
						m_config_list[0]->cfg().compress.compression_level=minimum_compressionlevel;
					}else{
						m_config_list[0]->cfg().compress.compression_level=_ttoi(compression_level.c_str());
					}
					return true;
				}

				case IDC_COMBO_COMPRESS_RUN_COMMAND:{
					//コマンド実行
					switch(sendItemMessage(LOWORD(wparam),CB_GETCURSEL,0,0)){
						case RUN_COMMAND_DISABLE:
							//無効
							m_config_list[0]->cfg().recompress.run_command.interactive=false;
							::EnableWindow(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),false);
							m_config_list[0]->cfg().recompress.run_command.command.clear();
							break;

						case RUN_COMMAND_INTERACTIVE:
							//インタラクティブ
							m_config_list[0]->cfg().recompress.run_command.interactive=true;
							::EnableWindow(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),false);
							m_config_list[0]->cfg().recompress.run_command.command.clear();
							break;

						case RUN_COMMAND_BATCH:
							//バッチ
							m_config_list[0]->cfg().recompress.run_command.interactive=false;
							::EnableWindow(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),true);
							sendMessage(WM_COMMAND,MAKEWPARAM(IDC_EDIT_COMPRESS_RUN_COMMAND,EN_CHANGE),0);
							break;
					}
				}

				default:
					break;
			}
			break;

		case EN_CHANGE:
			switch(LOWORD(wparam)){
				case IDC_EDIT_COMPRESS_RUN_COMMAND:{
					//コマンド実行[バッチ]
					std::vector<TCHAR> command(2048);

					::GetWindowText(getDlgItem(LOWORD(wparam)),
									&command[0],
									command.size());
					m_config_list[0]->cfg().recompress.run_command.command.assign(&command[0]);
					break;
				}
			}
			break;

		default:
			break;
	}
	return false;
}

INT_PTR CompressTab::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_SHOWWINDOW:
			//圧縮タブ「基底ディレクトリを除外」と解凍タブ「共通パスを除外」を同期させる
			if(wparam)setCurrentSettings();
			break;
		default:
			break;
	}
	return false;
}

void CompressTab::setCurrentSettings(){
	compress_config.setType(m_config_list[0]->cfg().compress.compression_type);
	if(compress_config.getType()==_T("@")){
		sendItemMessage(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,
						CB_SETCURSEL,
						(WPARAM)0,
						0
						);
	}else{
		for(size_t i=0;i<ARRAY_SIZEOF(combo_compression_list);i++){
			if(compress_config.getType()==combo_compression_list[i].type){
				sendItemMessage(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,
								CB_SETCURSEL,
								(WPARAM)i,
								0
								);
				break;
			}
		}
	}

	//パスワード
	setCheck(IDC_CHECKBOX_COMPRESS_PASSWORD,compress_config.password);

	//ヘッダ暗号化
	setCheck(IDC_CHECKBOX_COMPRESS_HEADER_ENCRYPTION,compress_config.header_encryption);

	//自己解凍
	setCheck(IDC_CHECKBOX_COMPRESS_SFX,compress_config.sfx);

	sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_COMPRESS_COMPRESSION_TYPE,CBN_SELCHANGE),0);

	//圧縮率
	int item_compression_level=0;

	switch(m_config_list[0]->cfg().compress.compression_level){
		case default_compressionlevel:
			item_compression_level=0;
			break;

		case maximum_compressionlevel:
			item_compression_level=1;
			break;

		case minimum_compressionlevel:
			item_compression_level=2;
			break;

		default:
			for(size_t i=3;i<ARRAY_SIZEOF(combo_compression_level_list[i]);i++){
				if(_ttoi(combo_compression_level_list[i])==m_config_list[0]->cfg().compress.compression_level){
					item_compression_level=i;
					break;
				}
			}
			break;
	}

	sendItemMessage(IDC_COMBO_COMPRESS_COMPRESSION_LEVEL,
					   CB_SETCURSEL,
					   (WPARAM)item_compression_level,
					   0);

	//個別に圧縮
	setCheck(IDC_CHECKBOX_COMPRESS_EACH_FILE,m_config_list[0]->cfg().compress.each_file);

	//新規作成
	setCheck(IDC_CHECKBOX_COMPRESS_CREATE_NEW,m_config_list[0]->cfg().compress.create_new);

	//更新日時を保持
	setCheck(IDC_CHECKBOX_COMPRESS_COPY_TIMESTAMP,m_config_list[0]->cfg().compress.copy_timestamp);

	//基底ディレクトリを除外
	setCheck(IDC_CHECKBOX_COMPRESS_EXCLUDE_BASE_DIR,m_config_list[0]->cfg().compress.exclude_base_dir!=0);

	//自動リネーム
	setCheck(IDC_CHECKBOX_COMPRESS_AUTO_RENAME,m_config_list[0]->cfg().general.auto_rename);

	//コマンドを実行
	sendItemMessage(IDC_COMBO_COMPRESS_RUN_COMMAND,
					   CB_SETCURSEL,
					   (WPARAM)((m_config_list[0]->cfg().recompress.run_command.disable())?
								RUN_COMMAND_DISABLE:
								((m_config_list[0]->cfg().recompress.run_command.interactive)?
								 RUN_COMMAND_INTERACTIVE:
								 RUN_COMMAND_BATCH)),
					   0);
	if(!m_config_list[0]->cfg().recompress.run_command.interactive&&
	   !m_config_list[0]->cfg().recompress.run_command.command.empty()){
		::SetWindowText(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),m_config_list[0]->cfg().recompress.run_command.command.c_str());
		::EnableWindow(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),true);
	}else{
		::EnableWindow(getDlgItem(IDC_EDIT_COMPRESS_RUN_COMMAND),false);
	}
}

tstring CompressTab::getExtension(){
	for(size_t i=0;i<ARRAY_SIZEOF(combo_compression_list);i++){
		if(compress_config.getType()==combo_compression_list[i].type){
			if(compress_config.getOptions(COMPRESS_CONFIG::NO_PASSWORD|COMPRESS_CONFIG::NO_HEADER_ENCRYPTION)==_T("sfx")){
				return _T(".exe");
			}
			return combo_compression_list[i].ext;
		}
	}
	return combo_compression_list[0].ext;
}
