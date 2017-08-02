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
	double m[16] = { 
		0.61264, 0.703219, 0.360769, 0, 0.787378, -0.582659, -0.201355, 0, 0.068609, 0.40742, -0.91066, 0, 19.364143, 4.e-6, 4.e-6, 1
	};
	//double m[16] = {
	//	0.00002499979166736832,0.9999750002083327,-0.007070949961324468,0.,
	//	0.9999750002083327,0.00002499979166736832,0.007070949961324468,0.,
	//	0.007070949961324468,-0.007070949961324468,-0.9999500004166654,0.,
	//	1.7748379029276935,1.7606960030050445,-1.9999500004166657,1.
	//};
	double nx, ny, nz;
	double cosTheta, sinTheta;
	double slide;
	double mx, my, mz;

	outStream.str(outStr);
	outStream << std::fixed;

	oxyde::math::complex eVal1 = 0., eVal2 = 0., eVal3 = 0.;
	oxyde::math::complex eVec1[3], eVec2[3], eVec3[3];
	oxyde::math::rotationEigenValuesAndVectors(m, eVal1, eVal2, eVal3, eVec1, eVec2, eVec3);

	outStream << "theMatrix = {";
	outStream << "{" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "},";
	outStream << "{" << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << "},";
	outStream << "{" << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << "},";
	outStream << "{" << m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << "}";
	outStream << "}" << std::endl;

	outStream << std::endl;

	outStream << "matrixEigenSystem = ";
	outStream << "{";
	outStream << "{" << prC(eVal1) << ", " << "{" << prC(eVec1[0]) << ", " << prC(eVec1[1]) << ", " << prC(eVec1[2]) << "}},";
	outStream << "{" << prC(eVal2) << ", " << "{" << prC(eVec2[0]) << ", " << prC(eVec2[1]) << ", " << prC(eVec2[2]) << "}},";
	outStream << "{" << prC(eVal3) << ", " << "{" << prC(eVec3[0]) << ", " << prC(eVec3[1]) << ", " << prC(eVec3[2]) << "}}";
	outStream << "}" << std::endl;

	outStream << std::endl;

	oxyde::math::getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);
	outStream << "Normal = {" << nx << ", " << ny << ", " << nz << "}" << endl;
	outStream << "Cos, Sin = {" << cosTheta << ", " << sinTheta << "}" << endl;

	outStream << "Slide = " << slide << endl;
	outStream << "Axis vector m = {" << mx << ", " << my << ", " << mz << "}" << endl;

	outStream << std::endl;

	float qs = 0., qx = 0., qy = 0., qz = 0., dqs = 0., dqx = 0., dqy = 0., dqz = 0.;
	oxyde::math::getDualQuaternionFromMatrix(m, qs, qx, qy, qz, dqs, dqx, dqy, dqz);

	outStream << "quatTest" << " = {";
	outStream << qs << "," << qx << "," << qy << "," << qz << ",";
	outStream << dqs << "," << dqx << "," << dqy << "," << dqz;
	outStream << "}.dualQuatUnit";

	outStream << std::endl << std::endl;

	resultString = outStream.str();
	OutputDebugString(resultString.c_str());
	return 0;
}