#include "KeyMap.h"

base::base() :state(false), state_before(false), charactor(0){}
base::base(CharacType rhs) : state(false), state_before(false), charactor(rhs){ }
StateType base::GetState_before()const
{
	return state_before;
}
StateType base::GetState()const
{
	return state; 
}
CharacType base::GetCharactor()const
{
	return charactor; 
}
StateType base::operator*()const
{
	return charactor;
}
void base::SetState(StateType &_state)
{
	if ((state >= 0 && _state<0) || (_state >= 0 && state<0))//只有在当前键位的状态改变时才刷新
	{
		state_before = state;
		state = _state;
		if (_state < 0)
			KeyDown();
		else
			KeyUp();
	}
}
void Key::ShowState()
{
	cout << state << "  ";
}
void Key::KeyDown()
{
	switch (charactor)
	{
	case VK_LBUTTON:
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//按下 state=-127 -128
		break;
	case VK_RBUTTON:
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);//按下 state=-127 -128
		break;
	default:
		keybd_event(charactor, 0, 0, 0); //按下 state=-127 -128
		break;
	}
}
void Key::KeyUp()
{
	switch (charactor)
	{
	case VK_LBUTTON:
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//放开 state=0 1
		break;
	case VK_RBUTTON:
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);//放开 state=0 1
		break;
	default:
		keybd_event(charactor, 0, KEYEVENTF_KEYUP, 0);//放开 state=0 1
		break;
	}
}
bool Key::RefreshState()
{
	state_before = state;
#ifdef ASNYC
	state = GetAsyncKeyState(charactor);
#else
	state = GetKeyState(charactor);
#endif
	return (state != state_before);
}//返回是否发生改变
void Mouse::ShowState()
{
	cout << "(" << pos.x << pos.y << ")  ";
}
void Mouse::SetState(POINT &in)
{
	if (in.x != pos.x || in.y != pos.y)
	{
		pos_before = pos;
		pos = in;
		SetCursorPos(in.x, in.y);
	}
}
bool Mouse::RefreshState()
{
	pos_before = pos;
	GetCursorPos(&pos);
	return (pos_before.x != pos.x || pos_before.y != pos.y);
}//返回是否发生改变

int KeyRecorder::FindCharac(CharacType &in)
{
	int length = Content.size(), i = 0;
	for (; i < length; i++)
	{
		if (in == Content[i]->GetCharactor())return i;//不能有多个相同的字符监听
	}
	return -1;
}
base &KeyRecorder::ReturnKey(CharacType in)
{
	int i = FindCharac(in);
	if (i == -1){ cout << "不存在这个字符的监听 " << in << endl; Sleep(2); exit(1); }
	return *(Content[i]);
}
bool KeyRecorder::InitialWithFile(string _file)
{
	FileOperation filein(_file,FileOperation::Read);
	if (!filein.is_open())
	{
		cout << "未能成功打开文件 ("<<_file<<")" << endl;
		return 1;
	}
	int i;
	short vk;//virtual_key转化成字符
	for (i = 0; !filein.eof(); i++)
	{
		vk = filein.ReadFile<short>();
		AddKey(static_cast<CharacType>(vk));
	}
	if (i > MAXIMUM_KEY_COUNTED){ cout << "设定的值可能大于预留空间,请重新编译以保证效率" << endl; }
	return 0;
}
void KeyRecorder::ResetVariables(bool _init){
	Content.clear();
	Content.reserve(MAXIMUM_KEY_COUNTED);
	statelist.clear();
	timeline.clear(); 

	if (_init)
	{
		if (InitialWithFile("setting.txt"))
		{
			Initial();
		}
	}
}//With Initialization
KeyRecorder::KeyRecorder() :recordstate(false), filename("Record.txt")
{
	ResetVariables(false);
	start_time = clock();
}
KeyRecorder::~KeyRecorder()
{
	int length = Content.size(), i = 0;
	for (; i < length; i++)
	{
		delete Content[i];
	}
}
void KeyRecorder::SetRecordState(bool in)
{
	recordstate = in;
	if (in == true)
	{
		ResetVariables();
		cout << "开始录制" << endl;
	}
	else
	{
		SaveFile();
	}
	start_time = clock();
}
bool KeyRecorder::GetRecordState()
{
	return recordstate;
}
void KeyRecorder::RecordPushBack()
{
	if (recordstate == true){
		int length = Content.size(), i = 0;
		vector<StateType>statetemp;
		statetemp.clear();
		statetemp.reserve(length);
		timeline.push_back(now_time - start_time);
		for (; i < length; i++)
		{
#ifdef ASNYC
			statetemp.push_back(Content[i]->GetState());
#else
			statetemp.push_back(Content[i]->GetState());
#endif
		}
		statelist.push_back(statetemp);
	}
}
void KeyRecorder::Refresh()
{
	int length = Content.size(), i = 0;
	bool push_state = false;
	for (; i < length; i++)
	{
		if (Content[i]->RefreshState())
		{
			push_state = true;
		}
	}
	if (push_state)
	{
		now_time = clock();
#ifndef DONT_SHOW_INFO
		ShowState();
#endif
		RecordPushBack();
	}
}
void KeyRecorder::ShowState()
{
	int length = Content.size(), i = 0;
	cout << now_time - start_time << "  ";
	for (; i < length; i++)
	{
		Content[i]->ShowState();
	}
	cout << endl;
}
void KeyRecorder::AddKey(CharacType in)
{
	int length = Content.size(), i = 0;
	for (; i < length; i++)
	{
		if (in < Content[i]->GetCharactor())break;//符号改变
		if (in == Content[i]->GetCharactor())return;//不能有多个相同的字符监听
	}
	Content.insert(Content.begin() + i, new Key(in));
}
void KeyRecorder::DeleteKey(CharacType in)
{
	int length = Content.size(), i = 0, flag = 0;
	for (; i < length; i++)
	{
		if (in == Content[i]->GetCharactor())
		{
			flag = 1;
			delete Content[i];
			Content.erase(Content.begin() + i);
		}
	}
	if (flag == 0){ cout << "没有找到指定的监听器!" << endl; }
}
void KeyRecorder::SaveFileCore(string name)
{

	FileOperation outfile(name, FileOperation::Write);
	int length = Content.size();
	outfile.WriteFile(length);
	outfile.WriteFile("  ");
	for (int i = 0; i < length; i++)
	{
		outfile.WriteFile(static_cast<short>(Content[i]->GetCharactor()));
		if (i == length - 1)continue;
		outfile.WriteFile("  ");
	}

	int length_state = statelist.size();
	list<time_t>::iterator timeline_iter = timeline.begin(); 
	list<vector<StateType>>::iterator statelist_iter = statelist.begin();
	for (int i = 0; i < length_state; i++)
	{
		outfile.WriteFile("\n");
		outfile.WriteFile(*timeline_iter);
		outfile.WriteFile("  ");
		timeline_iter++;
		for (int j = 0; j < length; j++)
		{
			outfile.WriteFile((*statelist_iter)[j]);
			if (j == length - 1)continue;
			outfile.WriteFile("  ");
		}
		statelist_iter++;
	}
#if 0
	ofstream outfile;
	outfile.open(name);
	int length = Content.size();
	outfile << length << "  ";
	for (int i = 0; i < length; i++)
	{
		outfile << (short)Content[i].GetCharactor() << "  ";
	}
	outfile << endl;

	int length_state = statelist.size();
	list<time_t>::iterator timeline_iter = timeline.begin();
	list<vector<StateType>>::iterator statelist_iter = statelist.begin();
	for (int i = 0; i < length_state; i++)
	{
		outfile << *timeline_iter << "  ";
		timeline_iter++;
		for (int j = 0; j < length; j++)
		{
			outfile << (*statelist_iter)[j] << "  ";
		}
		statelist_iter++;
		outfile << endl;
	}
	outfile.flush();
	outfile.close();
#endif
}
void KeyRecorder::SaveFile()
{
	SaveFileCore(filename);
}
void KeyRecorder::SaveFile(char *name)
{
	SaveFileCore(string(name));
}
void KeyRecorder::SaveFile(string name)
{
	SaveFileCore(name);
}

void KeyPlayer::ResetVariables()
{
	Content.clear();
	Content.reserve(MAXIMUM_KEY_COUNTED);
	LoadFileCore(filename);
}
KeyPlayer::KeyPlayer() :filename("record.txt")
{
	ResetVariables();
}
KeyPlayer::KeyPlayer(string name) :filename(name)
{
	ResetVariables();
}
KeyPlayer::~KeyPlayer(){
	int length = Content.size(), i = 0;
	for (; i < length; i++)
	{
		delete Content[i];
	}
}
void KeyPlayer::LoadFile(string name){ LoadFileCore(name); }
void KeyPlayer::LoadFile(char *name){ LoadFileCore(string(name)); }
void KeyPlayer::LoadFile(){ LoadFileCore(filename); }
void KeyPlayer::LoadFileCore(string name)
{
	FileOperation filein(name, FileOperation::Read);
	if (!filein.is_open())
	{
		cout << "没有这个文件.  (" << name << ")" << endl;
		Sleep(1);
		exit(1);
	}
	int length;
	short vk;//virtual_key
	length=filein.ReadFile<int>();
	for (int i = 0; i < length && !filein.eof(); i++)
	{
		vk = filein.ReadFile<short>();
		Content.push_back(new Key(BYTE(vk)));
	}
#ifdef InitState
	for (int i = 0; i < length; i++){ InitState.push_back(Content[i].GetState()); }
#endif
	time_t temp_1;
	StateType temp_2;
	vector<StateType> temp_3;
	while (!filein.eof())
	{
		temp_3.clear();
		temp_3.reserve(length);
		temp_1 = filein.ReadFile<time_t>();
		timeline.push_back(temp_1);
		for (int i = 0; i < length; i++)
		{
			temp_2 = filein.ReadFile<StateType>();
			temp_3.push_back(temp_2);
		}
		statelist.push_back(temp_3);
	}
	cout << "文件加载完毕" << endl;
#if 0
	ifstream filein;
	filein.open(name);
	if (!filein.is_open())
	{
		cout << "没有这个文件.  (" << name << ")" << endl;
		Sleep(1);
		exit(1);
	}
	int length;
	short vk;//virtual_key
	filein >> length;
	for (int i = 0; i < length && !filein.eof(); i++)
	{
		filein >> vk;
		Content.push_back(Key(BYTE(vk)));
	}
#ifdef InitState
	for (int i = 0; i < length; i++){ InitState.push_back(Content[i].GetState()); }
#endif
	time_t temp_1;
	StateType temp_2;
	vector<StateType> temp_3;
	while (!filein.eof())
	{
		temp_3.clear();
		temp_3.reserve(length);
		filein >> temp_1;
		timeline.push_back(temp_1);
		for (int i = 0; i < length; i++)
		{
			filein >> temp_2;
			temp_3.push_back(temp_2);
		}
		statelist.push_back(temp_3);
	}
	cout << "文件加载完毕" << endl;
	filein.close();
#endif
}
void KeyPlayer::Start()
{
	cout << "开始播放" << endl;
	int length = Content.size(), length_state = timeline.size();
	StateType state;
	list<time_t>::iterator timeline_iter = timeline.begin();
	list<vector<StateType>>::iterator statelist_iter = statelist.begin();
	time_t start_time = clock(), now_time, next_time;
	for (int i = 0; i < length_state; i++)
	{
		next_time = *timeline_iter; timeline_iter++;
		cout << next_time << "  ";
		do
		{
			now_time = clock();
		} while (now_time - start_time < next_time);
		for (int j = 0; j < length; j++)
		{
			state = (*statelist_iter)[j];
#ifndef DONT_SHOW_INFO
			cout << state << "  ";
#endif
			Content[j]->SetState(state);
		}
		statelist_iter++;
#ifndef DONT_SHOW_INFO
		cout << endl;
#endif
	}
}