module Components {
    @ TEst
    active component TestComponent {

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

        @ A state machine representing a device with on-off behavior
        state machine Device {

        @ A signal for turning the device on
        signal cmdOn

        @ A signal for turning the device off
        signal cmdOff

        @ The initial state is OFF
        initial enter OFF

        @ The ON state
        state ON {

            @ In the ON state, a cmdOff signal causes a transition to the OFF state
            on cmdOff enter OFF

        }

        @ The OFF state
        state OFF {

            @ In the OFF state, a cmdOff signal causes a transition to the ON state
            on cmdOn enter ON

        }

        }

    }
}