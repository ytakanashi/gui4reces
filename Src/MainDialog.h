﻿//MainDialog.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.6 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9
#define _MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9


#include"PrivateProfile.h"
#include"CompressTab.h"
#include"ExtractTab.h"
#include"OutputTab.h"
#include"PasswordTab.h"
#include"FilterTab.h"
#include"SplitTab.h"
#include"RemoveSourceTab.h"
#include"DirectoryTab.h"
#include"OtherTab.h"
#include"ModeTab.h"
#include"FileListView.h"
#include"resources/resource.h"


struct SIZE_INFO{
	HWND wnd;
	int width_diff;
	int height_diff;
	RECT parent_rect;
	RECT rect;
	POINT pt;
	SIZE_INFO(HWND parent_handle,HWND wnd_handle):
			wnd(NULL),
			width_diff(0),
			height_diff(0),
			parent_rect(),
			rect(),
			pt(){
		wnd=wnd_handle;

		::GetClientRect(parent_handle,&parent_rect);

		::GetWindowRect(wnd_handle,&rect);

		pt.x=rect.left;
		pt.y=rect.top;
		::ScreenToClient(parent_handle,&pt);

		width_diff=abs((parent_rect.right-parent_rect.left)-(rect.right-rect.left));
		height_diff=abs((parent_rect.bottom-parent_rect.top)-(rect.bottom-rect.top));
	}
};

class MainDialog:public sslib::Dialog{
public:
	MainDialog():
		Dialog(IDD_DIALOG_MAIN),
		m_tab(NULL),
		m_tab_list(),
		m_listview(NULL),
		m_create_shortcut_menu(),
		m_add_item_menu(),
		m_temp_dir(),
		m_wnd_height(0),
		m_wnd_width(0){
			m_config_list.push_back(new Config());
		}
	~MainDialog(){
		delete m_listview;
		m_listview=NULL;

		for(size_t i=0,list_size=m_tab_list.size();i<list_size;i++){
			if(m_tab_list[i]!=NULL){
				delete m_tab_list[i];
				m_tab_list[i]=NULL;
			}
		}

		delete m_tab;
		m_tab=NULL;

		for(size_t i=0,list_size=m_config_list.size();i<list_size;i++){
			if(m_config_list[i]!=NULL){
				delete m_config_list[i];
				m_config_list[i]=NULL;
			}
		}

	}

private:
	enum{
		TAB_COMPRESS,
		TAB_EXTRACT,
		TAB_OUTPUT,
		TAB_PASSWORD,
		TAB_FILTER,
		TAB_SPLIT,
		TAB_REMOVESOURCE,
		TAB_DIRECTORY,
		TAB_OTHER,
		TAB_MODE,
	};

	std::vector<Config*> m_config_list;

	sslib::Tab* m_tab;
	std::vector<TabBase*> m_tab_list;

	FileListView* m_listview;

	sslib::Menu m_create_shortcut_menu;
	sslib::Menu m_add_item_menu;

	//リストファイル用一時ディレクトリ
	tstring m_temp_dir;

	//リストビューより下に存在するコントロール対象
	std::vector<SIZE_INFO> m_wnd_size_list;

	int m_wnd_height;
	int m_wnd_width;

private:
	void setCurrentSettings();
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onDestroy();
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
	INT_PTR onNotify(WPARAM wparam,LPARAM lparam);
	INT_PTR onOk();
	INT_PTR onDropFiles(HDROP drop_handle);
	INT_PTR onSize(WPARAM wparam,LPARAM lparam);
	INT_PTR onGetMinMaxInfo(WPARAM wparam,LPARAM lparam);
#if 0
	INT_PTR onMouseMove(WPARAM wparam,LPARAM lparam);
	INT_PTR onLButtonUp(WPARAM wparam,LPARAM lparam);
#endif
	INT_PTR onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};


#endif //_MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9
