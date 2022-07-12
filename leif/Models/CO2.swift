//
//  CO2.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import Foundation

struct Intensity: Codable {
    let forecast: Int
    var actual: Int
    var index: String
}

struct CO2Data: Codable {
    let from: String
    let to: String
    var intensity: Intensity
}

struct CarbonIntensity: Codable {
    let data: CO2Data
    var fetched_at: TimeInterval
}

struct CO2ResponseIntensity: Codable {
    let forecast: Int?
    var actual: Int?
    var index: String? = "unknown fallback"
}

struct CO2ResponseData: Codable {
    let from: String?
    let to: String?
    var intensity: CO2ResponseIntensity?
}

struct CO2Response: Codable {
    let data: [CO2ResponseData]?
}
