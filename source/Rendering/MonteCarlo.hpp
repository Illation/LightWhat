#include "common.hpp"
#include <algorithm>

namespace MonteCarlo
{
	inline void toUnitDisc(float &rx, float &ay)
	{
		const float r = sqrtf(rx);
		const float phi = ay * PI2;
		rx = r * cosf(phi);
		ay = r * sinf(phi);
	}
	inline void tangentBitangent(vec3 N, vec3 &T, vec3 &B)
	{
		vec3 NU;
		if (N.x || N.z)NU = vec3(0, 1, 0);
		else NU = vec3(0, 0, 1);
		T = N.Cross(NU).Norm();
		B = N.Cross(T).Norm();
	}
	inline vec3 randomSampleHemisphere(vec3 N)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-1, 1);
		vec3 R = vec3((float)dis(gen), (float)dis(gen), (float)dis(gen)).Norm();
		return R.Dot(N) > 0 ? R : -R;
	}
	inline vec3 cosineSampleHemisphere(vec3 N, vec3 T, vec3 B)
	{

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);
		float u1 = (float)dis(gen);
		float u2 = (float)dis(gen);

		toUnitDisc(u1, u2);
		float costheta = sqrtf(max(1.0f - u1 * u1 - u2 * u2, 0.0f));
		return (T*u1 + N*costheta + B*u2);
	}
	inline vec3 uniformSampleCone(vec3 N, vec3 T, vec3 B, float angle)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);
		float u1 = (float)dis(gen);
		float u2 = (float)dis(gen);

		float z = cosf(angle*u1);
		float r = sqrtf(max(0.0f, 1.0f - z*z));
		float phi = u2 * PI2;
		float x = r * cosf(phi);
		float y = r * sinf(phi);

		return x * T + y * B + z * N;
	}
}