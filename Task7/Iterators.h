#pragma once
#include <algorithm>
#include <map>
#include <memory>
#include <functional>
#include <string> 

template<typename Vector>
class VectorIterator
{
public:
    using ValueType = typename Vector::ValueType;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;
public:
    VectorIterator(PointerType ptr)
        :m_Ptr(ptr){}
    VectorIterator& operator++()
    {
        m_Ptr++;
        return *this;
    }

    VectorIterator operator++(int)
    {
        VectorIterator iterator = *this;
        ++(*this);
        return iterator;
    }
    VectorIterator& operator--()
    {
        m_Ptr--;
        return *this;
    }

    VectorIterator operator--(int)
    {
        VectorIterator iterator = *this;
        --(*this);
        return iterator;
    }

    ReferenceType operator[](int index)
    {
        return *(m_Ptr + index);
    }

    PointerType operator->()
    {
        return m_Ptr;
    }
    ReferenceType operator*()
    {
        return *m_Ptr;
    } 
    bool operator==(const VectorIterator& other) const
    {
        return m_Ptr == other.m_Ptr;
    }
    bool operator!=(const VectorIterator& other) const
    {
        return !(*this == other);
    }
    
    friend  bool operator<(const VectorIterator& left, const VectorIterator& right)
    {
        return  left.m_Ptr < right.m_Ptr;
    }
    friend  bool operator>(const VectorIterator& left, const VectorIterator& right)
    {
        return  right.m_Ptr < left.m_Ptr;
    }
    friend  bool operator<=(const VectorIterator& left, const VectorIterator& right)
    {
        return  !(left.m_Ptr > right.m_Ptr);
    }
    friend  bool operator>=(const VectorIterator& left, const VectorIterator& right)
    {
        return  !(left.m_Ptr < right.m_Ptr);
    }
private:
    PointerType m_Ptr;
};



template <typename T>
class Vector
{
public:
    using ValueType = T;
    using Iterator = VectorIterator<Vector<T>>;
    using PointerType1 = std::unique_ptr<Vector>;
    using OperationType = std::function<T(const T& left, const T& right)>;
public:
    Vector()
    {
        ReAlloc(2);
    }
    
    Vector(const Vector& other)
        :m_Size(other.m_Size),
    m_Capacity(other.m_Capacity)
    {
        m_Data = new Vector[m_Size];
        memcpy(m_Data, other.m_Data, m_Size*sizeof(Vector));
    }

    Vector(Vector&& other) noexcept
        :m_Data(other.m_Data),
    m_Size(other.m_Size),
    m_Capacity(other.m_Capacity)
    {
        other.m_Size = 0;
        other.m_Capacity = 0;
        other.m_Data = nullptr;
    }
    
    Vector& operator=(const Vector& other)
    {
        if(this != &other)
        {
            ~Vector();
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;
            m_Data = new T[m_Size];
            memcpy(m_Data, other.m_Data, m_Size*sizeof(T));
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        ~Vector();
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = other.m_Data;
        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
        return *this;
    }
        
    ~Vector()
    {
        Clear();
        ::operator delete(m_Data, m_Capacity * sizeof(T));
    }
    const Vector Sort() const
    {
        return QuickSorter<T>::Sort(*this);
    }

    
    void PushBack(const T& value)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        new(&m_Data[m_Size]) T(value);
        m_Size++;
    }
    void PushBack(T&& value)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        new(&m_Data[m_Size]) T(std::move(value));
        m_Size++;
    }

    template<typename... Args>
    T& EmplaceBack(Args&&... args)
    {
        if(m_Size >= m_Capacity)
            ReAlloc(m_Capacity + m_Capacity / 2);

        new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
        return m_Data[m_Size++];
    }

    void PopBack()
    {
        if(m_Size > 0)
        {
            m_Size--;
            m_Data[m_Size].~T();
        }
    }

    void Clear()
    {
        for(size_t i = 0; i < m_Size; i++)
            m_Data[i].~T();

        m_Size = 0;
    }

    const T& operator[](size_t index) const
    {
        if(index >= m_Size)
        {
            // assert
        }
        return m_Data[index];
    }

    T& operator[](size_t index)
    {
        if(index >= m_Size)
        {
            // asssert
        }
        return m_Data[index];
    }

    friend std::ostream& operator<<(std::ostream& output,const Vector& Data)
    {
        output << '[';
        for(unsigned int j = 0; j < Data.Size();j++)
        {
            output << Data[j] << ',';
        }
        output << ']';
        return output;
    }
    size_t Size() const {return m_Size;}

    const void Print()
    {
        for(Iterator it = Begin(); it != End(); it++)
        {
            std::cout << *it << ' ';
        }
    }
    Iterator Next() const
    {
        return Iterator(m_Data++);
    }
    Iterator Begin() const
    {
        return Iterator(m_Data);
    }
    Iterator End() const
    {
        return Iterator(m_Data + m_Size);
    }

    T Calculate(const Vector<std::string>& operators)
    {
        if(operators.Size() == 0 )
        {
            std::cout << "Operators container is empty" << '\n';
        }
        T result = m_Data[0];
        for(unsigned int i = 0; i < operators.Size(); i++)
        {
            if(actions.count(operators[i]) == 0)
            {
                std::cout << "Operator not supported" << '\n';
                break;
            }
            result = actions[operators[i]](result, m_Data[i + 1]);
        }
        return result;
    }

    void ReAlloc(size_t newCapacity)
    {
        T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

        if(newCapacity < m_Size)
            m_Size = newCapacity;

        for(size_t i = 0; i < m_Size; i++)
            new(&newBlock[i])  T(std::move(m_Data[i]));

        for(size_t i = 0; i < m_Size; i++)
            m_Data[i].~T();
        
        ::operator delete(m_Data, m_Capacity * sizeof(T));
        m_Data = newBlock;
        m_Capacity = newCapacity;
    }
private:
    static std::map<std::string, OperationType> actions;
    T* m_Data = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
};

template<typename T>
std::map<std::string, std::function<T(const T&, const T&)>> Vector<T>::actions = 
{
    {"+", [](const T& left, const T& right){return (left + right);}},
    {"-", [](const T& left, const T& right){return (left - right);}},
    {"*", [](const T& left, const T& right){return (left * right);}}
};
