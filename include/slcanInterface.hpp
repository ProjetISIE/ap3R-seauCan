#include "CanManager.hpp"
#include <stdexcept>

const std::string defaultSlcanIfName{
    "slcan0"}; /**< Default Interface name for slcan */

/**
 * @brief Exception thrown when bit rate management is not supported
 */
class NoBitRateManagementException : public std::runtime_error {
  public:
    explicit NoBitRateManagementException()
        : std::runtime_error("BitRate non manageable with this interface") {}
};

/**
 * @brief slcan interface class
 */
class slcanInterface : public CanManager {
  private:
    const std::string ifname{defaultSlcanIfName}; /**< Interface name - to be
    used in real implementation */
  public:
    slcanInterface(); /**< Default constructor - to be implemented */
    explicit slcanInterface(std::string ifname_);   /**< Constructor taking
      interface name   (delegated ctor) */
    slcanInterface(const slcanInterface&) = delete; /**< non -copyable */
    slcanInterface(slcanInterface&&) = delete;      /**< non -movable */
    slcanInterface&
    operator=(const slcanInterface&) = delete;            /**< non -copyable */
    slcanInterface& operator=(slcanInterface&&) = delete; /**< non -movable */
    virtual ~slcanInterface(); /** to be implemented */
    explicit slcanInterface(bitRate_t br) noexcept = delete; /**< Deleted
    constructor - no bitRate constructor */
    /**
     * @brief Set bit rate (not supported)
     * @param br
     * @throws NoBitRateManagementException
     */
    virtual void setbitRate([[maybe_unused]] bitRate_t br) override {
        throw NoBitRateManagementException{};
    }
    /**
     * @brief Get bit rate (not supported)
     * @return bitRate_t
     * @throws NoBitRateManagementException
     */
    virtual bitRate_t getbitRate() override {
        throw NoBitRateManagementException{};
    }
    /**
     * @brief Emit a CAN message
     * @param msg
     */
    virtual void Emit(CanMessage& msg) override;
    /**
     * @brief
     *
     * @param msg
     * @return true if a message was received and stored in msg
     * @return false if no message was received
     */
    [[nodiscard]] virtual bool Receive(CanMessage& msg) override;
};
