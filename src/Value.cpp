//
// Copyright (c) 2017 
// Author: 3grbn3@gmail.com (Gerben)
//
// A wrapper for primitive types and char arrays
//

#include "Value.h"
#include <stdlib.h>
#include <cstring>

//#include <iostream>

namespace gve {

Value::Value() : value_(NULL), type_(t_null){
//  std::cout << "construct " << this << '\n';
};

Value::Value(const Value& other) :
  type_(other.type_),
  size_(other.size_)
{
//  std::cout << "copy " << &other << ' ' << this << '\n';
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
}

Value::Value(Value&& other) :
  type_(other.type_),
  size_(other.size_),
  value_(other.value_)
{
//  std::cout << "move " << other << ' ' << this << '\n';
  other.value_ = nullptr;
}

Value::Value(std::string& str) : type_(t_cstring){
//  std::cout << "construct string " << this << '\n';
  const char* value = str.c_str();
  int size = strlen(value) + 1;
  value_ = allocate(size);
  std::memcpy(value_, value, size);
}


Value::~Value(){
//  std::cout << "destruct:" << this << '\n';
  deallocate(value_);
  value_ = nullptr;
}

Value& Value::operator=(const Value&& other){
//  std::cout << "move assigned\n";
  type_ = other.type_;
  size_ = other.size_;
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
  return *this;
}

Value& Value::operator=(const Value& other){
//  std::cout << "copy assigned\n";
  type_ = other.type_;
  size_ = other.size_;
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
  return *this;
}


void Value::operator=(bool value){
    type_ = t_bool;
    set(value);
}

void Value::operator=(char value){
    type_ = t_char;
    set(value);
}

void Value::operator=(wchar_t value){
    type_ = t_wchar;
    set(value);
}

void Value::operator=(short value){
    type_ = t_int;
    set(value);
}

void Value::operator=(int value){
    type_ = t_int;
    set(value);
}

void Value::operator=(long value){
    type_ = t_long;
    set(value);
}

void Value::operator=(long long value){
    type_ = t_longlong;
    set(value);
}

void Value::operator=(float value){
    type_ = t_float;
    set(value);
}

void Value::operator=(double value){
    type_ = t_double;
    set(value);
}

void Value::operator=(long double value){
    type_ = t_longdouble;
    set(value);
}

void Value::operator=(const char* value){
  type_ = t_cstring;
  int size = strlen(value) + 1;
  value_ = allocate(size);
  std::memcpy(value_, value, size);
}

void Value::operator=(std::string& str){
  type_ = t_cstring;
  const char* value = str.c_str();
  int size = strlen(value) + 1;
  value_ = allocate(size);
  std::memcpy(value_, value, size);
}
/*
Value& Value::operator+=(int value){
    int v = *(int*)value_;
    v += value;
    set(v);
}
*/
Value::operator bool() const{
  if (type_ == t_cstring)
    return (bool)atoi((char*)value_);
  return value<bool>();
}

Value::operator char() const{
  return value<char>();
}

Value::operator wchar_t() const{
  return value<wchar_t>();
}

Value::operator short() const{
  return value<short>();
}

Value::operator int() const{
  return value<int>();
}

Value::operator long() const{
  return value<long>();
}

Value::operator long long() const{
  return value<long long>();
}

Value::operator float() const{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<float>();
}

Value::operator double() const{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<double>();
}

Value::operator long double() const{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<long double>();
}

Value::operator const char* () const{
  switch(type_){
    case t_cstring: return (const char*) value_;
    default:        return "";
  }
}

template<typename T>
T Value::value() const{
    switch(type_){
      case t_bool:       return *(bool*)        value_;
      case t_char:       return *(char*)        value_;
      case t_short:      return *(short*)       value_;
      case t_int:        return *(int*)         value_;
      case t_long:       return *(long*)        value_;
      case t_longlong:   return *(long long*)   value_;
      case t_float:      return *(float*)       value_;
      case t_double:     return *(double*)      value_;
      case t_longdouble: return *(long double*) value_;
//          case t_cstring:    return (char*)         value_;
      default:           return 0;
    }
}

template <typename T>
void Value::set(T value) {
  size_ = sizeof(T);
  value_ = allocate(size_);
  std::memcpy(value_, &value, size_);
}

void* Value::allocate(size_t size) {
  return malloc(size);
}

void Value::deallocate(void *ptr) {
  free(ptr);
}

std::ostream& operator<< (std::ostream& stream, const Value& value){
  switch (value.type_){
    case Value::t_bool:       stream << *(bool*)        value.value_; break;
    case Value::t_char:       stream << *(char*)        value.value_; break;
    case Value::t_wchar:      stream << *(wchar_t*)     value.value_; break;
    case Value::t_short:      stream << *(short*)       value.value_; break;
    case Value::t_int:        stream << *(int*)         value.value_; break;
    case Value::t_long:       stream << *(long*)        value.value_; break;
    case Value::t_longlong:   stream << *(long long*)   value.value_; break;
    case Value::t_float:      stream << *(float*)       value.value_; break;
    case Value::t_double:     stream << *(double*)      value.value_; break;
    case Value::t_longdouble: stream << *(long double*) value.value_; break;
    case Value::t_cstring:    stream <<  (char*)        value.value_; break;
  }
  return stream;
}

}
