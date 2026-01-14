#include "CanMessage.hpp"

bool CanMessage::isMessageValid() const noexcept {

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
        if (this->getId() > 0xFFFFFFF) {
            return false;
        }
        break;
    default: return false; break;
    }

    if (this->getDlc() > 8) {
        return false;
    }

    if (this->getDlc() != this->Data().size()) {
        return false;
    }

    return true;
}
