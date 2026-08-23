module Components {
    @ This component will handle the IMU state machine
    active component IMUManager {

        async command resetIMU opcode 0

        async command readSensorData opcode 1

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        event configureStatus(success: Fw.Success) \ 
            severity activity high \
            id 0 \
            format "Configure status was {}"

        event operationStatus(success: Fw.Success) \ 
            severity activity high \
            id 1 \
            format "Operation status was {}"

        telemetry temperature: U16

        @ Drives state machine transitions
        sync input port run: Svc.Sched


        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

        state machine IMUManager {

            @ Power Cycle Signal: Perform steps 1 and 2 in IMU Data Flow
            action powerCycle

            action readSensor

            action configureSensor

            signal configureTermination

            guard configureSuccess

            guard operationSuccess

            signal reset

            signal operationTermination

            signal initialized

            signal read

            initial enter START

            state START {

                entry do { powerCycle }

                on initialized enter CONFIG

            }

            state CONFIG {

                entry do { configureSensor }

                on configureTermination enter CONFIG_STAT

            }

            choice CONFIG_STAT {
                if configureSuccess enter IDLE else enter START
            }

            state IDLE {
                 
                on read enter OPER

                on reset enter START

            }

            state OPER {

                entry do { readSensor }

                on operationTermination enter OP_STAT

            }

            choice OP_STAT {
                if operationSuccess enter IDLE else enter START
            }
        }
    }
}