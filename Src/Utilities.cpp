//Utilities.cpp
//ゆーてぃりてぃーず

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.8 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"Utilities.h"


using namespace sslib;
namespace util{


bool PipeRedirect::launch(const TCHAR* cmd){
	begin();

	HANDLE read_handle=NULL;
	STARTUPINFO startup_info={sizeof(STARTUPINFO)};

	if(!::CreatePipe(&m_pipe.read,&m_pipe.write,NULL,0))return false;

	::DuplicateHandle(::GetCurrentProcess(),m_pipe.write,
					  ::GetCurrentProcess(),&read_handle,
					  0,true,DUPLICATE_SAME_ACCESS);
	SAFE_CLOSE(m_pipe.write);

	startup_info.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	startup_info.wShowWindow=SW_HIDE;
	startup_info.hStdOutput=read_handle;
	startup_info.hStdError=read_handle;

	std::vector<TCHAR> cmd_(lstrlen(cmd)+1);
	lstrcpyn(&cmd_[0],cmd,cmd_.size()+1);

	if(::CreateProcess(NULL,&cmd_[0],NULL,NULL,
					   true,
					   0//CREATE_NEW_PROCESS_GROUP/*CREATE_NO_WINDOW*/
					   ,
					   NULL,NULL,&startup_info,&m_process_info)){
		DWORD wait_result=0;
		std::vector<TCHAR> buffer(12,'\0');

		::WaitForInputIdle(m_process_info.hProcess,INFINITE);

		while((wait_result=::WaitForSingleObject(m_process_info.hProcess,0))!=WAIT_ABANDONED){
			DWORD avail_bytes=0;

			if(::PeekNamedPipe(m_pipe.read,NULL,0,NULL,&avail_bytes,NULL)&&
			   avail_bytes){
				DWORD read=0;

				buffer.assign(avail_bytes,'\0');

				if(::ReadFile(m_pipe.read,&buffer[0],buffer.size(),&read,NULL)){
					processBuffer(&buffer[0]);
				}
			}

			if(!messageLoop())break;

			if(wait_result==WAIT_OBJECT_0){
				break;
			}
		}

		SAFE_CLOSE(m_process_info.hThread);
		SAFE_CLOSE(m_process_info.hProcess);
		SAFE_CLOSE(read_handle);
		m_pipe.close();

	}

	end();
	return true;
}

//namespace util
}
