/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TWI_H__
#define __TWI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* offsetof is a gcc built-in function, this is the manual implementation */
#define OFFSETOF(type, member) ((uint32_t) (&(((type *)(0))->member)))

typedef struct i2c_s i2c_t;

struct i2c_s {
  /*  The 1st 2 members I2CName i2c
     *  and I2C_HandleTypeDef handle should
     *  be kept as the first members of this struct
     *  to have get_i2c_obj() function work as expected
     */
  I2C_TypeDef  *i2c;
  I2C_HandleTypeDef handle;
  void *__this;
  uint32_t pin_sda;
  uint32_t pin_scl;
  IRQn_Type irq;
};

///@brief I2C state
typedef enum {
  I2C_OK = 0,
  I2C_DATA_TOO_LONG = 1,
  I2C_NACK_ADDR = 2,
  I2C_NACK_DATA = 3,
  I2C_ERROR = 4,
  I2C_TIMEOUT = 5,
  I2C_BUSY = 6
} i2c_status_e;

/* Exported functions ------------------------------------------------------- */
void func_print(uint8_t data);
void i2c_init(i2c_t *obj);
void i2c_custom_init(i2c_t *obj, uint32_t timing, uint32_t addressingMode,
                     uint32_t ownAddress);
void i2c_deinit(i2c_t *obj);
void i2c_setTiming(i2c_t *obj, uint32_t frequency);
i2c_status_e i2c_master_write(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint16_t size);
i2c_status_e i2c_slave_write_IT(i2c_t *obj, uint8_t *data, uint16_t size);
i2c_status_e i2c_master_read(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint16_t size);

i2c_status_e i2c_IsDeviceReady(i2c_t *obj, uint8_t devAddr, uint32_t trials);

void i2c_attachSlaveRxEvent(i2c_t *obj, void (*function)(i2c_t *));
void i2c_attachSlaveTxEvent(i2c_t *obj, void (*function)(i2c_t *));

void I2C0_IRQHandler(void);
void I2C1_IRQHandler(void);
void I2C2_IRQHandler(void);
void I2C3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __TWI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
