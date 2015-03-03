#include "Includes.h"
#include "FileOperation.h"


	FileOperation::FileOperation(string _filename, Type _type) :filename(_filename), type(_type)
	{
		if (init(_filename, _type)){ /*cout << "δ�ܳɹ���" << endl;*/ }
	}
	FileOperation::~FileOperation(){ file.close(); }
	int FileOperation::init(){
		return init(filename, type);
	}
	bool FileOperation::is_open()
	{
		return file.is_open();
	}
	bool FileOperation::eof()
	{
		return file.eof();
	}
	FileOperation::FileOperation(){}
	FileOperation::FileOperation(const FileOperation& rhs){}
	int FileOperation::init(string _filename, Type _type)
	{
		if (!file.is_open())
		{
			if (_type == Read){
				file.open(_filename, ios_base::in);
			}
			else if (_type == Write)
			{
				file.open(_filename, ios_base::out);
			}
			if (!file.is_open())
			{
				return 1;//δ�ܴ�
			}
			return 0;//�ɹ���
		}
		return -1;//�Ѿ���
	}
