// ======================================================================
// \title  IMUManager.cpp
// \author xtilloo
// \brief  cpp file for IMUManager component implementation class
// ======================================================================

#include "Statemachinesproject/Components/IMUManager/IMUManager.hpp"
#include "IMU.cpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

IMUManager ::IMUManager(const char* const compName) : IMUManagerComponentBase(compName) {}

IMUManager ::~IMUManager() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void IMUManager ::run_handler(FwIndexType portNum, U32 context) {
    // TODO
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void IMUManager ::resetIMU_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    
    

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void IMUManager ::readSensorData_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Implementations for internal state machine actions
// ----------------------------------------------------------------------

void IMUManager ::Components_IMUManager_IMUManagerSM_action_powerCycle(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO

}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_readSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO
}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_configureSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO
}

// ----------------------------------------------------------------------
// Implementations for internal state machine guards
// ----------------------------------------------------------------------

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_configureSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) const {
    // TODO
}

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_operationSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) const {
    // TODO
}

}  // namespace Components
