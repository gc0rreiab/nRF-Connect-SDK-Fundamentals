#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

// Get node identifier symbol I2C0_NODE based on bn055 sensor label on device-tree overlay file
#define I2C0_NODE DT_NODELABEL(bno055)

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

    while (1)
    {

        // Delay the loop to perform one temperature reading each second
        k_sleep(K_MSEC(1000));
    }
}