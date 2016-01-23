//Utilities.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.7 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _UTILITIES_H_EFE9B03E_CA91_4541_99D8_A2CA1A3F5FDC
#define _UTILITIES_H_EFE9B03E_CA91_4541_99D8_A2CA1A3F5FDC


namespace util{


class PipeRedirect{
public:
	struct PIPE{
		HANDLE read;
		HANDLE write;
		PIPE():read(NULL),write(NULL){}
		void close(){SAFE_CLOSE(read);SAFE_CLOSE(write);}
	};
public:
	PipeRedirect():
	m_result(),m_pipe(),m_process_info(){}
	virtual ~PipeRedirect(){m_pipe.close();}
	tstring& buffer(){return m_result;}
protected:
	virtual void begin(){}
	virtual void end(){}
	virtual void processBuffer(const TCHAR* buffer){m_result.append(buffer);}
	virtual bool messageLoop(){
		MSG msg;

		if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message==WM_QUIT){
				return false;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		return true;
	}
public:
	bool launch(const TCHAR* cmd);
	tstring result(){return m_result;}
protected:
	tstring m_result;
	PIPE m_pipe;
	PROCESS_INFORMATION m_process_info;
};

//namespace util
}

#endif //_UTILITIES_H_EFE9B03E_CA91_4541_99D8_A2CA1A3F5FDC
