/*
 * modbus.h
 *
 *  Created on: Nov 29, 2022
 *      Author: ChanT
 */
/*
 * MASTER QUERY
 *
 * SLAVE ADDRESS     Function CODE                      DATA                               CRC(LSB followed by MSB)
 *    1 Byte              1Byte        START ADDRESS:2 Byte   NO_OF_POINTS:2 Bytes       2 Bytes
 *
 * SLAVE RESPONSE TO QUERY
 * SLAVE ADDRESS     Function CODE                      DATA                               CRC
 *    1 Byte              1Byte        BYTE_COUNT:1 Byte  REGISTER_DATA: n Bytes       2 Bytes
 *
 *
 * MASTER WRITE
 * SLAVE ADDRESS     Function CODE                                             DATA                                        CRC(LSB followed by MSB)
 *    1 Byte              1Byte        START ADDRESS:2 Byte   NO_OF_POINTS:2 Bytes BYTE_CCOUNT:1 BYTE DATA:n BYTES                  2 Bytes
 *
 * */
//modbus is big endian
#include "modbus_crc.h"
#include "string.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include <stdarg.h>
#include <math.h>

#ifndef SRC_MODBUS_RS485_MODBUS_H_
#define SRC_MODBUS_RS485_MODBUS_H_

#define READ_WRITE_COIL_STARTING_ADDRESS 1
#define READ_WRITE_COIL_ENDING_ADDRESS 10000

#define READ_ONLY_INPUT_STARTING_ADDRESS 10001
#define READ_ONLY_INPUT_ENDING_ADDRESS 20000

#define READ_ONLY_INPUT_REGISTER_STARTING_ADDRESS 30001//for short(2 byte) floats
#define READ_ONLY_INPUT_REGISTER_ENDING_ADDRESS 40000

#define READ_WRITE_HOLDING_REGISTER_STARTING_ADDRESS 40001//for 4 byte float
#define READ_WRITE_HOLDING_REGISTER_ENDING_ADDRESS 50000

#define MOD_DEVICE

#define SIZE_OF_CRC 2
#define SIZE_OF_FUNCTION_CODE 1
#define SIZE_OF_SLAVE_ADDRESS 1
#define SIZE_OF_BYTE_COUNT 1
#define SIZE_PER_REGISTER 2
#define SIZE_OF_START_ADDRESS 2
#define SIZE_OF_NUMBER_OF_POINTS 2
#define SIZE_OF_REGISTER_ADDRESS 2
#define TYPES_OF_REGISTER 2

#define DEVICE_ADDRESS_INDEX 0
#define FUNC_CODE_INDEX 1
#define BYTE_COUNT_INDEX 2
#define REGISTER_ADD_HB_INDEX 2
#define REGISTER_ADD_LB_INDEX 3
#define NUM_OF_POINTS_HB_INDEX 4
#define NUM_OF_POINTS_LB_INDEX 5
#define CRC_LB_INDEX 6
#define CRC_HB_INDEX 7

#define TYPES_OF_REGISTER 2
#define FIXED_FIELD_BYTE SIZE_OF_CRC+SIZE_OF_FUNCTION_CODE+SIZE_OF_SLAVE_ADDRESS
#define MAX_NUMBER_OF_DATA_BYTES_PER_QUERY 256 - FIXED_FIELD_BYTE
#define MAX_NUMBER_OF_DATA_BYTES 256
#define MAX_NUMBER_OF_HOLDING_REGISTER_PER_DEVICE 10000
#define MAX_NUMBER_OF_INPUT_REGISTER_PER_DEVICE 10000
#define MAX_NUMBER_OF_DEVICES 255

//words=2bytes float or int
//a modbus register stores 16bits
//coils occupy 1 bit
//number of points = number of coils

typedef enum
{
	ONE_BYTE=1,
	TWO_BYTE,
	FOUR_BYTE=4
}Size_of_Data;

typedef enum
{
	READ_REGISTER=1,
	READ_COIL,

	WRITE_REGISTER,
	WRITE_COIL,
}Request_t;

typedef enum
{
	INPUT,
	HOLDING,//for high precision float

}DataType_t;

typedef enum
{
	//READ
	READ_COILS=1,//read up to 2000 memory bits
	READ_DISCRETE_INPUTS,//read up to 2000 input bits
	READ_HOLDING_REGISTERS,//read up to 125 memory words
	READ_INPUT_REGISTERS,//read up to 125 input words,
	READ_AND_WRITE_HOLDING_REGISTERS=23,

	//WRITE
	WRITE_SINGLE_COIL=5,
	WRITE_SINGLE_REGISTER,
	WRITE_COILS=15,
	WRITE_HOLDING_REGISTERS=16,
}Function_Code_t;

enum device_illegal_code
{
	ILLEGAL_FUNCTION=1,
	ILLEGAL_DATA_ADDRESS,
	ILLEGAL_DATA_VALUE,
};

typedef union
{
	int data;

	uint8_t byte[4];

}Data_t;

typedef struct
{
	GPIO_TypeDef* TX_Enable_Port;
	uint16_t TX_Enable_Pin;
	uint8_t RXBuffer[MAX_NUMBER_OF_DATA_BYTES];
}Modbus_t;

UART_HandleTypeDef* Modbus_UART;
#if defined MOD_MASTER

typedef struct
{
	float* ptr;
	uint32_t address;
}Register_t;

typedef struct
{
	uint8_t Device_ID;
	Register_t** Register_DataBase;
	uint32_t number_of_registers[2];
	uint16_t starting_address;
	uint8_t  command;
}Modbus_Devices_t;

typedef struct {
	Modbus_t* Modbus_Master;
	uint8_t request;
	uint8_t Device_Address;
	uint8_t function_code;
	uint16_t number_of_points;
	Modbus_Devices_t* List_of_Devices;
	uint8_t number_of_devices;
	DataType_t REGISTER;
	uint32_t starting_address;
}Modbus_Master_t;
Modbus_Master_t ModBusMaster;
float angle[4];
float angle2[4];
uint8_t GetDeviceIndex(Modbus_Master_t* modbus_master);//get device index in from device array

uint8_t* PrepareTxBuffer(Modbus_Master_t *modbus,uint8_t device_add);
void Modbus_Master_Init(Modbus_Master_t* modbus_master,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin, UART_HandleTypeDef* Modbus_UART);
void Modbus_Device_Init(Modbus_Master_t* modbus_master,DataType_t REGISTER,uint8_t device_address,int number_of_data,...);//TODO search whether the device address init or not
void Modbus_Master_Write(Modbus_Master_t *modbus_master,uint8_t function_code,uint8_t Device_add,uint16_t starting_address, uint16_t number_of_points);
void Modbus_Master_Read(Modbus_Master_t *modbus_master,uint8_t function_code,uint8_t Device_add,uint16_t starting_address, uint16_t number_of_points);
#elif  defined MOD_DEVICE

typedef struct
{
	void* ptr;
	uint32_t address;
}Register_t;

typedef struct {

	Modbus_t* Modbus_Device;

	uint8_t Device_ID;

	Register_t** Register_DataBase;

	uint16_t starting_address;

	uint16_t ending_address;


	uint32_t number_of_registers[2];

	uint32_t number_of_data_to_return;

	uint16_t starting_index;

	DataType_t REGISTER;

	Function_Code_t Function_Code;
}Modbus_Device_t;

void Modbus_Device_Init(Modbus_Device_t* modbus_device,uint8_t device_id,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin,uint8_t Slave_ID, UART_HandleTypeDef* Modbus_UART);
void Modbus_RegisterDatabase_Init(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...);
uint8_t* PrepareTxBuffer(Modbus_Device_t *modbus_device);
void Modbus_Device_Callback(Modbus_Device_t* modbus_device);
void ModbusException(Modbus_Device_t* modbus_device,uint8_t illegal_code);
short ftofix16(float num);
Modbus_Device_t ModBusDevice;
#endif
void RS485Read(Modbus_t *modbus,uint8_t number_of_bytes);
void RS485Send(Modbus_t* modbus,uint8_t *data);
#if (!defined MOD_DEVICE && !defined MOD_MASTER)
#error /*You need to define whether this Modbus is master or device*/
/*If not using Modbus please comment this error*/
#endif
#endif /* SRC_MODBUS_RS485_MODBUS_H_ */
