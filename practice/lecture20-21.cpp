#include <iostream>

// represent a 2D matrix using 1D layout (row-major)
template <typename T>
class Matrix {

  public:

  Matrix(int M, int N);
  ~Matrix();
  
  // set Matrix(m, n) to v
  void set_value(int m, int n, int v);

  // get Matrix(m, n)
  int get_value(int m, int n) const;

  // get # columns
  int columns() const;
  int rows() const;

  // prints the content of the matrix
  void print() const;
  
  Matrix& operator -= (const Matrix& rhs);
  Matrix& operator += (const Matrix& rhs);
  Matrix& operator  = (const Matrix& rhs);

  bool operator == (const Matrix& rhs) const;
  bool operator != (const Matrix& rhs) const;

  Matrix operator + (const Matrix& rhs) const;
  Matrix operator - (const Matrix& rhs) const;
  Matrix operator * (const Matrix& rhs) const;  // matrix mult (your practice)

  private:
    
    int _M;      // number of rows
    int _N;      // number of cols

    T* _data;  // pointer to the memory address 
    // float* _data;
    // double* _data;
    
    void _check(const Matrix& rhs) const;
};

// Constructor
template <typename T>
Matrix<T>::Matrix(int M, int N) {
  _M = M;
  _N = N;

  // Matrix<int>: T => int;
  // Maitrx<float>: T => float
  // Matrix<double>: T => double
  _data = (T*)std::malloc(M*N*sizeof(T)); // borrow memory from OS
}

// Destructor
template <typename T>
Matrix<T>::~Matrix() {
  // RAII (resource acquisition is initialization)
  std::free(_data);
}
  
// Function: set Matrix(m, n) to v
template <typename T>
void Matrix<T>::set_value(int m, int n, int v) {
  _data[m*_N + n] = v;
}

// Function: get Matrix(m, n)
template <typename T>
int Matrix<T>::get_value(int m, int n) const {
  return _data[m*_N + n];
}

template <typename T>
int Matrix<T>::columns() const {
  return _N;
}

template <typename T>
int Matrix<T>::rows() const {
  return _M;
}

template <typename T>
void Matrix<T>::print() const {
  for(int m=0; m<_M; m++) {  // iterate each row m
    for(int n=0; n<_N; n++) {  // iterate each column at the row m
      std::cout << get_value(m, n) << ' ';
    }
    std::cout << "\n";
  }
}
    
template <typename T>
void Matrix<T>::_check(const Matrix& rhs) const {
  if(!(rhs._M == _M && rhs._N == _N)) {
    std::cout << "dimension mismatches!\n";
    std::exit(EXIT_FAILURE);
  }
}

//Matrix& operator += (const Matrix& rhs);
template <typename T>
Matrix<T>& Matrix<T>::operator += (const Matrix& rhs) {
  _check(rhs);
  for(int i=0; i<(_M * _N); i++) {
    _data[i] += rhs._data[i];
  }
  return *this;
}

//Matrix& operator -= (const Matrix& rhs);
template <typename T>
Matrix<T>& Matrix<T>::operator -= (const Matrix& rhs) {
  _check(rhs);
  for(int i=0; i<(_M * _N); i++) {
    _data[i] -= rhs._data[i];
  }
  return *this;
}

// a = b -> rhs will be b
template <typename T>
Matrix<T>& Matrix<T>::operator = (const Matrix& rhs) {
  _check(rhs);
  for(int i=0; i<(_M * _N); i++) {
    _data[i] = rhs._data[i];
  }
  return *this;
}

//bool operator == (const Matrix& rhs);
template <typename T>
bool Matrix<T>::operator == (const Matrix& rhs) const {
  if(!(rhs._M == _M && rhs._N == _N)) {
    return false;
  }
  for(int i=0; i<(_M * _N); i++) {
    if(_data[i] != rhs._data[i]) {
      return false;
    }
  }
  return true;
}


//bool operator != (const Matrix& rhs);
//if(a != b) => if (a.operator!=(b))
template <typename T>
bool Matrix<T>::operator != (const Matrix& rhs) const {
  return !(this->operator==(rhs));
  //return !((*this).operator==(rhs))  // same as the above line
}

// plus operator
// a + b => a.operator+(b) => *this + b
template <typename T>
Matrix<T> Matrix<T>::operator + (const Matrix& b) const {
  _check(b);

  Matrix c(_M, _N);

  for(int i=0; i<(_M * _N); i++) {
    c._data[i] = _data[i] + b._data[i];
    //c._data[i] = this->_data[i] + b._data[i];    // same above
    //c._data[i] = (*this)._data[i] + b._data[i];  // same again
  }
  
  return c;
}

// minus operator
// a - b => a.operator-(b) => *this - b
template <typename T>
Matrix<T> Matrix<T>::operator - (const Matrix& b) const {
  _check(b);

  Matrix c(_M, _N);

  for(int i=0; i<(_M * _N); i++) {
    c._data[i] = _data[i] - b._data[i];
  }
  
  return c;
}


// Main function: showing usage of Matrix
// From user's perspective
int main() {

  //Matrix<int> mat_of_integers;
  //Matrix<float> mat_of_floats;
  //Matrix<double> mat_of_doubles;

  const int M = 4;  // number of rows
  const int N = 5;  // number of cols

  Matrix<double> a(M, N), b(M, N), c(M, N);

  // initialize a
  for(int m=0; m<M; m++) {  // iterate each row m
    for(int n=0; n<N; n++) {  // iterate each column at the row m
      a.set_value(m, n, 10);
      b.set_value(m, n, 100);
    }
  }

  // print out the matrix content of a and b
  std::cout << "content of the matrix a: \n";
  a.print();
  
  std::cout << "content of the matrix b: \n";
  b.print();

  // assign b to a
  std::cout << "a = b\n";
  a = b;  // equivalent to explicitly calling a.operator= (b);
  
  std::cout << "content of the matrix a: \n";
  a.print();

  // a == b
  //std::cout << "a == b ? "  << (a == b) << '\n';
  std::cout << "a == b ? "  << (a.operator == (b)) << '\n';

  // add two matrices
  std::cout << "a += b\n";
  a += b;     // a.operator+=(b)
  a.print();

  // add tow matrices to a resulting matrix
  std::cout << "c = a + b\n";
  //c = a + b;  // 1st operator: +
                // 2nd operator: = 
  c = a + b;  // => c.operator=( a + b )
              // => c.operator=( a.operator+(b) )
  c.print();
  
  // minus
  std::cout << "c = a - b\n";
  //c = a - b;  // 1st operator: -
                // 2nd operator: = 
  c = a - b;  // => c.operator=( a - b )
              // => c.operator=( a.operator-(b) )
  c.print();

  //
  //std::cout << "content of the matrix c = a + b: \n";
  //for(int m=0; m<M; m++) {  // iterate each row m
  //  for(int n=0; n<N; n++) {  // iterate each column at the row m
  //    std::cout << c.get_value(m, n) << ' ';
  //  }
  //  std::cout << "\n";
  //}


  return 0;
}








