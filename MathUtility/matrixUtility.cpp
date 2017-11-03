#include <complex>
#include "dualQuaternionFunctions.h"
#include "matrixUtility.h"

namespace oxyde {
	namespace math {
		void getDualQuaternionParametersFromMatrix(double m[], double & cosTheta, double & sinTheta, double & nx, double & ny, double & nz, double & slide, double & mx, double & my, double & mz)
		{
			/*double a = 7.4;
			int b = 99;*/

			//double m[16] = {
			//					-0.5969557584733965,-0.7984778792366983,-0.0779544661652992,0.,
			//					-0.7984778792366983,0.600761060381651,-0.0389772330826496,0.,
			//					0.0779544661652992,0.0389772330826496,-0.9961946980917457,0.,
			//					0.,0.,0.,1.
			//};

			//double m[16] = {
			//	-0.5969557584733965, -0.7984778792366983, -0.0779544661652992, 0.,
			//	-0.7984778792366983, 0.600761060381651, -0.0389772330826496, 0.,
			//	0.0779544661652992, 0.0389772330826496, -0.9961946980917457, 0.,
			//	-3.4355131602946907, 1.077328391727392, -2.1131263973396943, 1.
			//};
			//double m[16] = {
			//	0.00002499979166736832,0.9999750002083327,-0.007070949961324468,0.,
			//	0.9999750002083327,0.00002499979166736832,0.007070949961324468,0.,
			//	0.007070949961324468,-0.007070949961324468,-0.9999500004166654,0.,
			//	1.7748379029276935,1.7606960030050445,-1.9999500004166657,1.
			//};

			oxyde::math::complex eVal1, eVal2, eVal3;
			oxyde::math::complex eVec1[3], eVec2[3], eVec3[3];

			oxyde::math::rotationEigenValuesAndVectors(m, eVal1, eVal2, eVal3, eVec1, eVec2, eVec3);

			//cout << "eVal1 = " << eVal1 << endl;
			//cout << "eVal2 = " << eVal2 << endl;
			//cout << "eVal3 = " << eVal3 << endl;

			//cout << endl;

			//cout << "eVec1 = {" << eVec1[0] << ", " << eVec1[1] << ", " << eVec1[2] << "}" << endl;
			//cout << "eVec2 = {" << eVec2[0] << ", " << eVec2[1] << ", " << eVec2[2] << "}" << endl;
			//cout << "eVec3 = {" << eVec3[0] << ", " << eVec3[1] << ", " << eVec3[2] << "}" << endl;

			nx = std::real(eVec1[0]);
			ny = std::real(eVec1[1]);
			nz = std::real(eVec1[2]);

			cosTheta = (-1 + m[0] + m[5] + m[10]) / 2.;
			//oxyde::math::complex sinTheta = ((eVal2 - eVal3)*(eVec1[2] * (eVec2[1] * eVec3[0] - eVec2[0] * eVec3[1]) + eVec1[1] * (-(eVec2[2] * eVec3[0]) + eVec2[0] * eVec3[2]) + eVec1[0] * (eVec2[2] * eVec3[1] - eVec2[1] * eVec3[2]))) /
			//	std::sqrt((std::pow(std::abs(eVec2[0] + eVec3[0]), 2) + std::pow(std::abs(eVec2[1] + eVec3[1]), 2) + std::pow(std::abs(eVec2[2] + eVec3[2]), 2))*
			//	(std::pow(std::abs(eVal2*eVec2[0] + eVal3*eVec3[0]), 2) + std::pow(std::abs(eVal2*eVec2[1] + eVal3*eVec3[1]), 2) + std::pow(std::abs(eVal2*eVec2[2] + eVal3*eVec3[2]), 2)));

			sinTheta = std::real(((eVal2 - eVal3)*(eVec1[2] * (eVec2[1] * eVec3[0] - eVec2[0] * eVec3[1]) + eVec1[1] * (-(eVec2[2] * eVec3[0]) + eVec2[0] * eVec3[2]) + eVec1[0] * (eVec2[2] * eVec3[1] - eVec2[1] * eVec3[2]))) /
				std::sqrt((std::pow(std::abs(eVec2[0] + eVec3[0]), 2) + std::pow(std::abs(eVec2[1] + eVec3[1]), 2) + std::pow(std::abs(eVec2[2] + eVec3[2]), 2))*
				(std::pow(std::abs(eVal2*eVec2[0] + eVal3*eVec3[0]), 2) + std::pow(std::abs(eVal2*eVec2[1] + eVal3*eVec3[1]), 2) + std::pow(std::abs(eVal2*eVec2[2] + eVal3*eVec3[2]), 2))));

			//oxyde::math::complex eulerForm(cosTheta, std::real(sinTheta));

			//cout << endl;
			//cout << "eulerForm = " << eulerForm << endl;

			slide = std::real(eVec1[0] * m[12] + eVec1[1] * m[13] + eVec1[2] * m[14]);

			//cout << "slide = " << slide << endl;

			mx = std::real((-0.5*(-2.*slide*eVec1[0] + 2.*m[12] + (-1. + cosTheta)*std::pow(eVec1[1], 2)*m[12] + (-1. + cosTheta)*std::pow(eVec1[2], 2)*m[12] + sinTheta*eVec1[2] * m[13] + eVec1[0] * eVec1[1] * (m[13] - 1.*cosTheta*m[13]) - 1.*sinTheta*eVec1[1] * m[14] + eVec1[0] * eVec1[2] * (m[14] - 1.*cosTheta*m[14]))) / sinTheta);
			my = std::real((0.5*(2.*slide*eVec1[1] + (-1. + cosTheta)*eVec1[0] * eVec1[1] * m[12] + sinTheta*eVec1[2] * m[12] - 1.*m[13] - 1.*cosTheta*m[13] + (-1. + cosTheta)*std::pow(eVec1[1], 2)*m[13] - 1.*sinTheta*eVec1[0] * m[14] + (-1. + cosTheta)*eVec1[1] * eVec1[2] * m[14])) / sinTheta);
			mz = std::real((0.5*(2.*slide*eVec1[2] - 1.*sinTheta*eVec1[1] * m[12] + (-1. + cosTheta)*eVec1[0] * eVec1[2] * m[12] + sinTheta*eVec1[0] * m[13] + (-1. + cosTheta)*eVec1[1] * eVec1[2] * m[13] - 1.*m[14] - 1.*cosTheta*m[14] + (-1. + cosTheta)*std::pow(eVec1[2], 2)*m[14])) / sinTheta);
		}

		void getDualQuaternionParametersFromMatrix(double m[], double & angle, double & nx, double & ny, double & nz, double & slide, double & mx, double & my, double & mz)
		{
			double cosTheta = 0.0;
			double sinTheta = 0.0;
			getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);
			complex euler(cosTheta, sinTheta);
			//angle = 6.28319 +std::real(complex(0, -1)*(std::log(euler)));
			angle = std::real(complex(0, -1)*(std::log(euler)));
		}

		void getDualQuaternionFromMatrix(double m[], float &qs, float &qx, float &qy, float &qz, float &dqs, float &dqx, float &dqy, float &dqz)
		{
			double nx, ny, nz;
			double angle;
			double slide;
			double mx, my, mz;

			oxyde::math::getDualQuaternionParametersFromMatrix(m, angle, nx, ny, nz, slide, mx, my, mz);

			//DUALQUAVAR(r);
			//dual_Versor(float theta, float ux, float uy, float uz, float s, float mx, float my, float mz, DUALQUAARG(r));
			oxyde::DQ::dual_Versor(angle,
				nx, ny, nz,
				slide,
				mx, my, mz,
				DUALQUACOMP(q));
		}
	}
}
