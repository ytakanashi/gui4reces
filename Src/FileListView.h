//FileListView.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.3 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF
#define _FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF



#include"resources/resource.h"

#if 0
class DragImage{
public:
	DragImage(HWND listview_handle,int item):
		m_image_list(NULL),
		m_parent_handle(::GetParent(listview_handle)){
			POINT item_pt;
			m_image_list=ListView_CreateDragImage(listview_handle,item,&item_pt);
			POINT cursor_pt;
			::GetCursorPos(&cursor_pt);
			::ScreenToClient(listview_handle/*m_parent_handle*/,&cursor_pt);
			ImageList_BeginDrag(m_image_list,0,cursor_pt.x-item_pt.x,cursor_pt.y-item_pt.y);
			ImageList_DragEnter(m_parent_handle,0,0);
			::SetCapture(m_parent_handle);
		}
	~DragImage(){
		ImageList_DragLeave(m_parent_handle);
		ImageList_EndDrag();
		ImageList_Destroy(m_image_list);
		ReleaseCapture();
		::ShowCursor(true);
	}
	void drag(int x,int y){
		POINT pt={x,y};
		::ClientToScreen(m_parent_handle,&pt);
		RECT rc;
		::GetWindowRect(m_parent_handle,&rc);
		ImageList_DragMove(pt.x-rc.left,pt.y-rc.top);
	}
private:
	HIMAGELIST m_image_list;
	HWND m_parent_handle;
public:
	HWND captured_wnd()const{return m_parent_handle;}
};
#endif

class FileListView:public sslib::ListView{
public:
	FileListView(HWND parent_handle):
		ListView(parent_handle,IDC_LIST1,true)
#if 0
		,m_drag_image(NULL)
#endif
		,m_listview_menu()
		{m_listview_menu.load(IDR_MENU_LISTVIEW);}
	~FileListView(){}
private:
#if 0
	DragImage* m_drag_image;
#endif
	sslib::Menu m_listview_menu;
private:
	LRESULT onNotify(WPARAM wparam,LPARAM lparam);
	LRESULT onMessage(UINT message,WPARAM wparam,LPARAM lparam);
#if 0
	INT_PTR onMouseMove(WPARAM wparam,LPARAM lparam);
	INT_PTR onLButtonUp(WPARAM wparam,LPARAM lparam);
public:
	HWND getCapture()const{return m_drag_image&&m_drag_image->captured_wnd()==::GetCapture()?m_drag_image->captured_wnd():NULL;}
#endif
};

#endif //_FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF
