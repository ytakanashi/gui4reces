//PrivateProfile.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.8 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _PRIVATEPROFILE_H_AF26760D_77B2_4C9D_9B67_4F0897D4AAF0
#define _PRIVATEPROFILE_H_AF26760D_77B2_4C9D_9B67_4F0897D4AAF0


enum MODE{
	MODE_RECOMPRESS=1<<0,
	MODE_COMPRESS=1<<1,
	MODE_EXTRACT=1<<2,
	MODE_LIST=1<<3,
	MODE_TEST=1<<4,
	MODE_SENDCOMMANDS=1<<5,
	MODE_VERSION=1<<6,
	MODE_SETTINGS=1<<7,
	MODE_DELETE=1<<8,
	MODE_RENAME=1<<9,
};

enum{
	default_compressionlevel=-1,
	minimum_compressionlevel=-2,
	maximum_compressionlevel=-3
};

namespace fileinfo{
struct FILEINFO{
	tstring name;
	long long size;
	DWORD attr;
	long long date_time;

	FILEINFO():name(),size(0),attr(0),date_time(-1){}
	FILEINFO(tstring& name_):name(name_),size(0),attr(0),date_time(0){}
	FILEINFO(tstring& name_,long long size_,DWORD attr_,long long date_time_):name(name_),size(size_),attr(attr_),date_time(date_time_){}
	bool operator()(const FILEINFO& fileinfo)const{
		return name==fileinfo.name;
	}
	bool operator<(const FILEINFO& fileinfo)const{
		return name<fileinfo.name;
	}
	//ディレクトリ若しくはサブ以下のファイル
	bool isDirectory(bool delimiter_check=true)const{
		return ((delimiter_check)?
				 (name.find_last_of(_T("\\/"))==name.length()-1):
				false)||
				 attr&FILE_ATTRIBUTE_DIRECTORY;
	}
};

struct FILEFILTER{
	long long min_size;
	long long max_size;
	DWORD attr;
	//除外フィルタ時のみ操作
	bool include_empty_dir;
	long long oldest_date;
	long long newest_date;
	std::list<tstring> pattern_list;
	bool recursive;
	bool regex;

	//注意:recursiveはfalseで初期化
	FILEFILTER():min_size(0),max_size(0),attr(0),include_empty_dir(true),oldest_date(-1),newest_date(-1),pattern_list(),recursive(false),regex(false){}
	bool empty()const{return min_size==0&&
							 max_size==0&&
							 attr==0&&
							 include_empty_dir&&
							 oldest_date==-1&&
							 newest_date==-1&&
							 pattern_list.empty();}
};


//namespace fileinfo
}

struct NODISPLAY{
	//進捗状況を表示しない
	bool no_information;
	//ログを表示しない
	bool no_log;
	//パスワードを表示しない
	bool no_password;
	//エラーメッセージを表示しない
	bool no_errmsg;
	NODISPLAY():no_information(false),no_log(false),no_password(false),no_errmsg(false){}
};

struct REMOVEREDUNDANTDIR{
	//二重ディレクトリを防ぐ
	bool double_dir;
	//同名の二重ディレクトリを防ぐ
	bool same_dir;
	//ファイル単体の場合作成しない
	bool only_file;
	REMOVEREDUNDANTDIR():double_dir(false),only_file(false),same_dir(false){}
};

struct OMITNUMBERANDSYMBOL{
	//ディレクトリ名末尾の数字を削除
	bool number;
	//ディレクトリ名末尾の記号を削除
	bool symbol;
	OMITNUMBERANDSYMBOL():number(false),symbol(false){}
};

struct CREATEDIROPTIMIZATION{
	//更新日時を書庫と同じにする
	bool copy_timestamp;
	//不要なディレクトリを作成しない
	REMOVEREDUNDANTDIR remove_redundant_dir;
	//出力ディレクトリ末尾の数字や記号を削除
	OMITNUMBERANDSYMBOL omit_number_and_symbol;
	CREATEDIROPTIMIZATION():copy_timestamp(false),remove_redundant_dir(),omit_number_and_symbol(){}
};

enum RMSRC{
	RMSRC_DISABLE=1<<0,
	RMSRC_RECYCLEBIN=1<<1,
	RMSRC_REMOVE=1<<2
};

struct RUNCOMMAND{
	//対話形式で指定
	bool interactive;
	//コマンド
	tstring command;
	RUNCOMMAND():interactive(false),command(){}
	bool disable()const{return !interactive&&command.empty();}
};

struct GENERAL{
	//テスト
	bool test;
	//バックグラウンドで動作
	bool background_mode;
	//処理終了後ウインドウを閉じる
	bool quit;
	//エラーが発生したら中断する
	bool pause_error;
	//ディレクトリ階層を無視して圧縮/解凍
	bool ignore_directory_structures;
	//指定したライブラリ名
	tstring selected_library_name;
	//指定したライブラリのプレフィックス(msのみ)
	tstring selected_library_prefix;
	//処理対象フィルタ
	fileinfo::FILEFILTER filefilter;
	//処理対象除外フィルタ
	fileinfo::FILEFILTER file_ex_filter;
	//パスワード
	tstring password;
	std::list<tstring> password_list;
	//出力先ディレクトリ
	tstring output_dir;
	//カレントディレクトリ基準で'/od','/of'の相対パスを処理
	bool default_base_dir;
	//出力ファイルが重複する場合リネーム
	bool auto_rename;
	//ソースを削除
	RMSRC remove_source;
	//アーカイバに送る文字コード(現在7-zip32.dllのみ対象)
	unsigned int arc_codepage;
	//パスワードリストファイル、リストファイルの文字コード
	sslib::File::CODEPAGE list_codepage;
	//文字をANSI(sjis)で出力
	bool ansi_stdout;
	//ユーザ独自のパラメータ
	tstring custom_param;
	//spiがあるディレクトリ
	tstring spi_dir;
	//b2eがあるディレクトリ
	tstring b2e_dir;
	//wcxがあるディレクトリ
	tstring wcx_dir;

	//実行時に出力先ディレクトリ選択
	//gui4reces専用項目
	bool choose_output_dir_each_time;

	//初期化
	GENERAL():
		background_mode(false),
		quit(true),
		ignore_directory_structures(false),
		selected_library_name(),
		selected_library_prefix(),
		filefilter(),
		file_ex_filter(),
		password(),
		password_list(),
		output_dir(),
		default_base_dir(false),
		auto_rename(false),
		remove_source(RMSRC_DISABLE),
		arc_codepage(0),
		list_codepage(sslib::File::SJIS),
		ansi_stdout(false),
		custom_param(),
		spi_dir(),
		b2e_dir(),
		wcx_dir(),
		choose_output_dir_each_time(false){}
};

struct RECOMPRESS{
	//新しいパスワード
	tstring new_password;
	//コマンド実行
	RUNCOMMAND run_command;
	RECOMPRESS():new_password(),run_command(){}
};

struct B2E{
	//圧縮形式
	tstring format;
	//圧縮メソッド
	tstring method;
	//自己解凍形式指定
	bool sfx;
	B2E():format(),method(),sfx(false){}
};

struct COMPRESS{
	//圧縮形式
	tstring compression_type;
	//個別圧縮
	bool each_file;
	//書庫新規作成
	bool create_new;
	//書庫強制新規作成
	//上書き先を予め削除して作成
	bool force_create_new;
	//基底ディレクトリを含まない
	//解凍の場合-1で共通パスをすべて除く
	int exclude_base_dir;
	//圧縮率
	int compression_level;
	//書庫分割数値
	tstring split_value;
	//出力ファイル名
	tstring output_file;
	//'/of'以下の引数をそのまま使用(拡張子を付加しない)
	bool raw_file_name;
	//更新日時を元書庫と同じにする
	bool copy_timestamp;
	//対象ディレクトリを再帰的検索
	bool recursive;
	//圧縮用b2eスクリプト
	B2E b2e;
	//元ファイルの拡張子を保持
	bool keep_extension;

	//実行時に出力ファイル名選択
	//gui4reces専用項目
	bool choose_output_file_each_time;

	COMPRESS():
		compression_type(_T("zip")),
		each_file(false),
		create_new(false),
		force_create_new(false),
		exclude_base_dir(0),
		compression_level(-1),
		split_value(),
		output_file(),
//		raw_file_name(false),
		//gui4recesでは必ず有効に
		raw_file_name(true),
		copy_timestamp(false),
		recursive(false),
		b2e(),
		keep_extension(false),

		choose_output_file_each_time(false){}
};

struct EXTRACT{
	//ファイル名が始まるIndex
	int index;
	//ディレクトリを作成する
	bool create_dir;
	//パスワードリストファイル
	tstring password_list_path;
	//'/c'最適化
	CREATEDIROPTIMIZATION create_dir_optimization;
	//ディレクトリのタイムスタンプを復元する
	bool directory_timestamp;
	EXTRACT():index(0),
		create_dir(false),
		password_list_path(),
		create_dir_optimization(),
		directory_timestamp(false){}
};

struct OUTPUTFILELIST{
	//FindFirst()/FindNext()/GetFileName()で出力する
	bool api_mode;

	OUTPUTFILELIST():api_mode(true){}
};

struct RENAME{
	//置換パターン
	typedef std::pair<tstring,tstring> pattern;
	std::list<pattern> pattern_list;
	//正規表現
	bool regex;

	RENAME():pattern_list(),regex(false){}
};

//gui4reces専用項目
struct GUI4RECES{
	//最前面表示
	bool top_most;
	//デフォルトプロファイル名
	tstring default_profile;
	//処理後gui4recesを終了する
	bool quit;
	//作業ディレクトリ
	tstring work_dir;
	//すぐに開始
	bool at_once;
	//ログウインドウを表示
	bool log;
	GUI4RECES():top_most(false),
		default_profile(),
		quit(false),
		work_dir(),
		at_once(false),
		log(false)
		{sslib::env::get(_T("TMP"),&work_dir);}
};

struct CONFIG{
	MODE mode;
	GENERAL general;
	//非表示
	NODISPLAY no_display;
	//再圧縮
	RECOMPRESS recompress;
	//圧縮
	COMPRESS compress;
	//解凍
	EXTRACT extract;
	//一覧出力
	OUTPUTFILELIST output_file_list;
	//リネーム
	RENAME rename;

	//gui4reces専用項目
	GUI4RECES gui4reces;

	CONFIG():
		mode(MODE_RECOMPRESS),
		general(),
		no_display(),
		recompress(),
		compress(),
		extract(),
		output_file_list(),
		rename(),
		gui4reces(){}
};

class Config:public sslib::CfgFile{
private:
	CONFIG m_cfg;
	static const CONFIG m_default_cfg;

private:
	template<typename T>
	void write(const TCHAR* section,const TCHAR* key,const T value,bool diff){
		if(diff){
			setData(section,key,value);
		}else{
			removeKey(section,key);
		}

		//キーが一つもなければセクションを削除
		if(isEmptySection(section)){
			removeSection(section);
		}
	}
	void write(const TCHAR* section,const TCHAR* key,const tstring value,bool diff){
		return write(section,key,value.c_str(),diff);
	}
	void write(const TCHAR* section,const TCHAR* key,const long long value,bool diff){
		return write(section,key,sslib::format(_T("%I64d"),value).c_str(),diff);
	}

public:
	Config():m_cfg(){}
	Config(const TCHAR* cfg_path):sslib::CfgFile(cfg_path),m_cfg(){}
	~Config(){}
	//cfgファイルへ書き込む
	bool save(bool include_gui4reces_section=false);
	//cfgファイルから読み込む
	bool load(bool include_gui4reces_section=false);
	//別のConfigから設定を取り込む
	void import(const Config& src,bool include_gui4reces_section=false);

	inline CONFIG& cfg(){return m_cfg;}
	inline const CONFIG& default_cfg()const{return m_default_cfg;}
};

#endif //_PRIVATEPROFILE_H_AF26760D_77B2_4C9D_9B67_4F0897D4AAF0
