
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
#include <cstddef>
#include <cmath>

template <class T, class Compare>
class heap;

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return 2 * currentIdx;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return leftChild(currentIdx) + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return std::floor(currentIdx / 2);
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    if (rightChild(currentIdx) >= _elems.size() 
    || higherPriority(_elems.at(leftChild(currentIdx)), _elems.at(rightChild(currentIdx)))) {
        return leftChild(currentIdx);
    } else return rightChild(currentIdx);
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    if (hasAChild(currentIdx)) {
        size_t minChildIndex = maxPriorityChild(currentIdx);
        if (higherPriority(_elems.at(minChildIndex), _elems.at(currentIdx))) {
            std::swap(_elems.at(minChildIndex), _elems.at(currentIdx));
            heapifyDown(minChildIndex);
        }
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    _elems.push_back(T());
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems) 
{
    _elems.push_back(T());
    for (auto elem : elems) _elems.push_back(elem);
    for (size_t i = parent(_elems.size() - 1); i >= 1; i--) heapifyDown(i);
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    T highestPriority = _elems.at(1);
    _elems.at(1) = _elems.at(_elems.size() - 1);
    _elems.pop_back();

    heapifyDown(1);

    return highestPriority;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    return _elems.at(1);
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem);

    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    _elems.at(idx) = elem;
    if (idx != root() && higherPriority(_elems.at(idx), _elems.at(parent(idx)))) {
        heapifyUp(idx);
    } else heapifyDown(idx);
}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return _elems.size() == 1;
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
