// ======================================================================
// \title  IMUManager.cpp
// \author xtilloo
// \brief  cpp file for IMUManager component implementation class
// ======================================================================

#include "Statemachinesproject/Components/IMUManager/IMUManager.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

IMUManager ::IMUManager(const char* const compName) : IMUManagerComponentBase(compName) {}

IMUManager ::~IMUManager() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void IMUManager ::TODO_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Components
