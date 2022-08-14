#include "pch.h"
#include "Test.h"

#include <thread>

int arr[20] = {};
std::vector<int> vec;

void foo(int startIdx, int amout, int value) 
{
	for (size_t i = startIdx; i < amout; i++) 
	{
		//arr[i] = value;
		vec.emplace_back(value);
		L_INFO("Foo: {0}, {1}", i, value);
	}
}
void bar(int startIdx, int amout, int value)
{
	for (size_t i = startIdx; i < amout; i++)
	{
		//arr[i] = value;
		vec.emplace_back(value);
		L_INFO("Bar: {0}, {1}", i, value);
	}
}

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	std::thread fooThread(foo, 0, 10, 1);
	std::thread barThread(bar, 10, 20, 0);

	fooThread.join();
	barThread.join();

	for (size_t i = 0; i < 20; i++) 
	{
		L_TRACE("Arr[{0}] = {1}", i, vec[i]);
	}
}
void Test::OnDetach()
{}

void Test::OnUpdate(vlr::Time time)
{}
