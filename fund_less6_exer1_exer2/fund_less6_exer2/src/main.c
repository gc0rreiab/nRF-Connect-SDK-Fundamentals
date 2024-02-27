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
int8_t BNO055_initialization_routine(uint8_t);

// Declaration of bno055 object
struct bno055_t my_bno055;
// Declaration of bno055_linear_accel_t struct to store sensor readings
struct bno055_accel_float_t accel_xyz;

// Define a macro to fetch the node identifier symbol for I2C0 based on its label
#define I2C0_NODE DT_NODELABEL(bno055)

// Get the device pointer for the I2C0 bus
static const struct i2c_dt_spec dev_i2c0 = I2C_DT_SPEC_GET(I2C0_NODE);

int main(void)
{
    int8_t err = 0;

    // Check if the I2C0 peripheral is ready
    if (!device_is_ready(dev_i2c0.bus))
    {
        printk("I2C bus %s is not ready!\n\r", dev_i2c0.bus->name);
        return -1;
    }
    else
        printk("I2C bus is ready: %s\n\r", dev_i2c0.bus->name);

    err = BNO055_initialization_routine(BNO055_I2C_ADDR2);

    if (err != 0)
    {
        printk("Error initializing the BNO055 sensor. Error n: %d\n\r", err);
        return -1;
    }

    err += bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
    err += bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
    err += bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);

    printk("Configurations Complete.\n\r");
    printk("Start scannings\n\r");

    while (1)
    {
        err = 0;
        struct bno055_accel_t accel_raw;
        err += bno055_read_accel_xyz(&accel_raw);

        // err += bno055_convert_float_accel_xyz_msq(&accel_xyz);

        if (err == 0)
            printk("X: %d ms2\tY: %d ms2 \tZ: %d ms2\n\r", accel_raw.x, accel_raw.y, accel_raw.z);
        else
            printk("Error reading data from the BNO055 sensor. Error n: %d\n\r", err);

        // Delay the loop to perform one temperature reading eachquarter of a second
        k_sleep(K_MSEC(200));
    }
}

// Integrate BNO055 API with Zephyr I2C write() and read() functions
int8_t BNO055_initialization_routine(uint8_t BNO055_I2C_ADDR)
{
    int8_t err = 0;

    my_bno055.bus_write = BNO055_I2C_bus_write;
    my_bno055.bus_read = BNO055_I2C_bus_read;
    my_bno055.delay_msec = BNO055_delay_msek;
    my_bno055.dev_addr = BNO055_I2C_ADDR;

    err += bno055_init(&my_bno055);

    return err;
}

int8_t BNO055_I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    int8_t com_rslt = BNO055_INIT_VALUE;
    uint8_t config[cnt + 1];
    config[0] = reg_addr;
    bytecpy(config + 1, reg_data, cnt);

    com_rslt = i2c_write(dev_i2c0.bus, config, cnt, dev_addr);

    if (com_rslt == 0)
        return BNO055_SUCCESS;
    else
        return BNO055_ERROR;
}

int8_t BNO055_I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{

    int8_t com_rslt = BNO055_INIT_VALUE;
    uint8_t config[1];
    config[0] = reg_addr;
    com_rslt = i2c_write_read(dev_i2c0.bus, dev_addr, config, 1, reg_data, cnt);

    if (com_rslt == 0)
        return BNO055_SUCCESS;
    else
        return BNO055_ERROR;
}

void BNO055_delay_msek(uint32_t msek)
{
    k_sleep(K_MSEC(msek));
}
