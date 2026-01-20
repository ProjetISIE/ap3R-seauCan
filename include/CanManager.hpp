#include "CanMessage.hpp"

enum class bitRate_t {
    KBPS10,   /**< 10 kb/s */
    KBPS20,   /**< 20 kb/s */
    KBPS62_5, /**< 62.5 kb/s */
    KBPS125,  /**< 125 kb/s */
    KBPS250,  /**< 250 kb/s */
    KBPS500,  /**< 500 kb/s */
    KBPS800,  /**< 800 kb/s */
    KBPS1000  /**< 1000 kb/s */
};

class CanManager {
  private:
    bitRate_t bitRate{bitRate_t::KBPS20}; /** 20 kb/s par défaut */

  private:
    /**
     * @brief Virtual "callback", called after each setbitRate
     * Empty function if not overrided */
    virtual void bitRateRefresh() {};

  public:
    CanManager() = default;                  /** Default Ctor is defaulted */
    CanManager(const CanManager&) = default; /** Copy Ctor is defaulted */
    CanManager(CanManager&&) = default;      /** Move Ctor is defaulted */
    CanManager&
    operator=(const CanManager&) = default; /** Copy operator is defaulted */
    CanManager&
    operator=(CanManager&&) = default; /** Move operator is defaulted */
    virtual ~CanManager() = default;   /** Dtor is defaulted */

    /**
     * @brief Constructor with bitRate
     * @param br The bit rate to set
     * @info this constructor can be deleted in derived classes if
     * bit rate management is not supported
     */
    explicit CanManager(bitRate_t br) noexcept : bitRate{br} {};

    /**
     * @brief bitRate accessors
     * @info virtual to allow overriding in derived classes (especially
     * for no bit rate management)
     */
    [[nodiscard]] virtual bitRate_t getbitRate() { return this->bitRate; };
    virtual void setbitRate(bitRate_t br) {
        this->bitRate = br;
        this->bitRateRefresh();
    };

    virtual void Emit(CanMessage& msg) = 0; /** Tx function - Pure virtual */

    [[nodiscard]] virtual bool
    Receive(CanMessage& msg) = 0; /** Rx function - Pure virtual */
};
