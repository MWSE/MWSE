return {
	type = "operator",
	overloads = {
		{ resultType = "niQuaternion", description = "Unary negation. The resulting quaternion represents the same rotation. It's used to get the closest rotation to another quaternion. `if q1:dot(targetQuat) < 0` then the closest path to reach `targetQuat` is `-targetQuat`. Used in the `slerp` method." },
	}
}
