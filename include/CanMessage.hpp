#include <array>
#include <cstdint>
#include <string>

constexpr uint8_t MAX_DLC = 8;

class CanMessage {

  public:

    enum class canType {
        DATA,
        REMOTE,
    };

    enum class canFormat {
        STD,
        EXT,
    };

  private:
    std::array<uint8_t, MAX_DLC> _Data;

  public:
    /**
     * @brief Checks if CAN frame is valid (ID OK with format, DLC value…)
     * @return true if frame is valid, false otherwise
     */
    [[nodiscard]] bool isMessageValid() const noexcept;

    /**
     * @brief  Creates a std::string from frame characteristics
     * @return std::string
     *
     * Std::string format : format;type;id;dlc;data;validity
     *   format : "STD" or "EXT"
     *   type : "DATA" or "RMT"
     *   id : 3 (std frame) or 8 (ext frame) hexadecimal digits representig the
           frame ID
     *   dlc : dlc value (decimal) data : each data byte is represented
     *     as a 2 digit hex value
     *     - No separators between data bytes
     *     Rq : if dlc=0 or if the frame is a remote one this field is empty
     *   Validity : "TRUE" if the frame is valid, "FALSE" if not
     * */
    [[nodiscard]] std::string toString() const noexcept;
};
