//gui4reces.cpp
//gui4recesメイン

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.2 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"gui4reces.h"
#include"MainDialog.h"
#include"resources/resource.h"


using namespace sslib;


void createInstance(){
	new Gui4reces;
}

bool Gui4reces::init(){
	return true;
}

void Gui4reces::cleanup(){
}

bool Gui4reces::run(CommandArgument& cmd_arg,int cmd_show){
	//Ctrl+Aをローカルフックして、エディットコントロールの文字列を全選択できるようにする
	CtrlAHook ctrlAHook;

	MainDialog dlg;
	dlg.setParam(&cmd_arg);
	dlg.doModal();

	return true;
}
