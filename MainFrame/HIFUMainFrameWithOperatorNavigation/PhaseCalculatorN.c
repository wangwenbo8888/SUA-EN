/*
 * PhaseCalculatorN.c
 *
 */

/* Include files */
#include <math.h>
#include <QDebug>

#include "SphericalCoord.h"

/* Variable Definitions */
#define pi 3.141592653589793
#define SOUND_SPEED 1540    //Unit: m/s

/* Function Definitions */
void PhaseCalculatorN(double freq, double coords_focus[3], int grade, uint8 phase_data[144])
{
//计算相位
	double deltaX,deltaY,deltaZ,distance,wave;
	int dat;
        for (int i=0; i<144; i++){
		deltaX = coords_focus[0] - SphericalCoord[i][0];
		deltaY = coords_focus[1] - SphericalCoord[i][1];
		deltaZ = coords_focus[2] - SphericalCoord[i][2];
		distance = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
                wave = distance / SOUND_SPEED * freq;
                dat = round((wave -floor(wave))*grade);
		if (dat >= grade) dat -= grade;
			if (dat < 0) dat += grade;
				phase_data[i] = dat;
//			qDebug() << i << ":" << deltaX  << deltaY << deltaZ << distance << wave << wave-(int)wave << round(wave) << grade << dat;
		}
}
void PhaseCalculatorC(double freq, double coords_focus[3], int grade, uint8 phase_data[144])
{
//计算相位
        double deltaX,deltaY,deltaZ,distance,wave;
        int dat;
        for (int i=0; i<216; i++){
                deltaX = coords_focus[0] - SphericalCoord216[i][0];
                deltaY = coords_focus[1] - SphericalCoord216[i][1];
                deltaZ = coords_focus[2] - SphericalCoord216[i][2];
                distance = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
                wave = distance / SOUND_SPEED * freq;
                dat = round((wave -floor(wave))*grade);
                if (dat >= grade) dat -= grade;
                        if (dat < 0) dat += grade;
                                phase_data[i] = dat;
//			qDebug() << i << ":" << deltaX  << deltaY << deltaZ << distance << wave << wave-(int)wave << round(wave) << grade << dat;
                }
}
