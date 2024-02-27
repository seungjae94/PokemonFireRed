#include "EngineSerializer.h"

UEngineSerializer::UEngineSerializer()
{
}

UEngineSerializer::~UEngineSerializer()
{
}

void UEngineSerializer::BufferResize(int _Size)
{
	Data.resize(_Size);
}

void UEngineSerializer::Write(void* _Data, unsigned int _Size)
{
	if (WriteOffset + _Size >= Data.size())
	{
		Data.resize(Data.capacity() * 2 + _Size);
	}

	// this�� ���� �ִ� �������� �ּ� &Data[WriteOffset]���� _Size ����Ʈ��ŭ�� ������
	// �Է����� ���� �ּ� _Data���� _Size ����Ʈ��ŭ�� �����͸� �����Ѵ�.
	// ���� ���ؼ� &Data[WriteOffset] <- _Data (this ������ <- �Է� ������)
	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += _Size;
}


void UEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	// �Է����� ���� �ּ� _Data���� _Size ����Ʈ��ŭ�� ������
	// this�� ���� �ִ� �������� �ּ� &Data[ReadOffset]���� _Size ����Ʈ��ŭ�� �����͸� �����Ѵ�.
	// ���� ���ؼ� _Data <- &Data[WriteOffset] (��� ������ <- this ������)
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += _Size;
}