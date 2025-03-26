#include <iostream>
#include <fstream>

#include "matrix.hpp"
#include "threadsPool.hpp"

const std::string Matrix::fileFormat = ".mtx";
std::shared_ptr<ThreadsPool> Matrix::threadPool = nullptr;

void rowToColumnMethod(const std::vector<int>& vectorRow, const Matrix& matrix, std::vector<int>& result) {
	result = std::vector<int>(matrix.cols, 0);

	for (size_t i = 0; i < matrix.cols; ++i) {
		int product = 0;
		for (size_t j = 0; j < vectorRow.size(); ++j) {
			product += vectorRow[j] * matrix[j][i];
		}

		result[i] = product;
	}
}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
	data = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
}

Matrix::Matrix(size_t rows, size_t cols, const std::string& FILENAME) : Matrix(rows, cols) {
	readFromFile(FILENAME);
}

size_t Matrix::getRows() const {
	return rows;
}

size_t Matrix::getCols() const {
	return cols;
}

void Matrix::fillMatrix() {
	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < cols; ++col) {
			data[row][col] = std::rand() % 100;
		}
	}
}

void Matrix::writeToFile(const std::string& FILENAME) const {
	std::ofstream file(FILENAME + fileFormat);

	if (!file.is_open()) {
		std::cerr << "Unable to open file" << std::endl;
		return;
	}

	file << *this;
	file.close();

	if (!file.fail()) {
		std::cout << "Data has been successfully saved to the file " << FILENAME + fileFormat << std::endl;
	} else {
		std::cerr << "Failed to save data" << std::endl;
	}

}

void Matrix::readFromFile(const std::string& FILENAME) {
	std::ifstream file(FILENAME + fileFormat);
	
	if (!file.is_open()) {
		std::cerr << "Unable to open file" << std::endl;
		return;
	}

	file >> *this;
	file.close();

	if (!file.fail()) {
		std::cout << "Data has been successfully loaded from the file " << FILENAME + fileFormat << std::endl;
	} else {
		std::cerr << "Failed to load data" << std::endl;
	}
}

bool Matrix::isAgree(const Matrix& right) const {
	return rows == right.cols;
}

std::vector<int>& Matrix::operator[](const size_t index) {
	return data[index];
}

const std::vector<int>& Matrix::operator[](const size_t index) const {
	return data[index];
}

void Matrix::createThreads(unsigned int numThreads) {
	threadPool = std::make_shared<ThreadsPool>(numThreads);
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
	for (size_t row = 0; row < matrix.rows; ++row) {
		for (size_t col = 0; col < matrix.cols; ++col) {
			os << matrix[row][col] << '\t';
		}
		os << std::endl;

	}

	return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
	for (size_t row = 0; row < matrix.rows; ++row) {
		for (size_t col = 0; col < matrix.cols; ++col) {
			is >> matrix[row][col];
		}
	}

	return is;
}

const Matrix operator*(const Matrix left, const Matrix right) {
	bool isAgree = left.isAgree(right);

	if (!Matrix::threadPool) {
		std::cerr << "Error: threadPool is not initialized!" << std::endl;
		std::terminate();
	}
	if (isAgree) {
		Matrix result(left.rows, right.cols);

		std::vector<std::future<void>> futures(result.rows);

		for (size_t i = 0; i < result.rows; ++i) {
			futures[i] = Matrix::threadPool->submit(rowToColumnMethod, std::ref(left[i]), std::ref(right), std::ref(result[i]));
		}

		for (auto& future : futures) {
			future.get();  // waiting for everyone at once
		}

		return result;
	} else {
		std::cerr << "Matrix dimensions do not agree for multiplication" << std::endl;
	}
}