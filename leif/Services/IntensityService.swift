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
    func getIntensity(url: URL) async throws -> CO2_Response {
        let path = try FileManager.default.url(
            for: .cachesDirectory,
            in: .userDomainMask,
            appropriateFor: nil,
            create: false
        )
        let disk = DiskStorage(path: path)
        let storage = CodableStorage(storage: disk)
        
//        Try and get from cache or make http call
        do {
            let cached: CO2_Response = try storage.fetch(for: "co2_response")

            if (cached.fetched_at != nil) {
//                Expire if over 30 minutes
                let diff = Date.now.timeIntervalSince1970 - cached.fetched_at!
                if (diff > 1800) {
                    throw NetworkError.expired
                }
            } else {
                throw NetworkError.expired
            }
            
            return cached
        } catch {
            let (data, response) = try await URLSession.shared.data(from: url)

            guard let httpResponse = response as? HTTPURLResponse,
                  httpResponse.statusCode == 200 else {
                throw NetworkError.invalidResponse
            }

            var json = try JSONDecoder().decode(CO2_Response.self, from: data)
            json.fetched_at = Date.now.timeIntervalSince1970

            try storage.save(json, for: "co2_response")
            return json
        }
    
    }
}
