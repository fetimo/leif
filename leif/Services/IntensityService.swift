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
    var storage: CodableStorage
    let dateFormatter: DateFormatter
    
    init() {
        self.storage = UserStorage().storage
        self.dateFormatter = DateFormatter()
        dateFormatter.locale = Locale(identifier: "en_US_POSIX")
        dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mmZ"
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
    
    func getRegionalForecast(regionID: Int) async throws -> CarbonForecast {
        // Try and get from cache or make http call
        let cacheName = "api:region:\(regionID):forecast"
        do {
            let cached: CarbonForecast = try storage.fetch(for: cacheName)
            
            let expiryDate = dateFormatter.date(from: cached.data.first!.to)!
            if Date.now > expiryDate {
                throw NetworkError.expired
            }
            
            return cached
        } catch {
            let from = Date.now.ISO8601Format()
            
            // By default get next 1.5 hours
            let toDate = Date.now.addingTimeInterval(60 * 60 * 1.5)
            let to = dateFormatter.string(from: toDate)
            
            let url = Constants.Urls.carbonIntensityRegionalForecast
                .appendingPathComponent(from)
                .appendingPathComponent(to)
                .appendingPathComponent("regionid")
                .appendingPathComponent(String(regionID))
            
            let (data, response) = try await URLSession.shared.data(from: url)
            
            guard let httpResponse = response as? HTTPURLResponse,
                  httpResponse.statusCode == 200 else {
                throw NetworkError.invalidResponse
            }
            
            let json = try JSONDecoder().decode(CO2RegionalForecastResponse.self, from: data)
            
            let normalisedCO2Data = json.data.data.map { item in
                return CO2Data(
                    from: item.from ?? Date.now.ISO8601Format(),
                    to: item.to ?? Date.now.ISO8601Format(),
                    intensity: Intensity(forecast: item.intensity?.forecast ?? 0,
                                         actual: item.intensity?.actual ?? item.intensity?.forecast ?? 0,
                                         index: item.intensity?.index ?? "unknown"
                                        )
                )
            }
            
            let normalised = CarbonForecast(data: normalisedCO2Data)
            try storage.save(normalised, for: cacheName)
            
            return normalised
        }
    }
    
    func getNationalForecast() async throws -> CarbonForecast {
        // Try and get from cache or make http call
        let cacheName = "api:national:forecast"
        do {
            let cached: CarbonForecast = try storage.fetch(for: cacheName)
            
            let expiryDate = dateFormatter.date(from: cached.data.first!.to)!
            if Date.now > expiryDate {
                throw NetworkError.expired
            }
            
            return cached
        } catch {
            let from = Date.now.ISO8601Format()
            
            // By default get next 1.5 hours
            let toDate = Date.now.addingTimeInterval(60 * 60 * 1.5)
            let to = dateFormatter.string(from: toDate)
            
            let url = Constants.Urls.carbonIntensityNationwide
                .appendingPathComponent(from)
                .appendingPathComponent(to)
            
            let (data, response) = try await URLSession.shared.data(from: url)
            
            guard let httpResponse = response as? HTTPURLResponse,
                  httpResponse.statusCode == 200 else {
                throw NetworkError.invalidResponse
            }
            
            let json = try JSONDecoder().decode(CO2Response.self, from: data)
            
            let normalisedCO2Data = json.data!.map { item in
                return CO2Data(
                    from: item.from ?? Date.now.ISO8601Format(),
                    to: item.to ?? Date.now.ISO8601Format(),
                    intensity: Intensity(forecast: item.intensity?.forecast ?? 0,
                                         actual: item.intensity?.actual ?? item.intensity?.forecast ?? 0,
                                         index: item.intensity?.index ?? "unknown"
                                        )
                )
            }
            
            let normalised = CarbonForecast(data: normalisedCO2Data)
            try storage.save(normalised, for: cacheName)
            
            return normalised
        }
    }
}
