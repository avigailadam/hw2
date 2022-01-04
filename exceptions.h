
#ifndef HW1_EXCEPTIONS_H
#define HW1_EXCEPTIONS_H
#include <memory>
#include <exception>


class AlreadyExist : public std::exception  {
public:

};
class NotExist : public std::exception {
public:

};
class InvalidInput : std::exception {
public:

};

class OutOfRange: std::exception {
public:

};



#endif //HW1_EXCEPTIONS_H
