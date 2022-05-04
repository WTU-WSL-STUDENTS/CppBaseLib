#include <Interface/IEnumerator.hpp>
#include <algorithm>
using namespace System::Interface;
using namespace std;

class Data
{
public:
	int val;
	Data() = default;
	Data(int v) : val(v) {}
};

template<class T = Data>
class WriteableIterator : public OutputIterator<WriteableIterator<T>, T>
{
private:
	WEAK_PTR(T) m_p;
public:
	WriteableIterator(WEAK_PTR(T) p) : m_p(p) {}
	void Increase(int n = 1)
	{
		m_p += n;
	}
	WriteableIterator& operator=(WriteableIterator& other)
	{
		m_p = other.m_p;
		return *this;
	}
	T* operator->() 
	{
		return m_p;
	}
	T& operator*()
	{
		return *m_p;
	}
};

template<class T = Data>
class ReadableIterator : public InputIterator<ReadableIterator<T>, T>
{
private:
	WEAK_PTR(T) m_p;
public:
	ReadableIterator(WEAK_PTR(T) p) : m_p(p) {}
	void Increase(int n = 1)
	{
		m_p += n;
	}
	ReadableIterator& operator=(ReadableIterator& other)
	{
		m_p = other.m_p;
		return *this;
	}
	const T* operator->() const
	{
		return m_p;
	}
	const T& operator*() const
	{
		return *m_p;
	}
};

template<class T = Data>
class ReadwriteableIterator : public ForwardIterator<ReadwriteableIterator<T>, T>
{
private:
	WEAK_PTR(T) m_p;
public:
	ReadwriteableIterator(WEAK_PTR(T) p) : m_p(p) {}
	void Increase(int n = 1)
	{
		m_p += n;
	}
	ReadwriteableIterator& operator=(ReadwriteableIterator& other)
	{
		m_p = other.m_p;
		return *this;
	}
	const T* operator->() const
	{
		return m_p;
	}
	const T& operator*() const
	{
		return *m_p;
	}
	T* operator->()
	{
		return m_p;
	}
	T& operator*()
	{
		return *m_p;
	}
};
template<class T = Data>
class ReadWriteSubIterator : public BidirectionalIterator<ReadWriteSubIterator<T>, T>
{
private:
	WEAK_PTR(T) m_p;
public:
	ReadWriteSubIterator(WEAK_PTR(T) p) : m_p(p) {}
	void Increase(int n = 1)
	{
		m_p += n;
	}
	void Decrease(int n = 1)
	{
		m_p -= n;
	}
	ReadWriteSubIterator& operator=(ReadWriteSubIterator& other)
	{
		m_p = other.m_p;
		return *this;
	}
	const T* operator->() const
	{
		return m_p;
	}
	const T& operator*() const
	{
		return *m_p;
	}
	T* operator->()
	{
		return m_p;
	}
	T& operator*()
	{
		return *m_p;
	}

};




const int arrayLen = 5;
Data buffer[arrayLen] = { 0 };
void PrintfBuffer()
{
	for (int i = 0; i < arrayLen; i++)
	{
		printf(" %d,", buffer[i].val);
	}
	printf("\n");
}

#include <iterator>

template<class D>
class SpanIterator final : public std::iterator<std::random_access_iterator_tag, D>
{
private:
	D* _data;

public:
	explicit SpanIterator(D* data) :
		_data{ data }
	{
	}

	SpanIterator(const SpanIterator& itertator) = default;

	SpanIterator& operator=(const SpanIterator& iterator) = default;

	SpanIterator& operator=(D* data)
	{
		_data = data;

		return *this;
	}

	operator bool() const
	{
		return _data != nullptr;
	}

	bool operator==(const SpanIterator& itertator) const
	{
		return _data == itertator._data;
	}

	bool operator!=(const SpanIterator& itertator) const
	{
		return _data != itertator._data;
	}

	SpanIterator& operator+=(const std::ptrdiff_t& movement)
	{
		_data += movement;

		return *this;
	}

	SpanIterator& operator-=(const std::ptrdiff_t& movement)
	{
		_data -= movement;

		return *this;
	}

	SpanIterator& operator++()
	{
		++_data;

		return *this;
	}

	SpanIterator& operator--()
	{
		--_data;

		return *this;
	}

	SpanIterator operator++(int)
	{
		auto temp = *this;

		++_data;

		return temp;
	}

	SpanIterator operator--(int)
	{
		auto temp = *this;

		--_data;

		return temp;
	}

	/*SpanIterator operator+(const std::ptrdiff_t& movement)
	{
		auto oldPtr = _data;

		_data += movement;

		auto temp = *this;

		_data = oldPtr;

		return temp;
	}

	SpanIterator operator-(const std::ptrdiff_t& movement)
	{
		auto oldPtr = _data;

		_data -= movement;

		auto temp = *this;

		_data = oldPtr;

		return temp;
	}*/

	D& operator*()
	{
		return *_data;
	}

	const D& operator*() const
	{
		return *_data;
	}

	D& operator->()
	{
		return _data;
	}
};

int main()
{
	WriteableIterator wit(buffer);
	*wit = 1;
	*++wit = 2;
	wit++;
	wit->val = 3;
	PrintfBuffer();
	ReadableIterator rit(buffer);
	//*rit = 1;		/* error */
	//rit->val = 3;	/* error */
	int n1 = (*rit++).val;
	int n2 = (*++rit).val;
	printf("readable iterator : %d, %d ( index-0 index-2 should be 1, 3)\n", n1, n2);
	ReadwriteableIterator rwit(buffer);
	rwit++;
	++rwit;
	*++rwit = 4;
	(++rwit)->val = 5;
	assert(4 == buffer[3].val);
	PrintfBuffer();
	ReadWriteSubIterator rwsit(buffer + arrayLen - 1);
	*rwsit-- = 10;
	*rwsit = 9;
	*--rwsit = 8;
	PrintfBuffer();
	SequenceMemoryEnumerator<Data> begin(buffer);
	SequenceMemoryEnumerator<Data> end(buffer + arrayLen);
	std::for_each(begin, end, [](const Data& item) {printf("%d\n", item.val); });


	double data[] = { 1, 2, 3 };
	sort(SpanIterator<double>(data), SpanIterator<double>(data + 3));
	return 0;
}