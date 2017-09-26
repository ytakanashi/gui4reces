//AboutDialog.cpp
//バージョン情報ダイアログ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.8 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`

#include"StdAfx.h"
#include"AboutDialog.h"

using namespace sslib;




LRESULT AboutDialog::Edit::onGetDlgCode(WPARAM wparam,LPARAM lparam){
	//ダイアログ表示時、エディットボックス内文字列が全選択されるのを防ぐ
	return ::CallWindowProc(default_proc(),handle(),WM_GETDLGCODE,wparam,lparam)&~DLGC_HASSETSEL;
}

LRESULT AboutDialog::Edit::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_GETDLGCODE:
			return onGetDlgCode(wparam,lparam);
		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
}

LRESULT AboutDialog::Link::onSetCursor(WPARAM wparam,LPARAM lparam){
	::SetCursor(m_hand_cursor);
	return true;
}

LRESULT AboutDialog::Link::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_SETCURSOR:
			return onSetCursor(wparam,lparam);
		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
}

INT_PTR AboutDialog::onInitDialog(WPARAM wparam,LPARAM lparam){
	//アイコンの設定(タイトルバー)
	setIcon(IDI_ICON1);

	//アイコンの読み込み
	m_icon=static_cast<HICON>(::LoadImage(inst(),
										  MAKEINTRESOURCE(IDI_ICON1),
										  IMAGE_ICON,
										  256,256,
										  LR_SHARED));
	if(m_icon==NULL){
		m_xp_mode=true;
		m_icon=static_cast<HICON>(::LoadImage(inst(),
											  MAKEINTRESOURCE(IDI_ICON1),
											  IMAGE_ICON,
											  48,48,
											  LR_SHARED));
	}

	//gui4recesのバージョンを書き込む
	::SetWindowText(getDlgItem(IDC_STATIC_VERSION),(tstring(_T("gui4reces Ver."))+SOFTWARE_VERSION).c_str());

	m_edit=new Edit(handle(),IDC_EDIT_VERSION,true);
	m_link=new Link(handle(),IDC_STATIC_URL,true);

	//各ライブラリの情報を追加
	if(param()){
		tstring* version_str=reinterpret_cast<tstring*>(param());
		HWND edit=getDlgItem(IDC_EDIT_VERSION);

		::SetFocus(edit);
		SendMessage(edit,EM_SETSEL,-1,-1);
		SendMessage(edit,EM_REPLACESEL,0,reinterpret_cast<LPARAM>(version_str->c_str()));
	}

	RECT wnd_rect={},edit_rect={};

	if(m_xp_mode){
		::GetWindowRect(getDlgItem(IDC_EDIT_VERSION),&edit_rect);
		::GetClientRect(handle(),&wnd_rect);

		POINT pt={edit_rect.left,edit_rect.top};
		::ScreenToClient(handle(),&pt);

		::SetWindowPos(getDlgItem(IDC_EDIT_VERSION),
					   NULL,
					   10,pt.y,
					   wnd_rect.right-20,
					   edit_rect.bottom-edit_rect.top,
					   SWP_NOZORDER);
	}

	::GetWindowRect(handle(),&wnd_rect);

	//onGetMinMaxInfo()用
	m_wnd_width=wnd_rect.right-wnd_rect.left;
	m_wnd_height=wnd_rect.bottom-wnd_rect.top;

	//ウインドウサイズ取得
	::GetClientRect(handle(),&wnd_rect);

	//リストボックスサイズ取得
	::GetWindowRect(getDlgItem(IDC_EDIT_VERSION),&edit_rect);

	m_width_diff=(wnd_rect.right-wnd_rect.left)-(edit_rect.right-edit_rect.left);
	m_height_diff=(wnd_rect.bottom-wnd_rect.top)-(edit_rect.bottom-edit_rect.top);
	return true;
}

INT_PTR AboutDialog::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDC_STATIC_URL:
			if(HIWORD(wparam)==STN_CLICKED){
				//ホームページへ
				std::vector<TCHAR> url(256);
				::GetWindowText(getDlgItem(LOWORD(wparam)),&url[0],url.size());
				::ShellExecute(NULL,_T("open"),&url[0],NULL,NULL,SW_SHOWNORMAL);
				return true;
			}
			break;
		default:
			break;
	}
	return true;
}

INT_PTR AboutDialog::onPaint(){
	PAINTSTRUCT ps;
	HDC dc=::BeginPaint(handle(),&ps);

	//アイコン描画
	if(!m_xp_mode){
		::DrawIconEx(dc,0,8,m_icon,256,256,0,NULL,DI_NORMAL);
	}else{
		::DrawIconEx(dc,200,13,m_icon,48,48,0,NULL,DI_NORMAL);
	}
	::EndPaint(handle(),&ps);
	return true;
}

INT_PTR AboutDialog::onSize(WPARAM wparam,LPARAM lparam){
	::SetWindowPos(getDlgItem(IDC_EDIT_VERSION),
				   NULL,
				   0,0,
				   LOWORD(lparam)-m_width_diff,
				   HIWORD(lparam)-m_height_diff,
				   SWP_NOMOVE|SWP_NOZORDER);
	return false;
}

INT_PTR AboutDialog::onGetMinMaxInfo(WPARAM wparam,LPARAM lparam){
	LPMINMAXINFO info=reinterpret_cast<LPMINMAXINFO>(lparam);

	info->ptMinTrackSize.x=m_wnd_width;
	info->ptMinTrackSize.y=m_wnd_height;
	return true;
}

INT_PTR AboutDialog::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_GETMINMAXINFO:
			return onGetMinMaxInfo(wparam,lparam);
		default:
			break;
	}
	return false;
}
