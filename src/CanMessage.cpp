#include "../include/CanMessage.hpp"

bool CanMessage::isMessageValid() noexcept {

    // Verif Format
    switch (this->getFormat()) {
    case CanMessage::canFormat::STD :
        // Verif ID
        if (this->getId() > 0x3FF) {
            return false;
        }
        break;
    case CanMessage::canFormat::EXT :
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

    switch(this->getFormat()){
        case CanMessage::canFormat::STD :
            form = "STD";
            break;
        case CanMessage::canFormat::EXT :
            form = "EXT";
            break;
        default :
            form = "FormatError";
            break;
    }

    switch(this->getType()){
        case CanMessage::canType::DATA :
            type = "DATA";
            break;
        case CanMessage::canType::REMOTE :
            type = "RMT";
            break;
        default :
            type = "TypeError";
            break;
    }

    out = std::format("{};{};{:0x};{:0d};{:0x};{}",
        form,
        type,
        this->getId(),
        this->getDlc(),
        this->Data(),
        (this->isMessageValid()? "TRUE" : "FALSE")
    );

    return out;
};

