#ifndef _STR2ID_H
#define _STR2ID_H
#include <stdint.h>
#include <string>
const uint64_t kuint64max = ((uint64_t) 0xFFFFFFFFFFFFFFFF);

const uint32_t kFingerPrintSeed = 19861202;
uint64_t MurmurHash64A(const void* key, int len, uint32_t seed);
uint64_t Fingerprint(const std::string& str);
uint64_t ComputeID(const std::string& str);

#endif //_STR2ID_H
