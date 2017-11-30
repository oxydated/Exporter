#include <utility>
#include <array>
#include <vector>
#include <algorithm>
#include "mathUtility.h"
#include "debugLog.h"

namespace oxyde {
	namespace math {

		using complex = std::complex<double>;

		void cubicRoots(double a, double b, double c, double d, complex& root1, complex& root2, complex&root3) {
			double qa = 3 * a;
			double qb = 2 * b;
			double qc = c;

			double deltaFirstDerivate = quadDelta(qa, qb, qc);
			double rootsFirstDerivate1 = 0.0, rootsFirstDerivate2 = 0.0;

			//theRealXStart[coeffs_] : =
			double theRealXStart = 0.0;
			//	If[4 * b ^ 2 - 12 * a*c >= 0, (*Delta Equal or Greater Than Zero*)
			if (deltaFirstDerivate >= 0.0) {
				//	>> >> >> >> >> >> >> >> deltaFirstDerivate:
				//	>> >> >> >> >> >> >> >> 4 * std::pow(b, 2) - 12 * a*c

				quadRoot(qa, qb, qc, rootsFirstDerivate1, rootsFirstDerivate2);
				double rootMajor = 0, rootMinor = 0;
				if (rootsFirstDerivate1 > rootsFirstDerivate2) {
					rootMajor = rootsFirstDerivate1;
					rootMinor = rootsFirstDerivate2;
				}
				else {
					rootMajor = rootsFirstDerivate2;
					rootMinor = rootsFirstDerivate1;
				}

				//		If[(a)>0, (*coefficient a Greater Than Zero*)
				if (a > 0) {
					//		If[
					//			(d + c*x + b*x ^ 2 + a*x ^ 3 / .{x->Min[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}]}) > 0, (*is the point above Zero*)
					if (cubicFunc(a, b, c, d, rootMinor) > 0) {
						//				>> >> >> >> >> >> >> >> cubicFunc(x) :
						//				>> >> >> >> >> >> >> >> d + c*x + b*std::pow(x, 2) + a*std::pow(x, 3)
						//				Min[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}] - 0.1,
						theRealXStart = rootMinor - 0.1;
					}
					else {
						//				>> >> >> >> >> >> >> >> rootsFirstDerivate :
						//				>> >> >> >> >> >> >> >> (-2 * b + std::sqrt(4 * std::pow(b, 2) - 12 * a*c)) / (6.*a)
						//				>> >> >> >> >> >> >> >> (-2 * b - std::sqrt(4 * std::pow(b, 2) - 12 * a*c)) / (6.*a)

						//				Max[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}] + 0.1
						theRealXStart = rootMajor + 0.1;
					}
					//		],
				}
				else {
					//		If[
					//			(d + c*x + b*x ^ 2 + a*x ^ 3 / .{x->Min[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}]}) > 0,
					if (cubicFunc(a, b, c, d, rootMinor) > 0) {
						//				Max[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}] + 0.1,
						theRealXStart = rootMajor + 0.1;
					}
					else {
						//				Min[{(-2 * b + std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a), (-2 * b - std::sqrt[4 * b ^ 2 - 12 * a*c]) / (6 * a)}] - 0.1
						theRealXStart = rootMinor - 0.1;
					}
					//		]
				}
				//		],
			}
			else {
				//		(a / std::abs[a])*(0.1)*
				double sign = (cubicFunc(a, b, c, d, -b / (3 * a)) > 0) ? -1 : 1;
				//				If[
				//					(d + c*x + b*x ^ 2 + a*x ^ 3 / .{x-> - b / (3 * a)}) > 0(*is the point above Zero*),
				//						-1,
				//						1
				//				] + -b / (3 * a)
				//						>> >> >> >> >> >> >> >> rootSecondDerivative:
				//					>> >> >> >> >> >> >> >> -b / (3.*a)
				theRealXStart = (a / std::abs(a))*(0.1)*sign + -b / (3 * a);
			}
			//		]
				//realRootString[coeffs_] : = NestWhileList[
				//	(-d + std::pow(x, 2)*(b + 2 * a*x)) / (c + x*(2 * b + 3 * a*x)),
				//		theRealXStart[coeffs],
				//		(std::abs[d + c*x + b*std::pow(x, 2) + a*std::pow(x, 3)] > 0.00000000001) &][[-1]]
				//		>> >> >> >> > std::abs(cubicFunc(a, b, c, d, x)) > 1.e-11
			double x = 0.0;
			for (x = theRealXStart;
				std::abs(cubicFunc(a, b, c, d, x)) > 1.e-14;
				x = (-d + std::pow(x, 2)*(b + 2 * a*x)) / (c + x*(2 * b + 3 * a*x)));
			//complex r(x, 0.0);
			root1 = complex(x, 0.0);
			quadRoot(
				-(d / std::pow(root1, 3)) - c / std::pow(root1, 2) - b / root1,
				-(d / std::pow(root1, 2)) - c / root1,
				-(d / root1),
				root2, root3);
		}

		enum class repeatedEigenvalues { None,  e0and1, e0and2, e1and2 };

		void rotationEigenValuesAndVectors(const double m[], complex& eVal1, complex& eVal2, complex& eVal3, complex eVec1[], complex eVec2[], complex eVec3[]) {

			oxyde::log::printMatrix(L"m", m);

			std::wstring outStr;
			std::wstring resultString;
			std::wostringstream outStream(outStr);

			cubicRoots(
				-1,
				m[0] + m[5] + m[10],
				m[1] * m[4] - m[0] * m[5] + m[2] * m[8] + m[6] * m[9] - m[0] * m[10] - m[5] * m[10],
				-(m[2] * m[5] * m[8]) + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] + m[0] * m[5] * m[10],
				eVal1, eVal2, eVal3);

			std::array<complex, 3>eigenvalues = { eVal1, eVal2, eVal3 };

			std::sort(eigenvalues.begin(), eigenvalues.end(),
				[](complex r, complex s) { return std::norm(r - 1.) < std::norm(s - 1.); }
			);

			eVal1 = eigenvalues[0];			eVal2 = eigenvalues[1];			eVal3 = eigenvalues[2];

			if (std::norm(eVal3 - 1.) < 0.0001) {
				eVec1[0] = 1.0;		eVec1[1] = 0.0;		eVec1[2] = 0.0;
				eVec2[0] = 0.0;		eVec2[1] = 1.0;		eVec2[2] = 0.0;
				eVec3[0] = 0.0;		eVec3[1] = 0.0;		eVec3[2] = 1.0;
				return;
			}

			repeatedEigenvalues theRepeated = repeatedEigenvalues::None;

			if (std::norm(eVal1 - eVal2) < 0.0001)
				theRepeated = repeatedEigenvalues::e0and1;

			if (std::norm(eVal1 - eVal3) < 0.0001)
				theRepeated = repeatedEigenvalues::e0and2;

			if (std::norm(eVal2 - eVal3) < 0.0001)
				theRepeated = repeatedEigenvalues::e1and2;

			std::array<std::array<complex, 3>, 3>eigenvectors;

			std::array<complex, 9>p;
			std::array<complex, 9>mo;

			std::array<complex, 3>v = { 0.0, 0.0, 0.0 };
			std::array<complex, 3>vo;

			for (int i = 0; i < 3; ++i) {
				complex eigV = eigenvalues[i];

				p[0] = -eigV + m[0];	p[3] = m[4];			p[6] = m[8];
				p[1] = m[1];			p[4] = -eigV + m[5];	p[7] = m[9];
				p[2] = m[2];			p[5] = m[6];			p[8] = -eigV + m[10];

				oxyde::log::print3x3ComplexMatrix(L"p", p.data());

				oxyde::math::solveLinearSystem(3, p.data(), v.data(), mo.data(), vo.data());

				oxyde::log::printNamedParameter(L"eigV", eigV);
				oxyde::log::printComplexVector(L"vo", vo[0], vo[1], vo[2]);
				oxyde::log::print3x3ComplexMatrix(L"mo", mo.data());

				//outStream.str(outStr);
				//outStream	<< std::fixed 
				//			<< L"{" << std::real(vo[0]) << L"+" << std::imag(vo[0]) << L" I ,"
				//			<< std::real(vo[1]) << L"+" << std::imag(vo[1]) << L" I ,"
				//			<< std::real(vo[2]) << L"+" << std::imag(vo[2]) << L" I }";
				//oxyde::log::printNamedParameter(L"vo", outStream.str());

				complex theNorm = std::sqrt(std::pow(std::abs(vo[0]), 2) + std::pow(std::abs(vo[1]), 2) + std::pow(std::abs(vo[2]), 2));

				eigenvectors[i][0] = vo[0] / theNorm;
				eigenvectors[i][1] = vo[1] / theNorm;
				eigenvectors[i][2] = vo[2] / theNorm;

				//outStream.str(outStr);
				//outStream << std::fixed
				//	<< L"{" << std::real(eigenvectors[i][0]) << L"+" << std::imag(eigenvectors[i][0]) << L" I ,"
				//	<< std::real(eigenvectors[i][1]) << L"+" << std::imag(eigenvectors[i][1]) << L" I ,"
				//	<< std::real(eigenvectors[i][2]) << L"+" << std::imag(eigenvectors[i][2]) << L" I }";
				//oxyde::log::printNamedParameter(L"eigenvectorsi", outStream.str());

				//outStream.str(outStr);
				//outStream << std::fixed
				//	<< std::real(theNorm) << L"+" << std::imag(theNorm);

				oxyde::log::printNamedParameter(L"theNorm", theNorm);
			}

			if (theRepeated != repeatedEigenvalues::None) {
				int i = 0, j = 0, k = 0;

				if (theRepeated == repeatedEigenvalues::e0and2) {
					i = 0;	j = 1;	k = 2;
				}

				if (theRepeated == repeatedEigenvalues::e1and2) {
					i = 2;	j = 0;	k = 1;
				}

				if (theRepeated == repeatedEigenvalues::e0and1) {
					i = 1;	j = 2;	k = 0;
				}	

				oxyde::log::printNamedParameter(L"i", i);
				oxyde::log::printNamedParameter(L"j", j);
				oxyde::log::printNamedParameter(L"k", k);

				eigenvectors[k][0] = -(eigenvectors[i][2] * eigenvectors[j][1]) + eigenvectors[i][1] * eigenvectors[j][2];
				eigenvectors[k][1] = eigenvectors[i][2] * eigenvectors[j][0] - eigenvectors[i][0] * eigenvectors[j][2];
				eigenvectors[k][2] = -(eigenvectors[i][1] * eigenvectors[j][0]) + eigenvectors[i][0] * eigenvectors[j][1];
			}

			eVec1[0] = eigenvectors[0][0];		eVec1[1] = eigenvectors[0][1];		eVec1[2] = eigenvectors[0][2];
			eVec2[0] = eigenvectors[1][0];		eVec2[1] = eigenvectors[1][1];		eVec2[2] = eigenvectors[1][2];
			eVec3[0] = eigenvectors[2][0];		eVec3[1] = eigenvectors[2][1];		eVec3[2] = eigenvectors[2][2];

			outStream.str(outStr);	
			outStream << std::fixed;
			outStream << L"Solve[{" << -1 << L", ";
			outStream << m[0] + m[5] + m[10] << L", ";
			outStream << m[1] * m[4] - m[0] * m[5] + m[2] * m[8] + m[6] * m[9] - m[0] * m[10] - m[5] * m[10] << L", ";
			outStream <<-(m[2] * m[5] * m[8]) + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] + m[0] * m[5] * m[10] << L"}";
			outStream << L".{x^3, x^2, x, 1}==0, x]";
			outStream << std::endl;

			oxyde::log::printText(outStream.str());

			outStream.str(outStr);
			outStream << std::fixed << std::real(eVal1) << L"+" << std::imag(eVal1) << L" I ";
			oxyde::log::printNamedParameter(L"eVal1", outStream.str());
			outStream.str(outStr);
			outStream << std::fixed << std::real(eVal2) << L"+" << std::imag(eVal2) << L" I ";
			oxyde::log::printNamedParameter(L"eVal2", outStream.str());
			outStream.str(outStr);
			outStream << std::fixed << std::real(eVal3) << L"+" << std::imag(eVal3) << L" I ";
			oxyde::log::printNamedParameter(L"eVal3", outStream.str());

			std::array<float, 16> mFloat = { m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15] };
			oxyde::log::printMatrix(L"m", mFloat.data());
			oxyde::log::printText(L"Eigenvalues[m[[1;;3, 1;;3]]]");


			outStream.str(outStr);
			outStream << std::fixed << L"{" << std::real(eVec1[0]) << L"+" << std::imag(eVec1[0]) << L" I ,"
				<< std::real(eVec1[1]) << L"+" << std::imag(eVec1[1]) << L" I ,"
				<< std::real(eVec1[2]) << L"+" << std::imag(eVec1[2]) << L" I }";
			oxyde::log::printNamedParameter(L"eVec1", outStream.str());
			outStream.str(outStr);
			outStream << std::fixed << L"{" << std::real(eVec2[0]) << L"+" << std::imag(eVec2[0]) << L" I ,"
				<< std::real(eVec2[1]) << L"+" << std::imag(eVec2[1]) << L" I ,"
				<< std::real(eVec2[2]) << L"+" << std::imag(eVec2[2]) << L" I }";
			oxyde::log::printNamedParameter(L"eVec2", outStream.str());
			outStream.str(outStr);
			outStream << std::fixed << L"{" << std::real(eVec3[0]) << L"+" << std::imag(eVec3[0]) << L" I ,"
				<< std::real(eVec3[1]) << L"+" << std::imag(eVec3[1]) << L" I ,"
				<< std::real(eVec3[2]) << L"+" << std::imag(eVec3[2]) << L" I }";
			oxyde::log::printNamedParameter(L"eVec3", outStream.str());

			oxyde::log::printText(L"Eigenvectors[m[[1;;3, 1;;3]]]");
		}

		double getAngleFromCosAndSin(double cosTheta, double sinTheta) {
			complex euler(cosTheta, sinTheta);
			return std::real(complex(0, -1)*(std::log(euler)));
		}
		void invertMatrix(const double m[], double o[])
		{
			double coadj = m[1] * m[7] * m[10] * m[12] - m[1] * m[6] * m[11] * m[12] - m[0] * m[7] * m[10] * m[13] + m[0] * m[6] * m[11] * m[13] - 
				m[1] * m[7] * m[8] * m[14] + m[0] * m[7] * m[9] * m[14] + m[1] * m[4] * m[11] * m[14] - m[0] * m[5] * m[11] * m[14] + m[3] * 
				(m[6] * (m[9] * m[12] - m[8] * m[13]) + m[5] * (-(m[10] * m[12]) + m[8] * m[14]) + m[4] * (m[10] * m[13] - m[9] * m[14])) + 
				m[1] * m[6] * m[8] * m[15] - m[0] * m[6] * m[9] * m[15] - m[1] * m[4] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15] + m[2] * 
				(m[7] * (-(m[9] * m[12]) + m[8] * m[13]) + m[5] * (m[11] * m[12] - m[8] * m[15]) + m[4] * (-(m[11] * m[13]) + m[9] * m[15]));

			o[0] = (m[7] * (-(m[10] * m[13]) + m[9] * m[14]) + m[6] * (m[11] * m[13] - m[9] * m[15]) + m[5] * (-(m[11] * m[14]) + m[10] * m[15])) / coadj;
			o[4] = (m[7] * (m[10] * m[12] - m[8] * m[14]) + m[6] * (-(m[11] * m[12]) + m[8] * m[15]) + m[4] * (m[11] * m[14] - m[10] * m[15])) / coadj;
			o[8] = (m[7] * (-(m[9] * m[12]) + m[8] * m[13]) + m[5] * (m[11] * m[12] - m[8] * m[15]) + m[4] * (-(m[11] * m[13]) + m[9] * m[15])) / coadj;
			o[12] = (m[6] * (m[9] * m[12] - m[8] * m[13]) + m[5] * (-(m[10] * m[12]) + m[8] * m[14]) + m[4] * (m[10] * m[13] - m[9] * m[14])) / coadj;
			o[1] = (m[3] * (m[10] * m[13] - m[9] * m[14]) + m[2] * (-(m[11] * m[13]) + m[9] * m[15]) + m[1] * (m[11] * m[14] - m[10] * m[15])) / coadj;
			o[5] = (m[3] * (-(m[10] * m[12]) + m[8] * m[14]) + m[2] * (m[11] * m[12] - m[8] * m[15]) + m[0] * (-(m[11] * m[14]) + m[10] * m[15])) / coadj;
			o[9] = (m[3] * (m[9] * m[12] - m[8] * m[13]) + m[1] * (-(m[11] * m[12]) + m[8] * m[15]) + m[0] * (m[11] * m[13] - m[9] * m[15])) / coadj;
			o[13] = (m[2] * (-(m[9] * m[12]) + m[8] * m[13]) + m[1] * (m[10] * m[12] - m[8] * m[14]) + m[0] * (-(m[10] * m[13]) + m[9] * m[14])) / coadj;
			o[2] = (m[3] * (-(m[6] * m[13]) + m[5] * m[14]) + m[2] * (m[7] * m[13] - m[5] * m[15]) + m[1] * (-(m[7] * m[14]) + m[6] * m[15])) / coadj;
			o[6] = (m[3] * (m[6] * m[12] - m[4] * m[14]) + m[2] * (-(m[7] * m[12]) + m[4] * m[15]) + m[0] * (m[7] * m[14] - m[6] * m[15])) / coadj;
			o[10] = (m[3] * (-(m[5] * m[12]) + m[4] * m[13]) + m[1] * (m[7] * m[12] - m[4] * m[15]) + m[0] * (-(m[7] * m[13]) + m[5] * m[15])) / coadj;
			o[14] = (m[2] * (m[5] * m[12] - m[4] * m[13]) + m[1] * (-(m[6] * m[12]) + m[4] * m[14]) + m[0] * (m[6] * m[13] - m[5] * m[14])) / coadj;
			o[3] = (m[3] * (m[6] * m[9] - m[5] * m[10]) + m[2] * (-(m[7] * m[9]) + m[5] * m[11]) + m[1] * (m[7] * m[10] - m[6] * m[11])) / coadj;
			o[7] = (m[3] * (-(m[6] * m[8]) + m[4] * m[10]) + m[2] * (m[7] * m[8] - m[4] * m[11]) + m[0] * (-(m[7] * m[10]) + m[6] * m[11])) / coadj;
			o[11] = (m[3] * (m[5] * m[8] - m[4] * m[9]) + m[1] * (-(m[7] * m[8]) + m[4] * m[11]) + m[0] * (m[7] * m[9] - m[5] * m[11])) / coadj;
			o[15] = (m[2] * (-(m[5] * m[8]) + m[4] * m[9]) + m[1] * (m[6] * m[8] - m[4] * m[10]) + m[0] * (-(m[6] * m[9]) + m[5] * m[10])) / coadj;
		}

		void multiplyMatrices(const double m[], const double n[], double o[]) {
			o[0] = m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3];
			o[4] = m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7];
			o[8] = m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11];
			o[12] = m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15];
			o[1] = m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3];
			o[5] = m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7];
			o[9] = m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11];
			o[13] = m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15];
			o[2] = m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3];
			o[6] = m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7];
			o[10] = m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11];
			o[14] = m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15];
			o[3] = m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3];
			o[7] = m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7];
			o[11] = m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11];
			o[15] = m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15];
		}

		void echelonForm(const int dim, const complex m[], const complex v[], complex om[], complex ov[])
		{
			for (int i = 0; i < dim; ++i) {
				ov[i] = v[i];
				for (int j = 0; j < dim; ++j) {
					om[j*dim + i] = m[j*dim + i];
				}
			}

			auto mat = [&](int row, int col)->complex& {return om[col*dim + row]; };

			int theRow = 0;
			int theColumn = 0;

			while (theRow < dim && theColumn < dim) {

				//func = Function[theInput, (
				//{ theRow, theColumn } = theInput;
				//posList = FirstPosition[M, SelectFirst[M[[theRow;; ]], (#[[theColumn]] != 0) &]];
				int pos = -1;
				for (int i = theRow; (i < dim) && (pos < 0); ++i) {
					double theNorm = std::norm(mat(i, theColumn) - complex(0.0, 0.0));
					if (theNorm > 0.0001) {
						pos = i;
					}
				}
				//If[!MissingQ[posList],
				if (pos != -1) {
					//	pos = posList[[1]];
					//Print[pos];

					complex theDenominator = mat(pos, theColumn);

					for (int i = 0; i < dim; ++i) {
						complex tmp = mat(pos, i);
						mat(pos, i) = mat(theRow, i);
						mat(theRow, i) = tmp / theDenominator;
						//{tmp, Vtmp} = { M[[pos]], V[[pos]] };
						//{M[[pos]], V[[pos]]} = { M[[theRow]], V[[theRow]] };
						//theDenominator = tmp[[theColumn]];
						//{M[[theRow]], V[[theRow]]} = { tmp / theDenominator, Vtmp / theDenominator };
					}

					complex vtmp = ov[pos];
					ov[pos] = ov[theRow];
					ov[theRow] = vtmp / theDenominator;

					//M;
					//MapIndexed[
					for (int i = 0; i < dim; ++i) {
						//	(
						//		If[#1[[theColumn]] != 0 && #2[[1]] != theRow,
						complex valueAtTheColumn = mat(i, theColumn);
						if (i != theRow) {
							for (int j = 0; j < dim; ++j) {
								//		M[[#2[[1]]]] = M[[#2[[1]]]] - #1[[theColumn]] M[[theRow]];
								mat(i, j) -= valueAtTheColumn * mat(theRow, j);
							}
							//	V[[#2[[1]]]] = V[[#2[[1]]]] - #1[[theColumn]] V[[theRow]]
							ov[i] -= valueAtTheColumn * ov[theRow];
						}
						//		]
					}
					//		) &, M];
					//Print[M, V];
					//{theRow + 1, theColumn + 1}
					theRow += 1;
					theColumn += 1;
				}
				else
					//,
				{
					//	Print["missing"];
					//{theRow, theColumn + 1}
					//]
					theColumn += 1;
				}
				//	)]

			}
			//	NestWhile[func, { 1, 1 }, (#[[1]] < 4 && #[[2]] < 4) &]
		}

		void solveLinearSystem(const int dim, const complex m[], const complex v[], complex om[], complex ov[]) {
			auto mat = [&](int row, int col)->complex& {return om[col*dim + row]; };
			echelonForm(dim, m, v, om, ov);

			//vec = {0, 0, 0}

			std::vector<complex>vec(dim, 0.);

			std::vector<int>basicVariableRow(dim, -1);

			std::vector<bool> isFreeVariable(dim, true);

			//leadingEntry =  Select[MapIndexed[{#1, #2[[1]]} &, M],   AnyTrue[#[[1]], (Norm[#] > 0.0001) &] &]

			//leadingVariables =  Map[(FirstPosition[#[[1]],  SelectFirst[#[[1]], (Norm[#] > 0.0001) &]][[1]]) &, leadingEntry]

			//freeVariables =  Reverse[Select[Range[3], (! MemberQ[leadingVariables, #]) &]]
			for (int i = 0; i < dim; ++i) {
				bool foundNotZero = false;

				for (int j = 0; j < dim && (!foundNotZero); ++j) {
					if (std::norm(mat(i, j)) > 0.0001) {
						foundNotZero = true;
						isFreeVariable[j] = false;
						basicVariableRow[j] = i;
					}
				}
			}

			//if[Length[freeVariables] > 0,
			//vec[[freeVariables[[1]]]] = 1.0]

			bool firstFreeVariable = false;
			for (int i = dim - 1; i >= 0; --i) {
				if (!firstFreeVariable) {
					if (isFreeVariable[i]) {
						firstFreeVariable = true;
						vec[i] = 1.;
					}
				}
			}

			//MapThread[(vec[[#2]] =     V[[#1[[2]]]] + (-#1[[1]]).vec) &, {leadingEntry,   leadingVariables}]
			for (int i = dim - 1; i >= 0; --i) {
				if (!isFreeVariable[i]) {
					int row = basicVariableRow[i];
					complex value = ov[row];
					for (int j = 0; j < dim; ++j) {
						value -= vec[j] * mat(row, j);
					}
					vec[i] = value;
				}
			}

			for (int i = 0; i < dim; ++i) {
				ov[i] = vec[i];
			}
		}

		void solveLinearSystem2(const int dim, const complex m[], const complex v[], complex om[], complex ov[])
		{
			auto mat = [&](int row, int col)->complex& {return om[col*dim + row]; };
			echelonForm(dim, m, v, om, ov);

			//	Map[(FirstPosition[#, SelectFirst[#, (Norm[#] > 0.0001) &]]) &, M];
			std::vector<bool> isFreeVariable(dim, true);
			for (int i = 0; i < dim; ++i) {
				bool foundFirstGreaterThanZero = false;
				for (int j = 0; (j < dim) && (!foundFirstGreaterThanZero) ; ++j) {
					if (std::norm(mat(i, j)) > 0.0001) {
						foundFirstGreaterThanZero = true;
						isFreeVariable[j] = false;
					}
				}
			}
			//	Print[Style[%, Background -> Yellow]];
			//	Select[Reverse[Range[3]], (! MemberQ[%%, {#}]) &];
			//	Print[Style[%, Background -> Pink]];

			//	MapIndexed[(
			bool isTheFirst = true;
			for (int k = 0; k < dim; ++k) {
				int dimIndex = dim - 1 - k;
				if (isFreeVariable[dimIndex]) {
					//	    Print[#2];
					//	    If[#2 == {1},
					if (isTheFirst) {
						isTheFirst = false;
						//	     V = V - M[[1 ;; 3, #1]];
						for (int i = 0; i < dim; ++i) {
							ov[i] = ov[i] - mat(i, dimIndex);
						}
						//	     V[[#1]] = 1,
						ov[dimIndex] = 1.0;
					}
					else {
						//	     V[[#1]] = 0
						ov[dimIndex] = 0.0;

					}
					//	     ];
					//	    ) &, %%];
				}
			}
			//	" "
			//	TraditionalForm[M]
			//	V
			//	sM.V
			//	sV
			//	Normalize[V]
		}
	}
}