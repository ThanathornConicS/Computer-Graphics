#include "pch.h"
#include "Test.h"

Test::Test()
	: Scene("GeneralTesting")
{}
Test::~Test()
{}

void Test::OnAttach()
{
	L_INFO("Attaching {0}...", this->m_debugName);
}
void Test::OnDetach()
{
	L_INFO("Detaching {0}...", this->m_debugName);
}

void Test::OnUpdate(vlr::Time time)
{

}
