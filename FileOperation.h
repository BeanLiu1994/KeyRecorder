#ifndef FILEOPERATION
#define FILEOPERATION
#include "Includes.h"

class FileOperation
{
public:
	enum Type{ Read, Write };
	FileOperation(string _filename, Type _type);
	~FileOperation();
	int init();
	bool is_open();
	bool eof(); 
	template<typename T>
	T ReadFile()
	{
			return ReadNormal<T>();
		}
	template<typename T>
	int WriteFile(T t)
	{
		WriteNormal(t);
		return 0;
	}

private:
	fstream file;
	string filename;
	Type type;
	FileOperation();
	FileOperation(const FileOperation& rhs);
	int init(string _filename, Type _type);
	template<typename T>
	T ReadNormal()
	{
		T t;
		file >> t;
		return t;
	}
	template<typename T>
	int WriteNormal(T t)
	{
		file << t;
		file.flush();
		return 0;
	}
	template<typename T>
	int ReadBit(T &buf)//������
	{
		if (type == Read){
			file.read((char*)(&buf), sizeof(buf));
			return 0;
		}
		else
		{
			return 1; //���Ͳ�����
		}
	}
	template<typename T>
	int WriteBit(T &buf)//������
	{
		if (type == Write){
			file.write((char*)(&buf), sizeof(buf));
			file.flush();
			return 0;
		}
		else
		{
			return 1; //���Ͳ�����
		}
	}
};

#endif