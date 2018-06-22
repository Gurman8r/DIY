#ifndef _ARGS_H_
#define _ARGS_H_

#include <iostream>
#include <string>
#include <vector>

namespace pi
{
	class Args
	{
	public:
		using value_type = std::string;
		using vector_type = std::vector<value_type>;
		using iterator = vector_type::iterator;
		using const_iterator = vector_type::const_iterator;
		using reverse_iterator = vector_type::reverse_iterator;
		using const_reverse_iterator = vector_type::const_reverse_iterator;
		using difference_type = vector_type::difference_type;
		using self_type = Args;

	public:
		Args();
		Args(const value_type& value);
		Args(const vector_type& values);
		Args(const std::initializer_list<value_type>& values);
		Args(const value_type& value, const value_type& delim);
		Args(const self_type& copy);
		~Args();

		const value_type	at(unsigned index) const;
		const value_type	back() const;
		const value_type	front() const;
		const value_type	str() const;
		const value_type	substr(unsigned index, unsigned count) const;

		const vector_type	subvec(unsigned index, unsigned count) const;
		const vector_type&	values() const;

		std::stringstream	sstream() const;

		const unsigned	count(const value_type& value) const;
		const unsigned	count(const_iterator first, const_iterator last, const value_type& value) const;
		const unsigned	indexOf(const value_type& value) const;
		const unsigned	size() const;

		const bool		contains(const value_type& value) const;
		const bool		empty() const;
		const bool		inRange(unsigned index) const;
		const bool		inRange(const_iterator it) const;

		const self_type	clone() const;
		const self_type	clone(unsigned index, unsigned count = 1) const;
		const self_type	clone(const self_type& other) const;
		const self_type	clone(const self_type& other, unsigned index, unsigned count = 1) const;
		const self_type	clone(const_iterator first, const_iterator last) const;

	public:
		self_type & assign(const vector_type& value);
		self_type&	clear();
		self_type&	copy(const self_type& other);
		self_type&	copy(const self_type& other, unsigned index);
		self_type&	copy(const self_type& other, unsigned index, unsigned count);
		self_type&	copy(const_iterator first, const_iterator last);
		self_type&	erase(unsigned index, unsigned count = 1);
		self_type&	erase(const_iterator it, unsigned count = 1);
		self_type&	erase(const_iterator first, const_iterator last);
		self_type&	insert(unsigned index, char value);
		self_type&	insert(unsigned index, const char* value);
		self_type&	insert(unsigned index, const value_type& value);
		self_type&	merge(unsigned index, unsigned count);
		self_type&	pop_back();
		self_type&	pop_front();
		self_type&	push_back(char value);
		self_type&	push_back(const char* value);
		self_type&	push_back(const value_type& value);
		self_type&	push_back(const vector_type& value);
		self_type&	push_back(const self_type& value);
		self_type&	push_front(char value);
		self_type&	push_front(const char* value);
		self_type&	push_front(const value_type& value);
		self_type&	push_front(const vector_type& value);
		self_type&	push_front(const self_type& value);
		self_type&	remove(const value_type& value);
		self_type&	removeAll(const value_type& value);
		self_type&	resize(unsigned size);
		self_type&	reverse();

	public:
		Args::const_iterator	find(const value_type& value, unsigned begin = 0) const;
		Args::const_iterator	find_first(const value_type& value) const;
		Args::const_iterator	find_first_not_of(const value_type& value, unsigned begin = 0) const;
		Args::const_iterator	find_last(const value_type& value) const;
		Args::const_iterator	find_last_not_of(const value_type& value) const;

	public:
		Args::iterator					begin();
		Args::iterator					end();
		Args::const_iterator			begin() const;
		Args::const_iterator			end()	const;
		Args::const_iterator			cbegin()const;
		Args::const_iterator			cend()	const;
		Args::reverse_iterator			rbegin();
		Args::reverse_iterator			rend();
		Args::const_reverse_iterator	crbegin() const;
		Args::const_reverse_iterator	crend() const;

	public:
		inline friend std::ostream& operator<<(std::ostream& out, const Args& value)
		{
			for (const value_type& s : value)
				out << s << " ";
			return out;
		}
		inline value_type operator[](unsigned index) const
		{
			if (inRange(index))
			{
				return m_values.at(index);
			}

			return value_type();
		};
		inline Args& operator=(const Args& copy)
		{
			m_values = copy.m_values;
			return (*this);
		}
		inline Args& operator=(const vector_type& value)
		{
			(*this) = Args(value);
			return (*this);
		}

	private:
		vector_type m_values;
	};
}

#endif // !_ARGS_H_
