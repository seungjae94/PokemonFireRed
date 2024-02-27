#pragma once
#include <vector>
#include <string>
#include <fstream>

class UCsvReader
{
public:
	// constructor destructor
	UCsvReader(std::string_view _FullPath);
	~UCsvReader();

	// delete Function
	UCsvReader(const UCsvReader& _Other) = delete;
	UCsvReader(UCsvReader&& _Other) noexcept = delete;
	UCsvReader& operator=(const UCsvReader& _Other) = delete;
	UCsvReader& operator=(UCsvReader&& _Other) noexcept = delete;

	std::vector<std::string> ReadLine();
	std::vector<std::vector<std::string>> ReadLines();

protected:

private:
	std::ifstream FileStream;
};

