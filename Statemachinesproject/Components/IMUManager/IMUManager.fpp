module Components {
    @ This component will handle the IMU state machine
    active component IMUManager {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command TODO opcode 0

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

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

            signal rateGrouptTick

            @ Power Cycle Signal: Perform steps 1 and 2 in IMU Data Flow
            action powerCycle

            action readSensor

            action configureSensor

            signal configSuccess

            signal operationSuccess

            signal reset

            signal initialized

            signal read

            state START {

                entry do { powerCycle }

                on initialized enter CONFIG

            }

            state CONFIG {

                entry do { configureSensor }

                if configureSuccess enter IDLE else enter START

            }

            state IDLE {
                 
                on read enter OPER

                on reset enter START

            }

            state OPER {

                entry do { readSensor }

                if operationSuccess enter IDLE else enter START

            }
        }
    }
}