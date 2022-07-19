//
//  Constants.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import Foundation

let base = "https://api.carbonintensity.org.uk"

struct Constants {
    struct Urls {
        static let carbonIntensityNationwide = URL(string: "\(base)/intensity")!
        static let carbonIntensityRegional = URL(string: "\(base)/regional/regionid")!
        static let carbonIntensityRegionalForecast = URL(string: "\(base)/regional/intensity")!
    }
}


let regions = [
    1: "North Scotland",
    2: "South Scotland",
    3: "North West England",
    4: "North East England",
    5: "Yorkshire",
    6: "North Wales",
    7: "South Wales",
    8: "West Midlands",
    9: "East Midlands",
    10: "East England",
    11: "South West England",
    12: "South England",
    13: "London",
    14: "South East England",
    15: "England",
    16: "Scotland",
    17: "Wales"
]
