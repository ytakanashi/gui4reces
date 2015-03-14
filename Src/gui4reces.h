//gui4reces.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//            gui4reces Ver.0.0.1.3 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _GUI4RECES_H_9894B53E_DF70_490a_A360_48270CBF327E
#define _GUI4RECES_H_9894B53E_DF70_490a_A360_48270CBF327E


class Gui4reces:public sslib::DialogApp{
public:
	Gui4reces(){}
	~Gui4reces(){}

public:
	bool init();
	bool run(sslib::CommandArgument& cmd_arg,int cmd_show);
	void cleanup();

};
#endif //_GUI4RECES_H_9894B53E_DF70_490a_A360_48270CBF327E
