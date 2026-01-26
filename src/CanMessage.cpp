#include "CanMessage.hpp"
#include <format>

bool CanMessage::isMessageValid() noexcept {
    // Verif Format
    switch (this->getFormat()) {
    case CanMessage::canFormat::STD:
        // Verif ID
        if (this->getId() > 0x3FF) {
            return false;
        }
        break;
    case CanMessage::canFormat::EXT:
        // Verif ID
        if (this->getId() > 0x1FFFFFFF) {
            return false;
        }
        break;
    default: return false; break;
    }
    if (this->getDlc() > 8) {
        return false;
    }
    return true;
};

std::string CanMessage::toString() noexcept {
    std::string form{""};
    std::string type{""};
    std::string data{""};
    std::string out{""};
    switch (this->getFormat()) {
    case CanMessage::canFormat::STD: form = "STD"; break;
    case CanMessage::canFormat::EXT: form = "EXT"; break;
    default: form = "FormatError"; break;
    }
    switch (this->getType()) {
    case CanMessage::canType::DATA: type = "DATA"; break;
    case CanMessage::canType::REMOTE: type = "RMT"; break;
    default: type = "TypeError"; break;
    }

    std::string data_hex;
    for (uint8_t i = 0; i < this->getDlc() && i < MAX_DLC; ++i)
        data_hex += std::format("{:02x}", this->Data()[i]);

    out = std::format("{};{};{:0x};{:0d};{};{}", form, type, this->getId(),
                      this->getDlc(), data_hex,
                      (this->isMessageValid() ? "TRUE" : "FALSE"));

    return out;
};
