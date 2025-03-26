#include <ctime> 

#include "matrix.hpp"
#include "timer.hpp"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // if used max threads;
    unsigned int numThreads = std::thread::hardware_concurrency() - 1;
    
    const std::string firstMatrixFile = "FirstMatrix1000x1000";
    const std::string secondMatrixFile = "SecondMatrix1000x1000";
    const std::string resultDotFile = "ResultDot";

    Matrix::createThreads(numThreads);

    Matrix firstMatrix(1000, 1000, firstMatrixFile);
    Matrix secondMatrix(1000, 1000, secondMatrixFile);

    Timer time;

    Matrix resultDot = firstMatrix * secondMatrix;

    resultDot.writeToFile(resultDotFile);

    return 0;
}