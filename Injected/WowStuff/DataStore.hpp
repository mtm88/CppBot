#pragma once

typedef struct
{
	int* vTable;
	int buffer;			//4
	int base;			//8
	int allocated;		//0xC
	int written;		//0x10
	int readed;			//0x14
}CDataStore;

class DataStore
{
public:
	DataStore(int opcode)
	{
		data = ((CDataStore*(__thiscall*)(CDataStore*))0x00401050)(&my_data);	//CDataStore *__thiscall CDataStore::GenPacket(CDataStore *this)
	}

	DataStore(CDataStore* ptr) :data{ ptr }, fake_readed{ ptr->readed } {}

	template<typename T>
	T Read()
	{		
		T result = *(T*)(data->buffer + data->readed);
		data->readed += sizeof(T);
		return result;
	}

	template<typename T>
	T ReadFake()
	{				
		T result = *(T*)(data->buffer + data->readed);
		fake_readed += sizeof(T);
		return result;
	}

	template<typename T>
	void Write(T value)
	{
		*(T*)(data->buffer + data->written) = value;
		data->written += sizeof(T);
		return;
	}

private:
	CDataStore my_data;
	CDataStore* data;
	int fake_readed;
};