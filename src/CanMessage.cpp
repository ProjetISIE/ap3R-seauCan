#include "CanMessage.hpp"
#include <format>

bool CanMessage::isMessageValid() const noexcept {
    switch (this->getFormat()) {
    case CanMessage::canFormat::STD:
        if (this->getId() > 0x7FF) return false;
        break;
    case CanMessage::canFormat::EXT:
        if (this->getId() > 0x1FFFFFFF) return false;
        break;
    default: return false;
    }
    if (this->getDlc() > MAX_DLC) return false;
    return true;
}

std::string CanMessage::toString() const noexcept {
    std::string form;
    switch (this->getFormat()) {
    case CanMessage::canFormat::STD: form = "STD"; break;
    case CanMessage::canFormat::EXT: form = "EXT"; break;
    default: form = "FormatError"; break;
    }

    std::string typeStr;
    switch (this->getType()) {
    case CanMessage::canType::DATA: typeStr = "DATA"; break;
    case CanMessage::canType::REMOTE: typeStr = "RMT"; break;
    default: typeStr = "TypeError"; break;
    }

    std::string dataHex;
    const auto& dataRef = this->Data();
    for (uint8_t i = 0; i < this->getDlc() && i < MAX_DLC; ++i)
        dataHex += std::format("{:02X}", dataRef[i]);

    return std::format("{};{};{:X};{};{};{}", form, typeStr, this->getId(),
                       this->getDlc(), dataHex,
                       (this->isMessageValid() ? "TRUE" : "FALSE"));
}
