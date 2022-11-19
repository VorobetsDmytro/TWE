#ifndef TWE_MATH_HPP
#define TWE_MATH_HPP

#define EPSILON 1.192092896e-07F
#define PI 3.14159265359f

#include <glm.hpp>

#include "math.h"

namespace TWE {
	class Math {
	public:
		[[nodiscard]] static float getAcos(float x) {
        	if (x < -1.f)
				x = -1.f;
			if (x > 1.f)
				x = 1.f;
			return acosf(x);
    	}
		[[nodiscard]] static float getAngle(const glm::quat &quat) {
			return 2.f * getAcos(quat.w);
		}
		[[nodiscard]] static glm::vec3 getAxis(const glm::quat &quat) {
			float squared = 1.f - quat.w * quat.w;
			if (squared < 10.f * EPSILON)
				return {1.f, 0.f, 0.f};           
			float s = 1.f / sqrt(squared);
			return { quat.x * s, quat.y * s, quat.z * s };
		}
	};
}

#endif