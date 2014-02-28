#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  sonar1,         sensorSONAR_mm)
#pragma config(Sensor, dgtl3,  sonar2,         sensorSONAR_mm)
#pragma config(Sensor, dgtl5,  armenc,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  armcalb,        sensorTouch)
#pragma config(Sensor, I2C_1,  I2C1,           sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  I2C2,           sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           rightback,     tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port2,           leftfr,        tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port3,           rightfr,       tmotorVex393HighSpeed, openLoop, reversed)
#pragma config(Motor,  port4,           lbase,         tmotorVex393, PIDControl, reversed, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port5,           rbase,         tmotorVex393, PIDControl, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port7,           lowarm,        tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port8,           leftext,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           rightext,      tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port10,          leftback,      tmotorVex393HighSpeed, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//void slaveMotor(tMotor nSlaveMotor,tMotor nMasterMotor);
int p1E;
int p10E;
float sensorAvgValue1;
float sensorAvgValue2;
float sensorAvgValue(tSensors sensorToAvg, float time);
void multiSensorAvg(tSensors sensorToAvg1, tSensors sensorToAvg2, float time);
void targetSonarSonar();
bool targetSonarEncoder(int precision);
//float plbase;
//float prbase;
float load1=1;
float load2=1;
float load3=1;
task pickUp();
//task motorControl();
//bool pidTrue = true;
//bool sinTrue;
void load();

task main()
{
	//StartTask(motorControl,7);
	nMotorEncoder[port1] = 0;
	nMotorEncoder[port10] = 0;
	while(true)
	{
		load();
		motor[rightback] = vexRT(Ch2)*0.80;
		motor[leftback] = vexRT(Ch3)*0.80;
		motor[leftfr] = vexRT(Ch3)*0.80;
		motor[rightfr] = vexRT(Ch2)*0.80;
		p1E = nMotorEncoder[port1];
		p10E = -nMotorEncoder[port10];
		if(vexRT[Btn6D]==1)
		{
			targetSonarSonar();
		}
		if(vexRT[Btn5D]==1)
		{
			while(!targetSonarEncoder(10))
			{
				//Do nothing. While just keeps running until returns true, which means it worked.
			}

		}
		if(vexRT[Btn5U]==1)
		{
			multiSensorAvg(sonar1,sonar2,2);
		}
		if(vexRT[Btn8U]==1)
		{
			motor[lbase]=127*load3;
			motor[rbase]=127*load3;
		}
		if(vexRT[Btn8D]==1)
		{
			motor[lbase]=10*load2;
			motor[rbase]=10*load2;
		}
		if((vexRT[Btn8D]!=1)&&(vexRT[Btn8U]!=1))
		{
			motor[lbase]=15*load1;
			motor[rbase]=15*load1;
		}
		if(vexRT[Btn8R])
		{
			motor[rightext]=75;
			motor[leftext]=75;
		}
		if(vexRT[Btn8L]==1)
		{
			motor[rightext]=-50;
			motor[leftext]=-50;
		}
		if((vexRT[Btn8R]!=1)&&(vexRT[Btn8L]!=1))
		{
			motor[rightext]=0;
			motor[leftext]=0;
		}
		if(vexRT[Btn7U]==1)
		{
			motor[lowarm]=127;
		}
		if(vexRT[Btn7D]==1)
		{
			motor[lowarm]=-127;
		}
		if((vexRT[Btn7D]!=1)&&(vexRT[Btn7U]!=1))
		{
			motor[lowarm]=0;
		}
		if(vexRT[Btn7L]==1)
		{
			StartTask(pickUp,7);
		}
		if(SensorValue[armcalb] == 1)
		{
			SensorValue[armenc] = 0;
		}
		wait1Msec(5);
	}
}
//This tells average value of a sensor <sensorToAvg> during given time <time> in seconds.
float sensorAvgValue(tSensors sensorToAvg, float time)
{
	int total=0;
	int avg=0;
	time=time/10;
	ClearTimer(T1);
	while((time10[T1]/100)< time)
	{
		total=total+SensorValue[sensorToAvg];
	}
	avg=total/time;
	return avg;
}
void multiSensorAvg(tSensors sensorToAvg1, tSensors sensorToAvg2, float time)
{
	float total1=0;
	float total2=0;
	int timeStep=0;
	ClearTimer(T2);
	while(time100[T2]<time)
	{
		total1=total1+SensorValue[sensorToAvg1];
		total2=total2+SensorValue[sensorToAvg2];
		timeStep++;
	}
	sensorAvgValue1=total1/timeStep;
	sensorAvgValue2=total2/timeStep;
	return;
}
//This method directs the robot to use sonar sensors actively in both target aquisition and approach.
void targetSonarSonar()
{
	if((SensorValue[sonar1]<500)||(SensorValue[sonar2]<500))
	{
		while(SensorValue[sonar1]>SensorValue[sonar2])
		{
			motor[leftback]= 25;
			motor[leftfr]= 25;
		}
		while(SensorValue[sonar1]<SensorValue[sonar2])
		{
			motor[rightback]= 25;
			motor[rightfr]= 25;
		}
		if(abs(SensorValue[sonar1]-SensorValue[sonar2])<10)
		{
			while((SensorValue[sonar1]>100)&&(SensorValue[sonar2]>100))
			{
				motor[rightback] = 20;
				motor[leftback] = 20;
				motor[leftfr] = 20;
				motor[rightfr] = 20;
			}
		}
		while((SensorValue[sonar1]<85)||(SensorValue[sonar2]<85))
		{
			motor[rightback] = -20;
			motor[leftback] = -20;
			motor[leftfr] = -20;
			motor[rightfr] = -20;
		}
	}
	return;
}
//This method uses the sonar for alignment then uses integrated encoders for approach.
bool targetSonarEncoder(int precision)
{
	float sonar1c;
	float sonar2c;
	float dist;
	float rev;
	if((SensorValue[sonar1]<700)||(SensorValue[sonar2]<700))
	{
		while(((SensorValue[sonar1]-SensorValue[sonar2]) < -precision)&&(vexRT[Btn6U]!=1))
		{
			motor[leftback]=-20;
			motor[rightback]=20;
			motor[rightfr]=20;
			motor[leftfr]=-20;
		}
		while(((SensorValue[sonar1]-SensorValue[sonar2]) > precision)&&(vexRT[Btn6U]!=1))
		{
			motor[leftback]=20;
			motor[rightback]=-20;
			motor[rightfr]=-20;
			motor[leftfr]=20;
		}
		if(abs(SensorValue[sonar1]-SensorValue[sonar2]) < precision &&(vexRT[Btn6U]!=1))
		{
				nMotorEncoder[port1] = 0;
				nMotorEncoder[port10] = 0;
				multiSensorAvg(sonar1,sonar2,0.5);
				sonar1c = sensorAvgValue1;
				sonar2c = sensorAvgValue2;
				dist = (sonar1c+sonar2c)/2;
				rev = (dist-10)/0.8142495;
				while((nMotorEncoder[port1] < rev)&&(-nMotorEncoder[port10] < rev)&&(vexRT[Btn6U]!=1))
					{
						if((abs(SensorValue[sonar1]-SensorValue[sonar2]) < precision))
						{
							return false;
						}
						motor[leftback]=25;
						motor[rightback]=25;
						motor[rightfr]=25;
						motor[leftfr]=25;
					}

			}
			else return false;
			return true;
	}
	return false;
}
task pickUp()
{
	while(true)
	{
		if(((abs(SensorValue[armenc]) - 5) < -1)&&(vexRT[Btn6D] != 1))
		{
			motor[lbase]=55;
			motor[rbase]=55;
		}
		if(((abs(SensorValue[armenc]) - 5) > 1)&&(vexRT[Btn6D] != 1))
		{
			motor[lbase]=0;
			motor[rbase]=0;
		}
		while(((abs(SensorValue[armenc]) - 5) > -1)&&((abs(SensorValue[armenc]) - 5) < 1)&&(vexRT[Btn6D] != 1))
		{
			motor[lowarm]=127;
			wait1Msec(3000);
			if(vexRT[Btn7R]==1)
			{
				StopTask(pickUp);
			}
		}
	}
}

void load()
{
	if((abs(SensorValue[armenc]) > 85)&&(abs(SensorValue[armenc]) < 105))
	{
		load1=0;
		load2=-10;
		load3=0.12;
	}
	if(abs(SensorValue[armenc]) < 85)
	{
		load1=1;
		load2=1;
		load3=1;
	}
	if(abs(SensorValue[armenc]) > 105)
	{
		load1=-1;
		load2=-24;
		load3=-0.416;
	}
}

/*task motorControl()
{
	float valA = SensorValue[armenc];
	ClearTimer(T3);
	float valB;
	float ddx;
	float error;
	motor[lbase]=25;
	motor[rbase]=25;
	while(true)
	{
		while(pidTrue == true)
		{
			float targetVal= (0.5*(plbase+prbase));
			wait1Msec(100);
			valB = abs(SensorValue[armenc]);
			ddx = (valB-valA)/(time100[T3]/10);
			valA = valB;
			ClearTimer(T3);
			error = ((targetVal-ddx)/targetVal);
			motor[lbase]= round(motor[lbase]-(motor[lbase]*error));
			motor[rbase]= round(motor[rbase]-(motor[rbase]*error));

			 if((0.5*(plbase+prbase) - ddx) > 30)
			{
				motor[lbase]= motor[lbase]+10;
				motor[rbase]= motor[rbase]+10;
			}
			if(((0.5*(plbase+prbase) - ddx) < 30)&&((0.5*(plbase+prbase) - ddx) > 15))
			{
				motor[rbase]= motor[rbase]+5;
				motor[lbase]= motor[lbase]+5;
			}
			if(((0.5*(plbase+prbase) - ddx) < 15)&&((0.5*(plbase+prbase) - ddx) > 0))
			{
				motor[lbase]= motor[lbase]+1;
				motor[rbase]= motor[rbase]+1;
			}
			if(((0.5*(plbase+prbase) - ddx) < 0)&&((0.5*(plbase+prbase) - ddx) > -15))
			{
				motor[rbase]= motor[rbase]-1;
				motor[lbase]= motor[lbase]-1;
			}
			if(((0.5*(plbase+prbase) - ddx) < 0)&&((0.5*(plbase+prbase) - ddx) > -15))
			{
				motor[rbase]= motor[rbase]-5;
				motor[lbase]= motor[lbase]-5;
			}
			if(((0.5*(plbase+prbase) - ddx) < -15)&&((0.5*(plbase+prbase) - ddx) > -30))
			{
				motor[rbase]= motor[rbase]-10;
				motor[lbase]= motor[lbase]-10;
			}
		}
	}
}
*/
