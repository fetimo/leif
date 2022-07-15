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

let dateFormatter = ISO8601DateFormatter()

class IntensityService {
    var storage: CodableStorage
    let cacheExpiry: Double = 1800
    
    init() {
        self.storage = UserStorage().storage
    }
    
    func getNationwideIntensity() async throws -> CarbonIntensity {
        let url = Constants.Urls.carbonIntensityNationwide
        
        // Try and get from cache or make http call
        do {
            let cached: CarbonIntensity = try storage.fetch(for: "api:national")
            let expiryDate = dateFormatter.date(from: cached.data.to)!
            guard Date.now > expiryDate else {
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
            
            guard let item = json.data?.first else {
                throw NetworkError.invalidResponse
            }
            
            let normalisedCO2Data = CO2Data(
                from: item.from ?? Date.now.ISO8601Format(),
                to: item.to ?? Date.now.ISO8601Format(),
                intensity: Intensity(forecast: item.intensity?.forecast ?? 0,
                                     actual: item.intensity?.actual ?? item.intensity?.forecast ?? 0,
                                     index: item.intensity?.index ?? "unknown"
                                    )
            )
            let normalised = CarbonIntensity(data: normalisedCO2Data)
            try storage.save(normalised, for: "api:national")
            
            return normalised
        }
    }
    
    func getRegionalIntensity(regionID: Int) async throws -> CarbonIntensity {
        let withRegion = Constants.Urls.carbonIntensityRegional.absoluteString + "/\(regionID)"
        let url = URL(string: withRegion)!
        
        // Try and get from cache or make http call
        do {
            let cached: CarbonIntensity = try storage.fetch(for: "api:national")
            let expiryDate = dateFormatter.date(from: cached.data.to)!
            guard Date.now > expiryDate else {
                throw NetworkError.expired
            }
            
            return cached
        } catch {
            let (data, response) = try await URLSession.shared.data(from: url)
            
            guard let httpResponse = response as? HTTPURLResponse,
                  httpResponse.statusCode == 200 else {
                throw NetworkError.invalidResponse
            }
            
            let json = try JSONDecoder().decode(CO2RegionalResponse.self, from: data)

            guard let regionData = json.data.first else {
                throw NetworkError.invalidResponse
            }
            
            guard let item = regionData.data.first else {
                throw NetworkError.invalidResponse
            }
            
            let normalisedCO2Data = CO2Data(
                from: item.from ?? Date.now.ISO8601Format(),
                to: item.to ?? Date.now.ISO8601Format(),
                intensity: Intensity(forecast: item.intensity?.forecast ?? 0,
                                     actual: item.intensity?.actual ?? item.intensity?.forecast ?? 0,
                                     index: item.intensity?.index ?? "unknown"
                                    )
            )
            let normalised = CarbonIntensity(data: normalisedCO2Data)
            try storage.save(normalised, for: "api:region:\(regionID)")

            return normalised
        }
        
    }
}
