#include <ctime> 

#include "matrix.hpp"
#include "timer.hpp"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // if used max threads;
    unsigned int numThreads = std::thread::hardware_concurrency() - 1;
    
    const std::string firstMatrixFile = "FirstMatrix10000x10000";
    const std::string secondMatrixFile = "SecondMatrix10000x10000";
    const std::string resultDotFile = "ResultDot";

    Matrix::createThreads(numThreads);

    Matrix firstMatrix(10000, 10000, firstMatrixFile);
    Matrix secondMatrix(10000, 10000, secondMatrixFile);

    Timer* time = new Timer();

    Matrix resultDot = firstMatrix * secondMatrix;

    delete time;

    resultDot.writeToFile(resultDotFile);

    return 0;
}
