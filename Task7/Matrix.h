#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Exception
{
    public:
    Exception() = default;
    virtual ~Exception() = default;
    virtual std::string get_message() const = 0;
};
class MathematicalOperationOverflowException : public Exception{};
class SumAndSubtractOverflowException: public MathematicalOperationOverflowException
{
    public:
    explicit SumAndSubtractOverflowException(const std::string& message)
        : exceptionMessage(message){}
    
    std::string get_message() const override {return exceptionMessage;}
    
    private:
    std::string exceptionMessage;
};
class MultiplyingOverflowException: public MathematicalOperationOverflowException
{
    public:
    std::string get_message() const override {return "The number of columns must equals to the number of other matrix raws. Update them";}
};
class matrixHeapMemoryOverflowException : public Exception{};
class ParsingStringmatrixOverflowException: public matrixHeapMemoryOverflowException
{
    public:
    std::string get_message() const override {return exception_;}
    ParsingStringmatrixOverflowException(const std::string& Message, const std::string& matrix)
        :exception_(Message + matrix){}
    private:
    std::string exception_;
};
class InvalidArgumentInmatrixException : public matrixHeapMemoryOverflowException
{
    public:
    std::string get_message() const override {return "Int range is from -2147483648 to 2147483647. Change it: ";}
};
class OverflowError :public  matrixHeapMemoryOverflowException
{
    public:
        std::string get_message() const override
        {return "Those matrix had already cleared, it's inaccessible.";}
};

void inline get_exception(Exception& error)
{
    std::cout <<"Error: "<< error.get_message() << std::endl;
}
template <typename T>
class matrix
{
public:
    explicit matrix(const std::string matrix)
        :Matrix(matrix)
    {
        matrix_rows_ = 0;
        matrix_columns_ = 0;
        check_input(matrix);
        find_string_matrix_size();
        create_matrix();
        parse_matrix_string(matrix);
        m_size_ = matrix_rows_*matrix_columns_;
    }
    
    matrix(const matrix& other)
        :   m_size_(other.m_size_),
        matrix_rows_(other.matrix_rows_),
        matrix_columns_(other.matrix_columns_)
    {
        data_ = new T[m_size_];
        memcpy(data_, other.data_, m_size_*sizeof(T));
    }
    matrix(matrix&& other) noexcept
        :   matrix_rows_(other.matrix_rows_),matrix_columns_(other.matrix_columns_)
    {
        m_size_ = matrix_columns_ * matrix_rows_;
        data_ = other.data_;

        other.m_size_ = 0;
        other.data_ = nullptr;
    }
    matrix& operator=(const matrix& other)
    {
        delete_matrix();
        m_size_ = other.m_size_;
        matrix_rows_ = other.matrix_rows_;
        matrix_columns_ = other.matrix_columns_;
        data_ = new T[m_size_];
        memcpy(data_, other.data_, m_size_*sizeof(T));
        return *this;
    }
    matrix& operator=(matrix&& other) noexcept
    {
        delete_matrix();
        m_size_ = other.m_size_;
        matrix_rows_ = other.matrix_rows_;
        matrix_columns_ = other.matrix_columns_;

        data_ = other.data_;
        other.data_ = nullptr;
        other.m_size_ = 0;
        return *this;
    }
    
    matrix(int const rows, int const columns, int const filling_number)
        : m_size_(rows*columns),matrix_rows_(rows), matrix_columns_(columns)
    {
        create_matrix();
        fill_matrix(filling_number);
    }

    matrix& operator+=(const T& other)
    {
        for(size_t i = 0; i < this->get_matrix_size(); i++)
        {
            if(other > std::numeric_limits<T>::max)
            {
                throw SumAndSubtractOverflowException("Ovewflow in operator+=()");
            }
            else if(other < std::numeric_limits<T>::min)
            {
                throw SumAndSubtractOverflowException("Underflow in operator+=()");
            }
            data_[i] += static_cast<T>(other);
        }
        return *this;
    }
    
    matrix& operator+=(const matrix& other)
    {
        if(check_matrices_sum_or_subtracting(other) && check_numbers_overflow(other))
        {
            for(unsigned int i = 0;i < other.m_size_;i++)
            {
                data_[i] += other.data_[i];
            }
        }
        return *this;
    }
    
    matrix& operator-=(const T& other)
    {
        for(size_t i = 0; i < this->get_matrix_size(); i++)
        {
            if(other > std::numeric_limits<T>::max())
            {
                throw SumAndSubtractOverflowException("Ovewflow in operator-=()");
            }
            else if(other < std::numeric_limits<T>::min())
            {
                throw SumAndSubtractOverflowException("Underflow in operator-=()");
            }
            data_[i] -= static_cast<T>(other);
        }
        return *this;
    }
    
    matrix& operator-=(const matrix& other)
    {
        if(check_matrices_sum_or_subtracting(other) && check_numbers_overflow(other))
        {
            for(unsigned int i = 0;i < m_size_;++i)
            {
                data_[i] -= other.data_[i];
            }
        }
        return *this;
    }
    
    matrix& operator*=(const T& other)
    {
        for(size_t i = 0; i < this->get_matrix_size(); i++)
        {
            if(other > std::numeric_limits<T>::max)
            {
                throw SumAndSubtractOverflowException("Ovewflow in operator*=()");
            }
            else if(other < std::numeric_limits<T>::min)
            {
                throw SumAndSubtractOverflowException("Underflow in operator*=()");
            }
            data_[i] *= static_cast<T>(other);
        }
        return *this;
    }
    
    matrix& operator*=(const matrix& other)
    {
        matrix otherMatrix = matrix(other);
        m_size_ = matrix_rows_ * otherMatrix.matrix_columns_;
        std::vector<T> res;
        if(check_matrices_multiplying(other))
        {
            for(int i = 0; i < matrix_rows_; i++)
            {
                T results = 0;
                for(int j = 0; j < otherMatrix.matrix_columns_; j++)
                {
                    for(int k = 0; k < matrix_columns_; k++)
                    {
                        results += (data_[k + i * matrix_columns_] * otherMatrix.data_[j + k * otherMatrix.matrix_columns_]);
                    }
                    res.push_back(results);
                }
            }
            matrix_columns_ = otherMatrix.matrix_columns_;
            data_ = new T[m_size_];
            for(int i = 0;i<matrix_rows_; i++)
            {
                for(int j = 0; j < matrix_columns_;j++)
                {
                    data_[j + i * matrix_columns_] = res[j + i * matrix_columns_];
                }
            }
        }
        return *this;
    }
    
    void create_matrix()
    {
        data_ = new T[matrix_rows_ * matrix_columns_];
    }
    
    void delete_matrix()
    {
        if(data_)
        {
            delete[] data_;
        }
    }
    
    void static check_input(std::string matrix)
    {
        unsigned int j =0;
        for(unsigned int i = 0; i < matrix.size();i++)
        {
            if (matrix[0] != '[')
            {
                throw  ParsingStringmatrixOverflowException("The wrong format of string massive. You forgot this '[': \n", matrix);
            }
            else if (matrix[matrix.size() - 1] != ']')
            {
                throw  ParsingStringmatrixOverflowException("The wrong format of string massive. You forgot this ']': \n", matrix);
            }
            else if (matrix[i] == ' ')
            {}
            else if (matrix[i] == ',')
            {}
            else if (matrix[i] == ';')
            {   
                j += i;
                if (i != j)
                {
                    throw  ParsingStringmatrixOverflowException("The amount of variables is incorrect. Add/Delete some.: \n", matrix);
                }           
            }
            else
                i++;
        }
    }
    
    int get_matrix_size() const
    {
        return matrix_columns_ * matrix_rows_;
    }
    
    int FindStrMatrixSize() const
    {
        int matrix_columns = 0;
        int matrix_rows  = 0;
        size_t current_row_columns = 0;
        for (char const ch : this->Matrix)
        {
            if (ch == '[') {}
            else if (ch == ']') {}
            else if (ch == ' ') {}
            else if (ch == ',')
            {
                current_row_columns++;
            }
            else if (ch == ';')
            {
                matrix_rows++;
                current_row_columns++;
                if (current_row_columns > matrix_columns)
                {
                    matrix_columns = current_row_columns;
                }
                current_row_columns = 0;
            }
        }
        matrix_rows++;
        current_row_columns++;
        if (current_row_columns > matrix_columns)
        {
            matrix_columns = current_row_columns;
        }
        return matrix_columns*matrix_rows;
    }
    
    void find_string_matrix_size()
    {
        size_t current_row_columns = 0;
        for (char const ch : this->Matrix)
        {
            if (ch == '[') {}
            else if (ch == ']') {}
            else if (ch == ' ') {}
            else if (ch == ',')
            {
                current_row_columns++;
            }
            else if (ch == ';')
            {
                matrix_rows_++;
                current_row_columns++;
                if (current_row_columns > matrix_columns_)
                {
                    matrix_columns_ = current_row_columns;
                }
                current_row_columns = 0;
            }
        }
        matrix_rows_++;
        current_row_columns++;
        if (current_row_columns > matrix_columns_)
        {
            matrix_columns_ = current_row_columns;
        }
    }

    template<typename T>
    T StringConvertToNumber(const std::string& str)
    {
        if constexpr (std::is_same_v<T, int>)
            return std::stoi(str);
        else if constexpr (std::is_same_v<T, float>)
            return std::stof(str);
        else if constexpr (std::is_same_v<T, double>)
            return std::stod(str);
    }
    
    void parse_matrix_string(const std::string& matrix)
    {
        int i = 0;
        int j = 0;
        std::string Token;
        for(char ch : matrix)
        {
            if (ch == '['){}
            else if (ch == ']')
            {
                data_[i * matrix_columns_ + j] = StringConvertToNumber<T>(Token);
                Token = "";
            }
            else if (ch == ' '){}
            else if (ch == ',')
            {
                data_[i * matrix_columns_ + j] = StringConvertToNumber<T>(Token);
                Token = "";
                j++;
            }
            else if (ch == ';')
            {
                data_[i * matrix_columns_ + j] = StringConvertToNumber<T>(Token);
                Token = "";
                i++;
                j = 0;                
            }
            else
                Token += ch;
        }    
    }
    
    void fill_matrix(int const filling_number)
    {
        if((filling_number < 0 && filling_number > std::numeric_limits<T>::min) || (filling_number > 0 && filling_number > std::numeric_limits<T>::max))
        {
            throw InvalidArgumentInmatrixException();
        }
        for (int i = 0; i < matrix_rows_ * matrix_columns_; i++)
            data_[i] = filling_number;    
    }
    
    void convert_to_string() const
    {
        if(data_ == nullptr)
        {
            throw OverflowError();
        }
        std::cout << '[';           
        for(int i = 0;i<matrix_rows_;i++)
        {
            for(int j = 0;j<matrix_columns_;j++)
            {
                std::cout  << data_[i * matrix_columns_ + j];
                if (j != matrix_columns_ - 1)
                    std::cout << ", ";
            }
            std::cout << " ;";
        }
        std::cout << ']' << std::endl;    
    }
    
    bool check_numbers_overflow(const matrix& other) const
    {
        for(int i = 0; i < other.get_matrix_size(); i++)
        {
            if(data_[i] >= 0 && other.data_[i] >= 0 && (data_[i] > std::numeric_limits<T>::max() - other.data_[i]))
            {
                throw SumAndSubtractOverflowException("You should decrease your variable because it's bigger than max size of int");
            }
            else if(data_[i] < 0 && other.data_[i] < 0 && data_[i] < (std::numeric_limits<T>::min() - other.data_[i]))
            {
                throw SumAndSubtractOverflowException("You should increase your variable because it's smaller than min size of int");
            }
            else {}
        }
        return true;
    }
    
    bool check_matrices_multiplying(const matrix& other)
    {
        if (this->matrix_columns_ != other.matrix_rows_)
        {
            throw MultiplyingOverflowException();
        }
        return true;    
    }
    
    bool check_matrices_sum_or_subtracting(const matrix& other)
    {
        if(matrix_columns_ != other.matrix_columns_ || matrix_rows_ != other.matrix_rows_)
        {
            throw SumAndSubtractOverflowException("To add or subtract matrices, you need the equality between Rows & Columns");
        }
        return true;    
    }
    
    int get_elements_sum() const
    {
        int result = 0;
        for(unsigned int i = 0; i < m_size_; i++)
        {
            result += data_[i];
        }
        return result;    
    }
    
    int GetMatrixDiagonalsSum() const
    {
        T sum = 0;
        for(int i = 0; i < matrix_rows_;i++)
        {
            for(int j = 0;j < matrix_columns_; j++)
            {
                if(i == j)
                {
                    sum += data_[i * matrix_columns_ + j];
                }
                else if(i + j == matrix_rows_ - 1)
                {
                    sum += data_[i * matrix_columns_ + j];
                }
            }
        }
        return sum;    
    }
    
    friend bool operator==(const matrix& left, const matrix& right)
    {
        return left.get_elements_sum() == right.get_elements_sum();
    }
    
    friend bool operator==(const char* left, const matrix& right)
    {
        return matrix(left) == right;
    }
    
    friend bool operator==(const matrix& left, const char* right)
    {
        return left == matrix(right);
    }
    
    friend bool operator!=(const matrix& left, const matrix& right)
    {
        return !(left == right);
    }
    
    friend bool operator!=(const char* left, const matrix& right)
    {
        auto const left_matrix = matrix(left);
        return !(left == right);
    }
    
    friend bool operator!=(const matrix& left, const char* right)
    {
        auto const right_matrix = matrix(right);
        return !(left == right);
    }
    
    friend matrix operator+(matrix left, const T& right)
    {
        return left += right;
    }
    
    friend matrix operator+(matrix left, const matrix& right)
    {
        return left += right;
    }
    
    friend matrix operator-(matrix left, const T& right)
    {
        return  left -= right;
    }
    
    friend matrix operator-(matrix left, const matrix& right)
    {
        return left -= right;
    }
    
    friend matrix operator*(matrix left, const matrix& right)
    {
        return left *= right;
    }

    friend matrix operator*(matrix left, const T& right)
    {
        return left *= right;
    }
    
    friend bool operator>(const matrix& left, const char* right)
    {
        return left > matrix(right);
    }
    
    friend bool operator>(const char* left, const matrix& right)
    {
        return matrix(left) > right;
    }
    
    friend bool operator>(const matrix& left, const matrix& right)
    {
        return left.get_elements_sum() > right.get_elements_sum();
    }
    
    friend bool operator<(const matrix& left, const matrix& right)
    {
        return right > left;
    }
    
    friend bool operator<(const char* left, const matrix& right)
    {
        auto const other_matrix = matrix(left);
        return right > other_matrix;
    }
    
    friend bool operator<(const matrix& left, const char* right)
    {
        auto const other_matrix = matrix(right);
        return other_matrix > left;
    }
    
    friend bool operator<=(const matrix& left, const char* right)
    {
        auto other_matrix = matrix(right);
        return !(left > right);
    }
    
    friend bool operator<=(const char* left, const matrix& right)
    {
        auto other_matrix = matrix(left);
        return !(left > right);
    }
    
    friend bool operator<=(const matrix& left, const matrix& right)
    {
        return !(right > left);
    }
    
    friend bool operator>=(const matrix& left, const char* right)
    {
        auto right_matrix = matrix(right);
        return !(left < right);
    }
    
    friend bool operator>=(const char* left, const matrix& right)
    {
        auto const left_matrix = matrix(left);
        return !(left < right);
    }
    
    friend bool operator>=(const matrix& left, const matrix& right)
    {
        return !(left < right);
    }
    
    friend  std::ostream& operator<<(std::ostream& output,const matrix& data)
    {
        output << '[';
        for(unsigned int i = 0; i < data.m_size_;i++)
        {
            output << data.data_[i] << ',';
        }
        output << ']';
        return output;
    }
    
    ~matrix()
    {
        delete_matrix();
    }
    private:
    std::string Matrix;
    unsigned int m_size_;
    int matrix_rows_, matrix_columns_;
    T* data_;
};