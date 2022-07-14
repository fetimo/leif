//
//  LeifViewModel.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import Foundation

let defaultIntensity = Intensity(forecast: 0, actual: 0, index: "unknown")
let defaultCO2Data = CO2Data(
    from: "",
    to: "",
    intensity: defaultIntensity
)
let defaultIVM = IntensityData(intensity: defaultCO2Data)

@MainActor
class LeifViewModel: ObservableObject {

    @Published var intensity: IntensityData
    /// Run timer every 2 minutes.
    @Published var timer = Timer.publish(every: 120, tolerance: 10, on: .main, in: .common).autoconnect()
    var storage: CodableStorage
    
    init() {
        self.storage = UserStorage().storage
        self.intensity = defaultIVM
    }
    
    private func timeChargingInHours() throws -> Float {
        let data: Totals = try storage.fetch(for: "watts")
        let measurements = data.measurements
        if measurements.count > 0 {
            let start = Float(measurements.first!.timestamp)
            let finish = Float(measurements.last!.timestamp)
            return (finish - start) / 60 / 60
        } else {
            return 0
        }
    }
    
    private func calculateAverageWatts() -> Float {
        do {
            /// Calculate the impact of the last charge.
            let timelapse: Totals = try storage.fetch(for: "watts")
            let watt_values: [Float] = timelapse.measurements.map { $0.value }
            let sum = watt_values.reduce(0, +)
            
            if (sum != 0 && watt_values.count != 0) {
                return sum / Float(watt_values.count)
            }
            
            return 0
        } catch {
            return 0
        }
    }

    func populateIntensity() async {
        do {
            let defaults = UserDefaults.standard

            guard let currentRegion = defaults.value(forKey: "Region") as? Int else {
                let impact = try await IntensityService().getNationwideIntensity()
                self.intensity = IntensityData(intensity: impact.data)
                return
            }
            
            let impact = try await IntensityService().getRegionalIntensity(regionID: currentRegion)
            self.intensity = IntensityData(intensity: impact.data)
        } catch {
            print("populateImpact error", error)
        }
    }
    
    func calculateImpact() -> Float {
        let average: Float = calculateAverageWatts()
        let carbonIntensity = self.intensity.data.actual
        
        /// Check all of these things otherwise return 0
        guard let hoursCharging = try? timeChargingInHours(),
              hoursCharging != 0, carbonIntensity != 0 else {
                return 0
            }
        return (hoursCharging / Float(carbonIntensity)) * average
    }
    
    func createOrFetchPreviousWatts() -> Totals {
        var payload: Totals;
        do {
            payload = try storage.fetch(for: "watts")
        } catch {
            payload = Totals(
                total_session: 0,
                total_overall: 0,
                measurements: []
            )
            
            try? storage.save(payload, for: "watts")
        }
        return payload
    }
    
    func removePreviousWatts() -> Totals {
        var totals = createOrFetchPreviousWatts()
        totals.total_session = 0
        totals.measurements = []
        try? storage.save(totals, for: "watts")
        return totals
    }
    
    func addNewMeasurement(payload: Totals, watts: Float) -> Totals {
        let data = WattageData(
            value: watts,
            timestamp: Date().timeIntervalSince1970
        )
        
        var newPayload = payload
        newPayload.total_session = calculateImpact()
        newPayload.total_overall = payload.total_overall + (newPayload.total_session - payload.total_session)
        newPayload.measurements.append(data)
        return newPayload
    }
}
