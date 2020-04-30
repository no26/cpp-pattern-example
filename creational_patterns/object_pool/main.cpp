#include <iostream>

#include "object_pool.hpp"

struct Foo
{
	std::string m_info { "Foo" };

	Foo() { std::cout << "default ctor" << std::endl; }

	Foo(const Foo &lvalue) { std::cout << "copy ctor" << std::endl; }
	Foo& operator=(const Foo &lvalue) { std::cout << "copy operand" << std::endl; return *this; }

	Foo(Foo &&rvalue) { std::cout << "move ctor" << std::endl; }
	Foo& operator=(Foo &&rvalue) { std::cout << "move operand" << std::endl; return *this; }

	~Foo() { std::cout << "default dtor" << std::endl; }

	std::string get_info() const
	{
		return m_info;
	}
};


int main()
{
	no26::object_pool<Foo> pool { 3 };

	{
		auto object = pool.get_object();
		object->m_info = "1";
		std::cout << "Hello " << object->get_info() << std::endl;
	}

	{
		auto object = pool.get_object();
		object->m_info = "2";
		std::cout << "Hello " << object->get_info() << std::endl;
	}

	{
		auto object = pool.get_object();
		object->m_info = "3";
		std::cout << "Hello " << object->get_info() << std::endl;
	}

	{
		auto object = pool.get_object();
		std::cout << "Hello " << object->get_info() << std::endl;
	}

	return 0;
}
