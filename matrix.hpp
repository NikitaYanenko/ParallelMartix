#ifndef MATRIX_H
#define MATRIX_H

#include "threadsPool.hpp"

class Matrix {
private:
	static const std::string fileFormat;

	static std::shared_ptr<ThreadsPool> threadPool;
	
	size_t rows;
	size_t cols;
	std::vector<std::vector<int>> data;

public:
	Matrix(size_t rows, size_t cols);
	Matrix(size_t rows, size_t cols, const std::string& FILENAME);
	~Matrix() = default;

	[[nodiscard]] size_t getRows() const;
	[[nodiscard]] size_t getCols() const;

	void fillMatrix();

	void writeToFile(const std::string& FILENAME) const;
	void readFromFile(const std::string& FILENAME);

	// for write/read in/from a file
	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
	friend std::istream& operator>>(std::istream& is, Matrix& matrix);

	// parallel matrix multiplication
	friend const Matrix operator*(const Matrix left, const Matrix right);
	friend void rowToColumnMethod(const std::vector<int>& vectorRow, const Matrix& matrix, std::vector<int>& result);

	// returns the flag whether multiplication is possible 
	[[nodiscard]] bool isAgree(const Matrix& right) const;

	// access to elements (2 variants). Modification and only-read (const)
	[[nodiscard]] std::vector<int>& operator[](const size_t index);
	[[nodiscard]] const std::vector<int>& operator[](const size_t index) const;

	static void createThreads(unsigned int numThreads);
};

#endif