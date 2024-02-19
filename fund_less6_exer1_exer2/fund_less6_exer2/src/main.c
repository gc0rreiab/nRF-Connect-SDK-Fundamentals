#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include "bno055.h" // Include bno055 header file

// Function prototypes to integrate Zephyr I2C functions into the BNO055 driver API
int8_t BNO055_I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);
int8_t BNO055_I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);
void BNO055_delay_msek(uint32_t msek);
int8_t I2C_routine(void);

// Declaration of bno055 object
struct bno055_t my_bno055;

// Define a macro to fetch the node identifier symbol for I2C0 based on its label
#define I2C0_NODE DT_NODELABEL(bno055)

// Get the device pointer for the I2C0 bus
static const struct i2c_dt_spec dev_i2c0 = I2C_DT_SPEC_GET(I2C0_NODE);

int main(void)
{
    uint8_t err = 0;

    // Check if the I2C0 peripheral is ready
    if (!device_is_ready(dev_i2c0.bus))
    {
        printk("I2C bus %s is not ready!\n\r", dev_i2c0.bus->name);
        return -1;
    }

    err = I2C_routine();
    err = bno055_init(&my_bno055);

    while (1)
    {

        // Delay the loop to perform one temperature reading each second
        k_sleep(K_MSEC(1000));
    }
}

// Integrate BNO055 API with Zephyr I2C write() and read() functions
int8_t I2C_routine(void)
{
    my_bno055.bus_write = BNO055_I2C_bus_write;
    my_bno055.bus_read = BNO055_I2C_bus_read;
    my_bno055.delay_msec = BNO055_delay_msek;
    my_bno055.dev_addr = BNO055_I2C_ADDR1;

    return BNO055_INIT_VALUE;
}

int8_t BNO055_I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    int8_t BNO055_iERROR = BNO055_INIT_VALUE;
    uint8_t config[cnt + 1];
    config[0] = reg_addr;
    bytecpy(config + 1, reg_data, cnt);

    BNO055_iERROR = i2c_write(dev_i2c0.bus, config, cnt, dev_addr);

    return BNO055_iERROR;
}

int8_t BNO055_I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{

    int8_t BNO055_iERROR = BNO055_INIT_VALUE;

    BNO055_iERROR = i2c_write_read(dev_i2c0.bus, dev_addr, &reg_addr, 1, reg_data, cnt);

    return BNO055_iERROR;
}

void BNO055_delay_msek(uint32_t msek)
{
    // Delay
    k_sleep(K_MSEC(msek));
}