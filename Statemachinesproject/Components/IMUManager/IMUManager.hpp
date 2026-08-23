// ======================================================================
// \title  IMUManager.hpp
// \author xtilloo
// \brief  hpp file for IMUManager component implementation class
// ======================================================================

#ifndef Components_IMUManager_HPP
#define Components_IMUManager_HPP

#include "Statemachinesproject/Components/IMUManager/IMUManagerComponentAc.hpp"

namespace Components {

class IMUManager final : public IMUManagerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct IMUManager object
    IMUManager(const char* const compName  //!< The component name
    );

    //! Destroy IMUManager object
    ~IMUManager();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command TODO
    //!
    //! TODO
    void TODO_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                         U32 cmdSeq            //!< The command sequence number
                         ) override;
};

}  // namespace Components

#endif
