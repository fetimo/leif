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
    let index: String
}

struct CO2_Data: Codable {
    let from: String
    let to: String
    var intensity: Intensity
}

struct CO2_Response: Codable {
    let data: [CO2_Data]
    var fetched_at: TimeInterval?
}

