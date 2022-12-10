#include "modbus.h"

#if defined MOD_DEVICE
/*
 *TODO:1.check whether the address used to init is occupied by other pointers
 *TODO:2.give chance to 4byte datas
 *TODO:3.implement quick sort
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void Modbus_Device_Init(Modbus_Device_t* modbus_device,uint8_t device_id,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin,uint8_t Slave_ID, UART_HandleTypeDef* Modbus_UART)
{
	modbus_device->Device_ID=device_id;

	Modbus_UART=Modbus_UART;

	modbus_device->Modbus_Device->TX_Enable_Pin=TX_Enable_Pin;

	modbus_device->Modbus_Device->TX_Enable_Port=TX_Enable_Port;

	modbus_device->Register_DataBase=malloc(TYPES_OF_REGISTER*sizeof(Register_t*));//allocate memomry for 2 rows

	if(modbus_device->Register_DataBase==NULL)
		return;

	modbus_device->Modbus_Device=malloc(sizeof(Modbus_t));

	if(modbus_device->Modbus_Device==NULL)
		return;

	modbus_device->number_of_registers[HOLDING]=modbus_device->number_of_registers[INPUT]=0;
}




void Modbus_RegisterDatabase_Init(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)//for each type of data(2byte,1byte,4bytes,need to allocate for each )
{

	if(	modbus_device->number_of_registers[REGISTER]==0)
	{
		modbus_device->Register_DataBase[REGISTER] = malloc((number_of_data )*sizeof(Register_t));

		if(modbus_device->Register_DataBase[REGISTER]==NULL)
			return;

	}

	else if(modbus_device->number_of_registers[REGISTER]>0)
		modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER] , (modbus_device->number_of_registers[REGISTER]+number_of_data ) * sizeof(Register_t) );

	number_of_data*=2;

	va_list valist;

	va_start(valist, number_of_data);

	for(int i=0;i<number_of_data/2;i++)
	{
		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]+i].ptr = va_arg(valist, float*);
		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]+i].address = va_arg(valist,uint32_t);
	}

	va_end(valist);

	if(REGISTER==INPUT)
		modbus_device->number_of_registers[REGISTER]+=number_of_data/2;

	else if(REGISTER==HOLDING)
		modbus_device->number_of_registers[REGISTER]+=number_of_data;
}


void Modbus_Device_RegisterDatabase_Append(Modbus_Device_t* modbus_device,DataType_t REGISTER,int number_of_data,...)
{
	if(	modbus_device->number_of_registers[REGISTER]==0)
		return;

	modbus_device->Register_DataBase[REGISTER]  = realloc(modbus_device->Register_DataBase[REGISTER]+number_of_data ,(	modbus_device->number_of_registers[REGISTER])*sizeof(Register_t));

	number_of_data*=2;

	va_list valist;

	va_start(valist, number_of_data);

	for(int i=0;i<number_of_data/2;i++)
	{
		modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]+i].ptr = va_arg(valist, float*);

		if(REGISTER==HOLDING)
			modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]+i ].address = (va_arg(valist,uint32_t) + 1);

		else if(REGISTER==INPUT)
			modbus_device->Register_DataBase[REGISTER][modbus_device->number_of_registers[REGISTER]+i ].address = (va_arg(valist,uint32_t) );
	}
	va_end(valist);
	modbus_device->number_of_registers[REGISTER]+=number_of_data/2;
}



void Modbus_Device_Callback(Modbus_Device_t* modbus_device)
{
	modbus_device->Function_Code=modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX];
	modbus_device->starting_index = ((modbus_device->Modbus_Device->RXBuffer[REGISTER_ADD_HB_INDEX]<<8)|modbus_device->Modbus_Device->RXBuffer[REGISTER_ADD_LB_INDEX]);  // start Register Address
	modbus_device->number_of_data_to_return= ((modbus_device->Modbus_Device->RXBuffer[NUM_OF_POINTS_HB_INDEX]<<8)|modbus_device->Modbus_Device->RXBuffer[NUM_OF_POINTS_LB_INDEX]);   // number to registers master has requested
	if(modbus_device->Function_Code%2==0)
	{
		modbus_device->REGISTER=INPUT;
		modbus_device->ending_address = modbus_device->starting_index + modbus_device->number_of_data_to_return- 1;  // end Register
	}
	else
	{
		modbus_device->REGISTER=HOLDING;
		modbus_device->ending_address = modbus_device->starting_index + modbus_device->number_of_data_to_return*2- 1;  // end Register
	}

	if ((modbus_device->number_of_data_to_return<1)||(modbus_device->number_of_data_to_return>MAX_NUMBER_OF_DATA_BYTES_PER_QUERY/SIZE_PER_REGISTER))  // maximum no. of Registers as per the PDF
	{
		ModbusException(modbus_device,ILLEGAL_DATA_VALUE);  // send an exception
		return;
	}

	if(	modbus_device->ending_address > modbus_device->number_of_registers[modbus_device->REGISTER])//if number of data requested by master is more than data available in database,send an exception
	{
		ModbusException(modbus_device,ILLEGAL_DATA_ADDRESS);   // send an exception
		return;
	}
	uint8_t* Tx_buffer=PrepareTxBuffer(modbus_device);
	RS485Send(modbus_device->Modbus_Device,Tx_buffer);
}


uint8_t* PrepareTxBuffer(Modbus_Device_t* modbus_device)
{

	uint8_t* Tx_buffer=malloc((FIXED_FIELD_BYTE+SIZE_OF_BYTE_COUNT+modbus_device->number_of_data_to_return*modbus_device->REGISTER)*sizeof(uint8_t));

	Tx_buffer[DEVICE_ADDRESS_INDEX]=modbus_device->Modbus_Device->RXBuffer[DEVICE_ADDRESS_INDEX];

	Tx_buffer[FUNC_CODE_INDEX]=modbus_device->Modbus_Device->RXBuffer[FUNC_CODE_INDEX];

	Tx_buffer[BYTE_COUNT_INDEX]=FIXED_FIELD_BYTE+SIZE_OF_BYTE_COUNT+modbus_device->number_of_data_to_return*modbus_device->REGISTER;

	uint32_t i=0,j=2,number_of_byte=0;

	for(i=modbus_device->starting_index;i<modbus_device->number_of_data_to_return;i++)
	{
		Data_t data={0};
		if(modbus_device->REGISTER==HOLDING)
		{
			float temp=*modbus_device->Register_DataBase[modbus_device->REGISTER][modbus_device->starting_address+i].ptr;
			number_of_byte=4;
			data.data=temp;
		}

		else if(modbus_device->REGISTER==HOLDING)
		{
			short temp=ftofix16(*modbus_device->Register_DataBase[modbus_device->REGISTER][modbus_device->starting_address+i].ptr);
			number_of_byte=2;
			data.data=temp;
		}

		for(int k=0;k<number_of_byte;k++)
			Tx_buffer[++j]=data.byte[k];
	}
	uint16_t crc = crc16(Tx_buffer, ++j);
	Tx_buffer[j]=crc&0xFF;
	Tx_buffer[++j]=(crc>>8)&0xFF;
	return Tx_buffer;
}


















void ModbusException(Modbus_Device_t* modbus_device,uint8_t illegal_code)
{
	uint8_t TxBuffer[8];

	TxBuffer[0]=modbus_device->Modbus_Device->RXBuffer[0];

	TxBuffer[1]=modbus_device->Modbus_Device->RXBuffer[1]|0x80;

	TxBuffer[2]=illegal_code;

	uint16_t crc = crc16(TxBuffer, 3);

	TxBuffer[3]=crc&0xFF;

	TxBuffer[4]=(crc>>8)&0xFF;

	RS485Send(modbus_device->Modbus_Device,TxBuffer);
}









#elif defined MOD_MASTER

void Modbus_Master_Init(Modbus_Master_t* modbus_master,GPIO_TypeDef* TX_Enable_Port,uint16_t TX_Enable_Pin, UART_HandleTypeDef* Modbus_UART)
{
	Modbus_UART=Modbus_UART;
	modbus_master->Modbus_Master->TX_Enable_Pin=TX_Enable_Pin;
	modbus_master->Modbus_Master->TX_Enable_Port=TX_Enable_Port;
}

void Modbus_Device_Init(Modbus_Master_t* modbus_master,DataType_t REGISTER,uint8_t device_address,int number_of_data,...)
{
	if(modbus_master->number_of_devices>MAX_NUMBER_OF_DEVICES)
		return;

	uint8_t index=0;

	if(modbus_master->number_of_devices==0)
	{

		modbus_master->Modbus_Master=malloc(sizeof(Modbus_t));

		if(modbus_master->Modbus_Master==NULL)
			return;

		modbus_master->number_of_devices=1;

		index=0;

		modbus_master->List_of_Devices = calloc(1,sizeof(Modbus_Devices_t));

		if(	modbus_master->List_of_Devices==NULL)
			return;

		modbus_master->List_of_Devices[index].Device_ID=device_address;

		modbus_master->List_of_Devices[index].Register_DataBase = calloc(TYPES_OF_REGISTER,sizeof(Register_t*));

		if(modbus_master->List_of_Devices[index].Register_DataBase==NULL)
			return;

		modbus_master->List_of_Devices[index].number_of_registers[HOLDING]=modbus_master->List_of_Devices[index].number_of_registers[INPUT]=0;

		modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]=calloc(number_of_data,sizeof(Register_t));

		if(	modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]==NULL)
			return;

	}

	else if(modbus_master->number_of_devices>0)
	{

		index=0;

		for(int i=0;i<modbus_master->number_of_devices;i++)
		{

			if(modbus_master->List_of_Devices[i].Device_ID == device_address)//if id init before
			{
				index=i;
				modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]=realloc(  modbus_master->List_of_Devices[index].Register_DataBase[REGISTER], (modbus_master->List_of_Devices[index].number_of_registers[REGISTER] + number_of_data)*sizeof(Register_t));
				break;
			}

			else if(i==modbus_master->number_of_devices-1)//new device, not init before
			{

				index=i+1;

				modbus_master->number_of_devices++;
				modbus_master->List_of_Devices=realloc( modbus_master->List_of_Devices , (modbus_master->number_of_devices)*sizeof(Modbus_Devices_t) );
				modbus_master->List_of_Devices[index].Device_ID=device_address;
				modbus_master->List_of_Devices[index].Register_DataBase=malloc(TYPES_OF_REGISTER*sizeof(Register_t*));

				if(modbus_master->List_of_Devices[index].Register_DataBase==NULL)
					return;

				modbus_master->List_of_Devices[index].number_of_registers[INPUT]=modbus_master->List_of_Devices[index].number_of_registers[HOLDING]=0;
				modbus_master->List_of_Devices[index].starting_address=0;
				modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]=malloc(number_of_data*sizeof(Register_t));

				if(modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]==NULL)
					return;
			}
		}
	}

	number_of_data*=2;

	va_list valist;

	va_start(valist, number_of_data);

	for(int i=0;i<number_of_data/2;i++)
	{
		modbus_master->List_of_Devices[index].Register_DataBase[REGISTER][modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+i ].ptr=va_arg(valist, float*);
		modbus_master->List_of_Devices[index].Register_DataBase[REGISTER][modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+i].address=va_arg(valist,uint32_t);
	}

	va_end(valist);

	modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+=number_of_data/2;//the device index that has been append new data

}

void Modbus_Device_Append_Register(Modbus_Master_t* modbus_master,DataType_t REGISTER,uint8_t device_address,int number_of_data,...)
{
	if(modbus_master->number_of_devices>MAX_NUMBER_OF_DEVICES)
		return;
	if(modbus_master->number_of_devices==0)
		return;
	uint8_t index=0;
	for(int i=0;i<modbus_master->number_of_devices;i++)
	{
		if(modbus_master->List_of_Devices[i].Device_ID == device_address)//found id in array of structure
		{
			index=i;
			modbus_master->List_of_Devices[index].Register_DataBase[REGISTER]=realloc(  modbus_master->List_of_Devices[index].Register_DataBase[REGISTER], (modbus_master->List_of_Devices[index].number_of_registers[REGISTER] + number_of_data)*sizeof(Register_t));
			break;
		}
		else if(i==modbus_master->number_of_devices-1)//index not found after looping all device id in all index
			return;

	}
	number_of_data*=2;
	va_list valist;
	va_start(valist, number_of_data);
	for(int i=0;i<number_of_data/2;i++)
	{
		modbus_master->List_of_Devices[index].Register_DataBase[REGISTER][ modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+i ].ptr=va_arg(valist, float*);
		modbus_master->List_of_Devices[index].Register_DataBase[REGISTER][modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+i].address=va_arg(valist,uint32_t);
	}
	va_end(valist);
	modbus_master->List_of_Devices[index].number_of_registers[REGISTER]+=number_of_data/2;//the device index that has been append new data
}


void Modbus_Master_Read_Register(Modbus_Master_t *modbus_master,DataType_t REGISTER,uint8_t Device_add,uint16_t starting_address, uint16_t number_of_points)
{
	uint8_t* TxData;
	if(REGISTER==HOLDING)
		modbus_master->function_code=READ_HOLDING_REGISTERS;
	else if(REGISTER==INPUT)
		modbus_master->function_code=READ_INPUT_REGISTERS;
	else
		return;
	modbus_master->Device_Address=Device_add;

	modbus_master->starting_address=starting_address;
	modbus_master->number_of_points=number_of_points;



	uint8_t  index = GetDeviceIndex(modbus_master);
	modbus_master->List_of_Devices[index].starting_address	= starting_address;
	modbus_master->List_of_Devices[index].command	= READ_REGISTER;
	TxData=PrepareTxBuffer(modbus_master, Device_add);
	RS485Send(modbus_master->Modbus_Master,TxData);
	RS485Read(modbus_master->Modbus_Master,number_of_points);
}

void Modbus_Master_Callback(Modbus_Master_t* modbus_master)
{

	uint8_t device_index=GetDeviceIndex(modbus_master);

	if(modbus_master->Modbus_Master->RXBuffer[FUNC_CODE_INDEX]%2==0)
		modbus_master->REGISTER=INPUT;

	else
		modbus_master->REGISTER=HOLDING;
	int number_of_floats = (modbus_master->Modbus_Master->RXBuffer[BYTE_COUNT_INDEX])/2;
	int data_byte_index=3;
	for(uint8_t data_number=0;data_number<number_of_floats;data_number++)
	{
		modbus_master-> List_of_Devices[device_index].Register_DataBase[modbus_master->REGISTER][(modbus_master->List_of_Devices[device_index].starting_address) + data_number ].ptr=(float*)( modbus_master->Modbus_Master->RXBuffer[data_byte_index]<<8 | modbus_master->Modbus_Master->RXBuffer[data_byte_index+1] );
		data_byte_index+=2;
	}



}

uint8_t* PrepareTxBuffer(Modbus_Master_t *modbus,uint8_t device_add)
{

	uint8_t *Tx_buffer;

	if(modbus->List_of_Devices[device_add].command==READ_REGISTER){
		Tx_buffer=(uint8_t*)calloc(FIXED_FIELD_BYTE+SIZE_OF_NUMBER_OF_POINTS+SIZE_OF_REGISTER_ADDRESS,sizeof(uint8_t));

		if(Tx_buffer==NULL)
			return NULL;

		Tx_buffer[DEVICE_ADDRESS_INDEX]=modbus->List_of_Devices[device_add].command;
		Tx_buffer[FUNC_CODE_INDEX]=modbus->Modbus_Master->RXBuffer[FUNC_CODE_INDEX];
		Tx_buffer[2] =(modbus->starting_address>>8)&0xFF;//high byte
		Tx_buffer[3] = modbus->starting_address;//low byte
		Tx_buffer[4] =(modbus->number_of_points>>8)&0xFF;//high byte
		Tx_buffer[5] = modbus->number_of_points;//low byte
		uint16_t crc = crc16(Tx_buffer, 6);
		Tx_buffer[6] = crc&0xFF;   // CRC LOW
		Tx_buffer[7] = (crc>>8)&0xFF;  // CRC HIGH
	}

	return Tx_buffer;

}

uint8_t GetDeviceIndex(Modbus_Master_t* modbus_master)//get device index in from device array
{
	uint8_t index =0;
	for(index=0;index<modbus_master->number_of_devices;index++)
	{
		if( modbus_master->List_of_Devices[index].Device_ID ==modbus_master->Modbus_Master->RXBuffer[DEVICE_ADDRESS_INDEX])
			break;
	}
	return index;
}

#endif

void RS485Read(Modbus_t *modbus,uint8_t number_of_bytes)
{
	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_RESET);
	HAL_UARTEx_ReceiveToIdle_IT(  Modbus_UART,&modbus->RXBuffer[0],number_of_bytes);
}

void RS485Send(Modbus_t *modbus,uint8_t *data)
{
	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(Modbus_UART, data, strlen((char*)data), 1000);
	HAL_GPIO_WritePin(modbus->TX_Enable_Port, modbus->TX_Enable_Pin, GPIO_PIN_RESET);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)//for master
{
	if(huart == Modbus_UART)
	{
#if defined MOD_MASTER
		Modbus_Master_Callback(&ModBusMaster);

#elif  defined MOD_DEVICE
		if(ModBusDevice.Device_ID==ModBusDevice.Modbus_Device->RXBuffer[0])
			Modbus_Device_Callback(&ModBusDevice);
#endif
	}
}

short ftofix16(float num)
{
	short i=0,f=0;

	if(fabs(num)>2047.999f)//out of short range
		return i;

	i=(short)num;
	f=(short)(fabs(num*16))& 15;
	return(i<<4)|f;

}

float fix16tof(int n)
{
	float s=1.0f;
	if(n<0)
	{
		s=-1.0f;
		n=-n;
	}
	return s*((float)(n>>4)+((n&15) /16.0f ) );
}

