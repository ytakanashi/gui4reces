//MainDialog.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
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
#include"OtherTab.h"
#include"FileListView.h"
#include"resources/resource.h"



class MainDialog:public sslib::Dialog{
public:
	MainDialog():
		Dialog(IDD_DIALOG_MAIN),
		m_tab(NULL),
		m_tab_list(),
		m_listview(NULL),
		m_create_shortcut_menu(NULL),
		m_create_shortcut_sub_menu(NULL),
		m_add_item_menu(NULL),
		m_add_item_sub_menu(NULL),
		m_temp_dir(){
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
		TAB_OTHER,
	};

	std::vector<Config*> m_config_list;

	sslib::Tab* m_tab;
	std::vector<TabBase*> m_tab_list;

	FileListView* m_listview;

	HMENU m_create_shortcut_menu,m_create_shortcut_sub_menu;
	HMENU m_add_item_menu,m_add_item_sub_menu;

	//リストファイル用一時ディレクトリ
	tstring m_temp_dir;

private:
	void setCurrentSettings();
	//メッセージハンドラ
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onDestroy();
	bool onCommand(WPARAM wparam,LPARAM lparam);
	bool onNotify(WPARAM wparam,LPARAM lparam);
	bool onOk();
	bool onDropFiles(HDROP drop_handle);
	bool onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};


#endif //_MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9
