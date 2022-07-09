//
//  Totals.swift
//  leif
//
//  Created by Tim Stone on 08/07/2022.
//

import Foundation

struct WattageData: Codable {
    let value: Float
    let timestamp: TimeInterval
}

struct Totals: Codable {
    var total_session: Float
    var total_overall: Float
    var measurements: [WattageData]
}
