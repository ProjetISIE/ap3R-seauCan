#ifndef NOBITRATEMANAGEMENTEXCEPTION_HPP
#define NOBITRATEMANAGEMENTEXCEPTION_HPP

#include <stdexcept>

/**
 * @brief Exception thrown when bit rate management is not supported
 */
class NoBitRateManagementException : public std::runtime_error {
  public:
    explicit NoBitRateManagementException()
        : std::runtime_error("BitRate non manageable with this interface") {}
};

#endif // NOBITRATEMANAGEMENTEXCEPTION_HPP
