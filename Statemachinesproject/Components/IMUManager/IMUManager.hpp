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
};

}  // namespace Components

#endif
