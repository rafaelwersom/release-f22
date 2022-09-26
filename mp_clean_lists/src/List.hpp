/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  return List<T>::ListIterator(tail_->next);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  while (head_ != NULL) {
    ListNode* next = head_->next;
    delete head_;
    head_ = next;
  }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  ListNode* newNode = new ListNode(ndata);
  newNode->next = head_;
  newNode->prev = NULL;
  
  if (head_ != NULL) {
    head_->prev = newNode;
  }
  head_ = newNode;
  if (tail_ == NULL) {
    tail_ = newNode;
  }

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;

  if (tail_ != NULL) {
    tail_->next = newNode;
  }
  tail_ = newNode;
  if (head_ == NULL) {
    head_ = newNode;
  }

  length_++;

}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != NULL && curr->prev != NULL) {
      curr->prev->next = NULL;
      curr->prev = NULL;
  }

  return curr;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  if (length_ < 3) return;

  int num_trios = length_ / 3;

  ListNode* one = head_;
  ListNode* two = head_->next;
  ListNode* three = head_->next->next;

  head_ = two;

  for (int i = 0; i < num_trios; i++) {
    ListNode* prev = one->prev;
    ListNode* next = three->next;

    three->next = one;
    one->prev = three;

    two->prev = prev;
    if (prev != NULL) prev->next = two;

    one->next = next;
    if (next != NULL) next->prev = one;

    if (three == tail_) tail_ = one;

    if (i < num_trios - 1) {
      one = next;
      two = next->next;
      three = next->next->next;
    }
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
  ListNode* tmp = tail_;
  tail_ = head_;
  head_ = tmp;
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  if (startPoint == NULL) return;
  
  ListNode* start_prev = startPoint->prev;
  ListNode* end_next = endPoint->next;
  ListNode* curr = startPoint;
  while (curr != end_next) {
    ListNode* tmp = curr->prev;
    curr->prev = curr->next;
    curr->next = tmp;
    curr = curr->prev;
  }
  startPoint->next = end_next;
  if (end_next != NULL) end_next->prev = startPoint;
  endPoint->prev = start_prev;
  if (start_prev != NULL) start_prev->next = endPoint;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  for (ListNode* start = head_; start != NULL; start = start->next) {
    ListNode* end = start;
    for (int i = 1; i < n && end->next != NULL; i++) end = end->next;
    if (start == head_) head_ = end;
    if (end == tail_) tail_ = start;
    reverse(start, end);
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  if (first == NULL) return second;
  if (second == NULL) return first;
  ListNode* new_head = (first->data < second->data) ? first : second;
  if (first->data < second->data) first = first->next;
  else second = second->next;
  for (ListNode* curr = new_head; first != NULL || second != NULL; curr = curr->next) {
    if (second == NULL || (first != NULL && first->data < second->data)) {
      first->prev = curr;
      curr->next = first;
      first = first->next;
    } else {
      second->prev = curr;
      curr->next = second;
      second = second->next;
    }
  }
  return new_head;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  if (chainLength == 1) return start;
  ListNode* second = start;
  for (int i = 0; i < chainLength / 2; i++) second = second->next;
  second->prev->next = NULL;
  second->prev = NULL;
  return merge(mergesort(start, chainLength / 2), mergesort(second, (chainLength / 2) - chainLength / 2));
}
