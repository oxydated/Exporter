#pragma once
#include "mathUtility.h"

namespace oxyde {
	namespace math {
		float getDualQuaternionParametersFromMatrix(double m[], double& cosTheta, double& sinTheta, double& nx, double& ny, double& nz, double& slide, double& mx, double& my, double& mz);
		void getDualQuaternionParametersFromMatrix(double m[], double& angle, double& nx, double& ny, double& nz, double& slide, double& mx, double& my, double& mz);
		void getDualQuaternionFromMatrix(double m[], float &qs, float &qx, float &qy, float &qz, float &dqs, float &dqx, float &dqy, float &dqz);
	}
}