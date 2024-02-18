#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

// Sensor registers addresses
#define SHT25_ADDR 0x40
#define SHT25_TRIGGER_TEMP 0xF3 // no-hold master mode
#define SHT25_TRIGGER_RH 0xF5   // no-hold master mode
#define SHT25_USER_REG_WRITE 0xE6
#define SHT25_USER_REG_READ 0xE7
#define SHT25_SOFT_RST 0xFE
#define SHT25_MASK_RESOLUTION_DEFAULT 0b01111110
#define SHT25_MASK_RESOLUTION_12_14 0b00000000
#define SHT25_MASK_RESOLUTION_8_12 0b00000001
#define SHT25_MASK_RESOLUTION_10_13 0b10000000
#define SHT25_MASK_RESOLUTION_11_11 0b10000001

// Get node identifier symbol I2C0_NODE based on bn055 sensor label on device-tree overlay file
#define I2C0_NODE DT_NODELABEL(sht25)

// Get the device pointer for the I2C0 bus
static const struct i2c_dt_spec dev_i2c0 = I2C_DT_SPEC_GET(I2C0_NODE);

int main(void)
{

    uint8_t ret = 0;

    // Check if the I2C0 peripheral is ready
    if (!device_is_ready(dev_i2c0.bus))
    {
        printk("I2C bus %s is not ready!\n\r", dev_i2c0.bus->name);
        return -1;
    }

    // Configure the SHT25 sensor measurements resolutions using the user register: HR: 8 bits, Temp: 12 bits
    // Note: based on the datasheet we have to keep the value of the reserved bits, so we have to read the data from the user regfister first
    uint8_t user_reg;
    uint8_t value_to_config;
    uint8_t config[2];

    // Read user register data
    config[0] = SHT25_USER_REG_READ;
    ret += i2c_write_read_dt(&dev_i2c0, &config[0], 1, &user_reg, 1);

    // Calculate the value to write in the user register without change the reserved bits values and other configurations
    value_to_config = (user_reg & SHT25_MASK_RESOLUTION_DEFAULT) | SHT25_MASK_RESOLUTION_8_12;
    config[2] = (uint8_t[2]){SHT25_USER_REG_WRITE, value_to_config};

    // Write the configuration to the user register
    ret += i2c_write_dt(&dev_i2c0, config, sizeof(config));

    if (ret != 0)
    {
        printk("Failed to configure I2C device address %x at Reg. %x \n", dev_i2c0.addr, config[0]);
        return -1;
    }

    while (1)
    {
        // Send the temperature measurement command
        config[0] = SHT25_TRIGGER_TEMP;
        ret = i2c_write_dt(&dev_i2c0, &config[0], 1);
        if (ret != 0)
        {
            printk("Failed to write the trigger temperature measurement command to the I2C device\r\n");
        }

        // Wait for sensor to complete its measurement
        k_sleep(K_MSEC(22));

        // Read the temperature data
        uint8_t temp_reading[2] = {0};
        ret = i2c_read_dt(&dev_i2c0, &temp_reading[0], 2);
        if (ret != 0)
        {
            printk("Failed read the temperature data from I2C device\r\n");
        }

        // Calculate Temperature value
        uint16_t tempRaw = 0;
        float temp;
        temp = -46.85 + (175.72 * tempRaw / 65536.0);
        printk("Temp = %f ºC\r\n", temp);

        // Delay the loop to perform one temperature reading each second
        k_sleep(K_MSEC(1000));
    }
}