//
//  IntensityService.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import Foundation

enum NetworkError: Error {
    case expired
    case invalidResponse
}

class IntensityService {
    func getIntensity(url: URL) async throws -> CarbonIntensity {
        let path = try FileManager.default.url(
            for: .cachesDirectory,
            in: .userDomainMask,
            appropriateFor: nil,
            create: false
        )
        let disk = DiskStorage(path: path)
        let storage = CodableStorage(storage: disk)
        
        // Try and get from cache or make http call
        do {
            let cached: CarbonIntensity = try storage.fetch(for: "co2_response")
            let diff = Date.now.timeIntervalSince1970 - cached.fetched_at
            guard diff < 1800 else {
                throw NetworkError.expired
            }
            
            return cached
        } catch {
            let (data, response) = try await URLSession.shared.data(from: url)

            guard let httpResponse = response as? HTTPURLResponse,
                  httpResponse.statusCode == 200 else {
                throw NetworkError.invalidResponse
            }

            let json = try JSONDecoder().decode(CO2Response.self, from: data)
            let item = json.data?.first
            
            let normalisedCO2Data = CO2Data(
                from: item?.from ?? Date.now.ISO8601Format(),
                to: item?.to ?? Date.now.ISO8601Format(),
                intensity: Intensity(forecast: item?.intensity?.forecast ?? 0,
                                     actual: item?.intensity?.actual ?? item?.intensity?.forecast ?? 0,
                                     index: item?.intensity?.index ?? "unknown"
                                    )
            )
            let normalised = CarbonIntensity(data: normalisedCO2Data, fetched_at: Date.now.timeIntervalSince1970)
            try storage.save(normalised, for: "co2_response")
            
            return normalised
        }
    
    }
}
