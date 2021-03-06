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
}

struct CarbonForecast: Codable {
    let data: [CO2Data]
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

struct CO2RegionalResponseData: Codable {
    let regionid: Int
    let dnoregion: String
    let shortname: String
    let data: [CO2ResponseData]
}

struct CO2RegionalResponse: Codable {
    let data: [CO2RegionalResponseData]
}

struct CO2RegionalForecastResponse: Codable {
    let data: CO2RegionalResponseData
}
