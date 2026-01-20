#ifndef NOBITRATEMANAGEMENTEXCEPTION_HPP

#include <stdexcept>

/**
 * @brief Lancée si le débit binaire passé n’est pas supporté
 */
class NoBitRateManagementException : public std::runtime_error {
  public:
    explicit NoBitRateManagementException()
        : std::runtime_error("BitRate non supporté par cette interface") {}
};

#endif // NOBITRATEMANAGEMENTEXCEPTION_HPP
