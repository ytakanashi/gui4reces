//AboutDialog.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.1 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC
#define _ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC


#include"resources/resource.h"
#include<windowsx.h>


class AboutDialog:public sslib::Dialog{
class Edit:public sslib::Control{
public:
	Edit(HWND parent_handle,UINT resource_id,bool sub_class=false):
		Control(parent_handle,resource_id,sub_class){}
	virtual ~Edit(){}
private:
	LRESULT onGetDlgCode(WPARAM wparam,LPARAM lparam);
	LRESULT onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};
class Link:public sslib::Control{
public:
	Link(HWND parent_handle,UINT resource_id,bool sub_class=false):
		Control(parent_handle,resource_id,sub_class),
			m_font(NULL),
			m_hand_cursor(LoadCursor(NULL,IDC_HAND)){
				LOGFONT log_font={};
				::GetObject(GetWindowFont(parent_handle),sizeof(LOGFONT),&log_font);
				log_font.lfUnderline=true;
				m_font=::CreateFontIndirect(&log_font);
				SetWindowFont(::GetDlgItem(parent_handle,resource_id),m_font,false);
			}
	virtual ~Link(){::DeleteFont(m_font);}
private:
	HFONT m_font;
	HCURSOR m_hand_cursor;
private:
	LRESULT onSetCursor(WPARAM wparam,LPARAM lparam);
	LRESULT onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};
public:
	AboutDialog():
	Dialog(IDD_DIALOG_ABOUT),
		m_icon(NULL),
		m_edit(NULL),
		m_link(NULL),
		m_xp_mode(false),
		m_width_diff(0),
		m_height_diff(0),
		m_wnd_width(0),
		m_wnd_height(0){}
	~AboutDialog(){delete m_edit;m_edit=NULL;delete m_link;m_link=NULL;}

private:
	HICON m_icon;
	Edit* m_edit;
	Link* m_link;
	bool m_xp_mode;
	int m_width_diff;
	int m_height_diff;
	int m_wnd_width;
	int m_wnd_height;

private:
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onCommand(WPARAM wparam,LPARAM lparam);
	bool onPaint();
	bool onSize(WPARAM wparam,LPARAM lparam);
	bool onGetMinMaxInfo(WPARAM wparam,LPARAM lparam);
	bool onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};


#endif //_ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC
