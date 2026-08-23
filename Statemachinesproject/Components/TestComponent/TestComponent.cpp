// ======================================================================
// \title  TestComponent.cpp
// \author xtilloo
// \brief  cpp file for TestComponent component implementation class
// ======================================================================

#include "Statemachinesproject/Components/TestComponent/TestComponent.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TestComponent ::TestComponent(const char* const compName) : TestComponentComponentBase(compName) {}

TestComponent ::~TestComponent() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void TestComponent ::TODO_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Components
