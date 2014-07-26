//AboutDialog.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC
#define _ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC


#include"resources/resource.h"



class AboutDialog:public sslib::Dialog{
public:
	AboutDialog():
	Dialog(IDD_DIALOG_ABOUT),
		m_icon(NULL),
		m_width_dist(0),
		m_height_dist(0),
		m_wnd_width(0),
		m_wnd_height(0){}
	~AboutDialog(){}

private:
	HICON m_icon;
	int m_width_dist;
	int m_height_dist;
	int m_wnd_width;
	int m_wnd_height;

private:
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onCommand(WPARAM wparam,LPARAM);
	bool onPaint();
	bool onSize(WPARAM wparam,LPARAM lparam);
	bool onGetMinMaxInfo(WPARAM wparam,LPARAM lparam);
	bool onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};


#endif //_ABOUTDIALOG_H_03BF8676_8FB4_4f14_842E_27A87C6E3DAC
