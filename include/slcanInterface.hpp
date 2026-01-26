#include "CanManager.hpp"
#include "NoBitRateManagementException.hpp"
#include <linux/can.h>
#include <mutex>
#include <queue>
#include <sys/socket.h>
#include <thread>

const std::string defaultSlcanIfName{
    "slcan0"}; /**< Default Interface name for slcan */

/**
 * @brief slcan interface class
 */
class slcanInterface : public CanManager {
  private:
    const std::string ifname{defaultSlcanIfName}; /**< Interface name - to be
    used in real implementation */
    const std::uint32_t sock;
    const std::jthread th;
    bool continueReception{true};
    std::queue<can_frame> receiveQueue;
    mutable std::mutex queueMutex;

  private:
    void receiveLoop();

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
     * @brief Wait to receive a CAN message
     *
     * @param msg
     * @return true if a message was received and stored in msg
     * @return false if no message was received
     */
    [[nodiscard]] virtual bool Receive(CanMessage& msg) override;
};
