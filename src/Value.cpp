#include "Value.h"
#include <stdlib.h>
#include <cstring>

using namespace ootb;

Value::Value() : value_(NULL), type_(t_null)
{
};

Value::Value(const Value& other)
:
  type_(other.type_),
  size_(other.size_)
{
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
}

Value::Value(Value&& other)
:
  type_(other.type_),
  size_(other.size_),
  value_(other.value_)
{
  other.value_ = nullptr;
}

/*
Value::Value(std::string& str) : type_(t_cstring){
//  std::cout << "construct string " << this << '\n';
  const char* value = str.c_str();
  int size = strlen(value) + 1;
  value_ = allocate(size);
  std::memcpy(value_, value, size);
}
*/

Value::~Value()
{
  deallocate(value_);
  value_ = nullptr;
}

Value& Value::operator=(const Value&& other)
{
  type_ = other.type_;
  size_ = other.size_;
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
  return *this;
}

Value& Value::operator=(const Value& other)
{
  type_ = other.type_;
  size_ = other.size_;
  value_ = allocate(size_);
  std::memcpy(value_, other.value_, size_);
  return *this;
}


void Value::operator=(bool value)
{
  type_ = t_bool;
  set(value);
}

void Value::operator=(char value)
{
  type_ = t_char;
  set(value);
}

void Value::operator=(wchar_t value)
{
  type_ = t_wchar;
  set(value);
}

void Value::operator=(short value)
{
  type_ = t_int;
  set(value);
}

void Value::operator=(int value)
{
  type_ = t_int;
  set(value);
}

void Value::operator=(long value)
{
  type_ = t_long;
  set(value);
}

void Value::operator=(long long value)
{
  type_ = t_longlong;
  set(value);
}

void Value::operator=(float value)
{
  type_ = t_float;
  set(value);
}

void Value::operator=(double value)
{
  type_ = t_double;
  set(value);
}

void Value::operator=(long double value)
{
  type_ = t_longdouble;
  set(value);
}

void Value::operator=(const char* value)
{
  type_ = t_cstring;
  int size = strlen(value) + 1;
  value_ = allocate(size);
  std::memcpy(value_, value, size);
}

void Value::operator=(String& value)
{
  type_ = t_cstring;
  int size = value.length() + 1;
  value_ = allocate(size);
  const char* cstr = value.c_str();
  std::memcpy(value_, cstr, size);
}

Value::operator bool() const
{
  if (type_ == t_cstring)
  {
    if (((char*)value_)[0] == 't') return true;
    if (((char*)value_)[0] == 'f') return false;
    return (bool)atoi((char*)value_);
  }
  return value<bool>();
}

Value::operator char() const
{
  return value<char>();
}

Value::operator wchar_t() const
{
  return value<wchar_t>();
}

Value::operator short() const
{
  return value<short>();
}

Value::operator int() const
{
  return value<int>();
}

Value::operator long() const
{
  return value<long>();
}

Value::operator long long() const
{
  return value<long long>();
}

Value::operator float() const
{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<float>();
}

Value::operator double() const
{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<double>();
}

Value::operator long double() const
{
  if (type_ == t_cstring)
    return atof((char*)value_);
  return value<long double>();
}

Value::operator const char* () const
{
  switch(type_){
    case t_cstring: return (const char*) value_;
    default:        return "";
  }
}

Value::operator String () const
{
  switch(type_){
/*
    case t_bool:       return String((bool*)value_);
    case t_char:       return String((char*)value_);
    case t_short:      return String((short*)value_);
    case t_int:        return String((int*)value_);
    case t_long:       return String((long*)value_);
    case t_longlong:   return String((long long*)value_);
    case t_float:      return String((float*)value_);
    case t_double:     return String((double*)value_);
    case t_longdouble:  v = *(long double*)value_; return String(value());
    */
    case t_cstring:    return String((char*)value_);
    default:        return String();
  }
}

template<typename T>
T Value::value() const
{
  switch(type_)
  {
    case t_bool:       return *(bool*)        value_;
    case t_char:       return *(char*)        value_;
    case t_short:      return *(short*)       value_;
    case t_int:        return *(int*)         value_;
    case t_long:       return *(long*)        value_;
    case t_longlong:   return *(long long*)   value_;
    case t_float:      return *(float*)       value_;
    case t_double:     return *(double*)      value_;
    case t_longdouble: return *(long double*) value_;
    default:           return 0;
  }
}

template <typename T>
void Value::set(T value)
{
  size_ = sizeof(T);
  value_ = allocate(size_);
  std::memcpy(value_, &value, size_);
}

void* Value::allocate(size_t size)
{
  return malloc(size);
}

void Value::deallocate(void *ptr)
{
  free(ptr);
}
