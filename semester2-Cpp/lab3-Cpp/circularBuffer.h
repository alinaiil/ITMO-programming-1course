#include <iterator>

template<typename T>
class CircularBuffer {
private:
    T *elements = nullptr;
    unsigned capacity = 0;
    unsigned length = 0;
    unsigned head = 0, tail = 0;
public:
    explicit CircularBuffer(unsigned capacity_) {
        elements = new T[capacity_];
        capacity = capacity_;
    }

    ~CircularBuffer() {
        delete[] elements;
    }

    void push_back(T element) {
        if (length == capacity) {
            head = (head + 1) % capacity;
        } else {
            length += 1;
        }
        elements[tail] = element;
        tail = (tail + 1) % capacity;
    }

    void pop_back() {
        if (length) {
            length -= 1;
            elements[tail - 1] = 0;
            tail = tail - 1;
        }
    }

    void push_front(T element) {
        if (length == capacity) {
            tail = (capacity + tail - 1) % capacity;
        } else {
            length += 1;
        }
        head = (capacity + head - 1) % capacity;
        elements[head] = element;
    }

    void pop_front() {
        if (length) {
            T *newElements = new T[capacity];
            for (unsigned i = 0; i < length - 1; i++) {
                newElements[i] = elements[(head + i + 1) % capacity];
            }
            delete[] elements;
            elements = newElements;
            length -= 1;
            head = 0;
            tail = length;
        }
    }

    T get_back() {
        return elements[(capacity + tail - 1) % capacity];
    }

    T get_front() {
        return elements[head];
    }

    T &operator[](unsigned i) {
        if (i >= length) {
            throw std::out_of_range("ERROR: index out of range!");
        } else {
            return elements[(head + i) % capacity];
        }
    }

    void changeCapacity(unsigned newCapacity) {
        if (newCapacity < length) throw std::logic_error("ERROR: wrong capacity!");
        capacity = newCapacity;
        T *newElements = new T[newCapacity];
        for (unsigned i = 0; i < length; i++) {
            newElements[i] = elements[(head + i) % capacity];
        }
        delete[] elements;
        elements = newElements;
        head = 0;
        tail = length;
    }

    friend std::ostream &operator<<(std::ostream &out, const CircularBuffer &circularBuffer) {
        for (unsigned i = 0; i < circularBuffer.length; i++) {
            out << circularBuffer.elements[(circularBuffer.head + i) % circularBuffer.capacity] << " ";
        }
        return out;
    }

    class BufferIterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T *, T &> {
    private:
        T *data;
        unsigned size;
        unsigned index;
        unsigned start;
    public:
        BufferIterator(const BufferIterator &bufferIterator) {
            data = bufferIterator.data;
            size = bufferIterator.size;
            index = bufferIterator.index;
            start = bufferIterator.start;
        }

        BufferIterator& operator=(const BufferIterator &bufferIterator) {
            if (&bufferIterator == this) {
                return *this;
            }
            data = bufferIterator.data;
            size = bufferIterator.size;
            index = bufferIterator.index;
            start = bufferIterator.start;
            return *this;
        }

        BufferIterator(T *data_, unsigned size_, unsigned index_, unsigned start_) {
            data = data_;
            size = size_;
            index = index_;
            start = start_;
        }

        T &operator*() {
            return data[(start + index) % size];
        }

        T *operator->() {
            return data + (start + index) % size;
        }

        BufferIterator &operator++() {
            index++;
            return *this;
        }

        BufferIterator operator++(int) {
            BufferIterator temp = *this;
            index++;
            return temp;
        }

        BufferIterator &operator--() {
            index--;
            return *this;
        }

        BufferIterator operator--(int) {
            BufferIterator temp = *this;
            index--;
            return temp;
        }

        BufferIterator &operator+=(ptrdiff_t n) {
            index += n;
            return *this;
        }

        BufferIterator operator+(ptrdiff_t n) {
            BufferIterator temp = *this;
            temp += n;
            return temp;
        }

        BufferIterator operator-=(ptrdiff_t n) {
            index -= n;
            return *this;
        }

        BufferIterator operator-(ptrdiff_t n) {
            BufferIterator temp = *this;
            temp -= n;
            return temp;
        }

        ptrdiff_t operator-(const BufferIterator &other) {
            return (index - other.index);
        }

        T &operator[](unsigned i) {
            if (i >= size) {
                throw std::out_of_range("ERROR: index out of range!");
            } else {
                return data[(start + i) % size];
            }
        }

        bool operator<(const BufferIterator &other) {
            return (index < other.index);
        }

        bool operator>(const BufferIterator &other) {
            return (index > other.index);
        }

        bool operator<=(const BufferIterator &other) {
            return (index <= other.index);
        }

        bool operator>=(const BufferIterator &other) {
            return (index >= other.index);
        }

        bool operator!=(const BufferIterator &other) {
            return (index != other.index);
        }

        bool operator==(const BufferIterator &other) {
            return (index == other.index);
        }
    };

    BufferIterator begin() {
        return BufferIterator(elements, capacity, 0, head);
    }

    BufferIterator end() {
        return BufferIterator(elements, capacity, length, head);
    }
};