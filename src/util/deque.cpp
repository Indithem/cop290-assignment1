export module ContgDeque;

#include <vector>
#include <iterator>

export template <typename T>
class Deque{
private:
    std::vector<T> data;
    // points to the first element
    size_t start;
    // points to the empty space where the next element can be inserted
    size_t end;
    // total size of the container
    size_t size;

public:
    Deque();
    ~Deque(){};

    void resize(size_t n);
    void push_back(T value);
    void push_front(T value);
    void pop_back();
    void pop_front();

    class iterator;
    iterator get_iterator();
    
};

template <typename T>
Deque<T>::Deque(){
    size = 0;
    start = 0;
    end = 0;
}

template <typename T>
void Deque<T>::resize(size_t n){
    data.resize(2*n);
    size = 2*n;
}

template <typename T>
void Deque<T>::push_back(T value){
    if ((end+1)%size == start){ 
        resize(size*2);
    }
    data[end] = value;
    end = (end+1)%size;
}

template <typename T>
void Deque<T>::push_front(T value){
    if ((start-1)%size == end){ 
        resize(size*2);
    }
    start = (start-1)%size;
    data[start] = value;
}

template <typename T>
void Deque<T>::pop_back(){
    if (end == start){
        throw std::out_of_range("Deque is empty");
    }
    end = (end-1)%size;
}

template <typename T>
void Deque<T>::pop_front(){
    if (end == start){
        throw std::out_of_range("Deque is empty");
    }
    start = (start+1)%size;
}

template <typename T>
class Deque<T>::iterator{
    vector<T>* container;
    // current iteration
    size_t iter;
    size_t end;
    size_t size;
public:
    iterator(vector<T>* container, size_t iter, size_t end, size_t size) 
        :container(container), iter(iter), end(end), size(size) {};

    T operator*(){
        return container->at(iter);
    }

    bool operator++(){
        if (iter == end){
            return false;
        }

        iter = (iter+1)%size;
        return true;
    }
};

template <typename T>
typename Deque<T>::iterator Deque<T>::get_iterator(){
    return iterator(&data, start, end, size);
}