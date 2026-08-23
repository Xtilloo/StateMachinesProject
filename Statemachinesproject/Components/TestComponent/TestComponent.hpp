// ======================================================================
// \title  TestComponent.hpp
// \author xtilloo
// \brief  hpp file for TestComponent component implementation class
// ======================================================================

#ifndef Components_TestComponent_HPP
#define Components_TestComponent_HPP

#include "Statemachinesproject/Components/TestComponent/TestComponentComponentAc.hpp"

namespace Components {

class TestComponent final : public TestComponentComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TestComponent object
    TestComponent(const char* const compName  //!< The component name
    );

    //! Destroy TestComponent object
    ~TestComponent();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command TODO
    //!
    //! TODO
    void TODO_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                         U32 cmdSeq            //!< The command sequence number
                         ) override;
};

}  // namespace Components

#endif
