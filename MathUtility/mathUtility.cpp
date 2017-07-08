#include <utility>
#include <array>
#include <algorithm>
#include "mathUtility.h"

namespace oxyde {
	namespace math {

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

		void rotationEigenValuesAndVectors(const double m[], complex& eVal1, complex& eVal2, complex& eVal3, complex eVec1[], complex eVec2[], complex eVec3[]) {
			cubicRoots(
				-1,
				m[0] + m[5] + m[10],
				m[1] * m[4] - m[0] * m[5] + m[2] * m[8] + m[6] * m[9] - m[0] * m[10] - m[5] * m[10],
				-(m[2] * m[5] * m[8]) + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] + m[0] * m[5] * m[10],
				eVal1, eVal2, eVal3);

			//using calcEigVec = [double]void(*)(complex, complex*);
			auto calcEigVecWithLines1and2 = [&m](complex l, complex* v)->void {
				v[0] = (l*m[8] - m[5] * m[8] + m[4] * m[9]) / (-std::pow(l, 2) + l*m[0] + m[1] * m[4] + l*m[5] - m[0] * m[5]);
				v[1] = (m[1] * m[8] + l*m[9] - m[0] * m[9]) / (-std::pow(l, 2) + l*m[0] + m[1] * m[4] + l*m[5] - m[0] * m[5]);
				v[2] = -1;
			};
			auto calcEigVecWithLines1and3 = [&m](complex l, complex* v)->void {
				v[0] = (l*m[4] + m[6] * m[8] - m[4] * m[10]) / (-std::pow(l, 2) + l*m[0] + m[2] * m[8] + l*m[10] - m[0] * m[10]);
				v[1] = -1;
				v[2] = (m[2] * m[4] + l*m[6] - m[0] * m[6]) / (-std::pow(l, 2) + l*m[0] + m[2] * m[8] + l*m[10] - m[0] * m[10]);
			};
			auto calcEigVecWithLines2and3 = [&m](complex l, complex* v)->void {
				v[0] = -1;
				v[1] = (l*m[1] + m[2] * m[9] - m[1] * m[10]) / (-std::pow(l, 2) + l*m[5] + m[6] * m[9] + l*m[10] - m[5] * m[10]);
				v[2] = (l*m[2] - m[2] * m[5] + m[1] * m[6]) / (-std::pow(l, 2) + l*m[5] + m[6] * m[9] + l*m[10] - m[5] * m[10]);
			};

			using eigPair = std::pair<complex&, complex*>;
			std::array<eigPair, 3> thePairs = { eigPair(eVal1, eVec1), eigPair(eVal2, eVec2),eigPair(eVal3, eVec3) };

			auto chooseFunction = [&](eigPair thePair)->void {
				complex &l = thePair.first;
				complex *n = thePair.second;

				complex v[3] = { complex(0.0, 0.0), complex(0.0, 0.0) , complex(0.0, 0.0) };

				complex crossLines1and2 = std::sqrt(std::pow(std::abs(std::pow(l, 2) - l*m[0] - m[1] * m[4] - l*m[5] + m[0] * m[5]), 2) + std::pow(std::abs(m[1] * m[8] + l*m[9] - m[0] * m[9]), 2) + std::pow(std::abs(l*m[8] - m[5] * m[8] + m[4] * m[9]), 2));
				complex crossLines1and3 = std::sqrt(std::pow(std::abs(-(m[2] * m[4]) - l*m[6] + m[0] * m[6]), 2) + std::pow(std::abs(-std::pow(l, 2) + l*m[0] + m[2] * m[8] + l*m[10] - m[0] * m[10]), 2) + std::pow(std::abs(-(l*m[4]) - m[6] * m[8] + m[4] * m[10]), 2));
				complex crossLines2and3 = std::sqrt(std::pow(std::abs(l*m[2] - m[2] * m[5] + m[1] * m[6]), 2) + std::pow(std::abs(l*m[1] + m[2] * m[9] - m[1] * m[10]), 2) + std::pow(std::abs(std::pow(l, 2) - l*m[5] - m[6] * m[9] - l*m[10] + m[5] * m[10]), 2));

				complex tester = std::real(crossLines1and2) < std::real(crossLines1and3) ? crossLines1and2 : crossLines1and3;
				using complexInt = std::pair<complex, int>;

				auto comparePair = [](auto a, auto b)->bool {return (std::real(a.first) < std::real(b.first)); };
				std::array<complexInt, 3> crossInt = { complexInt(crossLines1and2, 1), complexInt(crossLines1and3, 2), complexInt(crossLines2and3, 3) };
				switch ((*std::max_element(crossInt.begin(), crossInt.end(), comparePair)).second) {
				case 1:
					calcEigVecWithLines1and2(l, v);
					break;
				case 2:
					calcEigVecWithLines1and3(l, v);
					break;
				case 3:
					calcEigVecWithLines2and3(l, v);
					break;
				}
				n[0] = v[0] / std::sqrt(std::pow(std::abs(v[0]), 2) + std::pow(std::abs(v[1]), 2) + std::pow(std::abs(v[2]), 2));
				n[1] = v[1] / std::sqrt(std::pow(std::abs(v[0]), 2) + std::pow(std::abs(v[1]), 2) + std::pow(std::abs(v[2]), 2));
				n[2] = v[2] / std::sqrt(std::pow(std::abs(v[0]), 2) + std::pow(std::abs(v[1]), 2) + std::pow(std::abs(v[2]), 2));
				//auto chosenPair = (std::real(pair1and2.first) < std::real(pair1and3.first)) ? pair1and3 : pair1and2;
			};
			std::for_each(thePairs.begin(), thePairs.end(), chooseFunction);			
		}

		double getAngleFromCosAndSin(double cosTheta, double sinTheta) {
			complex euler(cosTheta, sinTheta);
			return std::real(complex(0, -1)*(std::log(euler)));
		}
	}
}