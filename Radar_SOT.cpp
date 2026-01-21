void runRadar(
    RadarConfig radar,
    vector<Target> targets,
    int timeSteps
) {
    vector<KalmanState> tracks;

    for (auto& t : targets) {
        tracks.push_back({t.x, t.y, t.vx, t.vy});
    }

    static default_random_engine gen;
    normal_distribution<double> noise(0, radar.noisePower);

    for (int t = 0; t < timeSteps; ++t) {
        cout << "\n=== TIME STEP " << t << " ===\n";

        for (size_t i = 0; i < targets.size(); ++i) {
            Target& tgt = targets[i];

            tgt.x += tgt.vx;
            tgt.y += tgt.vy;

            double range = hypot(tgt.x, tgt.y);
            double angle = atan2(tgt.y, tgt.x);

            double beamGain = arrayFactor(
                radar.arrayElements,
                radar.elementSpacing,
                radar.wavelength,
                angle,
                angle
            );

            double signal =
                receivedPower(radar, tgt) * beamGain;

            double clutter = generateClutter(radar.noisePower);
            double totalNoise = abs(noise(gen)) + clutter;

            if (cfarDetect(signal, totalNoise, radar.cfarThreshold)) {
                KalmanState pred =
                    kalmanPredict(tracks[i], 1.0);

                tracks[i] =
                    kalmanUpdate(
                        pred,
                        tgt.x + noise(gen),
                        tgt.y + noise(gen)
                    );

                cout << "Target " << i
                     << " DETECTED | Pos=("
                     << tracks[i].x << ", "
                     << tracks[i].y << ")\n";
            } else {
                cout << "Target " << i
                     << " NOT DETECTED\n";
            }
        }
    }
}
