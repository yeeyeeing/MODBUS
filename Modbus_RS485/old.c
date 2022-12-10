///*
// * old.c
// *
// *  Created on: 6 Dec 2022
// *      Author: ChanT
// */
//
///*modbus.c
// *
// *Created on: Nov 29, 2022
// *    Author: ChanT
// */
//#include "modbus.h"
//
//#if defined MOD_DEVICE
////static Holding_Register_Database_t Holding_Register_Data[]=//read-write register
////{
////		{&angle,"haha"},
////		{&angle2,"haha2"},
////};
////
////
////static const Input_Register_Database_t Input_Register_Data[]=//read only register
////{
////};
//void Modbus_Device_Init(Modbus_Device_t* modbus_device,uint8_t device_id,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin,uint8_t Slave_ID, UART_HandleTypeDef* Modbus_UART)
//{
//	modbus_device->Device_ID=device_id;
//
//	Modbus_UART=Modbus_UART;
//
//	modbus_device->Modbus_Device->TX_Enable_Pin=TX_Enable_Pin;
//
//	modbus_device->Modbus_Device->TX_Enable_Port=TX_Enable_Port;
//
//	modbus_device->Register_DataBase=malloc(TYPES_OF_REGISTER*sizeof(Register_t*));
//
//	modbus_device->number_of_registers[HOLDING]=modbus_device->number_of_registers[INPUT]=0;
//
//}
////void Modbus_Device_RegisterDatabase_Init(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)
////{
////
////	number_of_data*=2;
////
////	va_list valist;
////
////	va_start(valist, number_of_data);
////
////	if(	modbus_device->number_of_registers[REGISTER]==0)
////		modbus_device->Register_DataBase[REGISTER] = malloc(++modbus_device->number_of_registers[REGISTER] *    sizeof(Register_t));
////
////    else if(modbus_device->number_of_registers[REGISTER]>0)
////	modbus_device->Register_DataBase[REGISTER]  = realloc(	modbus_device->Register_DataBase[REGISTER]  , (++modbus_device->number_of_registers[REGISTER] ) * sizeof(Register_t) );
////
////	for( int i=0;i<number_of_data/2;i++)
////	{
////
////		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]-1].ptr = va_arg(valist, float*);
////
////		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]-1 ].address = va_arg(valist,uint32_t);
////
////
////		if(i==(number_of_data/2)-1)
////			continue;
////
////		else
////			modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER], (++modbus_device->number_of_registers[REGISTER]) * sizeof(Register_t));
////
////	}
////
////
////
////	va_end(valist);
////
////}
//void Modbus_Device_RegisterDatabase_Init(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)
//{
//
//	number_of_data*=2;
//
//	va_list valist;
//
//	va_start(valist, number_of_data);
//
//	if(	modbus_device->number_of_registers[REGISTER]==0)
//	{
//		modbus_device->number_of_registers[REGISTER]+=number_of_data/2;
//		modbus_device->Register_DataBase[REGISTER] = malloc(modbus_device->number_of_registers[REGISTER] *    sizeof(Register_t));
//	}
//	else if(modbus_device->number_of_registers[REGISTER]>0){
//		modbus_device->number_of_registers[REGISTER]+=number_of_data/2;
//		modbus_device->Register_DataBase[REGISTER]  = realloc(	modbus_device->Register_DataBase[REGISTER]  , 	modbus_device->number_of_registers[REGISTER] * sizeof(Register_t) );
//	}
//	for(int i=0;i<number_of_data/2;i++)
//	{
//		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]-1].ptr = va_arg(valist, float*);
//		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]-1 ].address = va_arg(valist,uint32_t);
//	}
//
//
//
//	va_end(valist);
//
//}
////void Modbus_Device_RegisterDatabase_Append(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)
////{
////	if(	modbus_device->number_of_registers[REGISTER]==0)
////		return;
////
////	number_of_data*=2;
////
////	va_list valist;
////
////	va_start(valist, number_of_data);
////
////	modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER] ,(	modbus_device->number_of_registers[REGISTER])*sizeof(Register_t));
////
////	int i=0;
////
////	for( i=0;i<number_of_data/2;i++)
////	{
////
////		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]].ptr = va_arg(valist, float*);
////
////		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER] ].address = va_arg(valist,uint32_t);
////
////		modbus_device->number_of_registers[REGISTER]++;
////
////		if(i==(number_of_data/2)-1)
////			continue;
////
////		else
////			modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER], (modbus_device->number_of_registers[REGISTER]+2) * sizeof(Register_t));
////
////	}
////
////
////
////	va_end(valist);
////
////}
//void Modbus_Device_RegisterDatabase_Append(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)
//{
//
//	if(	modbus_device->number_of_registers[REGISTER]==0)
//		return;
//	modbus_device->number_of_registers[REGISTER]+=number_of_data;
//	number_of_data*=2;
//
//	va_list valist;
//
//	va_start(valist, number_of_data);
//
//	modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER] ,(	modbus_device->number_of_registers[REGISTER])*sizeof(Register_t));
//
//	for(int i=0;i<number_of_data/2;i++)
//	{
//		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]].ptr = va_arg(valist, float*);
//		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER] ].address = va_arg(valist,uint32_t);
//	}
//	va_end(valist);
//}
//
//void Modbus_Device_Respond(Modbus_Device_t* modbus_device)
//{
//
//	uint16_t startAddr = ((modbus_device->Modbus_Device->RXBuffer[2]<<8)|modbus_device->Modbus_Device->RXBuffer[3]);  // start Register Address
//
//	uint16_t num = ((modbus_device->Modbus_Device->RXBuffer[4]<<8)|modbus_device->Modbus_Device->RXBuffer[5]);   // number to registers master has requested
//
//	modbus_device->starting_index=startAddr;
//
//	modbus_device->number_of_data_to_return=num;
//
//	if ((num<1)||(num>MAX_NUMBER_OF_DATA_BYTES_PER_QUERY/SIZE_OF_REGISTER))  // maximum no. of Registers as per the PDF
//	{
//		ModbusException (modbus_device,ILLEGAL_DATA_VALUE);  // send an exception
//		return;
//	}
//
//	uint16_t endAddr = startAddr+num-1;  // end Register
//
//	if (modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX]==READ_HOLDING_REGISTERS&&endAddr>modbus_device->number_of_registers[HOLDING])
//	{
//		ModbusException(modbus_device,ILLEGAL_DATA_ADDRESS);   // send an exception
//		return;
//	}
//
//	else if (modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX]==READ_INPUT_REGISTERS&&endAddr>modbus_device->number_of_registers[INPUT])
//	{
//		ModbusException(modbus_device,ILLEGAL_DATA_ADDRESS);   // send an exception
//		return;
//	}
//
//
//	switch(modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX])
//	{
//
//	case READ_COILS:
//
//
//
//		break;
//	case READ_DISCRETE_INPUTS:
//
//
//
//
//		break;
//	case READ_HOLDING_REGISTERS:
//		modbus_device->REGISTER=HOLDING;
//
//
//		break;
//	case READ_INPUT_REGISTERS:
//
//
//		break;
//	case WRITE_SINGLE_COIL:
//
//
//		break;
//	case WRITE_SINGLE_REGISTER:
//
//
//		break;
//	case WRITE_COILS:
//
//
//		break;
//	case WRITE_HOLDING_REGISTERS:
//
//
//		break;
//	case READ_AND_WRITE_HOLDING_REGISTERS:
//
//
//
//
//		break;
//
//
//
//	}
//
//	uint8_t* Tx_buffer=PrepareTxBuffer(modbus_device);
//
//	RS485Send(modbus_device->Modbus_Device,Tx_buffer);
//
//}
//
//
//
//uint8_t* PrepareTxBuffer(Modbus_Device_t* modbus_device)
//{
//
//	uint8_t* Tx_buffer=(uint8_t*)malloc(3*sizeof(uint8_t));
//
//	Tx_buffer[SLAVE_ADDRESS_INDEX]=modbus_device->Modbus_Device->RXBuffer[SLAVE_ADDRESS_INDEX];
//
//	Tx_buffer[FUNC_CODE_INDEX]=modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX];
//
//	Tx_buffer[BYTE_COUNT_INDEX]=modbus_device->number_of_data_to_return;
//
//	Tx_buffer=(uint8_t*)realloc(Tx_buffer,(3+modbus_device->number_of_data_to_return)*sizeof(uint8_t));
//
//
//	uint32_t i=0,j=3;
//
//	for(i=modbus_device->starting_index;i<modbus_device->number_of_data_to_return;i++)
//	{
//		uint32_t temp=*modbus_device->Register_DataBase[modbus_device->REGISTER][modbus_device->starting_address].ptr;
//
//		Tx_buffer[j]=(temp>>8)&0xFF;
//
//		Tx_buffer[j+1]=temp;
//
//		j++;
//	}
//
//	uint16_t crc = crc16(Tx_buffer, j);
//
//	Tx_buffer[j]=crc&0xFF;
//
//	Tx_buffer[j+1]=(crc>>8)&0xFF;
//
//	return Tx_buffer;
//}
//
//
//
//void ModbusException(Modbus_Device_t* modbus_device,uint8_t illegal_code)
//{
//	uint8_t TxBuffer[8];
//	TxBuffer[0]=modbus_device->Modbus_Device->RXBuffer[0];
//	TxBuffer[1]=modbus_device->Modbus_Device->RXBuffer[1]|0x80;
//	TxBuffer[2]=illegal_code;
//	uint16_t crc = crc16(TxBuffer, 3);
//	TxBuffer[3]=crc&0xFF;
//	TxBuffer[4]=(crc>>8)&0xFF;
//	RS485Send(modbus_device->Modbus_Device,TxBuffer);
//}
//
//
//
//
//
//
//
//#elif defined MOD_MASTER
//
//void Modbus_Master_Init(Modbus_Master_t* modbus_master,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin, UART_HandleTypeDef* Modbus_UART)
//{
//	Modbus_UART=Modbus_UART;
//
//	modbus_master->Modbus_Master->TX_Enable_Pin=TX_Enable_Pin;
//
//	modbus_master->Modbus_Master->TX_Enable_Port=TX_Enable_Port;
//
//}
//
//void Modbus_Device_Init(Modbus_Master_t* modbus_master,uint8_t device_address,int number_of_data,...)//TODO search whether the device address init or not
//{
//	number_of_data*=2;
//
//	va_list valist;
//
//	va_start(valist, number_of_data);
//
//	if(!modbus_master->device_number)
//		modbus_master->List_of_Devices = malloc(sizeof(modbus_master->List_of_Devices) )  ;
//
//	else if(modbus_master->device_number)
//		modbus_master->List_of_Devices = realloc(modbus_master->List_of_Devices, modbus_master->device_number * sizeof(modbus_master->List_of_Devices) )  ;
//
//
//	modbus_master->List_of_Devices[modbus_master->device_number].Holding_Register_DataBase =  malloc( sizeof(Holding_Register_Database_t) ) ;
//
//	for(int i=0;i<number_of_data/2;i++)
//	{
//
//		modbus_master->List_of_Devices[modbus_master->device_number].Holding_Register_DataBase[i].ptr=va_arg(valist, float*);
//
//		modbus_master->List_of_Devices[modbus_master->device_number].Holding_Register_DataBase[i].address=va_arg(valist,uint32_t);
//
//		if(i==(number_of_data/2)-1)
//			continue;
//
//		else
//			modbus_master->List_of_Devices[modbus_master->device_number].Holding_Register_DataBase = realloc(modbus_master->List_of_Devices[modbus_master->device_number].Holding_Register_DataBase, (i+2)*sizeof(Holding_Register_Database_t));
//
//	}
//	va_end(valist);
//
//	modbus_master->device_number++;
//}
//
//void Modbus_Master_Read_Register(Modbus_Master_t *modbus_master,uint8_t function_code,uint8_t Device_add,uint16_t starting_address, uint16_t number_of_points)
//{
//
//	uint8_t* TxData;
//
//	modbus_master->function_code=function_code;
//
//	modbus_master->starting_address=starting_address;
//
//	modbus_master->number_of_points=number_of_points;
//
//	modbus_master->device_address=Device_add;
//
//	modbus_master->request=READ_REGISTER;
//
//	uint8_t  index = GetDeviceIndex(modbus_master);
//
//	modbus_master->List_of_Devices[index].starting_address	= starting_address;
//
//	TxData=PrepareTxBuffer(modbus_master);
//
//	RS485Send(modbus_master->Modbus_Master,TxData);
//
//	RS485Read(modbus_master->Modbus_Master,number_of_points);
//
//}
//
//
//void Modbus_Master_Respond(Modbus_Master_t* modbus_master)
//{
//
//	uint8_t device_id=modbus_master->Modbus_Master->RXBuffer[SLAVE_ADDRESS_INDEX];
//
//	int device_index=0;
//
//	for(device_index=0;device_index<MAX_DEVICE_NUMBER;device_index++)
//	{
//		if(device_id==modbus_master->List_of_Devices[device_index].Device_ID)
//			break;
//	}
//
//	switch(modbus_master->Modbus_Master->RXBuffer[FUNC_CODE_INDEX])
//	{
//
//	case READ_COILS:
//
//
//
//		break;
//	case READ_DISCRETE_INPUTS:
//
//
//
//
//		break;
//	case READ_HOLDING_REGISTERS:;
//
//	int number_of_floats = (modbus_master->Modbus_Master->RXBuffer[BYTE_COUNT_INDEX]) / 2 ;
//
//	int data_byte_index=3;
//
//	for(uint8_t data_number=0;data_number<number_of_floats;data_number++)
//	{
//		modbus_master->List_of_Devices[device_index].Holding_Register_DataBase[ ( modbus_master->List_of_Devices[device_index].starting_address)+data_number ].ptr=(float*)( modbus_master->Modbus_Master->RXBuffer[data_byte_index]<<8 | modbus_master->Modbus_Master->RXBuffer[data_byte_index+1] );
//		data_byte_index+=2;
//	}
//	break;
//	case READ_INPUT_REGISTERS:
//		break;
//	}
//}
//
//uint8_t* PrepareTxBuffer(Modbus_Master_t *modbus)
//{
//
//	uint8_t *Tx_buffer;
//
//	if(modbus->request==READ_REGISTER){
//		Tx_buffer=(uint8_t*)calloc(FIXED_FIELD_BYTE+SIZE_OF_NUMBER_OF_POINTS+SIZE_OF_REGISTER_ADDRESS,sizeof(uint8_t));
//		Tx_buffer[SLAVE_ADDRESS_INDEX]=modbus->Modbus_Master->RXBuffer[SLAVE_ADDRESS_INDEX];
//		Tx_buffer[FUNC_CODE_INDEX]=modbus->Modbus_Master->RXBuffer[FUNC_CODE_INDEX];
//		Tx_buffer[2] =(modbus->starting_address>>8)&0xFF;//high byte
//		Tx_buffer[3] = modbus->starting_address;//low byte
//		Tx_buffer[4] =(modbus->number_of_points>>8)&0xFF;//high byte
//		Tx_buffer[5] = modbus->number_of_points;//low byte
//		uint16_t crc = crc16(Tx_buffer, 6);
//		Tx_buffer[6] = crc&0xFF;   // CRC LOW
//		Tx_buffer[7] = (crc>>8)&0xFF;  // CRC HIGH
//	}
//
//	return Tx_buffer;
//
//}
//uint8_t GetDeviceIndex(Modbus_Master_t* modbus_master)
//{
//	uint8_t index =0;
//
//	for(index=0;index<MAX_NUMBER_OF_DEVICES;index++)
//	{
//
//		if( modbus_master->List_of_Devices[index].Device_ID == modbus_master->device_address)
//			break;
//
//	}
//
//
//	return index;
//}
//
//#endif
//
//
//
//
//void RS485Read(Modbus_t *modbus,uint8_t number_of_bytes)
//{
//	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_RESET);
//	HAL_UARTEx_ReceiveToIdle_IT(  Modbus_UART,  &modbus->RXBuffer[0] ,number_of_bytes);
//}
//
//
//void RS485Send(Modbus_t *modbus,uint8_t *data)
//{
//	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_SET);
//	HAL_UART_Transmit(Modbus_UART, data, strlen((char*)data), 1000);
//	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_RESET);
//}
//
//
//
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)//for master
//{
//
//	if(huart == Modbus_UART)
//	{
//#if defined MOD_MASTER
//		Modbus_Master_Respond(&ModBusMaster);
//
//#elif  defined MOD_DEVICE
//		if(ModBusDevice.Device_ID==ModBusDevice.Modbus_Device->RXBuffer[0])
//			Modbus_Device_Respond(&ModBusDevice);
//#endif
//	}
//
//}
//
