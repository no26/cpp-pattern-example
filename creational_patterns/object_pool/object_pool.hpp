#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <cstddef> // std::size_t

#include <memory>
#include <queue>
#include <stdexcept>
#include <functional>

namespace no26
{

template<typename Obj>
class object_pool
{
private:
	struct obj_deleter;

	using object_type         = Obj;
	using object_type_ptr     = object_type*;
	using object_deleter      = obj_deleter;
	using object_type_wrapper = std::unique_ptr<object_type, object_deleter>;
	using object_queue        = std::queue<object_type_wrapper>;

public:

	object_pool( std::size_t nb_object ) :
		m_allocated_object { new object_type[nb_object] }
	{
		auto *raw_addr = m_allocated_object.get();
		for( std::size_t i { 0 }; i < nb_object; ++i )
			this->emplace( &raw_addr[i] );
	}

	object_pool() = delete;

	object_pool(const object_pool &lvalue) = delete;
	object_pool& operator=(const object_pool &lvalue) = delete;

	object_pool(object_pool &&rvalue) = default;
	object_pool& operator=(object_pool &&rvalue) = default;

	~object_pool() { m_is_valid = false; }

	object_type_wrapper get_object() 
	{
		auto object = std::move( m_object_queue.front() );
		m_object_queue.pop();

		return object;
	}

private:
	std::unique_ptr<object_type[]> m_allocated_object;
	object_queue                   m_object_queue;
	bool                           m_is_valid { true };

	void emplace ( object_type_ptr object )
	{
		if ( m_is_valid )
			m_object_queue.emplace( object, *this );
	}

	struct obj_deleter
	{
		object_pool &m_object_pool;

		obj_deleter(object_pool &pool) : 
			m_object_pool { pool } 
		{
		}

		void operator()(object_type_ptr object)
		{
			m_object_pool.emplace(object);
		}
	};
};

} // namespace::no26

#endif // OBJECT_POOL_HPP
