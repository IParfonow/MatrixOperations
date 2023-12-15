#pragma once
#include <functional>
#include "FileLoader.h"


template<typename T>
class Sorter
{
public:
    virtual ~Sorter() = default;
    using PredicateType = std::function<bool(const T& left, const T& right)>;
    virtual DynamicArray<T> Sort(const DynamicArray<T>& DataToSort) const = 0;
    virtual DynamicArray<T> SortWithPredicate(const DynamicArray<T>& DataToSort,
        const PredicateType& Predicate) const = 0;
};



template<typename T>
class QuickSorter : public Sorter<T>
{
public:
    int MatrixPartitionWithPredicate(DynamicArray<T> DataToSort,int left, int right, Sorter<T>::PredicateType Predicate) const
    {
        T pivot_value = DataToSort[right];
        int store_index = left;
        for(int i = left; i < right; i++)
        {
            if(Predicate(DataToSort[i] , pivot_value) == true)
            {
                store_index++;
                std::swap(DataToSort[i], DataToSort[store_index]);
            }
        }
        std::swap(DataToSort[store_index], DataToSort[right]);
        return store_index;
    }
    const DynamicArray<T> QuickSortWithPredicate(const DynamicArray<T>& DataToSort,int left, int right, Sorter<T>::PredicateType Predicate) const
    {
        if(left < right)
        {
            int pivot_index = MatrixPartitionWithPredicate(DataToSort,left, right, Predicate);
            QuickSortWithPredicate(DataToSort, left, pivot_index - 1, Predicate);
            QuickSortWithPredicate(DataToSort, pivot_index + 1,right, Predicate);
        }
        return DataToSort;
    }
    DynamicArray<T> Sort(const DynamicArray<T>& DataToSort) const override
    {
        auto Predicate = [](const T& left, const T& right){return left < right;};
        DynamicArray<T> SortedData = DataToSort;
        if(SortedData.size() <= 1)
        {
            return SortedData;
        }
        unsigned int pivot_index = SortedData.size()/2;
        T pivot = SortedData[pivot_index];
        DynamicArray<T> left, right;

        for(unsigned int i = 0; i < SortedData.size(); i++)
        {
            if(i == pivot_index)
            {
                continue;
            }
            if(Predicate(SortedData[i], pivot))
            {
                left.push_back(SortedData[i]);
            }
            else
            {
                right.push_back(SortedData[i]);
            }
        }
        Sort(left);
        Sort(right);

        SortedData.clear();
        SortedData.reserve(left.size() + 1 + right.size());
        SortedData.insert(SortedData.end(), left.begin(), left.end());
        SortedData.push_back(pivot);
        SortedData.insert(SortedData.end(), right.begin(), right.end());
        return SortedData;
    }
    DynamicArray<T> SortWithPredicate(const DynamicArray<T>& DataToSort,
        const Sorter<T>::PredicateType& Predicate) const override
    {
        DynamicArray<T> SortedData = DataToSort;
        return QuickSortWithPredicate(SortedData, 0, SortedData.size()-1, Predicate);
    }
};


template<typename T>
class StandartSorter : public Sorter<T>
{
public:
    DynamicArray<T> Sort(const DynamicArray<T>& DataToSort) const override
    {
        DynamicArray<T> SortedData = DataToSort;
        auto Predicate = [](const T& left, const T& right){return left < right;};
        for(unsigned int i = 0;i < SortedData.size();i++)
        {
            for(unsigned int j = 0; j < SortedData.size() - i - 1;j++)
            {
                if(Predicate(SortedData[j + 1],SortedData[j]))
                {
                    std::swap(SortedData[j],SortedData[j+1]);
                }
            }
        }
        return  SortedData;
    }
    DynamicArray<T> SortWithPredicate(const DynamicArray<T>& DataToSort,
        const Sorter<T>::PredicateType& Predicate) const override
    {
        DynamicArray<T> SortedData = DataToSort;
        for(unsigned int i = 0; i < SortedData.size(); i++)
        {
            for(unsigned int j = 0; j < SortedData.size() - i - 1; j++)
            {
                if(j == SortedData.size() - 1)
                {
                    return SortedData;
                }
                if(Predicate(SortedData[j+1], SortedData[j]))
                {
                    std::swap(SortedData[j+1], SortedData[j]);
                }
            }
        }
        return SortedData;
    }
};