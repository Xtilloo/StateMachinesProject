// ======================================================================
// \title  IMUManager.cpp
// \author xtilloo
// \brief  cpp file for IMUManager component implementation class
// ======================================================================

#include "Components/IMUManager/IMUManager.hpp"
#include <unistd.h>
#include "ImuHelpers.hpp"

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
    Drv::I2cStatus status = this->reset();
    // This is where we reset the IMU
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void IMUManager ::readSensorData_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    Drv::I2cStatus status = this->read();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Implementations for internal state machine actions
// ----------------------------------------------------------------------

void IMUManager ::Components_IMUManager_IMUManagerSM_action_powerCycle(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO
    Drv::I2cStatus status = this->reset();

    // Sleep 5 seconds after reset
    sleep(500000);

    // Then enable the sensor
    Drv::I2cStatus enableStatus = this->enable();

    // Then we need to raise the "initialized" signal
    this->IMUManagerInstance_sendSignal_initialized();
}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_readSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO
    Drv::I2cStatus status = this->read();
    this->IMUManagerInstance_sendSignal_operationTermination(Fw::Success::SUCCESS);
}

void IMUManager ::Components_IMUManager_IMUManagerSM_action_configureSensor(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal) {
    // TODO

    Drv::I2cStatus status = this->configure_device();

    this->IMUManagerInstance_sendSignal_configureTermination(Fw::Success::SUCCESS);
}

// ----------------------------------------------------------------------
// Implementations for internal state machine guards
// ----------------------------------------------------------------------

bool IMUManager ::Components_IMUManager_IMUManagerSM_guard_configureSuccess(
    SmId smId,
    Components_IMUManager_IMUManagerSM::Signal signal,
    const Fw::Success& value) const {
    
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
    const Fw::Success& value) const {
    // TODO

    if (value == Fw::Success::SUCCESS) {
        return true;
    }
    else {
        return false;
    }
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

Drv::I2cStatus IMUManager ::configure_device() {
    Fw::ParamValid paramValid;
    Drv::I2cStatus status = Drv::I2cStatus::I2C_OK;
    // Read accelerometer parameter and configure
    {
        U8 accel_config_sequence[] = {ACCEL_CONFIG_REGISTER, ACCEL_CONFIG_2G};
        Fw::Buffer writeBuffer(accel_config_sequence, sizeof(accel_config_sequence));
        Fw::Buffer readBuffer;
        status = this->bus_write(writeBuffer, readBuffer);
        if (status != Drv::I2cStatus::I2C_OK) {
            return status;
        }
    }
    // Read gyroscope parameter and configure
    {
        U8 gyro_config_sequence[] = {GYRO_CONFIG_REGISTER, GYRO_CONFIG_250DEG};
        Fw::Buffer writeBuffer(gyro_config_sequence, sizeof(gyro_config_sequence));
        Fw::Buffer readBuffer;
        status = this->bus_write(writeBuffer, readBuffer);
        if (status != Drv::I2cStatus::I2C_OK) {
            return status;
        }
    }
    return status;
}

Drv::I2cStatus IMUManager ::read() {
    U8 data[DATA_LENGTH];
    U8 registerAddress = DATA_BASE_REGISTER;

    Fw::Buffer writeBuffer(&registerAddress, 1);
    Fw::Buffer readBuffer(data, DATA_LENGTH);
    // If bus write fails, state machine is reset, so just return
    Drv::I2cStatus status = this->bus_write(writeBuffer, readBuffer);
    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }
    RawImuData raw = this->deserialize_raw_data(readBuffer);

    this->tlmWrite_temperature(raw.temperature);
    return status;
}

IMUManager::RawImuData IMUManager ::deserialize_raw_data(Fw::Buffer& buffer) {
    auto deserializer = buffer.getDeserializer();
    RawImuData raw;
    deserializer.deserializeTo(raw.acceleration[0]);
    deserializer.deserializeTo(raw.acceleration[1]);
    deserializer.deserializeTo(raw.acceleration[2]);
    deserializer.deserializeTo(raw.temperature);
    deserializer.deserializeTo(raw.gyroscope[0]);
    deserializer.deserializeTo(raw.gyroscope[1]);
    deserializer.deserializeTo(raw.gyroscope[2]);
    return raw;
}

Drv::I2cStatus IMUManager ::enable() {
    U8 power_on_sequence[] = {POWER_MGMT_REGISTER, POWER_ON_VALUE};
    Fw::Buffer writeBuffer(power_on_sequence, sizeof(power_on_sequence));
    Fw::Buffer readBuffer;
    return this->bus_write(writeBuffer, readBuffer);
}

}  // namespace Components
