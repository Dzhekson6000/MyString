#ifndef BASESTRINGBUILDER_H
#define BASESTRINGBUILDER_H

#include <list>

namespace doxyCraft
{
	template<class T>
	class BaseStringBuilder final
	{
	public:
		BaseStringBuilder() noexcept = default;
		~BaseStringBuilder() noexcept = default;
		
		template<class U>
		BaseStringBuilder(U &&str)
		{
			data.emplace_back(std::forward<T>(str));
		}
		
		template<class U>
		BaseStringBuilder &add(U &&str)
		{
			data.emplace_back(std::forward<T>(str));
			return *this;
		}
		
		template<class U>
		inline BaseStringBuilder &operator%(U &&str)
		{
			return add(str);
		}
		
		template<class U>
		inline BaseStringBuilder &operator<<(U &&str)
		{
			return add(str);
		}
		
		T build() const
		{
			T         rtrn;
			for( auto i : data )
			{
				rtrn += i;
			}
			return rtrn;
		}
		
		inline operator T() const
		{
			return build();
		}
	
	private:
		std::list<T> data;
	};
}


#endif //BASESTRINGBUILDER_H
