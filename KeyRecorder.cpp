#include "KeyMap.h"

int main()
{
	char c;
	int set=0;
	cout << "按 c 开始录制 按其他键播放录制的内容"<<endl;
	c = _getch();
	if (c == 'c' || c == 'C')
	{
		KeyRecorderForNearlyAll a;//使用前请重置
		a.ResetVariables();
		cout << "按下s开始录制,再次按下s将结束录制并保存.过程中按esc会直接保存并退出" << endl;
		for (;;)
		{
			while (!_kbhit()){
				a.Refresh();
				if (a.ReturnKey('R').GetState() >= 0 && set == 0)
				{
					a.SetRecordState(true);
					cout << "here to start" << endl;
					set = 1;
				}
				else if (a.ReturnKey('R').GetState() < 0 && set == 1)
				{
					set = 0;
					cout << "here to stop and save" << endl;
					a.SetRecordState(false);
				}
			}
			char c = _getch();
			switch (c)
			{
			case 's':
				if (a.GetRecordState() == false){ a.SetRecordState(true); }
					 else{ a.SetRecordState(false); } 
					 break;
					 //case 'c': cout << "激活连发" << endl; break;//这个还没有去做
			case 27:
				if (a.GetRecordState() == true){ a.SaveFile(); }
				return 0;
				break;
			default:break;
			}
		}
	}
	else
	{
		KeyPlayer b;
		cout << "按下任意键开始播放" << endl;
		_getch();
		b.Start();
	}

	return 0;
}