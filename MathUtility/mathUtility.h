#include <cmath>
#include <complex>

namespace oxyde {
	namespace math {
		using complex = std::complex<double>;

		template<typename T> inline T quadDelta(T a, T b, T c) {
			return std::pow(b, 2) - 4 * a*c;
		}

		template<typename T> inline void quadRoot(T a, T b, T c, T& root1, T& root2) {
			root1 = (-b + std::sqrt(std::pow(b, 2.) - 4. * a*c)) / (2.*a);
			root2 = (-b - std::sqrt(std::pow(b, 2.) - 4. * a*c)) / (2.*a);
		}

		template<typename T> inline T quadFunc(T a, T b, T c, T x) {
			return c + b*x + a*std::pow(x, 2);
		}

		template<typename T> inline T cubicFunc(T a, T b, T c, T d, T x) {
			return d + c*x + b*std::pow(x, 2) + a*std::pow(x, 3);
		}

		void cubicRoots(double a, double b, double c, double d, complex& root1, complex& root2, complex&root3);

		void rotationEigenValuesAndVectors(const double m[], complex& eVal1, complex& eVal2, complex& eVal3, complex eVec1[], complex eVec2[], complex eVec3[]);

		double getAngleFromCosAndSin(double cosTheta, double sinTheta);

		void invertMatrix(const double m[], double o[]);

		void multiplyMatrices(const double m[], const double n[], double o[]);
	}

}
