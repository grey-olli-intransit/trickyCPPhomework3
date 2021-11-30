#include <iostream>
#include <list>
#include <cassert>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
#pragma ide diagnostic ignored "EndlessLoop"
// Написать функцию, добавляющую в конец списка вещественных чисел элемент, значение которого равно
// среднему арифметическому всех его элементов.
void appendArithmeticMean(std::list<float> & _lst) {
    size_t total=0;
    float summ=0.0;
    if (_lst.empty())
        throw std::logic_error("List is empty.");
    for(auto item : _lst) {
        ++total;
        summ+=item;
    }
    summ/=(float)total;
    _lst.emplace_back(summ);
}

//Создать класс, представляющий матрицу. Реализовать в нем метод, вычисляющий определитель матрицы.
class Matrix {
private:
    int size;
    int** matrix;

    void getMatrixWithoutRowAndCol(int** origMatrix, int msize, int row, int col, int** newMatrix) {
        int offsetRow = 0;
        int offsetCol = 0;
        for(int i = 0; i < msize - 1; i++) {
            if(i == row) {
                offsetRow = 1;
            }
            offsetCol = 0;
            for(int j = 0; j < msize - 1; j++) {
                if(j == col) {
                    offsetCol = 1;
                }

                newMatrix[i][j] = origMatrix[i + offsetRow][j + offsetCol];
            }
        }
    }

    int getDeterminant(int** mmatrix, int msize) {
        int det = 0;
        int degree = 1;

        if(msize == 1) {
            return mmatrix[0][0];
        }
        if(msize == 2) {
            return mmatrix[0][0] * mmatrix[1][1] - mmatrix[0][1] * mmatrix[1][0];
        }

        int** newMatrix = new int*[msize-1];
        for(int i = 0; i < msize-1; i++) {
            newMatrix[i] = new int[msize-1];
        }
        for(int j = 0; j < msize; j++) {
            getMatrixWithoutRowAndCol(mmatrix, msize, 0, j, newMatrix);
            det = det + (degree * mmatrix[0][j] * getDeterminant(newMatrix, msize - 1));
            degree = -degree;
        }
        for(int i = 0; i < size-1; i++) {
            delete [] newMatrix[i];
        }
        delete [] newMatrix;

        return det;
    }

public:
    explicit Matrix(int size) : size(size) {
        if (size < 1)
            throw "Matrix size cannot be less then 1";
        matrix = new int*[size];
        for(int i = 0; i < size; i++) {
            matrix[i] = new int[size];
        }
    }

    ~Matrix() {
        for(int i = 0; i < size; i++) {
            delete [] matrix[i];
        }
        delete [] matrix;
    }

    void print() {
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j)
                std::cout << matrix[i][j] << " ";
            std::cout << std::endl;
        }
    }

    void fillInteractive() {
        std::cout << "Enter matrix (" << size << "x" << size << ") string by string:"
                  << std::endl;
        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                std::cin >> matrix[i][j];
    }

    void fillWithRandom() {
        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                matrix[i][j] = rand() % 10;
    }

    int calculateDeterminant() {
        return getDeterminant(this->matrix, this->size);
    }
};

// Реализовать собственный класс итератора, с помощью которого можно будет
// проитерироваться по диапазону целых чисел в цикле for-range-based.
template<typename ValueType>
class OwnIterator: public std::iterator<std::input_iterator_tag, ValueType> {
    friend class ArrayInt;
private:
    explicit OwnIterator(ValueType* p);
public:
    OwnIterator(const OwnIterator &it);

    bool operator!=(OwnIterator const& other) const;
    bool operator==(OwnIterator const& other) const;
    typename OwnIterator::reference operator*() const;
    OwnIterator& operator++();
private:
    ValueType* p;
};

template<typename ValueType>
OwnIterator<ValueType>::OwnIterator(ValueType *p) : p(p) {}

template<typename ValueType>
OwnIterator<ValueType>::OwnIterator(const OwnIterator& it) : p(it.p) {}

template<typename ValueType>
bool OwnIterator<ValueType>::operator!=(OwnIterator const& other) const {
    return p != other.p;
}

template<typename ValueType>
bool OwnIterator<ValueType>::operator==(OwnIterator const& other) const {
    return p == other.p;
}

template<typename ValueType>
typename OwnIterator<ValueType>::reference OwnIterator<ValueType>::operator*() const {
    return *p;
}

template<typename ValueType>
OwnIterator<ValueType> &OwnIterator<ValueType>::operator++() {
    ++p;
    return *this;
}

class ArrayInt {
private:
    int m_length;
    int *m_data;

public:
    typedef OwnIterator<int> iterator;
    typedef OwnIterator<const int> const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    ArrayInt() : m_length(0), m_data(nullptr) {}

    ArrayInt(int length) : m_length(length) {
        m_data = new int[length];
    }

    ArrayInt(const std::initializer_list<int> &list): ArrayInt(list.size()) {
        int count = 0;
        for (auto &element : list) {
            m_data[count] = element;
            ++count;
        }
    }

    ~ArrayInt() {
        delete[] m_data;
    }

    int& operator[](int index) {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }

    int getLength() { return m_length; }
};

ArrayInt::iterator ArrayInt::begin() {
    return iterator(m_data);
}

ArrayInt::iterator ArrayInt::end() {
    return iterator(m_data + m_length);
}
ArrayInt::const_iterator ArrayInt::begin() const {
    return const_iterator(m_data);
}

ArrayInt::const_iterator ArrayInt::end() const {
    return const_iterator(m_data + m_length);
}


int main() {

    ArrayInt integers{1,2,3,4,5,6,7};
    for(auto integer : integers) {
        std::cout << integer << " " ;
    }
    std::cout << std::endl;
    for(auto integerPtr=integers.begin();integerPtr!=integers.end();++integerPtr) {
        std::cout << *integerPtr  << " " ;
    }
    std::cout << std::endl;

    Matrix matrix(3);
    matrix.fillWithRandom();
    //matrix.fillInteractive();
    std::cout << "Matrix:" << std::endl;
    matrix.print();
    std::cout << "Determinant for this matrix: " << matrix.calculateDeterminant() << std::endl;

    std::list<float> lst{2.0, 3.0};
    for(auto item : lst) std::cout << item << " ";
    std::cout << std::endl;
    appendArithmeticMean(lst);
    for(auto item : lst) std::cout << item << " ";
    std::cout << std::endl;
    return 0;
}

#pragma clang diagnostic pop