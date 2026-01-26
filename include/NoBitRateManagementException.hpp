#ifndef NOBITRATEMANAGEMENTEXCEPTION_HPP
#define NOBITRATEMANAGEMENTEXCEPTION_HPP

#include <stdexcept>

/**
 * @brief Thrown if the interface does not support BitRate management
 */
class NoBitRateManagementException : public std::runtime_error {
  public:
    explicit NoBitRateManagementException()
        : std::runtime_error(
              "BitRate management not supported by this interface") {}
};

#endif // NOBITRATEMANAGEMENTEXCEPTION_HPP
