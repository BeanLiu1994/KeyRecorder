#include "KeyMap.h"

int main()
{
	char c;
	int set=0;
	cout << "�� c ��ʼ¼�� ������������¼�Ƶ�����"<<endl;
	c = _getch();
	if (c == 'c' || c == 'C')
	{
		KeyRecorderForNearlyAll a;//ʹ��ǰ������
		a.ResetVariables();
		cout << "����s��ʼ¼��,�ٴΰ���s������¼�Ʋ�����.�����а�esc��ֱ�ӱ��沢�˳�" << endl;
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
					 //case 'c': cout << "��������" << endl; break;//�����û��ȥ��
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
		cout << "�����������ʼ����" << endl;
		_getch();
		b.Start();
	}

	return 0;
}