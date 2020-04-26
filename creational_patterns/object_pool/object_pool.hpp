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
	struct object_deleter;
	using obj_type     = Obj;
	using obj_type_ptr = obj_type*;
	using obj_deleter  = object_deleter;
	using obj_unique_ptr = std::unique_ptr<obj_type, obj_deleter>;
	using array_of_obj = std::queue<obj_unique_ptr>;

public:
	object_pool( std::size_t nb_object )
	{
		for (std::size_t i = 0; i < nb_object; ++i) 
		{
			m_obj_array.emplace( new obj_type(), this );
		}
	}

	object_pool() = delete;

	object_pool(const object_pool &lvalue) = delete;
	object_pool& operator=(const object_pool &lvalue) = delete;
	
	object_pool(object_pool &&rvalue) = default;
	object_pool& operator=(object_pool &&rvalue) = default;

	~object_pool()
	{
		m_invalidate = true;
	}

	obj_unique_ptr get_object()
	{
		if ( m_obj_array.empty() )
			throw std::range_error("No more object available");

		auto obj = std::move(m_obj_array.front());
		m_obj_array.pop();

		return obj;
	}

private:
	bool         m_invalidate { false };
	array_of_obj m_obj_array;

	void emplace_object( obj_type *object )
	{
		m_obj_array.emplace(object, this);
	}

	struct object_deleter
	{
		object_deleter ( object_pool *pool ) :
			m_obj_pool { pool }
		{
		}

		void operator()(obj_type_ptr object)
		{
			if ( !m_obj_pool->m_invalidate )
				m_obj_pool->emplace_object(object);
			else
				delete object;
		}

		object_pool *m_obj_pool;
	};

}; // class object_pool

} // namespace::no26

#endif // OBJECT_POOL_HPP
