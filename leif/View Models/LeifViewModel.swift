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
let defaultForecastData = [defaultCO2Data]

@MainActor
class LeifViewModel: ObservableObject {

    @Published var intensity: IntensityData
    @Published var forecast: [CO2Data]
    
    /// Run timer every 2 minutes.
    @Published var timer = Timer.publish(every: 120, tolerance: 10, on: .main, in: .common).autoconnect()
    var storage: CodableStorage
    
    init() {
        self.storage = UserStorage().storage
        self.intensity = defaultIVM
        self.forecast = defaultForecastData
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
    
    func populateForecast() async {
        do {
            let defaults = UserDefaults.standard
            
            guard let currentRegion = defaults.value(forKey: "Region") as? Int else {
                return
            }
            
            let forecast = try await IntensityService().getRegionalForecast(regionID: currentRegion)
            self.forecast = forecast.data
        } catch {
            print("populateForecast error", error)
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
    
    func generateForecast() -> String {
        let data = self.forecast
        
        let dict = [
            "very high": 5,
            "high": 4,
            "medium": 3,
            "low": 2,
            "very low": 1
        ]
        
        let forecast = data.map { dict[$0.intensity.index] ?? 100 }
        
        let now = forecast[0]
        let next = forecast[1]
        let later = forecast[2]
        
        // Now is less than anytime in near future
        if now <= next && now < later {
            return "Charge now if needed"
        }
        
        // Now is worse than next 30 minutes
        if now > next {
            return "Charge in 30 minutes time"
        }
        
        if now > later {
            return "Charge in an hour or so"
        }

        return "Charge when needed"
    }
}
