//FileListView.cpp
//ファイルリストビュー

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.2 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"FileListView.h"


using namespace sslib;



LRESULT FileListView::onNotify(WPARAM wparam,LPARAM lparam){
	static HWND edit_handle=NULL;

	switch(((LPNMLISTVIEW)lparam)->hdr.code){
		case LVN_INSERTITEM:
		case LVN_DELETEITEM:
		case LVN_ITEMCHANGED:{
			int cnt=ListView_GetSelectedCount(handle());
			int all=ListView_GetItemCount(handle());

			::SetWindowText(::GetDlgItem(::GetParent(handle()),IDC_STATIC_LIST),
							(cnt&&cnt!=all)?
							format(_T("処理対象: 選択中の %d 個のファイル\n"),cnt).c_str():
							_T("処理対象: 全てのファイル"));
			ListView_SetColumnWidth(handle(),0,LVSCW_AUTOSIZE_USEHEADER);
			break;
		}

		case LVN_BEGINLABELEDIT:
			edit_handle=ListView_GetEditControl(handle());
			break;

		case LVN_ENDLABELEDIT:{
			std::vector<TCHAR> buffer(MAX_PATHW);

			::GetWindowText(edit_handle,&buffer[0],buffer.size());
			ListView_SetItemText(handle(),((NMLVDISPINFO*)lparam)->item.iItem,0,&buffer[0]);
			break;
		}

		case LVN_KEYDOWN:
			if(((LPNMLVKEYDOWN)lparam)->wVKey==VK_F2){
				ListView_EditLabel(handle(),getNextItem());
			}
			break;

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),WM_NOTIFY,wparam,lparam);
}

LRESULT FileListView::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_DROPFILES:
			//親に送る
			::PostMessage(parent_handle(),WM_DROPFILES,wparam,lparam);
			break;

		case WM_KEYDOWN:
			switch(wparam){
				case VK_DELETE:{
					//選択しているアイテムを削除
					int item=-1;

					for(;;){
						if((item=getNextItem())==-1)break;
						deleteItem(item);
					}
					break;
				}

				case 'A':
					if(::GetAsyncKeyState(VK_CONTROL)<0&&
					   ::GetAsyncKeyState(VK_SHIFT)>=0&&
					   ::GetAsyncKeyState(VK_MENU)>=0){
						//Ctrl+A押下ならば全選択
						selectAll();
					}
				default:
					break;
			}
			break;

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
}
