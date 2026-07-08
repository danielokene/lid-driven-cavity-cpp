#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>

#include "../src/matrix.h"

class CSVWriter
{
public:
    // Writes any Matrix object to a CSV file
    static void writeMatrix(const Matrix& matrix,
                            const std::string& filename);
};

#endif