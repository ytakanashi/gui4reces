//MainDialog.cpp
//メインダイアログ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.4 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"MainDialog.h"

#include"AboutDialog.h"
#include<shlobj.h>


using namespace sslib;



namespace{
	class DeferPos{
		public:
		DeferPos(int num=1):m_hdwp(NULL){m_hdwp=::BeginDeferWindowPos(num);}
		virtual ~DeferPos(){::EndDeferWindowPos(m_hdwp);}
		private:
			HDWP m_hdwp;
		public:
			bool move(HWND wnd,HWND insert_after,int x,int y,int cx,int cy,UINT flags){
				return NULL!=(m_hdwp=::DeferWindowPos(m_hdwp,wnd,insert_after,x,y,cx,cy,flags));
			}
	};

	class CfgNameDialog:public Dialog{
	public:
		CfgNameDialog(tstring& name,bool& is_default):
		Dialog(IDD_DIALOG_PROFILE_NAME),
		m_name(name),
		m_is_default(is_default){}
	private:
		tstring& m_name;
		bool & m_is_default;
	private:
		INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam){
			if(!m_name.empty()){
				::SetWindowText(getDlgItem(IDC_EDIT_PROFILE_NAME),m_name.c_str());

				if(m_is_default){
					sendItemMessage(IDC_CHECKBOX_PROFILE_DEFAULT,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				}
			}
			return true;
		}
		INT_PTR onOk(){
			std::vector<TCHAR> cfg_name(MAX_PATH);
			::GetWindowText(getDlgItem(IDC_EDIT_PROFILE_NAME),&cfg_name[0],cfg_name.size());
			m_name.assign(&cfg_name[0]);
			m_is_default=sendItemMessage(IDC_CHECKBOX_PROFILE_DEFAULT,BM_GETCHECK,0,0)!=0;
			return true;
		}
	};

	namespace shortcut{
		struct{
			const TCHAR* type;
			const int cmd;
		}const show_cmd_list[]={
			{_T("通常のウインドウ"),SW_SHOWNORMAL},
			{_T("最小化"),SW_SHOWMINNOACTIVE},
			{_T("最大化"),SW_SHOWMAXIMIZED},
		};
	}
	class CreateShortcutDialog:public Dialog{
	public:
		CreateShortcutDialog(bool& exec_reces,int& show_cmd,tstring& link_path,tstring& description):
		Dialog(IDD_DIALOG_CREATE_SHORTCUT),
		m_exec_reces(exec_reces),
		m_link_path(link_path),
		m_show_cmd(show_cmd),
		m_description(description){}
	private:
		bool& m_exec_reces;
		tstring& m_link_path;
		int& m_show_cmd;
		tstring& m_description;
	private:
		INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam){
			sendItemMessage((!m_exec_reces)?IDC_RADIO_SHORTCUT_GUI4RECES:IDC_RADIO_SHORTCUT_RECES,
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0);
			::SetWindowText(getDlgItem(IDC_EDIT_SHORTCUT_LINKPATH),m_link_path.c_str());
			for(size_t i=0;i<ARRAY_SIZEOF(shortcut::show_cmd_list);++i){
				sendItemMessage(IDC_COMBO_SHORTCUT_SHOWCMD,CB_ADDSTRING,0,(LPARAM)shortcut::show_cmd_list[i].type);
			}
			sendItemMessage(IDC_COMBO_SHORTCUT_SHOWCMD,CB_SETCURSEL,(WPARAM)0,0);
			::SetWindowText(getDlgItem(IDC_EDIT_SHORTCUT_DESCRIPTION),m_description.c_str());
			return true;
		}
		INT_PTR onCommand(WPARAM wparam,LPARAM lparam){
			switch(LOWORD(wparam)){
				case IDC_RADIO_SHORTCUT_GUI4RECES:{
					m_exec_reces=false;
					return true;
				}
				case IDC_RADIO_SHORTCUT_RECES:{
					m_exec_reces=true;
					return true;
				}
				default:
					break;
			}
			switch(HIWORD(wparam)){
				case EN_CHANGE:{
					std::vector<TCHAR> buffer(MAX_PATHW);

					switch(LOWORD(wparam)){
						case IDC_EDIT_SHORTCUT_LINKPATH:
							::GetWindowText(getDlgItem(LOWORD(wparam)),
											&buffer[0],
											buffer.size());
							m_link_path.assign(&buffer[0]);
							return true;
						case IDC_EDIT_SHORTCUT_DESCRIPTION:
							::GetWindowText(getDlgItem(LOWORD(wparam)),
											&buffer[0],
											buffer.size());
							m_description.assign(&buffer[0]);
							return true;
					}
					return false;
				}
			}
			return false;
		}
		INT_PTR onOk(){
			m_show_cmd=shortcut::show_cmd_list[sendItemMessage(IDC_COMBO_SHORTCUT_SHOWCMD,CB_GETCURSEL,0,0)].cmd;
			return true;
		}
	};

	class PipeRedirect{
	public:
		struct PIPE{
			HANDLE read;
			HANDLE write;
			PIPE():read(NULL),write(NULL){}
			void close(){SAFE_CLOSE(read);SAFE_CLOSE(write);}
		};
	public:
		PipeRedirect():
		m_result(),m_pipe(),m_process_info(){}
		virtual ~PipeRedirect(){m_pipe.close();}
		tstring& buffer(){return m_result;}
	protected:
		virtual void begin(){}
		virtual void end(){}
		virtual void processBuffer(const TCHAR* buffer){m_result.append(buffer);}
		virtual bool messageLoop(){
			MSG msg;

			if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
				if(msg.message==WM_QUIT){
					return false;
				}
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			return true;
		}
	public:
		bool launch(const TCHAR* cmd){
			begin();

			HANDLE read_handle=NULL;
			STARTUPINFO startup_info={sizeof(STARTUPINFO)};

			if(!::CreatePipe(&m_pipe.read,&m_pipe.write,NULL,0))return false;

			::DuplicateHandle(GetCurrentProcess(),m_pipe.write,
							  ::GetCurrentProcess(),&read_handle,
							  0,true,DUPLICATE_SAME_ACCESS);
			SAFE_CLOSE(m_pipe.write);

			startup_info.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
			startup_info.wShowWindow=SW_HIDE;
			startup_info.hStdOutput=read_handle;
			startup_info.hStdError=read_handle;

			std::vector<TCHAR> cmd_(lstrlen(cmd)+1);
			lstrcpyn(&cmd_[0],cmd,cmd_.size()+1);

			if(::CreateProcess(NULL,&cmd_[0],NULL,NULL,
							   true,
							   0//CREATE_NEW_PROCESS_GROUP/*CREATE_NO_WINDOW*/
							   ,
							   NULL,NULL,&startup_info,&m_process_info)){
				DWORD wait_result=0;
				std::vector<TCHAR> buffer(12,'\0');

				::WaitForInputIdle(m_process_info.hProcess,INFINITE);

				while((wait_result=::WaitForSingleObject(m_process_info.hProcess,0))!=WAIT_ABANDONED){
					DWORD avail_bytes=0;

					if(::PeekNamedPipe(m_pipe.read,NULL,0,NULL,&avail_bytes,NULL)&&
					   avail_bytes){
						DWORD read=0;

						buffer.assign(avail_bytes,'\0');

						if(::ReadFile(m_pipe.read,&buffer[0],buffer.size(),&read,NULL)){
							processBuffer(&buffer[0]);
						}
					}

					if(!messageLoop())break;

					if(wait_result==WAIT_OBJECT_0){
						break;
					}
				}

				SAFE_CLOSE(m_process_info.hThread);
				SAFE_CLOSE(m_process_info.hProcess);
				SAFE_CLOSE(read_handle);
				m_pipe.close();

			}

			end();
			return true;
		}
	protected:
		tstring m_result;
		PIPE m_pipe;
		PROCESS_INFORMATION m_process_info;
	};

	class PipeRedirectToTextBox:public PipeRedirect{
	class LogDialog:public Dialog{
	class Edit:public sslib::Control{
	public:
		Edit(HWND parent_handle,UINT resource_id,bool sub_class=false):
			Control(parent_handle,resource_id,sub_class){}
		virtual ~Edit(){}
	private:
		LRESULT onChar(WPARAM wparam,LPARAM lparam){
			if(wparam==VK_ESCAPE){
				DWORD exit_code=0;

				::GetExitCodeProcess(static_cast<PROCESS_INFORMATION*>(param())->hProcess,&exit_code);

				if(exit_code!=STILL_ACTIVE){
					sendParentMessage(WM_CLOSE,0,0);
					return true;
				}
			}
			return false;
		}
		LRESULT onKeyDown(WPARAM wparam,LPARAM lparam){
			if(::GetAsyncKeyState(VK_CONTROL)<0&&
			   ::GetAsyncKeyState(VK_SHIFT)>=0&&
			   ::GetAsyncKeyState(VK_MENU)>=0){
				switch(wparam){
					case 'A':{
						//全選択
						int length=sendMessage(WM_GETTEXTLENGTH,0,0);
						sendMessage(EM_SETSEL,0,length);
						return false;
					}
					case 'C':
						//コピー
						sendMessage(WM_COPY);
						return false;
					default:
						break;
				}
			}
			return true;
		}
		LRESULT onGetDlgCode(WPARAM wparam,LPARAM lparam){
			//ダイアログ表示時、エディットボックス内文字列が全選択されるのを防ぐ
			return ::CallWindowProc(default_proc(),handle(),WM_GETDLGCODE,wparam,lparam)&~DLGC_HASSETSEL;
		}
		LRESULT onMessage(UINT message,WPARAM wparam,LPARAM lparam){
			switch(message){
				case WM_CHAR:
					return onChar(wparam,lparam);
				case WM_KEYDOWN:
					if(!onKeyDown(wparam,lparam))return false;
					break;
				case WM_GETDLGCODE:
					return onGetDlgCode(wparam,lparam);
				default:
					break;
			}
			return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
		}
	};
	public:
		LogDialog():
		Dialog(IDD_DIALOG_LOG),
		m_edit(NULL),
		m_wnd_size(NULL),
		m_wnd_height(0),
		m_wnd_width(0){}
		virtual ~LogDialog(){
			env::del(_T("PASSWORD_DIALOG"));
			SAFE_DELETE(m_edit);
			SAFE_DELETE(m_wnd_size);
		}
	private:
		Edit* m_edit;
		SIZE_INFO* m_wnd_size;
		int m_wnd_height;
		int m_wnd_width;
	private:
		INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam){
			//アイコンの設定(タイトルバー)
			setIcon(IDI_ICON1);
			RECT rc={0};

			::GetWindowRect(handle(),&rc);
			m_wnd_width=rc.right-rc.left;
			m_wnd_height=rc.bottom-rc.top;
			m_wnd_size=new SIZE_INFO(handle(),getDlgItem(IDC_EDIT_LOG));

			sendItemMessage(IDC_EDIT_LOG,EM_LIMITTEXT,0,2*1024*1024);
			m_edit=new Edit(handle(),IDC_EDIT_LOG,true);
			m_edit->setParam(param());

			//reces側でダイアログを表示
			env::set(_T("PASSWORD_DIALOG"),
					 format(_T("%d"),handle()).c_str());
			return true;
		}
		INT_PTR onClose(){
			DWORD exit_code=0;

			::GetExitCodeProcess(static_cast<PROCESS_INFORMATION*>(param())->hProcess,&exit_code);

			if(exit_code==STILL_ACTIVE){
				//一時停止
				::DebugActiveProcess(static_cast<PROCESS_INFORMATION*>(param())->dwProcessId);
				if(::MessageBox(handle(),_T("処理を中断しますか"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES){
					//再開
					::DebugActiveProcessStop(static_cast<PROCESS_INFORMATION*>(param())->dwProcessId);
					//終了
					return Dialog::onClose();
				}else{
					//再開
					::DebugActiveProcessStop(static_cast<PROCESS_INFORMATION*>(param())->dwProcessId);
					//終了しない
					return true;
				}
			}else{
				return Dialog::onClose();
			}
			return false;
		}
		INT_PTR onSize(WPARAM wparam,LPARAM lparam){
			if(m_wnd_size==NULL)return false;

			DeferPos defer_pos;

			defer_pos.move(m_wnd_size->wnd,
						   NULL,
						   0,0,
						   LOWORD(lparam)-m_wnd_size->width_diff,
						   HIWORD(lparam)-m_wnd_size->height_diff,
						   SWP_NOMOVE|SWP_NOZORDER);
			return false;
		}
		INT_PTR onGetMinMaxInfo(WPARAM wparam,LPARAM lparam){
			LPMINMAXINFO info=reinterpret_cast<LPMINMAXINFO>(lparam);

			info->ptMinTrackSize.x=m_wnd_width;
			info->ptMinTrackSize.y=m_wnd_height;
			return true;
		}
		INT_PTR onMessage(UINT message,WPARAM wparam,LPARAM lparam){
			switch(message){
				case WM_GETMINMAXINFO:
					return onGetMinMaxInfo(wparam,lparam);
				default:
					break;
			}
			return false;
		}
	};
	public:
		PipeRedirectToTextBox():
		m_dialog(){}
		~PipeRedirectToTextBox(){::TerminateProcess(m_process_info.hProcess,0);}
	private:
		void begin(){
			m_dialog.setParam(&m_process_info);
			m_dialog.doModeless();
		}
		void processBuffer(const TCHAR* buffer){
			int length=m_dialog.sendItemMessage(IDC_EDIT_LOG,WM_GETTEXTLENGTH,0,0);

			m_dialog.sendItemMessage(IDC_EDIT_LOG,EM_SETSEL,length,length);
			m_dialog.sendItemMessage(IDC_EDIT_LOG,EM_REPLACESEL,0,(LPARAM)buffer);
		}
		bool messageLoop(){
			MSG msg;

			//reces側でパスワード入力ダイアログ表示した時のgui4recesのCPU使用率上昇を防ぐ
			if(::GetWindow(m_dialog.handle(),GW_CHILD))Sleep(1);

			if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
				if(!IsDialogMessage(m_dialog.handle(),&msg)){
					if(msg.message==WM_QUIT){
						::AttachConsole(m_process_info.dwProcessId);
						::SetConsoleCtrlHandler(NULL,true);
						::GenerateConsoleCtrlEvent(CTRL_C_EVENT,0);

						m_pipe.close();
						::WaitForSingleObject(m_process_info.hProcess,1000);
						::FreeConsole();
						::SetConsoleCtrlHandler(NULL,false);

						::PostQuitMessage(0);
						return false;
					}
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			return true;
		}
		void end(){
			MSG msg;

			processBuffer(_T("\r\n\r\n##########処理が全て終了しました。##########"));

			while(::GetMessage(&msg,NULL,0,0)){
				if(msg.message==WM_QUIT){
					::PostQuitMessage(0);
					break;
				}
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	private:
		LogDialog m_dialog;
	};

	namespace mode{
		//動作項目
		UINT idc_list[]={
			IDC_RADIO_MODE_RECOMPRESS,
			IDC_RADIO_MODE_COMPRESS,
			IDC_RADIO_MODE_EXTRACT,
			IDC_RADIO_MODE_LIST,
			IDC_RADIO_MODE_TEST,
			IDC_RADIO_MODE_DELETE
		};
	}
}

void MainDialog::setCurrentSettings(){
	//ダイアログ本体のみ
	//各タブはonInitDialog()にて

	//動作
	for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
		sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
	}

	switch(m_config_list[0]->cfg().mode){
		case MODE_COMPRESS:
			//圧縮
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_COMPRESS,0),0);
			break;

		case MODE_EXTRACT:
			//解凍
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_EXTRACT,0),0);
			break;

		case MODE_LIST:
			//リスト
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_LIST,0),0);
			break;

		case MODE_TEST:
			//テスト
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_TEST,0),0);
			break;

		case MODE_DELETE:
			//削除
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_DELETE,0),0);
			break;

		case MODE_RECOMPRESS:
			//再圧縮
			//fall through
		case MODE_SENDCOMMANDS:
		case MODE_VERSION:
		default:
			//再圧縮
			//その他諸々
			sendMessage(WM_COMMAND,MAKEWPARAM(IDC_RADIO_MODE_RECOMPRESS,0),0);
			break;
	}

	//最前面表示
	sendItemMessage(IDC_CHECKBOX_TOPMOST,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().gui4reces.top_most)?BST_CHECKED:BST_UNCHECKED,
					0
					);
	topMost(m_config_list[0]->cfg().gui4reces.top_most);

	//recesのウインドウを閉じる
	sendItemMessage(IDC_CHECKBOX_QUIT_RECES,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().general.quit)?BST_CHECKED:BST_UNCHECKED,
					0
					);

	//gui4recesを終了する
	sendItemMessage(IDC_CHECKBOX_QUIT_GUI4RECES,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().gui4reces.quit)?BST_CHECKED:BST_UNCHECKED,
					0
					);

	//すぐに開始する
	sendItemMessage(IDC_CHECKBOX_AT_ONCE,
					BM_SETCHECK,
					(WPARAM)(m_config_list[0]->cfg().gui4reces.at_once)?BST_CHECKED:BST_UNCHECKED,
					0
					);

	//コンボボックス対策
	::UpdateWindow(handle());
}

INT_PTR MainDialog::onInitDialog(WPARAM wparam,LPARAM lparam){
	//アイコンの設定(タイトルバー)
	setIcon(IDI_ICON1);

	//設定ファイルの読み込み
	m_config_list[0]->load(true);

	//一時ディレクトリ作成
	m_temp_dir=path::addTailSlash(tempfile::createDir(_T("g4r")));

	FileSearch fs;

	for(fs.first(tstring(path::getExeDirectory()+_T("\\cfg\\")).c_str());fs.next();){
		m_config_list.push_back(new Config(fs.filepath().c_str()));
		m_config_list.back()->load();
		sendItemMessage(IDC_COMBO_PROFILE,
						CB_ADDSTRING,
						0,
						(LPARAM)path::removeExtension(path::getFileName(fs.filepath())).c_str());
	}

	m_tab=new Tab(handle(),IDC_TAB1);
	if(m_tab==NULL)return false;

	m_tab_list.push_back(new CompressTab(m_config_list));
	m_tab_list.push_back(new ExtractTab(m_config_list));
	m_tab_list.push_back(new OutputTab(m_config_list));
	m_tab_list.push_back(new PasswordTab(m_config_list));
	m_tab_list.push_back(new FilterTab(m_config_list));
	m_tab_list.push_back(new SplitTab(m_config_list));
	m_tab_list.push_back(new RemoveSourceTab(m_config_list));
	m_tab_list.push_back(new OtherTab(m_config_list));
	m_tab_list.push_back(new ModeTab(m_config_list));

	for(size_t i=0,list_size=m_tab_list.size();i<list_size;i++){
		if(m_tab_list[i]!=NULL){
			m_tab_list[i]->doModeless(handle(),SW_HIDE);
		}
	}

	m_tab->insert(*m_tab_list[TAB_COMPRESS],_T("再圧縮/圧縮"),TAB_COMPRESS);
	m_tab->insert(*m_tab_list[TAB_EXTRACT],_T("再圧縮/解凍"),TAB_EXTRACT);
	m_tab->insert(*m_tab_list[TAB_OUTPUT],_T("出力"),TAB_OUTPUT);
	m_tab->insert(*m_tab_list[TAB_PASSWORD],_T("パスワード"),TAB_PASSWORD);
	m_tab->insert(*m_tab_list[TAB_FILTER],_T("フィルタ"),TAB_FILTER);
	m_tab->insert(*m_tab_list[TAB_SPLIT],_T("分割"),TAB_SPLIT);
	m_tab->insert(*m_tab_list[TAB_REMOVESOURCE],_T("処理後削除"),TAB_REMOVESOURCE);
	m_tab->insert(*m_tab_list[TAB_OTHER],_T("その他"),TAB_OTHER);
	m_tab->insert(*m_tab_list[TAB_MODE],_T("動作詳細"),TAB_MODE);

	//デフォルトタブ
	m_tab_list[TAB_COMPRESS]->showDialog();
	TabCtrl_SetCurFocus(m_tab->handle(),TAB_COMPRESS);

	m_listview=new FileListView(handle());
	if(m_listview==NULL)return false;

	RECT rc={0};

	::GetWindowRect(handle(),&rc);

	//onGetMinMaxInfo()用
	m_wnd_width=rc.right-rc.left;
	m_wnd_height=rc.bottom-rc.top;

	::GetWindowRect(m_listview->handle(),&rc);
	//リストビューにカラムを追加
	m_listview->insertColumn(0,
							 _T("ファイルやディレクトリを追加してください"),
							 LVCFMT_LEFT,
							 (rc.right-rc.left)*2);

	//リストビューのスタイルを変更
	m_listview->setExtendedListViewStyle(m_listview->getExtendedListViewStyle()|
										 //LVS_EX_FULLROWSELECT(行全体選択)
										 LVS_EX_FULLROWSELECT|
										 //LVS_EX_GRIDLINES(罫線表示)
										 LVS_EX_GRIDLINES|
										 //LVS_EX_CHECKBOXES(チェックボックス表示)
										 LVS_EX_CHECKBOXES);

	//サイズ変更時に場所を変更するアイテムたち
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_LIST1)));
	//処理終了後
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_GROUP_COMPLETE)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_CHECKBOX_QUIT_RECES)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_CHECKBOX_QUIT_GUI4RECES)));
	//処理
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_GROUP_PROCESS)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_CHECKBOX_AT_ONCE)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_CHECKBOX_TOPMOST)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_BUTTON_VERSION)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_BUTTON_HELP)));
	m_wnd_size_list.push_back(SIZE_INFO(handle(),getDlgItem(IDC_BUTTON_RUN)));

	//設定をコントロールに適用
	setCurrentSettings();

	//メニューを読み込む
	m_create_shortcut_menu.load(IDR_MENU_CREATE_SHORTCUT);
	m_add_item_menu.load(IDR_MENU_ADD_ITEM);



	//引数でプロファイルが指定された
	bool profile_opt=false;
	tstring profile_name;

	{
		//引数解析
		std::vector<tstring>& options=static_cast<CommandArgument*>(param())->options();
		std::vector<tstring>& filepaths=static_cast<CommandArgument*>(param())->filepaths();

		for(std::vector<tstring>::size_type i=0,size=options.size();i<size;++i){
			//先頭が'profile:'
			if(options[i].find(_T("profile:"))==0){
				profile_name.assign(options[i].substr(options[i].find(_T(":"))+1));
			}
		}

		profile_opt=!profile_name.empty();

		for(std::vector<tstring>::size_type i=0,size=filepaths.size();i<size;++i){
			//ファイルをリストに追加
			m_listview->setCheckState(m_listview->insertItem(filepaths[i].c_str()));
		}
	}


	//引数で指定されたプロファイル/デフォルトプロファイルを読み込む
	int index=sendItemMessage(IDC_COMBO_PROFILE,
							  CB_FINDSTRINGEXACT,
							  0,
							  (LPARAM)((profile_opt)?
							  profile_name.c_str():
							  m_config_list[0]->cfg().gui4reces.default_profile.c_str()));
	if(index!=CB_ERR){
		sendItemMessage(IDC_COMBO_PROFILE,
						CB_SETCURSEL,
						(WPARAM)index,
						0);
		sendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_PROFILE,CBN_SELCHANGE),0);
	}else{
		if(!profile_opt){
			m_config_list[0]->cfg().gui4reces.default_profile.clear();
		}
		::EnableWindow(getDlgItem(IDC_STATIC_PROFILE_DEFAULT),false);
	}

	//ドラッグ&ドロップを許可
	::DragAcceptFiles(handle(),true);

	if(m_config_list[0]->cfg().gui4reces.at_once){
		sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_RUN,0),0);
	}
	return true;
}

INT_PTR MainDialog::onDestroy(){
	fileoperation::deleteDirectory(m_temp_dir.c_str());
	m_config_list[0]->save(true);
	return true;
}

INT_PTR MainDialog::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_BUTTON_PROFILE_ADD:{
			//プロファイルを追加
			tstring cfg_name;
			bool is_default_profile=false;

			CfgNameDialog cfgNameDialog(cfg_name,is_default_profile);
			cfgNameDialog.doModal(handle());

			if(!path::isBadName(cfg_name.c_str())&&
			   !cfg_name.empty()&&
			   //重複を防ぐ
			   sendItemMessage(IDC_COMBO_PROFILE,CB_FINDSTRINGEXACT,0,(LPARAM)cfg_name.c_str())==CB_ERR){
				sendItemMessage(IDC_COMBO_PROFILE,
								CB_SETCURSEL,
								(WPARAM)sendItemMessage(IDC_COMBO_PROFILE,CB_ADDSTRING,0,(LPARAM)cfg_name.c_str()),
								0);

				tstring cfg_path(path::getExeDirectory()+_T("\\cfg\\"));
				fileoperation::createDirectory(cfg_path.c_str());
				cfg_path+=cfg_name+_T(".cfg");

				//cfgを新規作成
				m_config_list.push_back(new Config(cfg_path.c_str()));
				*m_config_list.back()=*m_config_list[0];
				m_config_list.back()->setFileName(cfg_path.c_str());
				m_config_list.back()->save();

				if(is_default_profile){
					//デフォルトに設定
					m_config_list[0]->cfg().gui4reces.default_profile.assign(cfg_name);
				}
			}else if(!cfg_name.empty()){
				if(path::isBadName(cfg_name.c_str())){
					MessageBox(handle(),_T("ファイル名に次の文字は使えません。\n\t\\ / : , ; * ? \" < > |"),_T("プロファイルの追加"),MB_ICONSTOP);
				}else if(sendItemMessage(IDC_COMBO_PROFILE,CB_FINDSTRINGEXACT,0,(LPARAM)cfg_name.c_str())!=CB_ERR){
					MessageBox(handle(),_T("その名前は既に登録されています。"),_T("プロファイルの追加"),MB_ICONSTOP);
				}
				sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_PROFILE_ADD,0),0);
			}
			return true;
		}

		case IDC_BUTTON_PROFILE_SAVE:{
			//プロファイルを保存
			int current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

			if(current_sel==CB_ERR){
				//新規作成
				sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_PROFILE_ADD,0),0);
				return true;
			}

			m_config_list[current_sel+1]->import(*m_config_list[0]);
			m_config_list[current_sel+1]->save();
			return true;
		}

		case IDC_BUTTON_PROFILE_EDIT:{
			//プロファイルを編集
			if(!sendItemMessage(IDC_COMBO_PROFILE,CB_GETCOUNT,0,0))break;

			int current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

			if(current_sel==CB_ERR)break;

			std::vector<TCHAR> combo_string(MAX_PATH);

			sendItemMessage(IDC_COMBO_PROFILE,
							CB_GETLBTEXT,
							(WPARAM)sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0),
							(LPARAM)&combo_string[0]);

			bool is_default_profile=m_config_list[0]->cfg().gui4reces.default_profile==&combo_string[0];
			bool checked_item=is_default_profile;
			tstring cfg_name(&combo_string[0]);
			tstring old_cfg_path(path::getExeDirectory()+_T("\\cfg\\")+&combo_string[0]+_T(".cfg"));

			CfgNameDialog cfgNameDialog(cfg_name,checked_item);
			cfgNameDialog.doModal(handle());
			if(!path::isBadName(cfg_name.c_str())&&
				!cfg_name.empty()){
				//リネーム
				if(::MoveFile(old_cfg_path.c_str(),(path::getExeDirectory()+_T("\\cfg\\")+cfg_name+_T(".cfg")).c_str())){
					sendItemMessage(IDC_COMBO_PROFILE,
									CB_DELETESTRING,
									(WPARAM)current_sel,
									0);
					sendItemMessage(IDC_COMBO_PROFILE,
									CB_INSERTSTRING,
									(WPARAM)current_sel,
									(LPARAM)cfg_name.c_str());
					sendItemMessage(IDC_COMBO_PROFILE,
									CB_SETCURSEL,
									(WPARAM)current_sel,
									0);

					if(is_default_profile&&
					   !checked_item){
						//デフォルト設定解除
						m_config_list[0]->cfg().gui4reces.default_profile.clear();
						::EnableWindow(getDlgItem(IDC_STATIC_PROFILE_DEFAULT),false);
					}else if(!is_default_profile&&
							 checked_item){
						//デフォルト設定
						m_config_list[0]->cfg().gui4reces.default_profile.assign(cfg_name);
						::EnableWindow(getDlgItem(IDC_STATIC_PROFILE_DEFAULT),true);
					}
				}else{
					MessageBox(handle(),_T("リネームに失敗しました"),_T("プロファイルの編集"),MB_ICONSTOP);
				}
			}else if(!cfg_name.empty()){
				MessageBox(handle(),_T("ファイル名に次の文字は使えません。\n\t\\ / : , ; * ? \" < > |"),_T("プロファイルの編集"),MB_ICONSTOP);
				sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_PROFILE_EDIT,0),0);
			}
			return true;
		}

		case IDC_BUTTON_PROFILE_REMOVE:{
			//プロファイルを削除
			if(!sendItemMessage(IDC_COMBO_PROFILE,CB_GETCOUNT,0,0))break;

			int current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

			if(current_sel==CB_ERR)break;

			if(::MessageBox(handle(),
							_T("選択しているプロファイルを削除しますか?"),
							_T("プロファイル削除の確認"),
							MB_YESNO|MB_ICONQUESTION)!=IDYES){
				break;
			}

			sendItemMessage(IDC_COMBO_PROFILE,
							CB_DELETESTRING,
							(WPARAM)current_sel,
							0);
			sendItemMessage(IDC_COMBO_PROFILE,
							CB_SETCURSEL,
							(WPARAM)(current_sel!=0)?current_sel-1:current_sel,
							0);

			if(m_config_list[current_sel+1]){
				::DeleteFile(m_config_list[current_sel+1]->filepath().c_str());
				SAFE_DELETE(m_config_list[current_sel+1]);
				m_config_list[current_sel+1]=NULL;

					if(m_config_list[0]->cfg().gui4reces.default_profile==
					   path::removeExtension(path::getFileName(m_config_list[current_sel+1]->filepath()))){
						m_config_list[0]->cfg().gui4reces.default_profile.clear();
					}
			}
			return true;
		}

		case IDC_BUTTON_PROFILE_SHORTCUT:{
			//ショートカット
			if(int id=m_create_shortcut_menu.popup(handle(),getDlgItem(LOWORD(wparam)))){
				int current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

				std::vector<TCHAR> link_dir(MAX_PATH);
				tstring link_path;

				if(id==IDM_SHORTCUT_GUI_DESKTOP||
				   id==IDM_SHORTCUT_CUI_DESKTOP){
					//Desktopパスを取得
					::SHGetSpecialFolderPath(NULL,&link_dir[0],CSIDL_DESKTOP,false);
				}else{
					//SendToパスを取得
					::SHGetSpecialFolderPath(NULL,&link_dir[0],CSIDL_SENDTO,false);
				}

				std::vector<TCHAR> exe_path_(MAX_PATH);
				tstring exe_path;

				if(::GetModuleFileName(NULL,&exe_path_[0],exe_path_.size())){
					exe_path.assign(&exe_path_[0]);

					bool exec_reces=id==IDM_SHORTCUT_CUI_DESKTOP||id==IDM_SHORTCUT_CUI_SENDTO;

					if(exec_reces){
						current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

						if(current_sel==CB_ERR){
							//新規作成
							sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_PROFILE_ADD,0),0);
							if((current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0))==CB_ERR){
								return true;
							}
						}
						exe_path.assign(path::addTailSlash(path::getParentDirectory(exe_path))+=_T("reces.exe"));
					}


					link_path.assign(&link_dir[0]);
					link_path+=_T("\\")+
						((current_sel==CB_ERR)?
						_T("gui4recesを起動"):
						 path::removeExtension(path::getFileName(m_config_list[current_sel+1]->filepath())))+
							_T(".lnk");

					int show_cmd=SW_SHOWNORMAL;
					tstring description(path::removeExtension(path::getFileName(m_config_list[current_sel+1]->filepath())));

					CreateShortcutDialog shortcut_dialog(exec_reces,show_cmd,link_path,description);
					if(IDCANCEL==shortcut_dialog.doModal(handle()))return true;

					exe_path.assign(path::addTailSlash(path::getParentDirectory(exe_path))+=((!exec_reces)?_T("gui4reces.exe"):_T("reces.exe")));

					if(exec_reces){
						current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

						if(current_sel==CB_ERR){
							//新規作成
							sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_PROFILE_ADD,0),0);
							if((current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0))==CB_ERR){
								return true;
							}
						}
					}

					tstring arg((!exec_reces)?_T("/profile:"):_T("/{"));
					const tstring& profile((!exec_reces)?
										   path::removeExtension(path::getFileName(m_config_list[current_sel+1]->filepath())):
										   m_config_list[current_sel+1]->filepath());

					if(str::containsWhiteSpace(profile))arg+=_T("\"");
					arg+=profile;
					if(str::containsWhiteSpace(profile))arg+=_T("\"");

					fileoperation::createShortcut(link_path.c_str(),
												  exe_path.c_str(),
												  ((current_sel==CB_ERR)?
												   NULL:
												   arg.c_str()),
												  show_cmd,
												  description.c_str(),
												  NULL);
				}
			}
			return true;
		}



		case IDC_CHECKBOX_TOPMOST:{
			//最前面表示
			m_config_list[0]->cfg().gui4reces.top_most=getCheck(LOWORD(wparam));
			topMost(m_config_list[0]->cfg().gui4reces.top_most);
			return true;
		}


		//動作
		case IDC_RADIO_MODE_RECOMPRESS:
			//再圧縮
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_RECOMPRESS;
			m_tab->select(TAB_COMPRESS);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("再圧縮(&R)"));
			return true;

		case IDC_RADIO_MODE_COMPRESS:
			//圧縮
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_COMPRESS;
			m_tab->select(TAB_COMPRESS);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("圧縮(&R)"));
			return true;

		case IDC_RADIO_MODE_EXTRACT:
			//解凍
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_EXTRACT;
			m_tab->select(TAB_EXTRACT);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("解凍(&R)"));
			return true;

		case IDC_RADIO_MODE_LIST:
			//リスト
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_LIST;
			m_tab->select(TAB_MODE);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("リスト(&R)"));
			return true;

		case IDC_RADIO_MODE_TEST:
			//テスト
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_TEST;
			m_tab->select(TAB_MODE);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("テスト(&R)"));
			return true;

		case IDC_RADIO_MODE_DELETE:
			//削除
			for(int i=0;i<ARRAY_SIZEOF(mode::idc_list);i++){
				sendItemMessage(mode::idc_list[i],BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			}
			sendItemMessage(LOWORD(wparam),
							BM_SETCHECK,
							(WPARAM)BST_CHECKED,
							0
							);
			m_config_list[0]->cfg().mode=MODE_DELETE;
			m_tab->select(TAB_FILTER);
			::SetWindowText(getDlgItem(IDC_BUTTON_RUN),_T("削除(&R)"));
			return true;


		//リスト
		case IDC_BUTTON_ADD:{
			//開く
			if(int id=m_add_item_menu.popup(handle(),getDlgItem(LOWORD(wparam)))){
				std::list<tstring> file_list;

				switch(id){
					case IDM_ADD_FILE:{
						//ファイル
						FileDialog file_dialog;
						tstring file_path;

						if(!file_dialog.doModalOpen(&file_list,
													handle(),
													true,
													_T("全てのファイル (*.*)\0*.*\0\0"),
													_T("処理したいファイルを選択してください"))){
							return false;
						}
						break;
					}

					case IDM_ADD_DIR:{
						//ディレクトリ
						FolderDialog folder_dialog;
						tstring file_path;

						if(!folder_dialog.doModalOpen(&file_path,
													  handle(),
													  _T("全てのディレクトリ (*.:)\0*.:\0\0"),
													  _T("処理したいディレクトリを選択してください"))){
							return false;
						}
						file_list.push_back(file_path);
						break;
					}

					default:
						break;
				}

				for(std::list<tstring>::iterator ite=file_list.begin(),end=file_list.end();ite!=end;++ite){
					m_listview->setCheckState(m_listview->insertItem(ite->c_str()));
				}
				::SetFocus(m_listview->handle());

				if(m_config_list[0]->cfg().gui4reces.at_once){
					sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_RUN,0),0);
				}
			}
			return true;
		}


		case IDC_BUTTON_REMOVE:
			//削除
			m_listview->sendMessage(WM_KEYDOWN,VK_DELETE,0);
			return true;

		case IDC_BUTTON_CLEAR:
			//クリア
			ListView_DeleteAllItems(m_listview->handle());
			::SetWindowText(getDlgItem(IDC_STATIC_LIST),_T(""));
			return true;


		case IDC_CHECKBOX_QUIT_RECES:
			//recesのウインドウを閉じる
			m_config_list[0]->cfg().general.quit=getCheck(LOWORD(wparam));
			return true;

		case IDC_CHECKBOX_QUIT_GUI4RECES:
			//gui4recesを終了する
			m_config_list[0]->cfg().gui4reces.quit=getCheck(LOWORD(wparam));
			return true;


		case IDC_CHECKBOX_AT_ONCE:
			//すぐに開始
			m_config_list[0]->cfg().gui4reces.at_once=getCheck(LOWORD(wparam));
			return true;


		case IDC_BUTTON_VERSION:{
			//バージョン情報
			tstring library_version;
			tstring cmd_line(_T("reces.exe "));

			if(!m_config_list[0]->cfg().general.spi_dir.empty()){
				cmd_line.append(format(_T("/Ds%s%s%s "),
									   (str::containsWhiteSpace(m_config_list[0]->cfg().general.spi_dir))?_T("\""):_T(""),
									   path::removeTailSlash(m_config_list[0]->cfg().general.spi_dir).c_str(),
									   (str::containsWhiteSpace(m_config_list[0]->cfg().general.spi_dir))?_T("\""):_T("")));
			}
			if(!m_config_list[0]->cfg().general.wcx_dir.empty()){
				cmd_line.append(format(_T("/Dw%s%s%s "),
									   (str::containsWhiteSpace(m_config_list[0]->cfg().general.wcx_dir))?_T("\""):_T(""),
									   path::removeTailSlash(m_config_list[0]->cfg().general.wcx_dir).c_str(),
									   (str::containsWhiteSpace(m_config_list[0]->cfg().general.wcx_dir))?_T("\""):_T("")));
			}
			cmd_line.append(_T("/mv"));

			PipeRedirect pipe;
			pipe.launch(_T("reces.exe /mv reces.exe"));
			pipe.launch(cmd_line.c_str());
			library_version=pipe.buffer();

			AboutDialog about_dialog;

			about_dialog.setParam(&library_version);
			about_dialog.doModal(handle());
			return true;
		}

		case IDC_BUTTON_HELP:{
			//ヘルプ
			::ShellExecute(NULL,
						   _T("open"),
						   (path::addTailSlash(
#ifndef _WIN64
							   path::getExeDirectory()
#else
							   path::getParentDirectory(path::getExeDirectory())
#endif
							   )+
							_T("gui4recesHelp.chm")).c_str(),
						   NULL,
						   NULL,
						   SW_SHOWNORMAL);
			return true;
		}

		case IDC_BUTTON_RUN:{
			//実行
			{
				bool empty=true;
				int cnt=m_listview->getItemCount();
				if(!cnt)break;

				for(int item=0;item<cnt;++item){
					if(m_listview->getCheckState(item)){
						empty=false;
						break;
					}
				}
				if(empty)break;
			}

			static bool working=false;

			if(working)break;

			working=true;

			tstring list_file_path(tempfile::create(_T("g4r"),m_temp_dir.c_str()));
			tstring password_list_path;
			//実行時ファイル指定用cfgファイル
			tstring oF_cfg_path(tempfile::generateName(_T("g4r"),m_temp_dir.c_str()));
			Config oF_cfg(oF_cfg_path.c_str());



			m_config_list[0]->save();

			tstring cmd(format(_T("reces.exe /{%s%s%s /@%s%s%s"),
							   (str::containsWhiteSpace(m_config_list[0]->filepath()))?_T("\""):_T(""),
							   m_config_list[0]->filepath().c_str(),
							   (str::containsWhiteSpace(m_config_list[0]->filepath()))?_T("\""):_T(""),
							   (str::containsWhiteSpace(list_file_path))?_T("\""):_T(""),
							   list_file_path.c_str(),
							   (str::containsWhiteSpace(list_file_path))?_T("\""):_T("")));

			if(m_config_list[0]->cfg().compress.choose_output_file_each_time&&
			   (m_config_list[0]->cfg().mode==MODE_RECOMPRESS||m_config_list[0]->cfg().mode==MODE_COMPRESS)){
				//ファイル名指定のみの設定ファイル追加
				cmd.append(format(_T(" /{%s%s%s"),
								  (str::containsWhiteSpace(oF_cfg_path))?_T("\""):_T(""),
								  oF_cfg_path.c_str(),
								  (str::containsWhiteSpace(oF_cfg_path))?_T("\""):_T("")));
			}

			//パスワードの処理
			std::vector<tstring> password_list;

			//dynamic_cast...
			if(((PasswordTab*)m_tab_list[TAB_PASSWORD])->PasswordTab::getPasswordList(&password_list)){
				password_list_path.assign(tempfile::create(_T("g4r"),m_temp_dir.c_str()));

				if(path::fileExists(password_list_path.c_str())){
					File password_list_file(password_list_path.c_str(),CREATE_ALWAYS,GENERIC_READ|GENERIC_WRITE,0,File::UTF16LE);

					for(std::vector<tstring>::iterator ite=password_list.begin(),end=password_list.end();ite!=end;++ite){
						password_list_file.writeEx(_T("%s\r\n"),ite->c_str());
					}
					if(password_list_file.getSize()){
						cmd.append(format(_T(" /pf%s%s%s"),
										  (str::containsWhiteSpace(password_list_path))?_T("\""):_T(""),
										  password_list_path.c_str(),
										  (str::containsWhiteSpace(password_list_path))?_T("\""):_T("")));
					}
				}
			}

			if(m_config_list[0]->cfg().mode==MODE_RECOMPRESS){
				//dynamic_cast...
				tstring new_password;

				if(((PasswordTab*)m_tab_list[TAB_PASSWORD])->PasswordTab::getNewPassword(&new_password)){
					cmd.append(format(_T(" /pn%s%s%s"),
									  (str::containsWhiteSpace(new_password))?_T("\""):_T(""),
									  new_password.c_str(),
									  (str::containsWhiteSpace(new_password))?_T("\""):_T("")));
				}
			}

			bool choose_each_time=m_config_list[0]->cfg().compress.choose_output_file_each_time&&
				m_config_list[0]->cfg().compress.each_file&&
					(m_config_list[0]->cfg().mode==MODE_RECOMPRESS||m_config_list[0]->cfg().mode==MODE_COMPRESS);


			topMost(false);
			showDialog(SW_HIDE);

			int item=0;
			//チェックの付いた項目の内先頭
			int header_item=0;

			do{
				{
					bool end=false;
					File list_file(list_file_path.c_str(),CREATE_ALWAYS,GENERIC_READ|GENERIC_WRITE,0,File::UTF16LE);

					for(int cnt=m_listview->getItemCount();;item++){
						if(item>=cnt){end=true;break;}
						if(m_listview->getCheckState(item)){
							if(!header_item)header_item=item;
							list_file.writeEx(_T("%s\r\n"),m_listview->getItemText(item).c_str());
							if(choose_each_time)break;
						}
					}

					if(!list_file.getSize())break;

					if(end&&choose_each_time)break;
				}

				if(m_config_list[0]->cfg().compress.choose_output_file_each_time&&
				   (m_config_list[0]->cfg().mode==MODE_RECOMPRESS||m_config_list[0]->cfg().mode==MODE_COMPRESS)){
					tstring ext(((CompressTab*)m_tab_list[TAB_COMPRESS])->CompressTab::getExtension());

					FileDialog file_dialog;

					if(!file_dialog.doModalSave(&oF_cfg.cfg().compress.output_file,
											   NULL,
											   _T("全てのファイル (*.*)\0*.*\0\0"),
											   _T("保存ファイル名を入力してください"),
												path::getParentDirectory(m_listview->getItemText((choose_each_time)?
																								 item:
																								 header_item)).c_str(),
												(path::getFileName(m_listview->getItemText((choose_each_time)?
																						  item:
																						  header_item))+ext).c_str())){
						break;
					}
					oF_cfg.save();
				}


#ifdef _DEBUG
				msg(cmd.c_str());
#endif

				if(m_config_list[0]->cfg().gui4reces.log){
					cmd+=_T(" /q");
				}

				TCHAR* cmd_buffer=new TCHAR[cmd.length()+1];

				lstrcpy(cmd_buffer,cmd.c_str());

				if(m_config_list[0]->cfg().gui4reces.work_dir!=
				   m_config_list[0]->default_cfg().gui4reces.work_dir&&
				   !m_config_list[0]->cfg().gui4reces.work_dir.empty()){
					//作業ディレクトリ指定
					env::set(_T("TMP"),m_config_list[0]->cfg().gui4reces.work_dir.c_str());
				}else{
					//作業ディレクトリ指定
					env::set(_T("TMP"),m_config_list[0]->default_cfg().gui4reces.work_dir.c_str());
				}

				if(!m_config_list[0]->cfg().gui4reces.log){
					STARTUPINFO startup_info={sizeof(STARTUPINFO)};
					PROCESS_INFORMATION process_info={};
					startup_info.dwFlags=STARTF_USESHOWWINDOW;
					startup_info.wShowWindow=SW_SHOWNORMAL;
					::CreateProcess(NULL,cmd_buffer,NULL,NULL,false,0,NULL,NULL,&startup_info,&process_info);
					::WaitForSingleObject(process_info.hProcess,INFINITE);
					SAFE_CLOSE(process_info.hThread);
					SAFE_CLOSE(process_info.hProcess);
				}else{
					PipeRedirectToTextBox pipe;
					pipe.launch(cmd_buffer);
				}
				SAFE_DELETE_ARRAY(cmd_buffer);

			}while(choose_each_time&&++item);

			if(!m_config_list[0]->cfg().gui4reces.quit){
				showDialog(SW_SHOWNORMAL);
				topMost(m_config_list[0]->cfg().gui4reces.top_most);
			}else{
				//gui4recesを終了する
				sendMessage(WM_COMMAND,(WPARAM)IDCANCEL,(LPARAM)0);
			}

			working=false;

			return true;
		}

		default:
			break;
	}

	switch(HIWORD(wparam)){
		case CBN_SELCHANGE:
			switch(LOWORD(wparam)){
				case IDC_COMBO_PROFILE:{
					//プロファイルの読み込み
					int current_sel=sendItemMessage(IDC_COMBO_PROFILE,CB_GETCURSEL,0,0);

					m_config_list[0]->import(*m_config_list[current_sel+1]);
					setCurrentSettings();
					for(size_t i=0,list_size=m_tab_list.size();i<list_size;i++){
						if(m_tab_list[i]!=NULL){
							m_tab_list[i]->setCurrentSettings();
						}
					}
					if(m_config_list[0]->cfg().gui4reces.default_profile==
					   path::removeExtension(path::getFileName(m_config_list[current_sel+1]->filepath()))){
						::EnableWindow(getDlgItem(IDC_STATIC_PROFILE_DEFAULT),true);
					}else{
						::EnableWindow(getDlgItem(IDC_STATIC_PROFILE_DEFAULT),false);
					}
					return true;
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

INT_PTR MainDialog::onNotify(WPARAM wparam,LPARAM lparam){
	switch(((LPNMHDR)lparam)->idFrom){
		case 0:
			if(((LPNMHDR)lparam)->hwndFrom==::GetDlgItem(m_listview->handle(),0)){
				switch(((LPNMHDR)lparam)->code){
					case NM_CLICK:
					case NM_RCLICK:
						::SetFocus(m_listview->handle());
						return true;
					default:
						break;
				}
			}
			break;
		case IDC_TAB1:
			switch(((LPNMHDR)lparam)->code){
				case TCN_SELCHANGE:
					for(size_t i=0,list_size=m_tab_list.size();i<list_size;i++){
						if(m_tab_list[i]!=NULL){
							m_tab_list[i]->showDialog(SW_HIDE);
						}
					}
					m_tab_list[TabCtrl_GetCurSel(m_tab->handle())]->showDialog();
					break;
				default:
					break;
			}//switch(((LPNMHDR)lparam)->code)
			break;
		case IDC_LIST1:
			//子へ投げる
			SendMessage(m_listview->handle(),WM_NOTIFY,wparam,lparam);
			break;
		default:
			break;
	}//switch(((LPNMHDR)lparam)->idFrom)
	return false;
}

INT_PTR MainDialog::onOk(){
	return false;
}

INT_PTR MainDialog::onDropFiles(HDROP drop_handle){
	DropFiles drop_files(drop_handle);

	//アイテムを追加
	for(size_t i=0,drop_files_count=drop_files.getCount(),count=m_listview->getItemCount();i<drop_files_count;i++){
		m_listview->setCheckState(m_listview->insertItem(drop_files.getFile(i).c_str(),count+i));
	}

	if(m_config_list[0]->cfg().gui4reces.at_once){
		sendMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_RUN,0),0);
	}
	return true;
}

INT_PTR MainDialog::onSize(WPARAM wparam,LPARAM lparam){
	if(!m_wnd_size_list.size())return false;

	DeferPos defer_pos(m_wnd_size_list.size());

	for(size_t i=0,list_size=m_wnd_size_list.size();i<list_size;i++){
		if(i==0){
			defer_pos.move(m_wnd_size_list[i].wnd,
						   NULL,
						   0,0,
						   LOWORD(lparam)-m_wnd_size_list[i].width_diff,
						   HIWORD(lparam)-m_wnd_size_list[i].height_diff,
						   SWP_NOMOVE|SWP_NOZORDER);
		}else{
			defer_pos.move(m_wnd_size_list[i].wnd,
						   NULL,
						   m_wnd_size_list[i].pt.x,
						   (HIWORD(lparam)-(m_wnd_size_list[i].parent_rect.bottom-m_wnd_size_list[i].parent_rect.top))+m_wnd_size_list[i].pt.y,
						   0,0,
						   SWP_NOSIZE|SWP_NOZORDER);
		}
	}
	return false;
}

INT_PTR MainDialog::onGetMinMaxInfo(WPARAM wparam,LPARAM lparam){
	LPMINMAXINFO info=reinterpret_cast<LPMINMAXINFO>(lparam);

	info->ptMinTrackSize.x=info->ptMaxTrackSize.x=m_wnd_width;
	info->ptMinTrackSize.y=m_wnd_height;
	return true;
}

INT_PTR MainDialog::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_GETMINMAXINFO:
			return onGetMinMaxInfo(wparam,lparam);
		case WM_SETFOCUS:
			::SetFocus(m_listview->handle());
			return true;
		default:
			break;
	}
	return false;
}
