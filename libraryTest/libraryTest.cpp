// libraryTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <Windows.h>
//#include "mathUtility.h"
#include "matrixUtility.h"

using namespace std;

namespace {
	std::wstring outStr;
	std::wstring resultString;
	std::wostringstream outStream(outStr);

	std::wstring prC(const oxyde::math::complex comp) {
		std::wostringstream myStream(L"");
		myStream << std::fixed;
		myStream << std::real(comp) << " + " << std::imag(comp) << " I ";
		return std::wstring(myStream.str());
		//return std::to_wstring(std::real(comp)) + L" + " + std::to_wstring(std::imag(comp)) + L" I ";
	}
}

int main()
{
	double a = 7.4;
	int b = 99;
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
	//	0.61264, 0.703219, 0.360769, 0, 0.787378, -0.582659, -0.201355, 0, 0.068609, 0.40742, -0.91066, 0, 19.364143, 4.e-6, 4.e-6, 1
	//};

	double m[16] = {
		1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.
	};

	//double m[16] = {
	//	-1., 0., 0., 0., 0., 0, -1., 0., 0., -1., 0, 0., -97.154259, 4.983389, -0.33222, 1.
	//};

	//double m[16] = {
	//	0.00002499979166736832,0.9999750002083327,-0.007070949961324468,0.,
	//	0.9999750002083327,0.00002499979166736832,0.007070949961324468,0.,
	//	0.007070949961324468,-0.007070949961324468,-0.9999500004166654,0.,
	//	1.7748379029276935,1.7606960030050445,-1.9999500004166657,1.
	//};

	outStream.str(outStr);
	outStream << std::fixed;

	oxyde::math::complex eVal1 = 0., eVal2 = 0., eVal3 = 0.;
	oxyde::math::complex eVec1[3], eVec2[3], eVec3[3];

	//oxyde::math::rotationEigenValuesAndVectors(m, eVal1, eVal2, eVal3, eVec1, eVec2, eVec3);

	float qs = 0., qx = 0., qy = 0., qz = 0., dqs = 0., dqx = 0., dqy = 0., dqz = 0.;
	oxyde::math::getDualQuaternionFromMatrix(m, qs, qx, qy, qz, dqs, dqx, dqy, dqz);

	outStream << "quatTest" << " = {";
	outStream << qs << "," << qx << "," << qy << "," << qz << ",";
	outStream << dqs << "," << dqx << "," << dqy << "," << dqz;
	outStream << "}.dualQuatUnit";

	outStream << std::endl;
	outStream << "mFromQuat = rotationMatrixFromDualQuat[%]" << std::endl;
	outStream << "\" \"" << std::endl;
	outStream << "TraditionalForm[m]" << std::endl << "TraditionalForm[mFromQuat]" << std::endl;
	outStream << "\" \"" << std::endl;

	///////

	double nx, ny, nz;
	double cosTheta, sinTheta;
	double slide;
	double mx, my, mz;

	oxyde::math::getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);

	outStream << "Style[\"Parameters for quatTest\", Background->Yellow]" << endl;
	outStream << "theNormal = {" << nx << ", " << ny << ", " << nz << "}" << endl;
	outStream << "{theCos, theSin} = {" << cosTheta << ", " << sinTheta << "}" << endl;

	outStream << "slide = " << slide << endl;
	outStream << "mVector = {" << mx << ", " << my << ", " << mz << "}" << endl;

	outStream << std::endl;
	///////

	resultString = outStream.str();
	OutputDebugString(resultString.c_str());



	//outStream << "theMatrix = {";
	//outStream << "{" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "},";
	//outStream << "{" << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << "},";
	//outStream << "{" << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << "},";
	//outStream << "{" << m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << "}";
	//outStream << "}" << std::endl;

	//outStream << std::endl;

	//outStream << "matrixEigenSystem = ";
	//outStream << "{";
	//outStream << "{" << prC(eVal1) << ", " << "{" << prC(eVec1[0]) << ", " << prC(eVec1[1]) << ", " << prC(eVec1[2]) << "}},";
	//outStream << "{" << prC(eVal2) << ", " << "{" << prC(eVec2[0]) << ", " << prC(eVec2[1]) << ", " << prC(eVec2[2]) << "}},";
	//outStream << "{" << prC(eVal3) << ", " << "{" << prC(eVec3[0]) << ", " << prC(eVec3[1]) << ", " << prC(eVec3[2]) << "}}";
	//outStream << "}" << std::endl;

	//outStream << std::endl;

	//float qs = 0., qx = 0., qy = 0., qz = 0., dqs = 0., dqx = 0., dqy = 0., dqz = 0.;
	//oxyde::math::getDualQuaternionFromMatrix(m, qs, qx, qy, qz, dqs, dqx, dqy, dqz);

	//outStream << "quatTest" << " = {";
	//outStream << qs << "," << qx << "," << qy << "," << qz << ",";
	//outStream << dqs << "," << dqx << "," << dqy << "," << dqz;
	//outStream << "}.dualQuatUnit";

	//outStream << std::endl << std::endl;


	//outStream.str(outStr);
	//outStream << std::fixed;

	//outStream << L"Testing echelon form" << endl;

	//oxyde::math::complex m9[9];
	//oxyde::math::complex v[3];

	//m9[0] = 2.;		m9[3] = 1.;		m9[6] = -1.;
	//m9[1] = -3.;	m9[4] = -1.;	m9[7] = 2.;
	//m9[2] = -2.;	m9[5] = 1.;		m9[8] = 2.;

	//v[0] = 8.;	v[1] = -11.;	v[2] = -3.;

	//m9[0] = 1.; m9[3] = 3.; m9[6] = 1.;
	//m9[1] = 1.; m9[4] = 1.; m9[7] = -1.;
	//m9[2] = 3.; m9[5] = 11.; m9[8] = 5.;

	//v[0] = 9; v[1] = 1; v[2] = 35;

	//m9[0] = oxyde::math::complex(0.1507953022318781, -0.7610461578719091);	m9[3] = oxyde::math::complex(0.2250154933885929, 0.);	m9[6] = oxyde::math::complex(0.8375329328867338, 0.);
	//m9[1] = oxyde::math::complex(-0.8577105422198061, 0.);	m9[4] = oxyde::math::complex(0.6636530098928153, -0.7610461578719091);	m9[7] = oxyde::math::complex(-0.5139162454684303, 0.);
	//m9[2] = oxyde::math::complex(-0.12816400313571813, 0.);	m9[5] = oxyde::math::complex(-0.9742408887572159, 0.);	m9[8] = oxyde::math::complex(0.8342505884605129, -0.7610461578719091);

	//m9[0] = oxyde::math::complex(-2., 0);
	//m9[3] = oxyde::math::complex(0., 0);
	//m9[6] = oxyde::math::complex(0., 0);
	//m9[1] = oxyde::math::complex(0., 0);
	//m9[4] = oxyde::math::complex(-2., 0);
	//m9[7] = oxyde::math::complex(0., 0);
	//m9[2] = oxyde::math::complex(0., 0);
	//m9[5] = oxyde::math::complex(0., 0);
	//m9[8] = oxyde::math::complex(0., 0);

	//m9[0] = oxyde::math::complex(0., 0);
	//m9[3] = oxyde::math::complex(0., 0);
	//m9[6] = oxyde::math::complex(0., 0);
	//m9[1] = oxyde::math::complex(0., 0);
	//m9[4] = oxyde::math::complex(0., 0);
	//m9[7] = oxyde::math::complex(0., 0);
	//m9[2] = oxyde::math::complex(0., 0);
	//m9[5] = oxyde::math::complex(0., 0);
	//m9[8] = oxyde::math::complex(2., 0);

	//v[0] = 0.;	v[1] = 0.;	v[2] = 0.;

	//oxyde::math::complex mo[9];
	//oxyde::math::complex vo[3];
	//oxyde::math::solveLinearSystem(3, m9, v, mo, vo);

	//outStream << "m9 = ";
	//outStream << "{";
	//outStream << "{" << m9[0] << ", " << m9[3] << ", " << m9[6] << "},";
	//outStream << "{" << m9[1] << ", " << m9[4] << ", " << m9[7] << "},";
	//outStream << "{" << m9[2] << ", " << m9[5] << ", " << m9[8] << "}";
	//outStream << "}" << std::endl;

	//outStream << std::endl;

	//outStream << "v = ";
	//outStream << "{" << v[0] << ", " << v[1] << ", " << v[2] << "}";
	//outStream << std::endl;

	//outStream << std::endl;

	//outStream << "mo = ";
	//outStream << "{";
	//outStream << "{" << mo[0] << ", " << mo[3] << ", " << mo[6] << "},";
	//outStream << "{" << mo[1] << ", " << mo[4] << ", " << mo[7] << "},";
	//outStream << "{" << mo[2] << ", " << mo[5] << ", " << mo[8] << "}";
	//outStream << "}" << std::endl;

	//outStream << std::endl;

	//outStream << "vo = ";
	//outStream << "{" << vo[0] << ", " << vo[1] << ", " << vo[2] << "}";
	//outStream << std::endl;

	//resultString = outStream.str();
	//OutputDebugString(resultString.c_str());

	return 0;
}