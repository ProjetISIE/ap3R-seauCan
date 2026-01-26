#include <array>
#include <cstdint>
#include <string>

#ifndef CAN_MESSAGE_HPP
#define CAN_MESSAGE_HPP

constexpr uint8_t MAX_DLC{8};

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
    uint32_t Id{0};
    canType Type{canType::DATA};
    canFormat Format{canFormat::STD};
    uint8_t Dlc{0};
    std::array<uint8_t, MAX_DLC> _Data{0};

  public:
    CanMessage() = default;
    CanMessage(const CanMessage&) = default;
    CanMessage(CanMessage&&) = default;
    CanMessage& operator=(const CanMessage&) = default;
    CanMessage& operator=(CanMessage&&) = default;
    virtual ~CanMessage() = default;

    CanMessage(canType type_, canFormat format_)
        : Type{type_}, Format{format_} {}
    CanMessage(uint32_t id_, canType type_, canFormat format_)
        : Id{id_}, Type{type_}, Format{format_} {}
    CanMessage(uint32_t id_, canType type_, canFormat format_, uint8_t dlc_)
        : Id{id_}, Type{type_}, Format{format_}, Dlc{dlc_} {}

    [[nodiscard]] uint32_t getId() const noexcept { return this->Id; }
    void setId(uint32_t id_) noexcept { this->Id = id_; }

    [[nodiscard]] canType getType() const noexcept { return this->Type; }
    void setType(canType type_) noexcept { this->Type = type_; }

    [[nodiscard]] canFormat getFormat() const noexcept { return this->Format; }
    void setFormat(canFormat format_) noexcept { this->Format = format_; }

    [[nodiscard]] uint8_t getDlc() const noexcept { return this->Dlc; }
    void setDlc(uint8_t dlc_) noexcept { this->Dlc = dlc_; }

    [[nodiscard]] std::array<uint8_t, MAX_DLC>& Data() noexcept {
        return this->_Data;
    }
    [[nodiscard]] const std::array<uint8_t, MAX_DLC>& Data() const noexcept {
        return this->_Data;
    }

    /**
     * @brief Checks if CAN frame is valid (ID OK with format, DLC value…)
     * @return true if frame valid, false otherwise
     */
    [[nodiscard]] bool isMessageValid() const noexcept;

    /**
     * @brief Creates std::string from frame characteristics
     * @return std::string
     * Format: format;type;id;dlc;data;validity
     *   format: "STD" or "EXT"
     *   type: "DATA" or "RMT"
     *   id: 3 (std) or 8 (ext) hex digits
     *   dlc: decimal value
     *   data: 2 hex digits per byte, no separators (empty if dlc=0 or remote)
     *   validity: "TRUE" or "FALSE"
     */
    [[nodiscard]] std::string toString() const noexcept;
};

#endif // CAN_MESSAGE_HPP
