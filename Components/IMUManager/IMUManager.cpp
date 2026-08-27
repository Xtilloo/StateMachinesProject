// ======================================================================
// \title  IMUManager.cpp
// \author xtilloo
// \brief  cpp file for IMUManager component implementation class
// ======================================================================

#include "Components/IMUManager/IMUManager.hpp"
#include <unistd.h>

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

IMUManager ::IMUManager(const char* const compName) : 
    IMUManagerComponentBase(compName),
    m_imuHelpers(
        [this](FwIndexType portNum, U32 addr, Fw::Buffer& writeBuffer, Fw::Buffer& readBuffer) { 
            return this->i2cWriteRead_out(0, addr, writeBuffer, readBuffer);
        },
        [this](FwIndexType portNum, U32 addr, Fw::Buffer& writeBuffer) {
            return this->i2cWrite_out(0, addr, writeBuffer);
        }
    ) {}

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
    Drv::I2cStatus status = m_imuHelpers.reset();
    // This is where we reset the IMU
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void IMUManager ::readSensorData_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    ImuHelpers::ImuData imuData;
    ImuHelpers::AccelerationRange accelerationRange = ImuHelpers::AccelerationRange::RANGE_2G;
    ImuHelpers::GyroscopeRange gyroscopeRange = ImuHelpers::GyroscopeRange::RANGE_250DEG;

    Drv::I2cStatus status = m_imuHelpers.read(imuData, accelerationRange, gyroscopeRange);
    this->tlmWrite_temperature(imuData.temperature);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Implementations for internal state machine actions
// ----------------------------------------------------------------------

void IMUManager ::Components_IMUManager_IMUManagerSM_action_powerCycle(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal
) {
    Drv::I2cStatus status = m_imuHelpers.reset();

    // Sleep 5 seconds after reset
    sleep(500000);

    // Then enable the sensor
    Drv::I2cStatus enableStatus = m_imuHelpers.enable();

    // Then we need to raise the "initialized" signal
    this->IMUManagerInstance_sendSignal_initialized();
}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_readSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal
) {
    ImuHelpers::ImuData imuData;
    ImuHelpers::AccelerationRange accelerationRange = ImuHelpers::AccelerationRange::RANGE_2G;
    ImuHelpers::GyroscopeRange gyroscopeRange = ImuHelpers::GyroscopeRange::RANGE_250DEG;

    Drv::I2cStatus status = m_imuHelpers.read(imuData, accelerationRange, gyroscopeRange);
    this->IMUManagerInstance_sendSignal_operationTermination(Fw::Success::SUCCESS);
}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_configureSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal
) {
    ImuHelpers::AccelerationRange accelerationRange = ImuHelpers::AccelerationRange::RANGE_2G;
    ImuHelpers::GyroscopeRange gyroscopeRange = ImuHelpers::GyroscopeRange::RANGE_250DEG;
    Drv::I2cStatus status = m_imuHelpers.configure_device(accelerationRange, gyroscopeRange);
    this->IMUManagerInstance_sendSignal_configureTermination(Fw::Success::SUCCESS);
}

// ----------------------------------------------------------------------
// Implementations for internal state machine guards
// ----------------------------------------------------------------------

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_configureSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal,
    const Fw::Success& value
) const {
    if (value == Fw::Success::SUCCESS) {
        return true;
    }
    else {
        return false;
    }
}

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_operationSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal,
    const Fw::Success& value
) const {
    if (value == Fw::Success::SUCCESS) {
        return true;
    }
    else {
        return false;
    }
}

// ---------------------------------------------------------------------
// User-Defined Implementations
// ---------------------------------------------------------------------

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
