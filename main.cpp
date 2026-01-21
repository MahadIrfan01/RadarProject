#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <stdexcept>

using namespace std;

/* =========================
   Physical Constants
   ========================= */
namespace Physics {
    constexpr double SPEED_OF_LIGHT = 3e8;          // m/s
    constexpr double PI = 3.141592653589793;
}

/* =========================
   Radar Configuration
   ========================= */
struct RadarConfig {
    double carrierFrequency;     // Hz
    double wavelength;            // meters
    double pulseWidth;            // seconds
    double noiseFloor;            // arbitrary units
    double detectionThreshold;    // SNR threshold
};

/* =========================
   Target Representation
   ========================= */
struct Target {
    double range;        // meters
    double velocity;     // m/s
    double angle;        // radians
    double radarCrossSection;
};

/* =========================
   Measurement Output
   ========================= */
struct RadarMeasurement {
    double estimatedRange;
    double estimatedVelocity;
    double x;
    double y;
    double snr;
};

/* =========================
   Utility Functions
   ========================= */
double dB(double value) {
    return 10 * log10(value);
}

double clamp(double val, double minVal, double maxVal) {
    return max(minVal, min(val, maxVal));
}

/* =========================
   Doppler Calculations
   ========================= */
double dopplerShift(double velocity, double wavelength) {
    return (2 * velocity) / wavelength;
}

/* =========================
   Radar Equation (Simplified)
   ========================= */
double receivedPower(const RadarConfig& radar, const Target& target) {
    double numerator = pow(radar.wavelength, 2) * target.radarCrossSection;
    double denominator = pow(4 * Physics::PI, 3) * pow(target.range, 4);
    return numerator / denominator;
}

/* =========================
   Noise Generator
   ========================= */
double generateNoise(double noiseFloor) {
    static random_device rd;
    static mt19937 gen(rd());
    normal_distribution<> dist(0.0, noiseFloor);
    return dist(gen);
}

/* =========================
   Core Radar Processing
   ========================= */
RadarMeasurement processTarget(
    const RadarConfig& radar,
    const Target& target
) {
    RadarMeasurement measurement{};

    double power = receivedPower(radar, target);
    double noise = generateNoise(radar.noiseFloor);
    double snr = power / abs(noise);

    if (snr < radar.detectionThreshold) {
        throw runtime_error("Target below detection threshold");
    }

    double doppler = dopplerShift(target.velocity, radar.wavelength);

    measurement.estimatedVelocity =
        (doppler * radar.wavelength) / 2;

    double timeDelay = (2 * target.range) / Physics::SPEED_OF_LIGHT;
    measurement.estimatedRange =
        (Physics::SPEED_OF_LIGHT * timeDelay) / 2;

    measurement.x =
        measurement.estimatedRange * cos(target.angle);
    measurement.y =
        measurement.estimatedRange * sin(target.angle);

    measurement.snr = dB(snr);

    return measurement;
}

/* =========================
   Radar System Simulation
   ========================= */
void runRadarSimulation(
    const RadarConfig& radar,
    const vector<Target>& targets
) {
    cout << fixed << setprecision(3);
    cout << "\n=== RADAR SCAN INITIATED ===\n";

    for (size_t i = 0; i < targets.size(); ++i) {
        cout << "\nTarget #" << i + 1 << ":\n";

        try {
            RadarMeasurement m =
                processTarget(radar, targets[i]);

            cout << "  Range      : " << m.estimatedRange << " m\n";
            cout << "  Velocity   : " << m.estimatedVelocity << " m/s\n";
            cout << "  Location   : (" << m.x << ", " << m.y << ")\n";
            cout << "  SNR        : " << m.snr << " dB\n";

        } catch (const exception& e) {
            cout << "  Detection Failed: " << e.what() << "\n";
        }
    }

    cout << "\n=== SCAN COMPLETE ===\n";
}

/* =========================
   Main Entry Point
   ========================= */
int main() {
    RadarConfig radar{
        10e9,        // 10 GHz (X-band)
        0.03,        // wavelength
        1e-6,        // pulse width
        1e-9,        // noise floor
        5.0          // detection threshold
    };

    vector<Target> targets = {
        { 15000, 250, 0.5, 10.0 },
        { 8000,  -120, 1.2, 3.5 },
        { 25000, 0, 2.1, 0.8 }
    };

    runRadarSimulation(radar, targets);
    return 0;
}
