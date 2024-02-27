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

	// this가 갖고 있는 데이터의 주소 &Data[WriteOffset]부터 _Size 바이트만큼의 공간에
	// 입력으로 들어온 주소 _Data부터 _Size 바이트만큼의 데이터를 복사한다.
	// 쉽게 말해서 &Data[WriteOffset] <- _Data (this 데이터 <- 입력 데이터)
	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += _Size;
}


void UEngineSerializer::Read(void* _Data, unsigned int _Size)
{
	// 입력으로 들어온 주소 _Data부터 _Size 바이트만큼의 공간에
	// this가 갖고 있는 데이터의 주소 &Data[ReadOffset]부터 _Size 바이트만큼의 데이터를 복사한다.
	// 쉽게 말해서 _Data <- &Data[WriteOffset] (출력 데이터 <- this 데이터)
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += _Size;
}