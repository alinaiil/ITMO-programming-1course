template<typename InputIt, typename UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate pred) {
    for (first; first != last; first++) {
        if (!pred(*first))
            return false;
    }
    return true;
}

template<typename InputIt, typename UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate pred) {
    for (first; first != last; first++) {
        if (pred(*first))
            return true;
    }
    return false;
}

template<typename InputIt, typename UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate pred) {
    for (first; first != last; first++) {
        if (pred(*first))
            return false;
    }
    return true;
}

template<typename InputIt, typename UnaryPredicate>
bool one_of(InputIt first, InputIt last, UnaryPredicate pred) {
    bool flag = false;
    for (first; first != last; first++) {
        if (pred(*first)) {
            if (flag) {
                return false;
            } else
                flag = true;
        }
    }
    return flag;
}

template<typename InputIt, typename BinaryPredicate>
bool is_sorted(InputIt first, InputIt last, BinaryPredicate pred) {
    for (first; first != (last - 1); first++) {
        if (!pred(*first, *(first + 1)))
            return false;
    }
    return true;
}

template<typename InputIt, typename UnaryPredicate>
bool is_partitioned(InputIt first, InputIt last, UnaryPredicate pred) {
    bool flag = false;
    bool one = pred(*first++);
    for (first; first != last; first++) {
        if (pred(*first) != one) {
            if (!flag)
                flag = true;
        } else if (flag)
            return false;
    }
    return true;
}

template<typename InputIt, typename T>
InputIt find_not(InputIt first, InputIt last, T value) {
    for (first; first != last; first++) {
        if (*first != value)
            return first;
    }
    return last;
}

template<typename InputIt, typename T>
InputIt find_backward(InputIt first, InputIt last, T value) {
    InputIt end = last;
    for ((last - 1); last != first - 1; last--) {
        if (*last == value)
            return last;
    }
    return end;
}

template<typename InputIt, typename UnaryPredicate>
bool is_palindrome(InputIt first, InputIt last, UnaryPredicate pred) {
    InputIt right = last - 1;
    for (first; first != last; first++) {
        if (first == right)
            break;
        if (pred(*first) != pred(*right--))
            return false;
    }
    return true;
}