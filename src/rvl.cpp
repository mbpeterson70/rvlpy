#include <stdio.h>
#include "rvl.h"
#include <pybind11/pybind11.h>

// namespace py = pybind11;

void HelloWorld() {
    printf("hello world.\n");
}

py::array_t<int> cpArray(py::array_t<int> fromArr, uint len) {
//    int* toArr = (int*) malloc(len * sizeof(int));
    py::buffer_info fromBuf = fromArr.request();
    int *fromPtr = static_cast<int *>(fromBuf.ptr);

    auto toArr = py::array_t<int>(len);
    py::buffer_info toBuf = toArr.request();
    int *toPtr = static_cast<int *>(toBuf.ptr);

    for (uint i=0; i < len; ++i) {
        toPtr[i] = fromPtr[i];
    }

    return toArr;
}

float somethingEasier(py::array_t<float> arr) {
    printf("%g", (float) arr.at(0));
    return (float) arr.at(0);
}

// https://www.microsoft.com/en-us/research/uploads/prod/2018/09/p100-wilson.pdf

int *buffer, *pBuffer, word, nibblesWritten;

// void EncodeVLE(int value)
// {
//     do
//     {
//         int nibble = value & 0x7; // lower 3 bits
//         if (value >>= 3) nibble |= 0x8; // more to come
//         word <<= 4;
//         word |= nibble;
//         if (++nibblesWritten == 8) // output word
//         {
//             *pBuffer++ = word;
//             nibblesWritten = 0;
//             word = 0;
//         }
//     } while (value);
// }


int DecodeVLE()
{
    unsigned int nibble;
    int value = 0, bits = 29;
    do
    {
        if (!nibblesWritten)
        {
            word = *pBuffer++; // load word
            nibblesWritten = 8;
        }
        nibble = word & 0xf0000000;
        value |= (nibble << 1) >> bits;
        word <<= 4;
        nibblesWritten--;
        bits -= 3;
    } while (nibble & 0x80000000);
    return value;
}


// int CompressRVL(short* input, char* output, int numPixels)
// {
//     buffer = pBuffer = (int*)output;
//     nibblesWritten = 0;
//     short *end = input + numPixels;
//     short previous = 0;
//     while (input != end)
//     {
//         int zeros = 0, nonzeros = 0;
//         for (; (input != end) && !*input; input++, zeros++);
//         EncodeVLE(zeros); // number of zeros
//         for (short* p = input; (p != end) && *p++; nonzeros++);
//         EncodeVLE(nonzeros); // number of nonzeros
//         for (int i = 0; i < nonzeros; i++)
//         {
//             short current = *input++;
//             int delta = current - previous;
//             int positive = (delta << 1) ^ (delta >> 31);
//             EncodeVLE(positive); // nonzero value
//             previous = current;
//         }
//     }
//     if (nibblesWritten) // last few values
//         *pBuffer++ = word << 4 * (8 - nibblesWritten);
//     return int((char*)pBuffer - (char*)buffer); // num bytes
// }


py::array_t<short> DecompressRVL(py::array_t<char> inputPy, int numPixels)
{
    py::buffer_info inputBuf = inputPy.request();
    char *input = static_cast<char *>(inputBuf.ptr);

    auto outputPy = py::array_t<short>(numPixels);
    py::buffer_info outputBuf = outputPy.request();
    short *output = static_cast<short *>(outputBuf.ptr);
    
    buffer = pBuffer = (int*)input;
    nibblesWritten = 0;
    short current, previous = 0;
    int numPixelsToDecode = numPixels;
    while (numPixelsToDecode)
    {
        int zeros = DecodeVLE(); // number of zeros
        // printf("zeros: %d", zeros);
        // return outputPy;
        numPixelsToDecode -= zeros;
        for (; zeros; zeros--)
            *output++ = 0;
        int nonzeros = DecodeVLE(); // number of nonzeros
        numPixelsToDecode -= nonzeros;
        for (; nonzeros; nonzeros--)
        {
            int positive = DecodeVLE(); // nonzero value
            int delta = (positive >> 1) ^ -(positive & 1);
            current = previous + delta;
            *output++ = current;
            previous = current;
        }
    }

    return outputPy;
}