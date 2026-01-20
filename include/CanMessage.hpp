#include <array>
#include <cstdint>
#include <string>

#ifndef __CAN_MESSAGE_HPP__
#define __CAN_MESSAGE_HPP__

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
    uint32_t Id;
    canType Type;
    canFormat Format;
    uint8_t Dlc;
    std::array<uint8_t, MAX_DLC> _Data;

  public:
    // default Constructeur
    CanMessage() = default;
    CanMessage(const CanMessage&) = default;
    CanMessage(CanMessage&&) = default;
    CanMessage& operator=(const CanMessage&) = default;
    CanMessage& operator=(CanMessage&&) = default;

    // default destructor
    virtual ~CanMessage() = default;

    // Constructor with parameter
    CanMessage(canType type, canFormat format) : Type{type}, Format{format} {};
    CanMessage(uint32_t id, canType type, canFormat format)
        : Id{id}, Type{type}, Format{format} {};
    CanMessage(uint32_t id, canType type, canFormat format, uint8_t dlc)
        : Id{id}, Type{type}, Format{format}, Dlc{dlc} {};

    // GET+SET : Id
    [[nodiscard]] uint32_t getId() noexcept { return this->Id; };
    void setId(uint32_t id) noexcept { this->Id = id; };

    // GET+SET Type
    [[nodiscard]] canType getType() noexcept { return this->Type; };
    void setType(canType type) noexcept { this->Type = type; };

    // GET+SET Format
    [[nodiscard]] canFormat getFormat() noexcept { return this->Format; };
    void setFormat(canFormat format) noexcept { this->Format = format; };

    // GET+SET Dlc
    [[nodiscard]] uint8_t getDlc() noexcept { return this->Dlc; };
    void setDlc(uint8_t dlc) noexcept { this->Dlc = dlc; };

    // Data
    [[nodiscard]] std::array<uint8_t, MAX_DLC>& Data() noexcept {
        return this->_Data;
    };

    /**
     * @brief Checks if CAN frame is valid (ID OK with format, DLC value…)
     * @return true if frame is valid, false otherwise
     */
    [[nodiscard]] bool isMessageValid() noexcept;

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
    [[nodiscard]] std::string toString() noexcept;
};

#endif
