//
//  LeifViewModel.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import Foundation

@MainActor
class LeifViewModel: ObservableObject {

    @Published var intensity: [IntensityViewModel] = []
    @Published var watts: Float = 0
    // Run timer every 30 minutes.
    @Published var timer = Timer.publish(every: 120, tolerance: 10, on: .main, in: .common).autoconnect()
    @Published var storage: CodableStorage?
    
    init() {
        do {
            let path = try FileManager.default.url(
                for: .cachesDirectory,
                in: .userDomainMask,
                appropriateFor: nil,
                create: false
            )
            let disk = DiskStorage(path: path)
            self.storage = CodableStorage(storage: disk)
        } catch {
            print("Error setting up cache")
        }
        
        Task {
            await self.populateImpact()
        }
        
    }
    
    private func timeChargingInHours() throws -> Float {
        let data: Totals = try storage!.fetch(for: "watts")
        let measurements = data.measurements
        if (measurements.count > 0) {
            let start = Float(measurements.first!.timestamp)
            let finish = Float(measurements.last!.timestamp)
            return (finish - start) / 60 / 60
        } else {
            return 0
        }
    }
    
    private func calculateAverageWatts() -> Float {
        do {
            // Calculate the impact of the last charge.
            let timelapse: Totals = try storage!.fetch(for: "watts")
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

    func populateImpact() async {
        do {
            let impact = try await IntensityService().getIntensity(url: Constants.Urls.latestImpact)
            // self.intensity = impact.data.map(IntensityViewModel.init)
            self.intensity = impact.data.map {
                var modified = $0
                // At one point actual returned null.
                modified.intensity.actual = modified.intensity.actual == nil ? modified.intensity.forecast : modified.intensity.actual;
                return IntensityViewModel.init(intensity: modified)
            }
        } catch {
            print("populateImpact error", error)
        }
    }
    
    func calculateImpact() -> Float {
        do {
            let average: Float = calculateAverageWatts()
            return (Float(try timeChargingInHours()) / Float(self.intensity.first!.data.actual)) * average
        } catch {
            return 0
        }
    }
    
    func createOrFetchPreviousWatts() -> Totals {
        var payload: Totals;
        do {
            payload = try storage!.fetch(for: "watts")
        } catch {
            payload = Totals(
                total_session: 0,
                total_overall: 0,
                measurements: []
            )
            
            try? storage!.save(payload, for: "watts")
        }
        return payload
    }
    
    func removePreviousWatts() -> Totals {
        var totals = createOrFetchPreviousWatts()
        totals.total_session = 0
        totals.measurements = []
        try? storage!.save(totals, for: "watts")
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
