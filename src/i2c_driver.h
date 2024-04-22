//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### I2C_DRIVER.H ##########################
//---------------------------------------------
#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_


// Config Defines --------------------------------------------------------------


// Module Exported Functions ---------------------------------------------------
void i2c_driver_update (void);
void i2c_driver_timeouts (void);
void i2c_driver_set_encod (unsigned char encoder_num, unsigned char encoder_pos);


#endif    /* _I2C_DRIVER_H_ */

//--- end of file ---//

