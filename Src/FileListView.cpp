//FileListView.cpp
//ファイルリストビュー

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.1 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"FileListView.h"


using namespace sslib;



LRESULT FileListView::onNotify(WPARAM wparam,LPARAM lparam){
	switch(((LPNMLISTVIEW)lparam)->hdr.code){
		case LVN_INSERTITEM:
		case LVN_DELETEITEM:
		case LVN_ITEMCHANGED:{
			int cnt=ListView_GetSelectedCount(handle());
			int all=ListView_GetItemCount(handle());
			VariableArgument va(_T("処理対象: 選択中の %d 個のファイル\n"),cnt);
			::SetWindowText(::GetDlgItem(::GetParent(handle()),IDC_STATIC_LIST),
							(cnt&&cnt!=all)?va.get():_T("処理対象: 全てのファイル"));
			ListView_SetColumnWidth(handle(),0,LVSCW_AUTOSIZE_USEHEADER);
			break;
		}

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),WM_NOTIFY,wparam,lparam);
}

LRESULT FileListView::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_DROPFILES:{
			DropFiles drop_files(reinterpret_cast<HDROP>(wparam));

			//アイテムを追加
			for(size_t i=0,drop_files_count=drop_files.getCount(),count=getItemCount();i<drop_files_count;i++){
				insertItem(drop_files.getFile(i).c_str(),count+i);
			}
			break;
		}

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
