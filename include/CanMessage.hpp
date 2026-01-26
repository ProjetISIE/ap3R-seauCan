#include <array>
#include <cstdint>
#include <string>

#ifndef CAN_MESSAGE_HPP
#define CAN_MESSAGE_HPP

constexpr uint8_t MAX_DLC{8};

class CanMessage {
  public:
    /**
     * @brief Type of CAN message
     */
    enum class canType {
        DATA,  /**< Data Frame */
        REMOTE, /**< Remote Transmission Request (RTR) Frame */
    };

    /**
     * @brief Format of CAN ID
     */
    enum class canFormat {
        STD, /**< Standard Frame (11-bit ID) */
        EXT, /**< Extended Frame (29-bit ID) */
    };

  private:
    uint32_t Id{0};
    canType Type{canType::DATA};
    canFormat Format{canFormat::STD};
    uint8_t Dlc{0};
    std::array<uint8_t, MAX_DLC> _Data{0};

  public:
    /**
     * @brief Default Constructor
     * Initializes an empty standard data frame with ID 0.
     */
    CanMessage() = default;
    
    /**
     * @brief Copy Constructor
     * @param other Message to copy
     */
    CanMessage(const CanMessage& other) = default;

    /**
     * @brief Move Constructor
     * @param other Message to move
     */
    CanMessage(CanMessage&& other) = default;

    /**
     * @brief Copy Assignment Operator
     * @param other Message to copy
     * @return Reference to this object
     */
    CanMessage& operator=(const CanMessage& other) = default;

    /**
     * @brief Move Assignment Operator
     * @param other Message to move
     * @return Reference to this object
     */
    CanMessage& operator=(CanMessage&& other) = default;

    /**
     * @brief Destructor
     */
    virtual ~CanMessage() = default;

    /**
     * @brief Constructor with Type and Format
     * @param type_ CAN Message Type (DATA/REMOTE)
     * @param format_ CAN ID Format (STD/EXT)
     */
    CanMessage(canType type_, canFormat format_)
        : Type{type_}, Format{format_} {}

    /**
     * @brief Constructor with ID, Type, and Format
     * @param id_ CAN Identifier
     * @param type_ CAN Message Type (DATA/REMOTE)
     * @param format_ CAN ID Format (STD/EXT)
     */
    CanMessage(uint32_t id_, canType type_, canFormat format_)
        : Id{id_}, Type{type_}, Format{format_} {}

    /**
     * @brief Full Constructor
     * @param id_ CAN Identifier
     * @param type_ CAN Message Type (DATA/REMOTE)
     * @param format_ CAN ID Format (STD/EXT)
     * @param dlc_ Data Length Code (0-8)
     */
    CanMessage(uint32_t id_, canType type_, canFormat format_, uint8_t dlc_)
        : Id{id_}, Type{type_}, Format{format_}, Dlc{dlc_} {}

    /**
     * @brief Get the CAN Identifier
     * @return uint32_t CAN ID
     */
    [[nodiscard]] uint32_t getId() const noexcept { return this->Id; }

    /**
     * @brief Set the CAN Identifier
     * @param id_ New CAN ID
     */
    void setId(uint32_t id_) noexcept { this->Id = id_; }

    /**
     * @brief Get the CAN Message Type
     * @return canType (DATA or REMOTE)
     */
    [[nodiscard]] canType getType() const noexcept { return this->Type; }

    /**
     * @brief Set the CAN Message Type
     * @param type_ New CAN Type
     */
    void setType(canType type_) noexcept { this->Type = type_; }

    /**
     * @brief Get the CAN ID Format
     * @return canFormat (STD or EXT)
     */
    [[nodiscard]] canFormat getFormat() const noexcept { return this->Format; }

    /**
     * @brief Set the CAN ID Format
     * @param format_ New CAN Format
     */
    void setFormat(canFormat format_) noexcept { this->Format = format_; }

    /**
     * @brief Get the Data Length Code
     * @return uint8_t DLC (0-8)
     */
    [[nodiscard]] uint8_t getDlc() const noexcept { return this->Dlc; }

    /**
     * @brief Set the Data Length Code
     * @param dlc_ New DLC (0-8)
     */
    void setDlc(uint8_t dlc_) noexcept { this->Dlc = dlc_; }

    /**
     * @brief Get a mutable reference to the data array
     * @return std::array<uint8_t, MAX_DLC>& Reference to data payload
     */
    [[nodiscard]] std::array<uint8_t, MAX_DLC>& Data() noexcept {
        return this->_Data;
    }

    /**
     * @brief Get a const reference to the data array
     * @return const std::array<uint8_t, MAX_DLC>& Const reference to data payload
     */
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
