//CompressTab.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.0.9 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0
#define _COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0


#include"TabBase.h"



class CompressTab:public TabBase{
public:
	CompressTab(std::vector<Config*>& config_list):
		TabBase(IDD_TAB_COMPRESS,config_list){}
	~CompressTab(){}
private:
	struct PRIVATE_CONFIG{
		//圧縮形式
		tstring type;
		//パスワード
		bool password;
		//ヘッダ暗号化
		bool header_encryption;
		//自己解凍
		bool sfx;
		PRIVATE_CONFIG():
			type(),password(false),header_encryption(false),sfx(false){}
		enum{
			NO_PASSWORD=1<<0,
			NO_HEADER_ENCRYPTION=1<<1,
			NO_SFX=1<<2
		};
		tstring getType()const{
			return type;
		}
		tstring getOptions(int filter=0)const{
			return tstring(((password&&!header_encryption&&!(filter&NO_PASSWORD))?_T("pw"):_T("")))+
				tstring(((header_encryption&&!(filter&NO_HEADER_ENCRYPTION))?_T("he"):_T("")))+
					tstring(((sfx&&!(filter&NO_SFX))?_T("sfx"):_T("")));
		}
		void setType(const tstring& type_){
			type=type_;
			//パスワード
			tstring::size_type pos=type.find(_T("pw"));

			if(pos!=tstring::npos){
				password=true;
				type.erase(pos,2);
			}else{
				password=false;
			}

			//ヘッダ暗号化
			pos=type.find(_T("he"));

			if(pos!=tstring::npos){
				header_encryption=true;
				type.erase(pos,2);
			}else{
				header_encryption=false;
			}

			//自己解凍
			pos=type.find(_T("sfx"));

			if(pos!=tstring::npos){
				sfx=true;
				type.erase(pos,3);
			}else{
				sfx=false;
			}
		}
	}m_private_config;
private:
	tstring getCompressionType()const{return m_private_config.getType();}
	tstring getCompressionTypeOptions(int filter=0)const{return m_private_config.getOptions(filter);}
	//メッセージハンドラ
	bool onInitDialog(WPARAM wparam,LPARAM lparam);
	bool onCommand(WPARAM wparam,LPARAM lparam);
public:
	void setCurrentSettings();
	tstring getExtension();
};

#endif //_COMPRESSTAB_H_B2E99FCD_CC2A_4fd5_B529_DC132A49A1C0
