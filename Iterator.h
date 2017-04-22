#ifndef ITERATOR_H_
#define ITERATOR_H_
#define MaxUTF8CharLen 4

#include "../../../Shared/Macros.h"
#include <stddef.h>
#include <iterator>
#include <type_traits>

namespace doxyCraft
{
	
	template<class T1, class T2>
	struct check_types_helper
	{
		template<typename T>
		struct _is_array_helper
		{
		private:
			template<typename C> static std::true_type test(C[]);
			template<typename C> static std::false_type test(...);
		public:
			typedef decltype(test<T>(0)) type;
		};
		
		template<typename T>
		struct _is_contaier_helper
		{
		private:
			template<typename C> static std::true_type test(typename std::iterator_traits<typename C::const_iterator>::pointer);
			template<typename C> static std::false_type test(...);
		public:
			typedef decltype(test<T>(0)) type;
		};
		
		template<class T>
		struct remove_array_type
		{
			typedef T type;
		};
		
		template<class T>
		struct remove_array_type<T[]>
		{
			typedef const T* type;
		};
		
		template<typename T> struct _is_array:public _is_array_helper<T>::type {};
		template<typename T> struct _is_container:public _is_contaier_helper<T>::type {};
		
		template<typename C> static std::true_type test(typename remove_array_type<C>::type);
		template<typename C> static std::true_type test(typename std::iterator_traits<typename C::const_iterator>::pointer);
		template<typename C> static std::false_type test(...);
		
		static const char* tp_;
		typedef typename std::__and_<
				std::__or_<_is_array<T1>, _is_container<T1>>,
				std::__or_<_is_array<T2>, _is_container<T2>>,
				decltype(test<T1>(tp_)),
				decltype(test<T2>(tp_))
		>::type type;
	};
	
	template<typename T1, typename T2> const char* check_types_helper<T1, T2>::tp_ = "char";
	
	template<typename _InIter1, typename _InIter2>
	using RequiredCharTypes = typename
	std::enable_if<
			check_types_helper<_InIter1, _InIter2>::type::value
	>::type;
	
	template<typename _InIter>
	using RequireIterator = typename
	std::enable_if<
			std::is_convertible
					<
							typename
							std::iterator_traits<_InIter>::iterator_category,
							std::input_iterator_tag
					>::value
	>::type;
	
	template<class T>
	auto begin(T &v)->decltype(&*std::begin(v))
	{
		return &*std::begin(v);
	}
	
	template<class T>
	auto begin(T* v)->decltype(v)
	{
		return v;
	};
	
	template<class T>
	auto end(T &v)->decltype(&*std::end(v))
	{
		return &*std::end(v);
	}
	
	template<class T>
	auto end(T* v)->decltype(v)
	{
		return v + strlen(v);
	}
	
	template<class _It>
	class IteratorBase:public std::iterator
			<
					typename std::iterator_traits<_It>::iterator_category,
					typename std::iterator_traits<_It>::value_type,
					typename std::iterator_traits<_It>::difference_type,
					typename std::iterator_traits<_It>::pointer,
					typename std::iterator_traits<_It>::reference
			>
	{
	public:
		IteratorBase();
		virtual ~IteratorBase() = default;
		
		IteratorBase<_It> operator+(const size_t n) const;
		IteratorBase<_It> operator-(const size_t n) const;
		
		virtual IteratorBase<_It> &operator++();
		virtual IteratorBase<_It> &operator--();
		
		virtual IteratorBase<_It> &operator+=(size_t right);
		virtual IteratorBase<_It> &operator-=(size_t right);
		
		virtual bool operator==(IteratorBase<_It> &other) const;
		virtual bool operator!=(IteratorBase<_It> &other) const;
		
		virtual bool operator>(const IteratorBase<_It> &right) const;
		virtual bool operator<(const IteratorBase<_It> &right) const;
		
		virtual operator char*() const;
		
		static size_t _distance(IteratorBase it1, IteratorBase it2);
	
	SYNTHESIZE_READONLY(size_t, _iteration, Iteration);
	};
	
	using Iterator = IteratorBase<char*>;
	
	
	template<class _It>
	IteratorBase<_It>::IteratorBase():_iteration(0)
	{
	}
	
	template<class _It>
	IteratorBase<_It> IteratorBase<_It>::operator+(const size_t n) const
	{
		IteratorBase<_It> ret;
		ret += n;
		return ret;
	}
	
	template<class _It>
	IteratorBase<_It> IteratorBase<_It>::operator-(const size_t n) const
	{
		IteratorBase<_It> ret;
		ret -= n;
		return ret;
	}
	
	template<class _It>
	IteratorBase<_It> &IteratorBase<_It>::operator++()
	{
		++_iteration;
		return *this;
	}
	
	template<class _It>
	IteratorBase<_It> &IteratorBase<_It>::operator--()
	{
		if( _iteration > 0 )
		{
			--_iteration;
		}
		return *this;
	}
	
	template<class _It>
	IteratorBase<_It> &IteratorBase<_It>::operator+=(size_t right)
	{
		_iteration += right;
		return *this;
	}
	
	template<class _It>
	IteratorBase<_It> &IteratorBase<_It>::operator-=(size_t right)
	{
		int newIteration = _iteration - right;
		_iteration = newIteration > 0 ? _iteration - right : 0;
		return *this;
	}
	
	template<class _It>
	bool IteratorBase<_It>::operator==(IteratorBase<_It> &other) const
	{
		return _iteration == other._iteration;
	}
	
	template<class _It>
	bool IteratorBase<_It>::operator!=(IteratorBase<_It> &other) const
	{
		return _iteration != other._iteration;
	}
	
	template<class _It>
	bool IteratorBase<_It>::operator>(const IteratorBase<_It> &right) const
	{
		return _iteration > right._iteration;
	}
	
	template<class _It>
	bool IteratorBase<_It>::operator<(const IteratorBase<_It> &right) const
	{
		return _iteration < right._iteration;
	}
	
	template<class _It>
	IteratorBase<_It>::operator char*() const
	{
		return nullptr;
	}
	
	template<class _It>
	size_t IteratorBase<_It>::_distance(IteratorBase it1, IteratorBase it2)
	{
		return it2.getIteration() - it1.getIteration();
	}
}
#endif //ITERATOR_H_
