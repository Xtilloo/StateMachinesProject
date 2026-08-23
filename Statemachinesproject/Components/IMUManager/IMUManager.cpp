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
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void IMUManager ::run_handler(FwIndexType portNum, U32 context) {
    // TODO
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void IMUManager ::resetIMU_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    
    // This is where we reset the IMU
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

    return false;
}

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_operationSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) const {
    // TODO
    return false;

}

Drv::I2cStatus IMUManager::reset() {
    // Attempt to write the reset data
    U8 reset_sequence[] = {POWER_MGMT_REGISTER, RESET_VALUE};
    Fw::Buffer writeBuffer(reset_sequence, sizeof(reset_sequence));
    Fw::Buffer readBuffer;
    return this->bus_write(writeBuffer, readBuffer);
}

Drv::I2cStatus IMUManager ::bus_write(Fw::Buffer& writeBuffer, Fw::Buffer& readBuffer) {
    Drv::I2cStatus status;
    FW_ASSERT(writeBuffer.isValid());
    if (readBuffer.isValid()) {
        status = this->i2cWriteRead_out(0, this->m_address, writeBuffer, readBuffer);
    } else {
        status = this->i2cWrite_out(0, this->m_address, writeBuffer);
    }
    return status;
}

}  // namespace Components
