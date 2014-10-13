//TabBase.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.1 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _TABBASE_H_E72BB24F_5775_40f1_93EC_3344FB0DE0FE
#define _TABBASE_H_E72BB24F_5775_40f1_93EC_3344FB0DE0FE


#include"PrivateProfile.h"
#include"resources/resource.h"



class TabBase:public sslib::Dialog{
public:
	TabBase(UINT resource_id,std::vector<Config*>& config_list):
		Dialog(resource_id),
		m_config_list(config_list){}
	~TabBase(){}
protected:
	std::vector<Config*>& m_config_list;
public:
	virtual void setCurrentSettings()=0;
};

#endif //_TABBASE_H_E72BB24F_5775_40f1_93EC_3344FB0DE0FE
