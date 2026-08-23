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
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Drives state machine transitions
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command resetIMU
    void resetIMU_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                             U32 cmdSeq            //!< The command sequence number
                             ) override;

    //! Handler implementation for command readSensorData
    void readSensorData_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                   U32 cmdSeq            //!< The command sequence number
                                   ) override;

  private:
    // ----------------------------------------------------------------------
    // Implementations for internal state machine actions
    // ----------------------------------------------------------------------

    //! Implementation for action powerCycle of state machine Components_IMUManager_IMUManagerSM
    //!
    //! Power Cycle Signal: Perform steps 1 and 2 in IMU Data Flow
    void Components_IMUManager_IMUManagerSM_action_powerCycle(
        SmId smId,                                         //!< The state machine id
        Components_IMUManager_IMUManagerSM::Signal signal  //!< The signal
        ) override;

    //! Implementation for action readSensor of state machine Components_IMUManager_IMUManagerSM
    //!
    //! This action is what reads the sensor measurements
    void Components_IMUManager_IMUManagerSM_action_readSensor(
        SmId smId,                                         //!< The state machine id
        Components_IMUManager_IMUManagerSM::Signal signal  //!< The signal
        ) override;

    //! Implementation for action configureSensor of state machine Components_IMUManager_IMUManagerSM
    //!
    //! This action is what configures the sensor
    void Components_IMUManager_IMUManagerSM_action_configureSensor(
        SmId smId,                                         //!< The state machine id
        Components_IMUManager_IMUManagerSM::Signal signal  //!< The signal
        ) override;

    Drv::I2cStatus bus_write(Fw::Buffer& writeBuffer, Fw::Buffer& readBuffer);


  private:
    // ----------------------------------------------------------------------
    // Implementations for internal state machine guards
    // ----------------------------------------------------------------------

    //! Implementation for guard configureSuccess of state machine Components_IMUManager_IMUManagerSM
    //!
    //! This guards whether we can move into IDLE or START
    bool Components_IMUManager_IMUManagerSM_guard_configureSuccess(
        SmId smId,                                         //!< The state machine id
        Components_IMUManager_IMUManagerSM::Signal signal  //!< The signal
    ) const override;

    //! Implementation for guard operationSuccess of state machine Components_IMUManager_IMUManagerSM
    bool Components_IMUManager_IMUManagerSM_guard_operationSuccess(
        SmId smId,                                         //!< The state machine id
        Components_IMUManager_IMUManagerSM::Signal signal  //!< The signal
    ) const override;

    Drv::I2cStatus IMUManager::reset();

    U8 m_address = 0x68;

    static constexpr U8 DATA_LENGTH = (6 + 1) * sizeof(U16);  // 6 DoF + temperature
    static constexpr U8 DATA_BASE_REGISTER = 0x3B;
    static constexpr U8 DEVICE_DEFAULT_ADDRESS = 0x68;
    static constexpr U8 POWER_MGMT_REGISTER = 0x6B;
    static constexpr U8 RESET_VALUE = 0x80;
    static constexpr U8 POWER_ON_VALUE = 0x00;
    static constexpr U8 GYRO_CONFIG_REGISTER = 0x1B;
    static constexpr U8 ACCEL_CONFIG_REGISTER = 0x1C;

    // Configuration values for the accelerometer and gyroscope
    static constexpr U8 ACCEL_CONFIG_2G = 0x00;
    static constexpr U8 ACCEL_CONFIG_4G = 0x08;
    static constexpr U8 ACCEL_CONFIG_8G = 0x10;
    static constexpr U8 ACCEL_CONFIG_16G = 0x18;
    static constexpr U8 GYRO_CONFIG_250DEG = 0x00;
    static constexpr U8 GYRO_CONFIG_500DEG = 0x08;
    static constexpr U8 GYRO_CONFIG_1000DEG = 0x10;
    static constexpr U8 GYRO_CONFIG_2000DEG = 0x18;
    static constexpr F32 TEMPERATURE_SCALAR = 340.0f;
    static constexpr F32 TEMPERATURE_OFFSET = 36.53f;

    //! RawImuData: basic structure of imu data as read from the device
    struct RawImuData {
        I16 acceleration[3];
        I16 temperature;
        I16 gyroscope[3];
    };
};

}  // namespace Components

#endif
