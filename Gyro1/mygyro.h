// MyGyro.h

#ifndef __MYGYRO_H_
#define __MYGYRO_H_

#define SAMPLE_RATE 50				// 400Hz/8
#define ANGULAR_RATE_PER_VOLT 50	// 20mV/deg/sec
#define ADC_BITS_PER_VOLT 1024/5	// 2^10/5

#define GYRO_RATE_SCALE_FACTOR 25/103 // ANGULAR_RATE_PER_VOLT / ADC_BITS_PER_VOLT 250/1024

#define GYRO_ANGLE_SCALE_FACTOR 5L/924L // GYRO_RATE_FUDGE_FACTOR / SAMPLE_RATE 5/1024

#define TRUE 1
#define FALSE 0

void 	gyroInitialize(void);
void 	gyroUpdate(void);
long int gyroAngle(void);
void 	gyroAngleReset(void);
void 	gyroDisable(void);
void 	gyroEnable(void);

#endif