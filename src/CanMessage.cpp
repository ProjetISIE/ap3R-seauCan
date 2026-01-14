#include "../include/CanManager.hpp"

bool CanManager::isMessageValid() const noexcept {

    // Verif Format
    switch(this->getFormat()) {
        case canFormat::STD :
            // Verif ID
            if( this->getId() > 0h3FF){ 
                return false;
            }
            break;
        case canFormat::EXT :
            // Verif ID
            if( this->getId() > 0hFFFFFFF){ 
                return false;
            }
            break;
        case default :
            return false;
            break;
    }

    if(this->getDlc() > 8){
        return false;
    }

    if(this->getDlc() != this->Data().size()){
        return false;
    }

    return true;

}