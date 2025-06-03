
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401

struct Color {

    static const int size;

    float r;
    float g;
    float b;

    Color() : r(1.0f), g(1.0f), b(1.0f) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {
        clamp();
    }

    Color operator*(const float& scalar) const {
        return Color(r * scalar, g * scalar, b * scalar).clamp();
    }

    Color& operator*=(const float& scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return clamp();
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b).clamp();
    }

    static Color lerp(Color startColor, Color endColor, float t) {
        t = std::clamp(t, 0.0f, 1.0f);

        startColor *= (1.0f - t);
        endColor *= t;
        auto result = startColor + endColor;

        return result.clamp();
    }

    Color& clamp() {
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        return *this;
    }


    bool operator==(const Color& other) const {
        return r == other.r &&
            g == other.g &&
            b == other.b;
    }

    Color& roundColor() {
        r = round(r * 100) / 100;
        g = round(g * 100) / 100;
        b = round(b * 100) / 100;
        return *this;
    }
};
const int Color::size = 3;

namespace Conversion {
    float ToGameUnits(float Meters) {
        return 39.37007874 * Meters;
    }

    float ToMeters(float GameUnits) {
        return GameUnits / 39.37007874;
    }
};

namespace Utils {
    inline float RandomFloat() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        return dis(gen);
    }

    inline float RandomRange(float min, float max) {
        if (min > max) {
            float Temp = min;
            min = max;
            max = Temp;
        }
        return RandomFloat() * (max - min) + min;
    }
}

namespace WeaponIDs {
    //Sniper ammo weapons
    int WEAPON_SENTINEL = 2;
    int WEAPON_CHARGE_RIFLE = 89;
    int WEAPON_LONGBOW = 94;
    int WEAPON_WINGMAN = 139;
    //Shotgun ammo weapons
    int WEAPON_MOZAMBIQUE = 117;
    int WEAPON_EVA8 = 99;
    int WEAPON_PEACEKEEPER = 128;
    int WEAPON_MASTIFF = 114;
    //Light ammo weapons
    int WEAPON_P2020 = 133;
    int WEAPON_RE45 = 86;
    int WEAPON_ALTERNATOR = 84;
    int WEAPON_R301 = 0;
    int WEAPON_SPITFIRE = 135;
    int WEAPON_G7 = 104;
    //Heavy ammo weapons
    int WEAPON_CAR = 144;
    int WEAPON_RAMPAGE = 7;
    int WEAPON_3030 = 143;
    int WEAPON_HEMLOCK = 106;
    int WEAPON_FLATLINE = 102;
    int WEAPON_PROWLER = 123;
    //Energy ammo weapons
    int WEAPON_NEMESIS = 145;
    int WEAPON_VOLT = 142;
    int WEAPON_TRIPLE_TAKE = 137;
    int WEAPON_LSTAR = 111;
    int WEAPON_HAVOC = 98;
    //Legendary ammo weapons
    int WEAPON_HANDS = 145;
    int WEAPON_THROWING_KNIFE = 202;
    int WEAPON_R99 = 130;
    int WEAPON_DEVOTION = 91;
    int WEAPON_BOCEK = 3;
    int WEAPON_KRABER = 109;
    int WEAPON_THERMITE_GRENADE = 164;
    int WEAPON_RAMPART_SHEILA_STATIONARY = 14;
};
