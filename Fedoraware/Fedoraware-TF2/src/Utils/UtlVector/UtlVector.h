#pragma once
#include <cassert>
#include <memory>
#include <algorithm>

template <typename T>
void Construct(T* pMemory) {
    new (pMemory) T();
}

template <typename T>
void CopyConstruct(T* pMemory, T const& src) {
    new (pMemory) T(src);
}

template <typename T>
void Destruct(T* pMemory) {
    pMemory->~T();
}

template <typename T>
class CUtlMemory {
public:
    CUtlMemory(size_t nGrowSize = 0, size_t nInitSize = 0);
    CUtlMemory(T* pMemory, size_t numElements);
    ~CUtlMemory();

    T& operator[](size_t i);
    T const& operator[](size_t i) const;
    T& Element(size_t i);
    T const& Element(size_t i) const;

    bool IsIdxValid(size_t i) const;

    T* Base();
    T const* Base() const;

    void SetExternalBuffer(T* pMemory, size_t numElements);

    size_t NumAllocated() const;
    size_t Count() const;

    void Grow(size_t num = 1);

    void Purge();

    bool IsExternallyAllocated() const;

    void SetGrowSize(size_t size);

private:
    std::unique_ptr<T[]> m_pMemory;
    size_t m_nAllocationCount;
    size_t m_nGrowSize;
};

template <typename T>
CUtlMemory<T>::CUtlMemory(size_t nGrowSize, size_t nInitSize)
    : m_pMemory(nInitSize ? std::make_unique<T[]>(nInitSize) : nullptr),
      m_nAllocationCount(nInitSize),
      m_nGrowSize(nGrowSize) {}

template <typename T>
CUtlMemory<T>::CUtlMemory(T* pMemory, size_t numElements)
    : m_pMemory(pMemory, pMemory + numElements),
      m_nAllocationCount(numElements),
      m_nGrowSize(0) {}

template <typename T>
CUtlMemory<T>::~CUtlMemory() {
    Purge();
}

template <typename T>
T& CUtlMemory<T>::operator[](size_t i) {
    assert(IsIdxValid(i));
    return m_pMemory[i];
}

template <typename T>
T const& CUtlMemory<T>::operator[](size_t i) const {
    assert(IsIdxValid(i));
    return m_pMemory[i];
}

template <typename T>
T& CUtlMemory<T>::Element(size_t i) {
    assert(IsIdxValid(i));
    return m_pMemory[i];
}

template <typename T>
T const& CUtlMemory<T>::Element(size_t i) const {
    assert(IsIdxValid(i));
    return m_pMemory[i];
}

template <typename T>
bool CUtlMemory<T>::IsIdxValid(size_t i) const {
    return i < m_nAllocationCount;
}

template <typename T>
T* CUtlMemory<T>::Base() {
    return m_pMemory.get();
}

template <typename T>
T const* CUtlMemory<T>::Base() const {
    return m_pMemory.get();
}

template <typename T>
void CUtlMemory<T>::SetExternalBuffer(T* pMemory, size_t numElements) {
    Purge();
    m_pMemory.reset(pMemory, pMemory + numElements);
    m_nAllocationCount = numElements;
    m_nGrowSize = 0;
}

template <typename T>
size_t CUtlMemory<T>::NumAllocated() const {
    return m_nAllocationCount;
}

template <typename T>
size_t CUtlMemory<T>::Count() const {
    return m_nAllocationCount;
}

template <typename T>
void CUtlMemory<T>::Grow(size_t num) {
    size_t new_size = std::max(m_nAllocationCount + num, m_nAllocationCount * 2);
    std::unique_ptr<T[]> new_memory(new T[new_size]);
    std::uninitialized_copy(m_pMemory.get(), m_pMemory.get() + m_nAllocationCount, new_memory.get());
    m_pMemory.swap(new_memory);
    m_nAllocationCount = new_size;
}

template <typename T>
void CUtlMemory<T>::Purge() {
    m_pMemory.reset();
    m_nAllocationCount = 0;
}

template <typename T>
void CUtlMemory<T>::SetGrowSize(size_t size) {
    m_nGrowSize = size;
}

template <typename T>
bool CUtlMemory<T>::IsExternallyAllocated() const {
    return m_nGrowSize == 0;
}
