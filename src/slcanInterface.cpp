#include "slcanInterface.hpp"

/**
 * @brief Emit a CAN message
 * @param msg
 */
void slcanInterface::Emit(CanMessage& msg) { return; };

/**
 * @brief Wait to receive a CAN message
 *
 * @param msg
 * @return true if a message was received and stored in msg
 * @return false if no message was received
 */
[[nodiscard]] bool Receive(CanMessage& msg) { return false; };
