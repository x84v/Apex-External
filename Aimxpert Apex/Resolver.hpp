
class Resolver {
public:
    static QAngle CalculateAngle(Vector3D from, Vector3D to) {
        Vector3D newDirection = to.Subtract(from);
        newDirection.Normalize();

        float pitch = -asinf(newDirection.z) * (180 / M_PI);
        float yaw = atan2f(newDirection.y, newDirection.x) * (180 / M_PI);

        return QAngle(pitch, yaw);
    }

    static float GetBulletDrop(Vector3D localPosition, Vector3D targetPosition, float bulletSpeed, float bulletGravity) {
        float time = GetTimeToTarget(localPosition, targetPosition, bulletSpeed);
        bulletGravity *= 700.0f;
        return 0.5f * bulletGravity * time * time;
    }

    static Vector2D CalculateAngles(Vector3D from, Vector3D to) {
        Vector3D newDirection = to.Subtract(from);
        newDirection.Normalize();

        float yaw = atan2f(newDirection.y, newDirection.x) * (180.0f / M_PI);
        float pitch = -asinf(newDirection.z) * (180.0f / M_PI);

        return Vector2D(pitch, yaw);
    }

    static Vector3D GetTargetPosition(const Vector3D& targetPosition, Vector3D targetVelocity, float time) {
        return targetPosition.Add((targetVelocity.Multiply(time)));
    }

    static float GetTimeToTarget(Vector3D startPosition, Vector3D endPosition, float bulletSpeed) {
        float distance = (endPosition.Subtract(startPosition)).Magnitude();
        return distance / bulletSpeed;
    }

    static float GetBasicBulletDrop(Vector3D startPosition, Vector3D endPosition, float bulletSpeed, float bulletDropRate) {
        float time = GetTimeToTarget(startPosition, endPosition, bulletSpeed);
        bulletDropRate *= 750.0f;
        return 0.5f * bulletDropRate * time * time;
    }

    static Vector3D GetTargetPosition(Vector3D startPosition, Vector3D endPosition, Vector3D targetVelocity, float bulletSpeed) {
        float time = GetTimeToTarget(startPosition, endPosition, bulletSpeed);
        return GetTargetPosition(endPosition, targetVelocity, time);
    }

    static bool CalculateAimRotation(Vector3D startPosition, Vector3D endPosition, Vector3D targetVelocity, float bulletSpeed, QAngle& result) {
        endPosition = GetTargetPosition(startPosition, endPosition, targetVelocity, bulletSpeed);
        result = CalculateAngle(startPosition, endPosition);
        return true;
    }

    static void CalculateAimRotationBasicDrop(Vector3D startPosition, Vector3D endPosition, Vector3D targetVelocity, float bulletSpeed, float bulletScale, QAngle& result) {
        endPosition.z += GetBasicBulletDrop(startPosition, endPosition, bulletSpeed, bulletScale);
        result = CalculateAngle(startPosition, endPosition);
    }

    static bool CalculateAimRotationNew(Vector3D start, Vector3D targetPosition, Vector3D targetVelocity, float bulletSpeed, float bulletScale, int steps, QAngle& result) {
        const float gravity = 750.0f / bulletScale;

        float angle = 0;
        float time = 0.0;
        float timeStep = 1.0f / steps;

        for (int i = 0; i < steps; i++) {
            Vector3D predictedPosition = GetTargetPosition(targetPosition, targetVelocity, time);
            if (!OptimalAngle(start, predictedPosition, bulletSpeed, gravity, angle))
                continue;

            Vector3D direction = predictedPosition.Subtract(start);
            float horizontalDistance = direction.Magnitude2D();
            float travelTime = horizontalDistance / (bulletSpeed * cosf(angle));
            if (travelTime <= time) {
                result.x = -angle * (180 / M_PI);
                result.y = atan2f(direction.y, direction.x) * (180 / M_PI);
                return true;
            }
            time += timeStep;
        }

        targetPosition = GetTargetPosition(start, targetPosition, targetVelocity, bulletSpeed);
        result = CalculateAngle(start, targetPosition);
        return true;
    }

    static float GetTimeToTarget(const Vector3D start, const Vector3D end, const float speed, const float gravity) {
        float horizontalDistance = start.Distance2D(end);
        float heightDifference = end.z - start.z;
        return (speed * sqrtf(2 * sinf(M_PI / 4) / gravity)) + (sqrtf(2 * sinf(M_PI / 4) * (sinf(M_PI / 4) - 2 * heightDifference) / gravity));
    }

    static bool OptimalAngle(const Vector3D start, const Vector3D end, const float speed, const float gravity, float& angle) {
        float horizontalDistance = start.Distance2D(end);
        float heightDifference = end.z - start.z;

        float root = powf(speed, 4) - gravity * (gravity * powf(horizontalDistance, 2) + 2 * heightDifference * powf(speed, 2));
        if (root < 0.0)
            return false;

        float term1 = powf(speed, 2) - sqrt(root);
        float term2 = gravity * horizontalDistance;

        angle = atanf(term1 / term2);
        return true;
    }

    static double solveTimeOfFlight(double v, double g, Vector3D v_t, Vector3D s_t, Vector3D s_w, Vector3D v_w, double initial_guess = 0.01, double tolerance = 1e-3, int max_iterations = 1000)
    {
        auto equation = [&](double t)
            {
                return
                    std::pow(v_t.x - v_w.x + (s_t.x - s_w.x) / t, 2) +
                    std::pow(v_t.y - v_w.y + (s_t.y - s_w.y) / t, 2) +
                    std::pow(0.5 * g * t + v_t.z - v_w.z + (s_t.z - s_w.z) / t, 2) - std::pow(v, 2);
            };

        auto derivative = [&](double t)
            {
                double term1 = v_t.x - v_w.x + (s_t.x - s_w.x) / t;
                double term2 = v_t.y - v_w.y + (s_t.y - s_w.y) / t;
                double term3 = 0.5 * g * t + v_t.z - v_w.z + (s_t.z - s_w.z) / t;
                double term1_derivative = -1.0 * (s_t.x - s_w.x) / std::pow(t, 2);
                double term2_derivative = -1.0 * (s_t.y - s_w.y) / std::pow(t, 2);
                double term3_derivative = 0.5 * g - 1.0 * (s_t.z - s_w.z) / std::pow(t, 2);
                return 2.0 * (term1 * term1_derivative + term2 * term2_derivative + term3 * term3_derivative);
            };

        double t = initial_guess;
        for (int i = 0; i < max_iterations; ++i)
        {
            double f_value = equation(t);
            double f_derivative_value = derivative(t);

            if (std::abs(f_value) < tolerance)
            {
                break;
            }
            double new_t = t - f_value / f_derivative_value;

            if (new_t < 0.0)
            {
                new_t = t / 2.0;
            }
            t = new_t;
        }
        if (t < 0.0 || t > 2.5)
            t = 0.01;
        return t;
    }
};

int RoundHalfEven(float x) {
    return (x >= 0.0)
        ? static_cast<int>(std::round(x))
        : static_cast<int>(std::round(-x)) * -1;
}

float AL1AF0(float num) {
    if (num > 0) return max(num, 1.0f);
    return min(num, -1.0f);
}