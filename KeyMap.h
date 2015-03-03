#ifndef KEYMAP
#define KEYMAP
#include "FileOperation.h"
#include "Includes.h"
#define MAXIMUM_KEY_COUNTED 100//����趨С��Ҳû����,ֻ��Ӱ��Ч��
//#define ASNYC//�����ƵĹ���
//#define DONT_SHOW_INFO//�����Ҫ������Ϣ���Կ���ע�͵�
#ifdef ASNYC
typedef int StateType;
#else
typedef SHORT StateType;
#endif
typedef unsigned char CharacType;
class base 
{
protected:
	StateType state;//true��ʾ����
	StateType state_before;//ǰһ��״̬
	CharacType charactor;//������ַ�
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
class Mouse :public base//ֻ��λ��,��ť״̬�� VK_LBUTTON VK_RBUTTON Ӧ���Ǹ�singleton
{
protected:
	virtual void SetState(StateType &state){};
	virtual void KeyDown(){};
	virtual void KeyUp(){};
	POINT pos,pos_before;
public:
	Mouse() :base(0xFF){ RefreshState(); }//��0xFF�ָ����,��winuser.h����0xFF��reserved,���������ָ���Ĳ���ͻ,�������������.
	virtual void SetState(POINT &in);
	virtual void ShowState();
	virtual bool RefreshState();
};

class KeyRecorder
{
protected:
	vector<base*> Content;//��ȡ�����Ľӿ�
	list<vector<StateType>> statelist;//�洢״̬��λ��
	list<time_t> timeline;//״̬��Ӧ��ʱ��
	bool recordstate;//¼��״̬
	time_t start_time, now_time;
	string filename;

	void RecordPushBack();
	void AddKey(CharacType in);
	void DeleteKey(CharacType in);
	void SaveFileCore(string name);
	bool InitialWithFile(string _file);
	int FindCharac(CharacType &in);
	
	virtual void Initial()
	{}//�����������ļ�ʱ�Ĳ���
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
		AddKey('Z');//�ô�д
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
		AddKey(VK_SPACE);//������һЩ��������
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
	vector<base*> Content;//��ȡ�����Ľӿ�
	list<vector<StateType>> statelist;//�洢״̬��λ��
	list<time_t> timeline;//״̬��Ӧ��ʱ��
	string filename;

	void LoadFileCore(string name);
public:
	void ResetVariables();
	KeyPlayer();//Ĭ���ļ���record.txt
	KeyPlayer(string name);
	~KeyPlayer();
	void LoadFile(string name);
	void LoadFile(char *name);
	void LoadFile();
	void Start();
};

#endif