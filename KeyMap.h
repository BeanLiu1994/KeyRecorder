#ifndef KEYMAP
#define KEYMAP
#include "FileOperation.h"
#include "Includes.h"
#define MAXIMUM_KEY_COUNTED 100//这个设定小了也没问题,只是影响效率
//#define ASNYC//不完善的功能
//#define DONT_SHOW_INFO//如果不要按键信息回显可以注释掉
#ifdef ASNYC
typedef int StateType;
#else
typedef SHORT StateType;
#endif
typedef unsigned char CharacType;
class base 
{
protected:
	StateType state;//true表示按下
	StateType state_before;//前一个状态
	CharacType charactor;//代表的字符
	virtual void SetState(POINT &in)=0;
public:
	base();
	base(CharacType rhs);
	StateType GetState_before()const;
	StateType GetState()const;
	CharacType GetCharactor()const;
	StateType operator*()const;
	virtual void SetState(StateType &state);
	virtual void ShowState() = 0;
	virtual void KeyDown() = 0;
	virtual void KeyUp() = 0;
	virtual bool RefreshState() = 0;
};
class Key :public base
{
protected:
	virtual void SetState(POINT &in){};
public:
	Key() :base(){}
	Key(CharacType rhs) :base(rhs){ RefreshState(); }
	virtual void ShowState();
	virtual void KeyDown();
	virtual void KeyUp();
	virtual bool RefreshState();
};
class Mouse :public base//只是位置,按钮状态用 VK_LBUTTON VK_RBUTTON 应该是个singleton
{
protected:
	virtual void SetState(StateType &state){};
	virtual void KeyDown(){};
	virtual void KeyUp(){};
	POINT pos,pos_before;
public:
	Mouse() :base(0xFF){ RefreshState(); }//将0xFF分给鼠标,查winuser.h发现0xFF是reserved,可以用来分给别的不冲突,将会排在最后面.
	virtual void SetState(POINT &in);
	virtual void ShowState();
	virtual bool RefreshState();
};

class KeyRecorder
{
protected:
	vector<base*> Content;//读取按键的接口
	list<vector<StateType>> statelist;//存储状态的位置
	list<time_t> timeline;//状态对应的时间
	bool recordstate;//录制状态
	time_t start_time, now_time;
	string filename;

	void RecordPushBack();
	void AddKey(CharacType in);
	void DeleteKey(CharacType in);
	void SaveFileCore(string name);
	bool InitialWithFile(string _file);
	int FindCharac(CharacType &in);
	
	virtual void Initial()
	{}//不存在设置文件时的操作
	/*
	char setter = 'A';
	while (setter <= 'Z')
	{
		AddKey(setter++);
	}*/
public:
	KeyRecorder();
	~KeyRecorder();
	base &ReturnKey(CharacType in);
	void ResetVariables(bool _init = true);
	void SetRecordState(bool in);
	bool GetRecordState();
	void ShowState();
	void Refresh();
	void SaveFile(string name);
	void SaveFile(char *name);
	void SaveFile();
};
class KeyRecorderForIwanna :public KeyRecorder
{
private:
	virtual void Initial()
	{
		AddKey(VK_UP);
		AddKey(VK_DOWN);
		AddKey(VK_LEFT);
		AddKey(VK_RIGHT);
		AddKey(VK_SHIFT);
		AddKey('Z');//用大写
		AddKey('R');
		AddKey('Q');
	}
};
class KeyRecorderForNearlyAll :public KeyRecorder
{
private:
	virtual void Initial()
	{
		char setter = 'A';
		while (setter <= 'Z')
		{
			AddKey(setter++);

		}
		setter = '0';
		while (setter <= '9')
		{
			AddKey(setter++);
		}
		AddKey(VK_UP);
		AddKey(VK_DOWN);
		AddKey(VK_LEFT);
		AddKey(VK_RIGHT);
		AddKey(VK_SHIFT);
		AddKey(VK_SHIFT);//ctrl
		AddKey(VK_CONTROL);//ctrl
		AddKey(VK_TAB);//tab
		AddKey(VK_LWIN);//win
		AddKey(VK_MENU);//alt
		AddKey(VK_RETURN);//enter
		AddKey(VK_SPACE);//下面有一些不起作用
		AddKey(VK_OEM_COMMA);
		AddKey(VK_OEM_PERIOD);
		AddKey(VK_BACK);
		AddKey(VK_OEM_1);
		AddKey(VK_OEM_2);
		AddKey(VK_OEM_3);
		AddKey(VK_OEM_4);
		AddKey(VK_OEM_5);
		AddKey(VK_OEM_6);
		AddKey(VK_OEM_7);
		AddKey(VK_OEM_PLUS);
		AddKey(VK_OEM_MINUS);
	}
};

class KeyPlayer
{
private:
#ifdef InitState
	vector<StateType> InitState;
#endif
	vector<base*> Content;//读取按键的接口
	list<vector<StateType>> statelist;//存储状态的位置
	list<time_t> timeline;//状态对应的时间
	string filename;

	void LoadFileCore(string name);
public:
	void ResetVariables();
	KeyPlayer();//默认文件名record.txt
	KeyPlayer(string name);
	~KeyPlayer();
	void LoadFile(string name);
	void LoadFile(char *name);
	void LoadFile();
	void Start();
};

#endif