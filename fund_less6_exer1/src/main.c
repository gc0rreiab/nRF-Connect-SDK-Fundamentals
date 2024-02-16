#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

//Sensor registers addresses
#define SHT25_ADDR          0x40
#define SHT25_TRIGGER_TEMP  0xF3    // no-hold master mode
#define SHT25_TRIGGER_RH    0xF5    // no-hold master mode
#define SHT25_USER_REG_W    0xE6
#define SHT25_USER_REG_R    0xE7
#define SHT25_SOFT_RST      0xFE

// Get node identifier symbol I2C0_NODE based on bn055 sensor label on device-tree overlay file
#define I2C0_NODE DT_NODELABEL(sht25)

// Get the device pointer for the I2C0 bus
static const struct i2c_dt_spec dev_i2c0 = I2C_DT_SPEC_GET(I2C0_NODE);

int main(void){

    uint8_t ret;

    // Check if the I2C0 peripheral is ready 
    if (!device_is_ready(dev_i2c0.bus)) {
	    printk("I2C bus %s is not ready!\n\r", dev_i2c0.bus->name);
	    return -1;
    }

    // Configure the SHT25 sensor using the user register
    // Measurements resolutions configured: Temp: 12 bits and HR: 8 bits
    uint8_t config[2] = {SHT25_USER_REG_R, 0x8C};
	ret = i2c_write_dt(&dev_i2c0, config, sizeof(config));
	if(ret != 0){
		printk("Failed to write to I2C device address %x at Reg. %x \n", dev_i2c0.addr,config[0]);
		return -1;
	}

    uint8_t temp_reading[2]= {0};
    uint8_t sensor_regs[2] ={STTS751_TEMP_LOW_REG,STTS751_TEMP_HIGH_REG};
    ret = i2c_write_read_dt(&dev_i2c,&sensor_regs[0],1,&temp_reading[0],1);
    if(ret != 0){
	    printk("Failed to write/read I2C device address %x at Reg. %x \r\n", dev_i2c.addr,sensor_regs[0]);
    }
    ret = i2c_write_read_dt(&dev_i2c,&sensor_regs[1],1,&temp_reading[1],1);
    if(ret != 0){
	    printk("Failed to write/read I2C device address %x at Reg. %x \r\n", dev_i2c.addr,sensor_regs[1]);
    }

    while(1){


    }



}