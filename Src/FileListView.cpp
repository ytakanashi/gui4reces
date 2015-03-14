//FileListView.cpp
//ファイルリストビュー

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.3 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"FileListView.h"


using namespace sslib;


LRESULT FileListView::onNotify(WPARAM wparam,LPARAM lparam){
	static HWND edit_handle=NULL;

	switch(((LPNMLISTVIEW)lparam)->hdr.code){
		case NM_RCLICK:{
			::SetFocus(handle());

			if(!getItemCount())break;

			POINT pt;

			::GetCursorPos(&pt);
			if(int id=m_listview_menu.popup(parent_handle(),pt.x,pt.y,TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_TOPALIGN|TPM_RETURNCMD)){
				//リストから削除
				if(id==IDM_LISTVIEW_REMOVE){sendMessage(WM_KEYDOWN,VK_DELETE,0);break;}
				//選択範囲を反転
				if(id==IDM_LISTVIEW_INVERSE){
					for(int item=-1;;){
						if((item=getNextItem(item,LVNI_ALL))==-1)break;
						setCheckState(item,!getCheckState(item));
					}
				}else{
					//選択
					//選択解除
					//全選択
					//全解除
					for(int item=-1;;){
						if((item=getNextItem(item,
											 (id==IDM_LISTVIEW_SELECT||id==IDM_LISTVIEW_DESELECT)?
											 LVNI_ALL|LVIS_SELECTED:
											 LVNI_ALL
											 ))==-1)break;
						setCheckState(item,id==IDM_LISTVIEW_SELECT||id==IDM_LISTVIEW_ALLSELECT);
					}
				}
			}else{
				break;
			}
			//fall through
		}
		case LVN_ITEMCHANGED:
			//チェックボックスに変化があったか
			if((((LPNMLISTVIEW)lparam)->uNewState&LVIS_STATEIMAGEMASK)==
			   (((LPNMLISTVIEW)lparam)->uOldState&LVIS_STATEIMAGEMASK)){
				break;
			}
			//fall through
		case LVN_INSERTITEM:
		case LVN_DELETEITEM:{
			int cnt=0;
			int all=getItemCount();

			for(int i=0;i<all;i++)if(getCheckState(i))++cnt;

			::SetWindowText(::GetDlgItem(::GetParent(handle()),IDC_STATIC_LIST),
							(cnt&&cnt!=all)?
							format(_T("処理対象: %d 個中の %d 個のファイル\n"),all,cnt).c_str():
							(cnt)?_T("処理対象: 全てのファイル"):
							_T(""));
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
			}else if(((LPNMLVKEYDOWN)lparam)->wVKey==VK_SPACE){
				for(int item=-1;;){
					if((item=getNextItem(item))==-1)break;
					//フォーカスがあるアイテムは自動で反転してくれる
					if(!getItemState(item,LVIS_FOCUSED)){
						setCheckState(item,!getCheckState(item));
					}
				}
			}
			break;

#if 0
		case LVN_BEGINDRAG:
			m_drag_image=new DragImage(handle(),((LPNMLISTVIEW)lparam)->iItem);
			break;
#endif

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),WM_NOTIFY,wparam,lparam);
}

#if 0
INT_PTR FileListView::onMouseMove(WPARAM wparam,LPARAM lparam){
	if(m_drag_image)m_drag_image->drag(LOWORD(lparam),HIWORD(lparam));
	return true;
}

INT_PTR FileListView::onLButtonUp(WPARAM wparam,LPARAM lparam){
	SAFE_DELETE(m_drag_image);
	return true;
}
#endif

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
					for(int item=-1;(item=getNextItem())!=-1;){
						deleteItem(item);
					}
					//チェックボックス付リストビューだと一部LVN_DELETEITEMが来ない?ため
					NMHDR nmhdr={};

					nmhdr.idFrom=wparam;
					nmhdr.hwndFrom=handle();
					nmhdr.code=LVN_DELETEITEM;
					sendMessage(WM_NOTIFY,LOWORD(wparam),(LPARAM)&nmhdr);
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

#if 0
		case WM_MOUSEMOVE:
			return onMouseMove(wparam,lparam);

		case WM_LBUTTONUP:
			return onLButtonUp(wparam,lparam);
#endif

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
}
