# ImuHelpers class

## Overview

The `ImuHelpers` class provides the core implementation for the MPU-6050 IMU. Developers should import the ImuHelpers class into their F' component and use the provided methods to interface with the MPU-6050 IMU:

- `reset()`: Resets the IMU
- `read_reset(value)`: Reads the power management register to validate a reset
- `enable()`: Enables the IMU
- `configure_device(accelerationRange, gyroscopeRange)`: Configures the IMU accelerometer and gyroscope
- `read(imuData, accelerationRange, gyroscopeRange)`: Reads raw IMU data and converts it based on the data conversion factors

The goal of the `ImuHelpers` class is to allow developers participating in the advanced F-Prime workshop to focus on the F-Prime concepts, not low-level interfacing with the IMU.

## Setup

### 1. Import `ImuHelpers.hpp` into the component's `.hpp`

The first thing you'll want to do is import the helpers header file into the components header file:

`#include "Path/To/File/ImuHelpers.hpp"`

### 2. Create an `ImuHelpers` object in the component's `.hpp`

Make an object for the helpers class in the component's header file:

```cpp
private:
    ImuHelpers m_imuHelpers;
```

### 3. Add I2c ports in the component's `.fpp`

Inside your component's `.fpp` file, you'll want to add ports that interface with the `LinuxI2cDriver`.

```fpp
@ Port for I2c bus communication
output port i2cWriteRead: Drv.I2cWriteRead

@ Port for I2c bus communication
output port i2cWrite: Drv.I2c
```

### 4. Update the component's constructor method

In the components `.cpp` file, you'll want to update the constructor to pass the `i2cWriteRead` and `i2cWrite` ports to the helper class:

So, initially, the component constructor should look like:

```cpp
IMUManager ::IMUManager(const char* const compName) : IMUManagerComponentBase(compName) {}
```

Afterwards, the constructor should look like:

```cpp
IMUManager ::IMUManager(const char* const compName) : 
    IMUManagerComponentBase(compName),
    m_imuHelpers(
        [this](FwIndexType portNum, U32 addr, Fw::Buffer& writeBuffer, Fw::Buffer& readBuffer) { 
            return this->i2cWriteRead_out(portNum, addr, writeBuffer, readBuffer);
        },
        [this](FwIndexType portNum, U32 addr, Fw::Buffer& writeBuffer) {
            return this->i2cWrite_out(portNum, addr, writeBuffer);
        }
    ) {}
```

The segment above is passing the component's I2c driver methods to the helper class. The `i2cWriteRead_out` and `i2cWrite_out` methods are generated when you include the following ports in the `.fpp` file:

```fpp
@ Port for I2C bus communication
output port i2cWriteRead: Drv.I2cWriteRead

@ Port for I2C bus communication
output port i2cWrite: Drv.I2c
```

You'll want to make sure the methods that you pass into the helper class match the name of your ports. For example, `i2cWriteRead` is the port name, and `i2cWriteRead_out` is the method generated for that port.

The reason we are passing the I2c driver methods into the helper class is because the helper class itself is not an F' component and therefore does not implement the `LinuxI2cDriver` ports required for I2c interfacing when using F-Prime. So, by passing the I2c driver methods we are essentially allowing the helper class to use the component's auto-generated I2c driver methods to interface with the I2c bus.

### 5. Register the helper file in `CMakeLists.txt`

The F-Prime build system must know how to compile the helper class source code along with your component's source code.

Add `"${CMAKE_CURRENT_LIST_DIR}/ImuHelpers.cpp"` into your component's `CMakeLists.txt` file under the `SOURCES` section of the `register_fprime_library()`:

```
register_fprime_library(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/YourComponent.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/YourComponent.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ImuHelpers.cpp" # Added helper source
)
```

### 6. Build

Run: `fprime-util purge && fprime-util generate && fprime-util build`