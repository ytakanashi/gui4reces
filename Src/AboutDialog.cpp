//AboutDialog.cpp
//バージョン情報ダイアログ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`

#include"StdAfx.h"
#include"AboutDialog.h"

using namespace sslib;



namespace{
	bool setClipboardText(HWND wnd_handle,const tstring& text){
		bool result=false;
		int buffer_size=0;

		if(!::OpenClipboard(wnd_handle))return false;

		buffer_size=text.size()*sizeof(TCHAR)+sizeof(TCHAR);
		HGLOBAL hg=::GlobalAlloc(GMEM_MOVEABLE,buffer_size);
		if(hg!=NULL){
			memcpy(::GlobalLock(hg),text.c_str(),buffer_size);
			::GlobalUnlock(hg);
			::EmptyClipboard();
#ifdef UNICODE
			if(::SetClipboardData(CF_UNICODETEXT,hg))result=true;
#else
			if(::SetClipboardData(CF_TEXT,hg))result=true;
#endif
		}
		::CloseClipboard();
		return result;
	}
}

bool AboutDialog::onInitDialog(WPARAM wparam,LPARAM lparam){
	//アイコンの設定(タイトルバー)
	setIcon(IDI_ICON1);

	//アイコンの読み込み
	m_icon=static_cast<HICON>(::LoadImage(inst(),
										  MAKEINTRESOURCE(IDI_ICON1),
										  IMAGE_ICON,
										  256,256,
										  LR_SHARED));

	//gui4recesのバージョンを書き込む
	::SetWindowText(getDlgItem(IDC_STATIC_VERSION),(tstring(_T("gui4reces Ver."))+SOFTWARE_VERSION).c_str());

	//各ライブラリの情報を追加
	if(param()){
		std::list<tstring>* library_list=reinterpret_cast<std::list<tstring>*>(param());

		for(std::list<tstring>::iterator ite=library_list->begin(),
			end=library_list->end();
			ite!=end;
			++ite){
			::SendMessage(getDlgItem(IDC_LIST_VERSION),LB_ADDSTRING,0,reinterpret_cast<LPARAM>(ite->c_str()));
		}
	}

	sendItemMessage(IDC_LIST_VERSION,LB_SETCURSEL,0,0);

	RECT wnd_rect={},list_rect={};
	//ウインドウサイズ取得
	::GetClientRect(handle(),&wnd_rect);
	//リストボックスサイズ取得
	::GetWindowRect(getDlgItem(IDC_LIST_VERSION),&list_rect);
	m_width_dist=(wnd_rect.right-wnd_rect.left)-(list_rect.right-list_rect.left);
	m_height_dist=(wnd_rect.bottom-wnd_rect.top)-(list_rect.bottom-list_rect.top);

	//onGetMinMaxInfo()用
	::GetWindowRect(handle(),&wnd_rect);
	m_wnd_width=wnd_rect.right-wnd_rect.left;
	m_wnd_height=wnd_rect.bottom-wnd_rect.top;

	return true;
}

bool AboutDialog::onCommand(WPARAM wparam,LPARAM){
	switch(LOWORD(wparam)){
		case IDC_BUTTON_VERSION_COPY:{
			//コピー
			int index=sendItemMessage(IDC_LIST_VERSION,LB_GETCURSEL,0,0);
			std::vector<TCHAR> buffer(sendItemMessage(IDC_LIST_VERSION,
													  LB_GETTEXTLEN,
													  index,
													  0)+1);

			sendItemMessage(IDC_LIST_VERSION,LB_GETTEXT,index,(LPARAM)&buffer[0]);
			setClipboardText(handle(),&buffer[0]);
			break;
		}
	}
	return true;
}

bool AboutDialog::onPaint(){
	PAINTSTRUCT ps;
	HDC dc=::BeginPaint(handle(),&ps);

	//アイコン描画
	::DrawIconEx(dc,0,8,m_icon,256,256,0,NULL,DI_NORMAL);
	::EndPaint(handle(),&ps);
	return true;
}

bool AboutDialog::onSize(WPARAM wparam,LPARAM lparam){
	::SetWindowPos(getDlgItem(IDC_LIST_VERSION),
				   NULL,
				   0,0,
				   LOWORD(lparam)-m_width_dist,
				   HIWORD(lparam)-m_height_dist,
				   SWP_NOMOVE|SWP_NOZORDER);
	return false;
}

bool AboutDialog::onGetMinMaxInfo(WPARAM wparam,LPARAM lparam){
	LPMINMAXINFO info=reinterpret_cast<LPMINMAXINFO>(lparam);

	info->ptMinTrackSize.x=m_wnd_width;
	info->ptMinTrackSize.y=m_wnd_height;
	return true;
}

bool AboutDialog::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_GETMINMAXINFO:
			return onGetMinMaxInfo(wparam,lparam);
		default:
			break;
	}
	return false;
}
