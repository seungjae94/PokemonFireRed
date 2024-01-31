#pragma once

#include "EnginePath.h"


class UEngineFile : public UEnginePath
{
public:
	// constructor destructor
	UEngineFile();
	UEngineFile(std::filesystem::path _Path);
	~UEngineFile();

protected:

private:

};

