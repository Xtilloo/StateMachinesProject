// ======================================================================
// \title  ImuHelpers.hpp
// \author mstarch
// \brief  hpp file for defining constants and types for ImuHelpers.cpp
// ======================================================================

#ifndef ImuHelpers_HPP
#define ImuHelpers_HPP
#include "Fw/FPrimeBasicTypes.hpp"
#include "Fw/Buffer/Buffer.hpp"
#include "Drv/LinuxI2cDriver/LinuxI2cDriver.hpp"
#include <functional>

namespace Components {

    class ImuHelpers {
    public:
        // I2c write read function
        using I2cWriteReadFn = std::function<Drv::I2cStatus(FwIndexType, U32, Fw::Buffer&, Fw::Buffer&)>;

        // I2c write function
        using I2cWriteFn = std::function<Drv::I2cStatus(FwIndexType, U32, Fw::Buffer&)>;

        // Constructor to bind the i2c port drivers to this helper class
        ImuHelpers(I2cWriteReadFn i2cWriteRead, I2cWriteFn i2cWrite);

        //! RawImuData: basic structure of imu data as read from the device
        struct RawImuData {
            I16 acceleration[3];
            I16 temperature;
            I16 gyroscope[3];
        };

        // Struct representing X, Y, Z data
        struct GeometricVector3 {
            F32 x;
            F32 y;
            F32 z;
        };

        // Struct representing ImuData
        struct ImuData {
            // Accelerations from the accelerometer
            GeometricVector3 acceleration;

            // Angular rates from the Gyroscope
            GeometricVector3 rotation;

            // Temeperature in degrees Celsius
            F32 temperature;
        };

        // Range of the accelerometer in G's, integer values represent the conversion factor for the raw values from
        // the accelerometer registers to Gs
        enum AccelerationRange : U16 {
            RANGE_2G = 16384,
            RANGE_4G = 8192,
            RANGE_8G = 4096,
            RANGE_16G = 2048,
        };

        // Range of the gyroscope in degrees per second, integer values represent the conversion factor for the raw values from
        // the gyroscope registers to 10ths of a degree per second
        enum GyroscopeRange : U16 {
            RANGE_250DEG = 1310,
            RANGE_500DEG = 655,
            RANGE_1000DEG = 328,
            RANGE_2000DEG = 164,
        };

        // --------------------------------
        // User-defined functions
        // --------------------------------

        // Attempt to write reset data
        Drv::I2cStatus reset();

        // Read the reset data
        Drv::I2cStatus read_reset(U8& value);

        // Write power on value to the power management register
        Drv::I2cStatus enable();

        // Configure the range for accelerometer and gyroscope
        Drv::I2cStatus configure_device(AccelerationRange accelerationRange, GyroscopeRange gyroscopeRange);

        // Read data register, deserialize data, convert raw data then populate provided imuData reference
        Drv::I2cStatus read(ImuData& imuData, AccelerationRange accelerationRange, GyroscopeRange gyroscopeRange);

    private:
        
        // -----------------------------
        // Constants
        // -----------------------------

        // Register values
        static constexpr U8 address = 0x68;
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

        I2cWriteReadFn m_i2cWriteRead;
        I2cWriteFn m_i2cWrite;

        // write write_buffer to the i2c bus at the address provided
        Drv::I2cStatus bus_write(
            Fw::Buffer& writeBuffer, 
            Fw::Buffer& readBuffer
        );

        U8 accelerometer_range_to_register(AccelerationRange range);
        U8 gyroscope_range_to_register(GyroscopeRange range);
        RawImuData deserialize_raw_data(Fw::Buffer& buffer);
        ImuData convert_raw_data(const RawImuData& raw,
                                      const AccelerationRange& accelerationRange,
                                      const GyroscopeRange& gyroscopeRange);
    };

}
#endif